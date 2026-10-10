#include "common.h"
#include "hash_table_iterator.h"
#include "list_iterator.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "backend.h"


static cart_t *cart_create()
{
    cart_t *cart = calloc(1, sizeof(cart_t));
    cart->merchandise = ioopm_hash_table_create(string_knr_hash, string_compare);
    return cart;
}

static cart_list_t *cart_list_create()
{
    cart_list_t *cart_list = calloc(1, sizeof(cart_list_t));
    cart_list->carts = ioopm_list_create();
    return cart_list;
}

static loc_pair_t *list_pair_create() {
    loc_pair_t *list_pair = calloc(1, sizeof(loc_pair_t));
    return list_pair;
}

static void free_cart_info(cart_t *cart) {
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(cart->merchandise);
    elem_t c_value;
    while (!ioopm_hash_table_iterator_at_end(it)) {
        c_value = ioopm_hash_table_iterator_current_value(it);
        free(c_value.cinfo->shelf);
        free(c_value.cinfo);
        ioopm_hash_table_iterator_advance(it);
    }
    ioopm_hash_table_iterator_destroy(it);
}

static void free_cart(cart_t *cart) {
    free_cart_info(cart);
    ioopm_hash_table_destroy(cart->merchandise);
    free(cart);
}


static void free_cart_list(cart_list_t *cart_list) {
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

void ioopm_create_cart(cart_list_t *cart_list)
{
    cart_t *cart = cart_create();
    cart_list->id_counter++;
    cart->id = cart_list->id_counter;

    ioopm_list_append(cart_list->carts, crt_elem(cart));
}

void remove_cart(cart_t *cart, ioopm_hash_table_t *ht_sl, ioopm_hash_table_t *ht_n) {

    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(cart->merchandise);
    elem_t current_info;
    elem_t current_name;
    s_t current_S;
    elem_t result;

    while(!ioopm_hash_table_iterator_at_end(it)) {

        current_info = ioopm_hash_table_iterator_current_value(it);
        current_name = ioopm_hash_table_iterator_current_key(it);
        size_t current_amount = current_info.cinfo->amount;

       // if (!ioopm_hash_table_lookup(ht_sl, current_info.cinfo->shelf, &result)) {
       //      ioopm_hash_table_insert(ht_sl, current_info.cinfo->shelf, current_name.s);
       // }

        // ht_n available_stock:
        ioopm_hash_table_lookup(ht_n, current_name, &current_S);
        current_S.item->available_stock += current_amount;


        // ht_n locations:
        ioopm_list_iterator_t *list_it = ioopm_list_iterator_create(current_S.locations);
        if (ioopm_list_iterator_at_end(list_it)) {
            //felmeddelande
        }
        // Fylller på den första hyllan i listan med alla items som ska läggas tillbaka?
        elem_t tmp = ioopm_list_iterator_current(list_it);
        loc_pair_t *current_link = tmp.p;
        current_link->stock += current_amount;
        
        /*

        while (!ioopm_list_iterator_at_end(list_it)) {

            if (strcmp(current_link->shelf, current_info.cinfo->shelf) == 0) {
                current_link->stock += current_amount;
                break;
            }

            ioopm_list_iterator_advance(list_it);
            tmp = ioopm_list_iterator_current(list_it);
            current_link = tmp.p;
        }

        if (ioopm_list_iterator_at_end(list_it)) {
            loc_pair_t *link = list_pair_create();
            link->shelf = current_info->shelf;
            link->stock = current_amount;
            ioopm_list_append(current_S->locations, link);
        }
        */
        ioopm_list_iterator_destroy(list_it);
        ioopm_hash_table_iterator_advance(it);
    }

    ioopm_hash_table_iterator_destroy(it);
    free_cart(cart);
}

void add_to_cart(ioopm_hash_table_t *cart, char *name, size_t amount) {
    
}


void remove_from_cart(ioopm_hash_table_t *cart, char *name, size_t amount);


size_t calc_costs(ioopm_hash_table_t *cart);

//Checkout
