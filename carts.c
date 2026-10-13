#include "common.h"
#include "hash_table_iterator.h"
#include "list_iterator.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "carts.h"
#include "backend.h"


static cart_t *cart_create() {
    cart_t *cart = calloc(1, sizeof(cart_t));
    cart->merchandises = ioopm_hash_table_create(string_knr_hash, string_compare);
    return cart;
}

cart_list_t *cart_list_create() {
    cart_list_t *cart_list = calloc(1, sizeof(cart_list_t));
    cart_list->carts = ioopm_list_create();
    return cart_list;
}

static loc_pair_t *list_pair_create() {
    loc_pair_t *list_pair = calloc(1, sizeof(loc_pair_t));
    return list_pair;
}

static cart_merch_t *cart_merch_create() {
    cart_merch_t *cart_merch = calloc(1, sizeof(cart_merch_t));
    return cart_merch;
}

static void free_cart_merch(cart_t *cart) {
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(cart->merchandises);
    elem_t c_value;
    while (!ioopm_hash_table_iterator_at_end(it)) {
        c_value = ioopm_hash_table_iterator_current_value(it);
        ioopm_hash_table_iterator_advance(it);
        free(c_value.cmerch->name);
        free(c_value.cmerch);
    }
    ioopm_hash_table_iterator_destroy(it);
}

static void free_cart(cart_t *cart) {
    free_cart_merch(cart);
    ioopm_hash_table_destroy(cart->merchandises);
    free(cart);
}

void free_cart_list(cart_list_t *cart_list) {
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(cart_list->carts);
    elem_t current;
    while (!ioopm_list_iterator_at_end(it)) {
        current = ioopm_list_iterator_current(it);
        free_cart(current.crt);
        ioopm_list_iterator_advance(it);
    }
    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(cart_list->carts);
    free(cart_list);
}

static int find_cart_index(cart_list_t *cart_list, size_t cart_id) {

    ioopm_list_iterator_t *cart_it = ioopm_list_iterator_create(cart_list->carts);
    int index = 0;

    while (!ioopm_list_iterator_at_end(cart_it)) {

        if (ioopm_list_iterator_current(cart_it).crt->id == cart_id) {
            ioopm_list_iterator_destroy(cart_it);
            return index;
        }

        ioopm_list_iterator_advance(cart_it);
        index++;
    }

    // varukorgen finns inte
    ioopm_list_iterator_destroy(cart_it);
    return -1;
}

cart_t *ioopm_create_cart(cart_list_t *cart_list) {

    cart_t *cart = cart_create();
    cart_list->id_counter++;
    cart->id = cart_list->id_counter;

    ioopm_list_append(cart_list->carts, crt_elem(cart));

    return cart;
}

void ioopm_remove_cart(cart_list_t *cart_list, size_t cart_id, ioopm_hash_table_t *ht_n) {

    // plocka ut cart index från listan:
    int index = find_cart_index(cart_list, cart_id);

    // om cart inte finns i cart_list:
    if (index < 0) {
        printf("cart %zu does not exist\n", cart_id);
        return;
    }

    cart_t *cart = ioopm_list_get(cart_list->carts, (size_t) index).crt;

    // lämna tillbaka items:
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(cart->merchandises);
    cart_merch_t *merch;
    elem_t S;

    while(!ioopm_hash_table_iterator_at_end(it)) {

        merch = (cart_merch_t*)ioopm_hash_table_iterator_current_value(it).cmerch;

        // ht_n available_stock:
        if (ioopm_hash_table_lookup(ht_n, string_elem(merch->name), &S)) {
            S.st->item->available_stock += merch->amount;
        }

        ioopm_hash_table_iterator_advance(it);

    }

    ioopm_hash_table_iterator_destroy(it);

    // ta bort cart från cart_list:
    ioopm_list_remove(cart_list->carts, (size_t) index);

    // frigör cart:
    free_cart(cart);
}

void ioopm_add_to_cart(cart_t *cart, ioopm_hash_table_t *ht_n, char *name, size_t amount) {

    // kolla så att amount är större än 0
    if (amount == 0) {
        printf("You must add at least 1 element to your cart\n");
        return;
    }

    // kolla i ht så att varan finns
    elem_t S;
    bool exists_in_ht_n = ioopm_hash_table_lookup(ht_n, string_elem(name), &S);
    if (!exists_in_ht_n) {
        printf("%s does not exist\n", name);
        return;
    }
    
    // kolla så availible stock finns
    size_t av_stock = S.st->item->available_stock;
    if (av_stock < amount) {
        printf("Sorry, we do not have %zu items in stock\n", amount);
        return;
    }

    // minska av_stock
    S.st->item->available_stock -= amount;


    // kolla om namnet redan finns i cart
    elem_t cart_merch_old;
    bool exists_in_cart = ioopm_hash_table_lookup(cart->merchandises, string_elem(name), &cart_merch_old);

    // om det gör det öka amount
    if (exists_in_cart) {
        cart_merch_old.cmerch->amount += amount;
    }

    // om inte lägg in en ny cart_merch
    else {
        cart_merch_t *cart_merch_new = cart_merch_create();
        cart_merch_new->name = strdup(name);
        cart_merch_new->amount = amount;
        ioopm_hash_table_insert(cart->merchandises, string_elem(cart_merch_new->name), cmerch_elem(cart_merch_new));
    }
}

