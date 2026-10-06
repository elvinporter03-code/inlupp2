#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>
#include <stdbool.h>

#define int_elem(x)   ((elem_t) { .i = (x) })
#define bool_elem(x)  ((elem_t) { .b = (x) })
#define string_elem(x) ((elem_t) { .s = (x) })
#define ptr_elem(x) ((elem_t) { .p = (x) })

typedef struct entry entry_t;
typedef struct hash_table ioopm_hash_table_t;
typedef union element elem_t;
typedef struct merchandise merch_t;
typedef struct S s_t;
typedef bool ioopm_eq_function(elem_t a, elem_t b);
typedef size_t ioopm_hash_function(elem_t key);
typedef struct list ioopm_list_t;
typedef struct list_node ioopm_list_node_t;
typedef struct list_pair loc_pair_t;

union element {
  char *s;
  int i;
  bool b;
  void *p;
};

struct list
{
    ioopm_list_node_t *first;
    ioopm_list_node_t *last;
    size_t size;
};

struct list_node
{
    elem_t head;
    ioopm_list_node_t *tail;
};


struct entry
{
  elem_t key;     // holds the key
  entry_t *next;  // points to the next entry (possibly NULL)
  elem_t value;   // holds value corresponding to key
};

struct hash_table
{
  float load_factor;            // Maximum average number of entries allowed per bucket before resizing
  size_t no_buckets;            // Number of buckets in the hash table
  entry_t **buckets;            // Array of pointers, each pointing to the first entry in a bucket
  size_t ht_size;               // holds the amount of entries for O(1) lookup
  ioopm_hash_function *hash;    // Function to hash the desired kind of key  
  ioopm_eq_function *is_equal;  // Function to check if the desired kind of key is equal to another
};

struct merchandise {
    int stock;
    int price;
    char *desc;
    char *name;
};

struct S {
    merch_t *item;
    ioopm_list_t *locations;
};

struct list_pair {
  char *shelf;
  size_t stock;
};

#endif