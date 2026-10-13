#pragma once
#include "common.h"

/// @brief creates an empty list of carts with an id-counter
/// @return the empty list of carts and the id-counter
cart_list_t *cart_list_create();

/// @brief frees a list of carts and all carts and their content
/// @param cart_list the list of carts that is removed
void free_cart_list(cart_list_t *cart_list);

/// @brief creates a cart with an unique ID and adds it to cart_list
/// @param cart_list the list of cart that the cart is added to
cart_t *ioopm_create_cart(cart_list_t *cart_list);

/// @brief removes a cart from cart_list and returns availible stock for all items in the cart
/// @param cart_list the list of carts operated upon
/// @param cart_id the ID of the cart that is removed
/// @param ht_n the merchandise register
void ioopm_remove_cart(cart_list_t *cart_list, size_t cart_id, ioopm_hash_table_t *ht_n);

/// @brief adds a product to the cart or increases the number of the products in the cart
/// @param cart the cart operated upon
/// @param name the name of the product
/// @param amount the number of products added to the cart
/// @param ht_n the merchandise register
void ioopm_add_to_cart(cart_t *cart, char *name, size_t amount, ioopm_hash_table_t *ht_n);

/// @brief removes a product from cart or decreases the number of the products in the cart
/// @param cart the cart operated upon
/// @param name the name of the product
/// @param amount the number of products removed from the cart
/// @param ht_n the merchandise register
void ioopm_remove_from_cart(cart_t *cart, char *name, size_t amount, ioopm_hash_table_t *ht_n);

/// @brief calculates the total price of all products in the cart
/// @param cart the cart operated upon
/// @return the total price
int ioopm_calc_costs(cart_t *cart, ioopm_hash_table_t *ht_n);

/// @brief calculates the total price of all products in the cart and chechs out the cart
/// @param cart_list the list of carts operated upon
/// @param cart_id the ID of the cart that is checked out
/// @param ht_n the merchandise register
int ioopm_checkout(cart_list_t *cart_list, size_t cart_id, ioopm_hash_table_t *ht_n);