void ioopm_remove_from_cart(cart_t *cart, char *name, size_t amount, ioopm_hash_table_t *ht_n) {

    // kolla så amount inte är 0
    if (amount == 0) {
        printf("You must remove at least 1 element from your cart\n");
        return;
    }

    // kolla så att name finns mha ht_lookup
    elem_t cart_merch;
    bool exists = ioopm_hash_table_lookup(cart->merchandises, string_elem(name), &cart_merch);
    if (!exists) {
        printf("the item %s does not exist in your cart", name);
        return;
    }

    // kolla så att den finns i htn!!!!!!

    // kolla så dess stock inte är mindre än amount
    if (cart_merch.cmerch->amount < amount) {
        printf("the cart does not contain %zu of %s", amount, name);
        return;
    }

    // kolla om det är lika stort ta då bort hela elementet och frigör det
    if (cart_merch.cmerch->amount == amount) {
        elem_t result;
        ioopm_hash_table_remove(cart->merchandises, string_elem(name), &result);
        free(result.cmerch->name);
        free(result.cmerch);
    }

    // om stock är större minska bara det
    else {
        cart_merch.cmerch->amount -= amount;
    }

    // lägg tillbaka i availible stock i ht_n
    elem_t S; 
    ioopm_hash_table_lookup(ht_n, string_elem(name), &S);
    S.st->item->available_stock += amount;

}

int ioopm_calc_costs(cart_t *cart, ioopm_hash_table_t *ht_n) {

    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(cart->merchandises);
    int total_cost = 0;
    elem_t cart_merch;
    elem_t S;

    while (!ioopm_hash_table_iterator_at_end(it)) {
        cart_merch = ioopm_hash_table_iterator_current_value(it);

        // plocka ut kolla då den finns
        if (!ioopm_hash_table_lookup(ht_n, string_elem(cart_merch.cmerch->name), &S)) {
            printf("%s does not exist in stock", cart_merch.cmerch->name);
            return -1;
        }

        total_cost += cart_merch.cmerch->amount * S.st->item->price;
        ioopm_hash_table_iterator_advance(it);
    }

    ioopm_hash_table_iterator_destroy(it);
    return total_cost;
}

int ioopm_checkout(cart_list_t *cart_list, size_t cart_id, ioopm_hash_table_t *ht_n) {
    
    // plocka ut cart index från listan:
    int index = find_cart_index(cart_list, cart_id);

    // om cart inte finns i cart_list:
    if (index < 0) {
        printf("cart %zu does not exist\n", cart_id);
        return -2;
    }

    cart_t *cart = ioopm_list_get(cart_list->carts, (size_t) index).crt;

    // ta i loop och ta bort från stock

    ioopm_hash_table_iterator_t *merch_it = ioopm_hash_table_iterator_create(cart->merchandises);
    cart_merch_t *merch;
    elem_t S;

    while(!ioopm_hash_table_iterator_at_end(merch_it)) {

        // ta bort amount från stock:
        merch = (cart_merch_t*)ioopm_hash_table_iterator_current_value(merch_it).cmerch;

        ioopm_hash_table_lookup(ht_n, string_elem(merch->name), &S);

        // ta bort amount från shelfes, från första till sista i listan:
        size_t amount_temp = merch->amount;
        ioopm_list_iterator_t *shelf_it = ioopm_list_iterator_create(S.st->locations);
        loc_pair_t *current;

        while (amount_temp > 0) {

            if (ioopm_list_iterator_at_end(shelf_it)) {
                printf("Not enought stock in locations, does not match availible stock\n");
                ioopm_list_iterator_destroy(shelf_it);
                S.st->item->stock -= merch->amount - amount_temp;
                ioopm_hash_table_iterator_destroy(merch_it);
                return -2;
            }

            current = ioopm_list_iterator_current(shelf_it).p;

            if (current->stock > 0) {

                if (current->stock > amount_temp) {
                    current->stock -= amount_temp;
                    amount_temp = 0;
                }

                else {

                    amount_temp -= current->stock;
                    current->stock = 0;
                }   
            }

            ioopm_list_iterator_advance(shelf_it);
        }

        ioopm_list_iterator_destroy(shelf_it);
        ioopm_hash_table_iterator_advance(merch_it);

        S.st->item->stock -= merch->amount - amount_temp;
    }

    ioopm_hash_table_iterator_destroy(merch_it);

    int total_cost = ioopm_calc_costs(cart, ht_n);

    // ta bort cart från cart_list:
    ioopm_list_remove(cart_list->carts, (size_t) index);

    // frigör cart:
    free_cart(cart);

    return total_cost;
}
