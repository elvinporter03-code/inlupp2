#pragma once
#include "common.h"

/// @brief adds merchandise to db without increasing stock
// Skapar merch_t med stock = 0 och stoppar in det i htn
/// @param ht_n the db operated upon
/// @param name the name of the item
/// @param desc a description of the item
/// @param prize the price of the item
void add_merchandise(ioopm_hash_table_t *ht_n, char *name, char *desc, size_t prize);

/// @brief Listar alla items 
// Itererar genom alla items och skriver ut dessa i terminalen i batches om max 20, sedan får användaren välja att fortsätta eller sluta
/// @param ht_n the db operated upon
void list_merchandise(ioopm_hash_table_t *ht_n);

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
/// @param prize 
void edit_merchandise(ioopm_hash_table_t *ht_n, ioopm_hash_table_t *ht_sl, char *name, char *desc, size_t prize);

/// @brief Shows contents of locationlist
// iterera över hela listan och printa plats och antal.
/// @param ht_n 
/// @param name 
void show_stock(ioopm_hash_table_t *ht_n, char *name);

/// @brief 
// Insertar i htsl
// Måste checka om shelf har innehåll redan
// Uppdaterar ht_n för att matcha
/// @param ht_sl 
/// @param ht_n 
/// @param shelf
/// @param name 
void replenish(ioopm_hash_table_t *ht_sl, ioopm_hash_table_t *ht_n, char *name, char *shelf);

/// @brief Creates an empty shopping cart
/// @return pointer to the cart
ioopm_hash_table_t *create_cart();

/// @brief frees the cart and destroys its content
/// @param cart 
void remove_cart(ioopm_hash_table_t *cart);

/// @brief 
// is the full amount in stock?
// tar bort från db
// lägger till i cart
/// @param cart 
/// @param name 
/// @param amount 
void add_to_cart(ioopm_hash_table_t *cart, char *name, size_t amount);

/// @brief Removes item from cart and puts it in a black hole
// Kolla om mängden finns av itemet
// Remove från cart
/// @param cart 
/// @param name 
/// @param amount 
void remove_from_cart(ioopm_hash_table_t *cart, char *name, size_t amount);

/// @brief Amount * price of every merch_t in cart
/// @param cart 
/// @return total price
size_t calc_costs(ioopm_hash_table_t *cart);

