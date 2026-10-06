#include "common.h"
#include "hash_table_iterator.h"
#include "list_iterator.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

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
    while(!ioopm_list_iterator_at_end(it)){
        elem_t current = ioopm_hash_table_iterator_current_value(it);
        s_t *name = current.p;
        name->item
        destroy_entry_htn(htn, );
        ioopm_list_iterator_advance(it);
    }
    ioopm_hash_table_destroy(htn);
    ioopm_hash_table_destroy(htsl);
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
s_t *S_create(char *name, char *desc, size_t price) {
    // skapar merch 
    merch_t *merch = calloc(1, sizeof(merch_t));
    merch->name = name;
    merch->desc = desc;
    merch->price = price;

    // skapar S med merch och null
    s_t *S = calloc(1, sizeof(s_t));
    S->item = merch;

    return S;
}
/*
// Skapar merch_t med stock = 0 och stoppar in det i htn
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

}*/

void list_merchandise(ioopm_hash_table_t *ht_n){
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht_n);
    char *result[ht_n->ht_size];
    int i = 0;
    while(!ioopm_hash_table_iterator_at_end(it)){
        result[i] = it->current_entry->key.s;
        i++;
    }
    while(i >= 0){
        printf("%s \n", result[i]);
        i--;
        if(i % 20 == 0){
            char *ans = ask_question_string("Vill du fortsätta? (Y/N)\n");
            to_upper_case(ans);
            if(ans[0] != 'Y'){
                return;
            }
        }
    }
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

static void *destroy_entry_htn(ioopm_hash_table_t *ht_n, char *name){
    elem_t result;
    ioopm_hash_table_lookup(ht_n, string_elem(name), &result);
    s_t *to_free = result.p;
    free(to_free->item);
    ioopm_list_destroy(to_free->locations);
}


void remove_merchandise(ioopm_hash_table_t *ht_n, ioopm_hash_table_t *ht_sl, char *name){

    s_t *item_s = lookup_htn(ht_n, name);
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(item_s->locations);
    elem_t tmp;
    while(!ioopm_list_iterator_at_end(it)){
        ioopm_hash_table_remove(ht_sl, ioopm_list_iterator_current(it), &tmp);
        ioopm_list_iterator_advance(it);
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

    if(string_compare(string_elem(name_new), string_elem(name_old))){
        // tar bort alla instanser av den gamla varan utifall namnet ändrats
        elem_t tmp;
        ioopm_hash_table_remove(ht_n, string_elem(name_old), &tmp);

        // Uppdaterar varje shelf med den gamla varan med den nya utifall namnet ändrats
        ioopm_list_iterator_t *it = ioopm_list_iterator_create(locs); 
        while(!ioopm_list_iterator_at_end(it)){
            ioopm_hash_table_insert(ht_sl, ioopm_list_iterator_current(it), string_elem(name_new));
            ioopm_list_iterator_advance(it);
        }
    }
    // Uppdaterar eller sätter in nya beroende på om namnet ändrats
    ioopm_hash_table_insert(ht_n, string_elem(name_new), ptr_elem(to_insert)); 

}

void show_stock(ioopm_hash_table_t *ht_n, char *name){
    s_t *item_s = lookup_htn(ht_n, name);

    ioopm_list_iterator_t *it = ioopm_list_iterator_create(sort(item_s->locations));
    while(!ioopm_list_iterator_at_end(it)){
        loc_pair_t *current = list_fetch(it);
        printf("Hylla: %s innehåller %ld %s \n", current->shelf, current->stock, name);
        ioopm_list_iterator_advance(it);
    }
}

void replenish(ioopm_hash_table_t *ht_sl, ioopm_hash_table_t *ht_n, char *name, char *shelf);
ioopm_hash_table_t *create_cart();
void remove_cart(ioopm_hash_table_t *cart);
void add_to_cart(ioopm_hash_table_t *cart, char *name, size_t amount);
void remove_from_cart(ioopm_hash_table_t *cart, char *name, size_t amount);
size_t calc_costs(ioopm_hash_table_t *cart);

void main_loop(ioopm_hash_table_t *ht_sl, ioopm_hash_table_t *ht_n){
    char *name;
    while(true){
        char *ans = ask_question_string("Välj ett menyalternativ \n");
        printf("%s \n", ans);
            to_upper_case(ans);
            switch(ans[0]){
                case 'A': 
                    puts("Du tryckte a");
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
                    char *desc = ask_question_string("Ny description? \n");
                    size_t prize = ask_question_int("Hur mycket kostar itemet? \n");
                    if(confirmation()) edit_merchandise(ht_n, ht_sl, name_old, name_new, desc, prize);
                break;

                case 'S':
                    name = ask_question_string("Vilket item vill du ta bort? \n");
                    show_stock(ht_n, name);
                break;

                case 'P':
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

                default:
                return;
                break;
            }
    }
}

int main(){
    ioopm_hash_table_t *ht_n = ioopm_hash_table_create(string_knr_hash, string_compare);
    ioopm_hash_table_t *ht_sl = ioopm_hash_table_create(string_knr_hash, string_compare);

    main_loop(ht_n, ht_sl); //MÅNGA MINNESLÄCKOR, INGET ÄR FRIAT PROPERLY

    destructor(ht_n, ht_sl); //shoppingcarts
    return 0;
}