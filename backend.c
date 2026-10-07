#include "common.h"
#include "hash_table_iterator.h"
#include "list_iterator.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

static void destroy_loc_pairs(ioopm_list_t *list){
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(list);
    while(!ioopm_list_iterator_at_end(it)){
        elem_t tmp = ioopm_list_iterator_current(it);
        loc_pair_t *tmp_l = tmp.p; 
        free(tmp_l)
        ioopm_list_iterator_advance(it);
    }
    ioopm_list_iterator_destroy(it);
}

static void destroy_entry_htn(ioopm_hash_table_t *ht_n, char *name){
    elem_t result;
    if(ioopm_hash_table_lookup(ht_n, string_elem(name), &result)){
        s_t *to_free = result.p;
        free(to_free->item->desc);
        free(to_free->item->name);
        free(to_free->item);
        destroy_loc_pairs(to_free->locations);
        ioopm_list_destroy(to_free->locations);
        free(to_free);
    }
}

static size_t string_knr_hash(elem_t key)
{
  const char *str = key.s;
  size_t result = 0;
  while (*str != '\0')
  {
    result = result * 31 + ((unsigned char)*str);
    str++;
  }
  return result;
}

static bool string_compare(elem_t str1, elem_t str2)
{
  const char *string1 = str1.s;
  const char *string2 = str2.s;

  return strcmp(string1, string2) == 0;
}


void destructor(ioopm_hash_table_t *htn, ioopm_hash_table_t *htsl){
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(htn);
    while(!ioopm_hash_table_iterator_at_end(it)){
        elem_t current = ioopm_hash_table_iterator_current_value(it);
        s_t *name = current.p;
        char* key = name->item->name;
        destroy_entry_htn(htn, key);
        ioopm_hash_table_iterator_advance(it);
    }
    ioopm_hash_table_destroy(htn);
    ioopm_hash_table_destroy(htsl);
    ioopm_hash_table_iterator_destroy(it);
}

ioopm_list_t *sort(ioopm_list_t *list){
    //STUB
    return list;
}

bool confirmation(){
    char *ans = ask_question_string("Säker? (Y/N) \n");
    to_upper_case(ans);
    if(ans[0] == 'Y'){
        return true;
    }
    return false;
}

// skapar en S:
static s_t *S_create(char *name, char *desc, size_t price) {
    // skapar merch 
    merch_t *merch = calloc(1, sizeof(merch_t));
    merch->name = name;
    merch->desc = desc;
    merch->price = price;

    // skapar S med merch och null
    s_t *S = calloc(1, sizeof(s_t));
    S->item = merch;
    S->locations = ioopm_list_create();
    return S;
}

void add_merchandise(ioopm_hash_table_t *ht_n, char *name, char *desc, size_t price) {
    // skapa en S
    s_t *item = S_create(name, desc, price);

    // lägg in den i ht_n med name som nyckel ger felmeddelande om den redan finns
    bool exists = ioopm_hash_table_has_key(ht_n, string_elem(name));
    if (exists) {
        printf("%s already exists\n", name);
    }
    else {
        ioopm_hash_table_insert(ht_n, string_elem(name), st_elem(item));
    }
}

void list_merchandise(ioopm_hash_table_t *ht_n){
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht_n);
    char *result[ht_n->ht_size];
    int i = 0;
    while(!ioopm_hash_table_iterator_at_end(it)){
        result[i] = it->current_entry->key.s;
        i++;
        ioopm_hash_table_iterator_advance(it);
    }
    int n = 0;
    while(n < i){
        printf("%s \n", result[n]);
        n++;
        if(n % 20 == 0){
            char *ans = ask_question_string("Vill du fortsätta? (Y/N)\n");
            to_upper_case(ans);
            if(ans[0] != 'Y'){
                return;
            }
        }
    }
    ioopm_hash_table_iterator_destroy(it);
}

static s_t *lookup_htn(ioopm_hash_table_t *ht_n, char *name){
    elem_t item;
    ioopm_hash_table_lookup(ht_n, string_elem(name), &item);
    return item.p;
}

static loc_pair_t *list_fetch(ioopm_list_iterator_t *it){
    elem_t current = ioopm_list_iterator_current(it);
    loc_pair_t *tmp = current.p;
    return tmp;
}



void remove_merchandise(ioopm_hash_table_t *ht_n, ioopm_hash_table_t *ht_sl, char *name){
    elem_t tmp;

    s_t *item_s = lookup_htn(ht_n, name);
    ioopm_list_t *l = item_s->locations;
    if(!ioopm_list_is_empty(l)){
        ioopm_list_iterator_t *it = ioopm_list_iterator_create(l);
        while(!ioopm_list_iterator_at_end(it)){
            ioopm_hash_table_remove(ht_sl, ioopm_list_iterator_current(it), &tmp);
            ioopm_list_iterator_advance(it);
        }
        ioopm_list_iterator_destroy(it);
    }
    destroy_entry_htn(ht_n, name);
    ioopm_hash_table_remove(ht_n, string_elem(name), &tmp);
}


