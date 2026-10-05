#include "common.h"
#include "hash_table_iterator.h"
#include "linked_list.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct merchandise merch_t;
typedef struct S s_t;

struct merchandise {
    int stock;
    int price;
    char *desc;
    char *name;
};

struct S {
    merch_t item;
    ioopm_list_t locations;
};


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
    ioopm_hash_table_destroy(htn);
    ioopm_hash_table_destroy(htsl);
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

// Skapar merch_t med stock = 0 och stoppar in det i htn
void add_merchandise(ioopm_hash_table_t *ht_n, char *name, char *desc, size_t price) {
    // skapa en S
    s_t *item = S_create(name, desc, price);

    // lägg in den i ht_n med name som nyckel ger felmeddelande om den redan finns
    bool new = ioopm_hash_table_insert(ht_n, name, item); // måste lägga in s_t i elem_t
    if (!new) {
        printf("%s already exists \n", name);
    }

}

void list_merchandise(ioopm_hash_table_t *ht_n){
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht_n);
    char *result[ht_n->ht_size];
    int i = 0;
    while(!ioopm_hash_table_iterator_at_end(it)){
        result[i] = it->current_entry->key.s;
        i++;
    }
    for(int i = 0; i < ht_n->ht_size / 20; i++){
        if(i % 20 == 0 && i > 0){
            char *ans = ask_question_string("Vill du fortsätta lista items? (Y/N)\n");
            to_upper_case(ans);
            if(ans[0] != 'Y'){
                return;
            }
        }
        print
    }
}



void remove_merchandise(ioopm_hash_table_t *ht_n, ioopm_hash_table_t *ht_sl, char *name);
void edit_merchandise(ioopm_hash_table_t *ht_n, char *name, char *desc, size_t prize);
void show_stock(ioopm_hash_table_t *ht_n, char *name);
void replenish(ioopm_hash_table_t *ht_sl, ioopm_hash_table_t *ht_n, char *name, char *shelf);
ioopm_hash_table_t *create_cart();
void remove_cart(ioopm_hash_table_t *cart);
void add_to_cart(ioopm_hash_table_t *cart, char *name, size_t amount);
void remove_from_cart(ioopm_hash_table_t *cart, char *name, size_t amount);
size_t calc_costs(ioopm_hash_table_t *cart);


void main_loop(ioopm_hash_table_t *ht_sl, ioopm_hash_table_t *ht_n){
    while(true){
        char *ans = ask_question_string("Välj ett menyalternativ \n");
        printf("fungerar whoo %s \n", ans);
            to_upper_case(ans);
            switch(ans[0]){
                case 'A': 
                case 'L':
                char **merch = list_merchandise(ht_n);
                case 'D':
                case 'E':
                case 'S':
                case 'P':
                case 'C':
                case 'R':
                case '+':
                case '-':
                case '=':
                case 'O':
            }
    }
}

int main(){
    ioopm_hash_table_t *ht_n = ioopm_hash_table_create(string_knr_hash, string_compare);
    ioopm_hash_table_t *ht_sl = ioopm_hash_table_create(string_knr_hash, string_compare);

    main_loop(ht_n, ht_sl);

    destructor(ht_n, ht_sl); //shoppingcarts
    return 0;
}