#include "common.h"
#include "hash_table_iterator.h"
#include "list_iterator.h"
#include "carts.h"
#include "backend.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>



// STATISKA:

static cart_t *cart_create() {

    cart_t *cart = calloc(1, sizeof(cart_t));
    cart->merchandises = ioopm_hash_table_create(string_knr_hash, string_compare);
    return cart;
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


// PUBLIKA:

cart_list_t *cart_list_create() {

    cart_list_t *cart_list = calloc(1, sizeof(cart_list_t));
    cart_list->carts = ioopm_list_create();
    return cart_list;
}

void free_cart_list(cart_list_t *cart_list) {

    ioopm_list_iterator_t *it = ioopm_list_iterator_create(cart_list->carts);
    elem_t current;

    // frigör alla carts och deras innehåll
    while (!ioopm_list_iterator_at_end(it)) {
        current = ioopm_list_iterator_current(it);
        free_cart(current.crt);
        ioopm_list_iterator_advance(it);
    }

    // frigör listan i cart_list samt strukten cart_list
    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(cart_list->carts);
    free(cart_list);
}

cart_t *ioopm_create_cart(cart_list_t *cart_list) {

    cart_t *cart = cart_create();
    cart_list->id_counter++;
    cart->id = cart_list->id_counter;

    ioopm_list_append(cart_list->carts, crt_elem(cart));

    printf("Your cart id is: %zu\n", cart_list->id_counter);
    return cart;
}

void ioopm_remove_cart(cart_list_t *cart_list, size_t cart_id, ioopm_hash_table_t *ht_n) {

    // plocka ut varukorgens index i listan av korgar
    int index = find_cart_index(cart_list, cart_id);

    // returnerar ifall kundvagnen inte finns i listan
    if (index < 0) {
        printf("cart %zu does not exist\n", cart_id);
        return;
    }

    cart_t *cart = ioopm_list_get(cart_list->carts, (size_t) index).crt;

    // lämna tillbaka alla varor till availible stock
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(cart->merchandises);
    cart_merch_t *merch;
    elem_t S;

    while(!ioopm_hash_table_iterator_at_end(it)) {

        merch = (cart_merch_t*)ioopm_hash_table_iterator_current_value(it).cmerch;

        if (ioopm_hash_table_lookup(ht_n, string_elem(merch->name), &S)) {
            S.st->item->available_stock += merch->amount;
        }

        ioopm_hash_table_iterator_advance(it);
    }

    ioopm_hash_table_iterator_destroy(it);

    // ta bort varukorgen från listan av korgar
    ioopm_list_remove(cart_list->carts, (size_t) index);

    // frigör varukorgen
    free_cart(cart);
}

void ioopm_add_to_cart(cart_list_t *cart_list, size_t cart_id, char *name, size_t amount, ioopm_hash_table_t *ht_n) {

    // plocka ut varukorgens index i listan av korgar
    int index = find_cart_index(cart_list, cart_id);

    // returnerar ifall kundvagnen inte finns i listan
    if (index < 0) {
        printf("cart %zu does not exist\n", cart_id);
        return;
    }

    cart_t *cart = ioopm_list_get(cart_list->carts, (size_t) index).crt;

    // kolla så att amount är större än 0
    if (amount == 0) {
        printf("You must add at least 1 item to your cart\n");
        return;
    }

    // kolla i merchandise registret så att varan finns
    elem_t S;
    bool exists_in_ht_n = ioopm_hash_table_lookup(ht_n, string_elem(name), &S);
    if (!exists_in_ht_n) {
        printf("%s does not exist\n", name);
        return;
    }
    
    // kolla så att det finns tillräckligt många availible stock
    size_t av_stock = S.st->item->available_stock;
    if (av_stock < amount) {
        printf("Sorry, we do not have %zu items in stock\n", amount);
        return;
    }

    // minska availible stock
    S.st->item->available_stock -= amount;


    // kolla om produkten redan finns i kundvagnen
    elem_t cart_merch_old;
    bool exists_in_cart = ioopm_hash_table_lookup(cart->merchandises, string_elem(name), &cart_merch_old);

    // om det gör det öka antalet
    if (exists_in_cart) {
        cart_merch_old.cmerch->amount += amount;
    }

    // om inte lägg in en ny produkt
    else {
        cart_merch_t *cart_merch_new = cart_merch_create();
        cart_merch_new->name = strdup(name);
        cart_merch_new->amount = amount;
        ioopm_hash_table_insert(cart->merchandises, string_elem(cart_merch_new->name), cmerch_elem(cart_merch_new));
    }
}

void ioopm_remove_from_cart(cart_list_t *cart_list, size_t cart_id, char *name, size_t amount, ioopm_hash_table_t *ht_n) {

    // plocka ut varukorgens index i listan av korgar
    int index = find_cart_index(cart_list, cart_id);

    // returnerar ifall kundvagnen inte finns i listan
    if (index < 0) {
        printf("cart %zu does not exist\n", cart_id);
        return;
    }

    cart_t *cart = ioopm_list_get(cart_list->carts, (size_t) index).crt;

    // kolla så att antalet inte är noll
    if (amount == 0) {
        printf("You must remove at least 1 item from your cart\n");
        return;
    }

    // kolla så att varan finns i varukorgen
    elem_t cart_merch;
    bool exists = ioopm_hash_table_lookup(cart->merchandises, string_elem(name), &cart_merch);
    if (!exists) {
        printf("the item %s does not exist in your cart\n", name);
        return;
    }

    // kolla så att varan finns i det merchandise registret
    elem_t S;
    exists = ioopm_hash_table_lookup(ht_n, string_elem(name), &S);
    if (!exists) {
        printf("%s does not exist\n", name);
        return;
    }

    // kolla så att vi inte försöker ta bort fler än vad som finns i varukorgen
    if (cart_merch.cmerch->amount < amount) {
        printf("the cart does not contain %zu of %s\n", amount, name);
        return;
    }

    // kolla om vi tar bort alla och ta bort varan från varukorgen
    if (cart_merch.cmerch->amount == amount) {
        elem_t result;
        ioopm_hash_table_remove(cart->merchandises, string_elem(name), &result);
        free(result.cmerch->name);
        free(result.cmerch);
    }

    // om vi tar bort färre än vad som finns minskar vi antalet i varukorgen
    else {
        cart_merch.cmerch->amount -= amount;
    }

    // lägg tillbaka antalet i available stock inuti lagret
    S.st->item->available_stock += amount;

}

int ioopm_calc_costs(cart_list_t *cart_list, size_t cart_id, ioopm_hash_table_t *ht_n) {

    // plocka ut varukorgens index i listan av korgar
    int index = find_cart_index(cart_list, cart_id);

    // returnerar ifall kundvagnen inte finns i listan
    if (index < 0) {
        printf("cart %zu does not exist\n", cart_id);
        return -1;
    }

    cart_t *cart = ioopm_list_get(cart_list->carts, (size_t) index).crt;

    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(cart->merchandises);
    int total_cost = 0;
    elem_t cart_merch;
    elem_t S;

    while (!ioopm_hash_table_iterator_at_end(it)) {
        cart_merch = ioopm_hash_table_iterator_current_value(it);

        // plocka ut produkten och kolla så att den finns
        if (!ioopm_hash_table_lookup(ht_n, string_elem(cart_merch.cmerch->name), &S)) {
            printf("%s does not exist in stock", cart_merch.cmerch->name);
            ioopm_hash_table_iterator_destroy(it);
            return -1;
        }

        // multiplicera antalet av produkten med dess pris
        total_cost += cart_merch.cmerch->amount * S.st->item->price;
        ioopm_hash_table_iterator_advance(it);
    }

    ioopm_hash_table_iterator_destroy(it);
    return total_cost;
}

int ioopm_checkout(cart_list_t *cart_list, size_t cart_id, ioopm_hash_table_t *ht_n) {
    
    // plocka ut varukorgens index i listan av korgar
    int index = find_cart_index(cart_list, cart_id);

    // om varukorgen inte finns med i listan av korgar
    if (index < 0) {
        printf("cart %zu does not exist\n", cart_id);
        return -2;
    }

    cart_t *cart = ioopm_list_get(cart_list->carts, (size_t) index).crt;
    ioopm_hash_table_iterator_t *merch_it = ioopm_hash_table_iterator_create(cart->merchandises);
    cart_merch_t *merch;
    elem_t S;

    // för varje produkt i varukorgen
    while(!ioopm_hash_table_iterator_at_end(merch_it)) {

        // ta bort mängden av produkten från lagret
        merch = (cart_merch_t*)ioopm_hash_table_iterator_current_value(merch_it).cmerch;

        ioopm_hash_table_lookup(ht_n, string_elem(merch->name), &S);

        // ta bort mängden av produkten från hyllorna, från första till sista
        size_t amount_temp = merch->amount;
        ioopm_list_iterator_t *shelf_it = ioopm_list_iterator_create(S.st->locations);
        loc_pair_t *current;

        while (amount_temp > 0) {

            // ifall det inte finns tillräckligt många varor på hyllorna
            if (ioopm_list_iterator_at_end(shelf_it)) {
                printf("Not enought stock in locations, does not match availible stock\n");
                ioopm_list_iterator_destroy(shelf_it);
                S.st->item->stock -= merch->amount - amount_temp;
                ioopm_hash_table_iterator_destroy(merch_it);
                return -2;
            }

            current = ioopm_list_iterator_current(shelf_it).p;

            // om det finns varor på hyllan
            if (current->stock > 0) {

                // om det finns fler varor på hyllan än mängden vi tar bort
                if (current->stock > amount_temp) {
                    current->stock -= amount_temp;
                    amount_temp = 0;
                }

                // om det finns lika många eller färre
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

    int total_cost = ioopm_calc_costs(cart_list, cart_id, ht_n);

    // ta bort varukorgen från listan av korgar:
    ioopm_list_remove(cart_list->carts, (size_t) index);

    // frigör varukorgen:
    free_cart(cart);

    return total_cost;
}
