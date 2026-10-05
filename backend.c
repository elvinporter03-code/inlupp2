#include <stdbool.h>
#include "common.h"
#include "hash_table_iterator.h"
#include "linked_list.h"
#include "utils.h"
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

void main_loop(){

}

int main(){
    ioopm_hash_table_t *ht_n;
    ioopm_hash_table_t *ht_sl;
    constructor(ht_n, ht_sl);

    main_loop();

    destructor(ht_n, ht_sl); //shoppingcarts
    return 0;
}