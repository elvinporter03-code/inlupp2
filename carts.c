#include "common.h"
#include "hash_table_iterator.h"
#include "list_iterator.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

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

void remove_cart(ioopm_hash_table_t *cart, ioopm_hash_table_t *ht_sl, ioopm_hash_table_t *ht_n) {

    // i whileloop gör
    // plocka ut cinfo
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(cart->merchandise);
    elem_t current_info;
    elem_t current_name;
    elem_t current_S;
    elem_t result;

    while(!ioopm_hash_table_iterator_at_end(it)) {
        current_info = ioopm_hash_table_iterator_current_value(it);
        current_name = ioopm_hash_table_iterator_current_key(it);
        current_amount = current_merch.cinfo->amount;
        if (!ioopm_hash_table_lookup(ht_sl, current_info.cinfo->shelf, &result)) {
             ioopm_hash_table_insert(ht_sl, current_info.cinfo->shelf, current_name.s);
        }

        // hantera ht_n

        // hämta current_s från htn mha current_name
        current_S = ioopm_hash_table_lookup(ht_n, current_name.s, &current_S);

        // sätt current_s.st->merchandise->available_stock += current_amount;
        current_S->merchandise->available_stock += current_amount;

        // skapa iterator för current_s->locations 

        ioopm_list_iterator_t *list_it = ioopm_list_iterator_create(current_S->locations);
        // gå igenom listan tills vi hittar rätt shelf
        elem_t tmp = ioopm_list_iterator_current(list_it);
        loc_pair_t *current_link = tmp.p;

        while(strcmp(current_link>shel-f, current_info->shelf) != 0)
        // sätt dess stock += current_amount.

        // kör iterator advance

        

        //ioopm_hash_table_insert(ht_sl, current_merch.cinfo->shelf, current_amount);

        // hantera ht_sl:

    }

    // använd shelf som key och amount som value
    // inserta i shelf

    // plocka ut S 
    // kör insert på S->locations
    // kör add på S->merch->av_stock

    // ta namnet och 
    // freea cart mha free_cart_list
}

void add_to_cart(ioopm_hash_table_t *cart, char *name, size_t amount);
void remove_from_cart(ioopm_hash_table_t *cart, char *name, size_t amount);
size_t calc_costs(ioopm_hash_table_t *cart);