void edit_merchandise(ioopm_hash_table_t *ht_n, ioopm_hash_table_t *ht_sl, char *name_old, char *name_new, char *desc, size_t price){

    // Hämta all tidigare info om itemet
    s_t *item_s = lookup_htn(ht_n, name_old);

    // skapar ny(?) info om  itemet
    merch_t *tmp = calloc(1, sizeof(merch_t));
    tmp->desc = desc;
    tmp->name = name_new;
    tmp->price = price; 
    tmp->stock = item_s->item->stock;

    // skapar nya itemet
    ioopm_list_t *locs = item_s->locations;
    s_t *to_insert = calloc(1, sizeof(s_t));
    to_insert->item = tmp;
    to_insert->locations = locs;

    if(!string_compare(string_elem(name_new), string_elem(name_old))){
        // tar bort alla instanser av den gamla varan utifall namnet ändrats
        elem_t tmp;
        ioopm_hash_table_remove(ht_n, string_elem(name_old), &tmp);
        if(!ioopm_list_is_empty(locs)){
            // Uppdaterar varje shelf med den gamla varan med den nya utifall namnet ändrats
            ioopm_list_iterator_t *it = ioopm_list_iterator_create(locs); 
            while(!ioopm_list_iterator_at_end(it)){
                ioopm_hash_table_insert(ht_sl, ioopm_list_iterator_current(it), string_elem(name_new));
                ioopm_list_iterator_advance(it);
            }
            ioopm_list_iterator_destroy(it);
        }
    }
    // Uppdaterar eller sätter in nya beroende på om namnet ändrats
    ioopm_hash_table_insert(ht_n, string_elem(name_new), ptr_elem(to_insert)); 

}

void show_stock(ioopm_hash_table_t *ht_n, char *name){
    s_t *item_s = lookup_htn(ht_n, name);
    if(item_s->item->stock != 0){
        ioopm_list_iterator_t *it = ioopm_list_iterator_create(sort(item_s->locations));
        while(!ioopm_list_iterator_at_end(it)){
            loc_pair_t *current = list_fetch(it);
            printf("Hylla: %s innehåller %ld %s \n", current->shelf, current->stock, name);
            ioopm_list_iterator_advance(it);
        }
        ioopm_list_iterator_destroy(it);
    }
}

void replenish(ioopm_hash_table_t *ht_sl, ioopm_hash_table_t *ht_n, char *name, char *shelf, size_t amount){
    ioopm_hash_table_insert(ht_sl, string_elem(shelf), string_elem(name));
    s_t *item = lookup_htn(ht_n, name);
    loc_pair_t *tmp = calloc(1, sizeof(loc_pair_t));
    tmp->shelf = shelf;
    item->item->stock += amount;
    tmp->stock = amount;
    item->item->available_stock += amount;
    ioopm_list_insert(item->locations, item->locations->size, ptr_elem(tmp));
}

ioopm_hash_table_t *create_cart();
// CART:

static cart_t *cart_create() {
    cart_t *cart = calloc(1, sizeof(cart_t));
    cart->merchandise = ioopm_hash_table_create(string_knr_hash, string_compare);
    return cart;
}

static cart_list_t *cart_list_create() {
    cart_list_t *cart_list = calloc(1, sizeof(cart_list_t));
    cart_list->carts = ioopm_list_create();
    return cart_list;
}

void ioopm_create_cart(cart_list_t *cart_list){
    cart_t *cart = cart_create();
    cart_list->id_counter++;
    cart->id = cart_list->id_counter;

    ioopm_list_append(cart_list->carts, crt_elem(cart));
}

void remove_cart(ioopm_hash_table_t *cart);
void add_to_cart(ioopm_hash_table_t *cart, char *name, size_t amount);
void remove_from_cart(ioopm_hash_table_t *cart, char *name, size_t amount);
size_t calc_costs(ioopm_hash_table_t *cart);


// MAIN


void main_loop(ioopm_hash_table_t *ht_sl, ioopm_hash_table_t *ht_n){
    char *name;
    char *desc;
    char *shelf;
    bool running = true;
    while(running){
        char *ans = ask_question_string("Välj ett menyalternativ \n");
        printf("%s \n", ans);
            to_upper_case(ans);
            switch(ans[0]){
                case 'A': 
                    puts("Du tryckte a");
                    char *name = ask_question_string("Vilket item vill du lägga till? \n");
                    desc = ask_question_string("Description? \n");
                    size_t price = ask_question_int("Hur mycket kostar itemet? \n");
                    add_merchandise(ht_n, name, desc, price);
                    break;

                case 'L':
                    list_merchandise(ht_n);
                    break;

                case 'D':
                    name = ask_question_string("Vilket item vill du ta bort? \n");
                    if(confirmation()) remove_merchandise(ht_n, ht_sl, name);
                    break;

                case 'E':
                    char *name_old = ask_question_string("Vilket item vill du ta ändra? \n");
                    char *name_new = ask_question_string("Nytt namn? \n");
                    desc = ask_question_string("Ny description? \n");
                    size_t prize = ask_question_int("Hur mycket kostar itemet? \n");
                    if(confirmation()) edit_merchandise(ht_n, ht_sl, name_old, name_new, desc, prize);
                break;

                case 'S':
                    name = ask_question_string("Vilket item vill du visa stock för? \n");
                    show_stock(ht_n, name);
                break;

                case 'P':
                    shelf = ask_question_shelf("Vilken hylla vill du lägga till på \n");
                    name = ask_question_string("Vilket item vill du lägga till fler av? \n");
                    size_t amount = ask_question_int("Hur många vill du fylla på med? \n");
                    replenish(ht_sl, ht_n, name, shelf, amount);
                break;

                case 'C':
                break;

                case 'R':
                break;

                case '+':
                break;

                case '-':
                break;

                case '=':
                break;

                case 'O':
                break;
                
                case 'Q':
                    running = false;
                break;

                default:
                    running = false;
                break;
            }
    }
    (void)name;
}

int main(){
    ioopm_hash_table_t *ht_n = ioopm_hash_table_create(string_knr_hash, string_compare);
    ioopm_hash_table_t *ht_sl = ioopm_hash_table_create(string_knr_hash, string_compare);

    main_loop(ht_n, ht_sl); 

    destructor(ht_n, ht_sl); //shoppingcarts
    return 0;
}