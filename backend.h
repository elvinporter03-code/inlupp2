#pragma once
#include "common.h"

/// @brief adds merchandise to db without increasing stock
// Skapar merch_t med stock = 0 och stoppar in det i htn
/// @param ht_n the db operated upon
/// @param name the name of the item
/// @param desc a description of the item
/// @param prize the price of the item
void add_merchandise(ioopm_hash_table_t *ht_n, char *name, char *desc, size_t price);

/// @brief Listar alla items 
// Itererar genom alla items och skriver ut dessa i terminalen i batches om max 20, sedan får användaren välja att fortsätta eller sluta
/// @param ht_n the db operated upon
/// @return Array of strings
char **list_merchandise(ioopm_hash_table_t *ht_n);

/// @brief Removes merchandise completely from db
// Loopar igenom alla locations med itemet och removear det, och sedan från htn också
/// @param ht_n the db operated upon
/// @param ht_sl the db operated upon
/// @param name 
void remove_merchandise(ioopm_hash_table_t *ht_n, ioopm_hash_table_t *ht_sl, char *name);

/// @brief Edits content of merch_t
// Updatera htn
/// @param ht_n 
/// @param name 
/// @param desc 
/// @param price 
void edit_merchandise(ioopm_hash_table_t *ht_n, ioopm_hash_table_t *ht_sl, char *name_old, char *name_new, char *desc, size_t price);

/// @brief Shows contents of locationlist
// iterera över hela listan och printa plats och antal.
/// @param ht_n the db operated upon
/// @param name the merchandise operated upon
void show_stock(ioopm_hash_table_t *ht_n, char *name);

/// @brief 
// Insertar i htsl
// Måste checka om shelf har innehåll redan
// Uppdaterar ht_n för att matcha
/// @param ht_sl 
/// @param ht_n 
/// @param shelf
/// @param name 
void replenish(ioopm_hash_table_t *ht_sl, ioopm_hash_table_t *ht_n, char *name, char *shelf, size_t amount);

/// @brief Förstör allt och friar allt
/// @param ht_n 
/// @param ht_sl 
void destructor(ioopm_hash_table_t *ht_n, ioopm_hash_table_t *ht_sl);

size_t string_knr_hash(elem_t key);
bool string_compare(elem_t str1, elem_t str2);
