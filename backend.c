#include <stdbool.h>
#include "common.h"
#include "hash_table_iterator.h"
#include "linked_list.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

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

void constructor(ioopm_hash_table_t *htn, ioopm_hash_table_t *htsl){
    htn = ioopm_hash_table_create(string_knr_hash, string_compare);
    htsl = ioopm_hash_table_create(string_knr_hash, string_compare);
}

void destructor(ioopm_hash_table_t *htn, ioopm_hash_table_t *htsl){
    ioopm_hash_table_destroy(htn);
    ioopm_hash_table_destroy(htsl);
}

void add_merchandise(ioopm_hash_table_t *ht_n, char *name, char *desc, size_t prize);
void list_merchandise(ioopm_hash_table_t *ht_n);
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
        printf("fungerar wohoo %s \n", ans);
        if(is_menu_letter(ans)){
            to_upper_case(ans);
            switch(ans[0]){
                case 'A': 
                case 'L':
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
}

int main(){
    ioopm_hash_table_t *ht_n;
    ioopm_hash_table_t *ht_sl;
    constructor(ht_n, ht_sl);

    main_loop(ht_n, ht_sl);

    destructor(ht_n, ht_sl); //shoppingcarts
    return 0;
}