#pragma once
#include "common.h"


cart_list_t *cart_list_create();

void free_cart_list(cart_list_t *cart_list);

/// @brief creates a cart with an unique ID and adds it to cart_list
/// @param cart_list the list cart is added to
cart_t *ioopm_create_cart(cart_list_t *cart_list);

/// @brief removes a cart from cart_list and returns availible stock
/// @param cart_list 
/// @param cart_id 
/// @param ht_n 
void ioopm_remove_cart(cart_list_t *cart_list, size_t cart_id, ioopm_hash_table_t *ht_n);

/// @brief 
/// @param cart 
/// @param ht_n 
/// @param name 
/// @param amount 
void ioopm_add_to_cart(cart_t *cart, ioopm_hash_table_t *ht_n, char *name, size_t amount);

/// @brief 
/// @param cart 
/// @param name 
/// @param amount 
/// @param ht_n 
void ioopm_remove_from_cart(cart_t *cart, char *name, size_t amount, ioopm_hash_table_t *ht_n);

/// @brief 
/// @param cart 
/// @return 
int ioopm_calc_costs(cart_t *cart, ioopm_hash_table_t *ht_n);

/// @brief 
/// @param cart_list 
/// @param cart_id 
/// @param ht_n 
int ioopm_checkout(cart_list_t *cart_list, size_t cart_id, ioopm_hash_table_t *ht_n);