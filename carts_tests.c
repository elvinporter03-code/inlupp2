
/*

#include <CUnit/Basic.h>
#include <stdlib.h>
#include "backend.h"
#include "carts.h"
#include "hash_table.h"
#include "linked_list.h"


ioopm_hash_table_t *init() {
    ioopm_hash_table_t *ht_n = ioopm_hash_table_create(string_knr_hash, string_compare);
    ioopm_hash_table_t *ht_sl = ioopm_hash_table_create(string_knr_hash, string_compare);
    char *name = "Bok";
    char *desc = "En intressant bok";
    char *shelf = "A12";
    size_t price = 15;
    size_t amount = 12;
    add_merchandise(ht_n, name, desc, price);

    char *name2 = "Saft";
    char *desc2 = "Blå saft";
    char *shelf21 = "B5";
    char *shelf22 = "C11";
    size_t price2 = 20;
    size_t amount21 = 3;
    size_t amount22 = 4;

    add_merchandise(ht_n, name2, desc2, price2);

    replenish(ht_sl, ht_n, name, shelf, amount);
    replenish(ht_sl, ht_n, name2, shelf21, amount21);
    replenish(ht_sl, ht_n, name2, shelf22, amount22);

    ioopm_hash_table_destroy(ht_sl);

    return ht_n;
}

void test_create_destroy() {

    ioopm_hash_table_t *ht_n = init();
    ioopm_hash_table_t *ht_sl = ioopm_hash_table_create(string_knr_hash, string_compare);

    cart_list_t *cart_list = cart_list_create();
    CU_ASSERT_TRUE(ioopm_list_is_empty(cart_list->carts));

    // ta bort en tom cart
    ioopm_create_cart(cart_list);
    CU_ASSERT_EQUAL(ioopm_list_size(cart_list->carts), 1);
    ioopm_remove_cart(cart_list, 1, ht_n);
    CU_ASSERT_TRUE(ioopm_list_is_empty(cart_list->carts));
    
    // ta bort en cart som inte finns
    ioopm_remove_cart(cart_list, 4, ht_n);
    CU_ASSERT_TRUE(ioopm_list_is_empty(cart_list->carts));

    // lägg in en cart i en tom lista
    cart_t *cart2 = ioopm_create_cart(cart_list);
    CU_ASSERT_EQUAL(ioopm_list_size(cart_list->carts), 1);
    CU_ASSERT_EQUAL(cart2->id, 2);

    // lägg till en cart i en icke tom lista
    cart_t *cart3 = ioopm_create_cart(cart_list);
    CU_ASSERT_EQUAL(ioopm_list_size(cart_list->carts), 2);

    char *name = "Bok";

    // lägg till i cart
    ioopm_add_to_cart(cart3, name, 10, ht_n);
    
    // ta bort en icke tom cart
    ioopm_remove_cart(cart_list, 3, ht_n);
    CU_ASSERT_EQUAL(ioopm_list_size(cart_list->carts), 1);


    free_cart_list(cart_list);
    destructor(ht_n, ht_sl);

}

void test_add() {
    cart_list_t *cart_list = cart_list_create();
    cart_t *cart = ioopm_create_cart(cart_list);
    ioopm_hash_table_t *ht_n = init();
    ioopm_hash_table_t *ht_sl = ioopm_hash_table_create(string_knr_hash, string_compare);
    

    char *name = "Bok";
    char *name2 = "Saft";
    char *name3 = "Äpple";

    // lägg till på tom
    ioopm_add_to_cart(cart, name, 10, ht_n);
    int cost1 = ioopm_calc_costs(cart, ht_n);
    CU_ASSERT_EQUAL(cost1, 150);

    // lägg till på icke tom
    ioopm_add_to_cart(cart, name2, 5, ht_n);
    int cost2 = ioopm_calc_costs(cart, ht_n);
    CU_ASSERT_EQUAL(cost2, 250);

    // fyll på en och använd hela availible stock
    ioopm_add_to_cart(cart, name, 2, ht_n);
    int cost3 = ioopm_calc_costs(cart, ht_n);
    CU_ASSERT_EQUAL(cost3, 280);

    // lägg till fler än vad som finns i stock
    ioopm_add_to_cart(cart, name2, 3, ht_n);
    int cost4 = ioopm_calc_costs(cart, ht_n);
    CU_ASSERT_EQUAL(cost3, cost4);

    // lägg till 0
    ioopm_add_to_cart(cart, name2, 0, ht_n);
    int cost5 = ioopm_calc_costs(cart, ht_n);
    CU_ASSERT_EQUAL(cost3, cost5);

    // en som inte finnt i ht_n
    ioopm_add_to_cart(cart, name3, 5, ht_n);
    CU_ASSERT_EQUAL(ioopm_calc_costs(cart, ht_n), cost3);

    ioopm_remove_cart(cart_list, 1, ht_n);
    free_cart_list(cart_list);

    destructor(ht_n, ht_sl);
}

void test_remove() {
    cart_list_t *cart_list = cart_list_create();
    cart_t *cart = ioopm_create_cart(cart_list);
    ioopm_hash_table_t *ht_n = init();
    ioopm_hash_table_t *ht_sl = ioopm_hash_table_create(string_knr_hash, string_compare);
    

    char *name = "Bok";
    size_t amount = 11;

    char *name2 = "Saft";
    size_t amount2 = 5;

    char *name3 = "Äpple";


    ioopm_add_to_cart(cart, name, amount, ht_n);
    int cost1 = ioopm_calc_costs(cart, ht_n);
    CU_ASSERT_EQUAL(cost1, 165);

    // med en i 
    ioopm_remove_from_cart(cart, name, 1, ht_n);
    CU_ASSERT_EQUAL(ioopm_calc_costs(cart, ht_n), 150);

    ioopm_add_to_cart(cart, name2, amount2, ht_n);
    int cost2 = ioopm_calc_costs(cart, ht_n);
    CU_ASSERT_EQUAL(cost2, 250);

    // med flera i
    ioopm_remove_from_cart(cart, name, 5, ht_n);
    int cost3 = ioopm_calc_costs(cart, ht_n);
    CU_ASSERT_EQUAL(cost3, 175);

    // ta bort en helt
    ioopm_remove_from_cart(cart, name2, 5, ht_n);
    int cost4 = ioopm_calc_costs(cart, ht_n);
    CU_ASSERT_EQUAL(cost4, 75);

    // ta bort för många
    ioopm_remove_from_cart(cart, name, 7, ht_n);
    int cost5 = ioopm_calc_costs(cart, ht_n);
    CU_ASSERT_EQUAL(cost5, 75);

    // ta bort 0 st
    ioopm_remove_from_cart(cart, name, 0, ht_n);
    int cost6 = ioopm_calc_costs(cart, ht_n);
    CU_ASSERT_EQUAL(cost6, 75);

    // ta bort en som redan tagits bort från cart
    ioopm_remove_from_cart(cart, name2, 6, ht_n);
    CU_ASSERT_EQUAL(ioopm_calc_costs(cart, ht_n), 75);

    // ta bort en som inte finns i cart
    ioopm_remove_from_cart(cart, name3, 5, ht_n);
    CU_ASSERT_EQUAL(ioopm_calc_costs(cart, ht_n), 75);



    int total_cost = ioopm_checkout(cart_list, 1, ht_n);
    CU_ASSERT_EQUAL(cost6, total_cost);
    printf("%i", total_cost);

    free_cart_list(cart_list);
    destructor(ht_n, ht_sl);
}

void test_checkout() {

    ioopm_hash_table_t *ht_n = init();
    ioopm_hash_table_t *ht_sl = ioopm_hash_table_create(string_knr_hash, string_compare);
    cart_list_t *cart_list = cart_list_create();

    // kod 

    // på icke existerande kundvagn
    int cost = ioopm_checkout(cart_list, 1, ht_n);
    CU_ASSERT_EQUAL(cost, -2);

    // lägg in två, fyll name2 i första och lägg in name1 i den också. låt den andra vara tom.
    cart_t *cart1 = ioopm_create_cart(cart_list);
    ioopm_create_cart(cart_list);

    char *name = "Bok";
    char *name2 = "Saft";

    ioopm_add_to_cart(cart1, name, 4, ht_n);
    ioopm_add_to_cart(cart1, name2, 7, ht_n);

    // ta bort den med merch i, ska ge ett tal, kolla så att listsize är 1
    cost = ioopm_checkout(cart_list, 1, ht_n);
    CU_ASSERT_EQUAL(cost, 200);
    CU_ASSERT_EQUAL(ioopm_list_size(cart_list->carts), 1);

    // ta bort den sista och kolla så is empty stämmer och att det är 0
    cost = ioopm_checkout(cart_list, 2, ht_n);
    CU_ASSERT_EQUAL(cost, 0);
    CU_ASSERT_TRUE(ioopm_list_is_empty(cart_list->carts));


    free_cart_list(cart_list);
    destructor(ht_n, ht_sl);
}
*/



#include <CUnit/Basic.h>
#include <stdlib.h>
#include <stdio.h>
#include "backend.h"
#include "carts.h"
#include "hash_table.h"
#include "linked_list.h"

ioopm_hash_table_t *init()
{
    ioopm_hash_table_t *ht_n =
        ioopm_hash_table_create(string_knr_hash, string_compare);
    ioopm_hash_table_t *ht_sl =
        ioopm_hash_table_create(string_knr_hash, string_compare);

    char *name = "Bok";
    char *desc = "En intressant bok";
    char *shelf = "A12";
    size_t price = 15;
    size_t amount = 12;

    add_merchandise(ht_n, name, desc, price);

    char *name2 = "Saft";
    char *desc2 = "Blå saft";
    char *shelf21 = "B5";
    char *shelf22 = "C11";
    size_t price2 = 20;
    size_t amount21 = 3;
    size_t amount22 = 4;

    add_merchandise(ht_n, name2, desc2, price2);

    replenish(ht_sl, ht_n, name, shelf, amount);
    replenish(ht_sl, ht_n, name2, shelf21, amount21);
    replenish(ht_sl, ht_n, name2, shelf22, amount22);

    ioopm_hash_table_destroy(ht_sl);

    return ht_n;
}

void test_create_destroy()
{
    ioopm_hash_table_t *ht_n = init();
    ioopm_hash_table_t *ht_sl =
        ioopm_hash_table_create(string_knr_hash, string_compare);

    cart_list_t *cart_list = cart_list_create();

    CU_ASSERT_TRUE(ioopm_list_is_empty(cart_list->carts));

    // Ta bort en tom cart
    ioopm_create_cart(cart_list);
    CU_ASSERT_EQUAL(ioopm_list_size(cart_list->carts), 1);

    ioopm_remove_cart(cart_list, 1, ht_n);
    CU_ASSERT_TRUE(ioopm_list_is_empty(cart_list->carts));

    // Ta bort en cart som inte finns
    ioopm_remove_cart(cart_list, 4, ht_n);
    CU_ASSERT_TRUE(ioopm_list_is_empty(cart_list->carts));

    // Lägg in en cart i en tom lista.
    // ID 2 eftersom ID 1 redan har använts.
    ioopm_create_cart(cart_list);
    CU_ASSERT_EQUAL(ioopm_list_size(cart_list->carts), 1);

    // Lägg till en cart i en icke tom lista.
    // Den får ID 3.
    ioopm_create_cart(cart_list);
    CU_ASSERT_EQUAL(ioopm_list_size(cart_list->carts), 2);

    char *name = "Bok";

    // Lägg till i cart med ID 3
    ioopm_add_to_cart(cart_list, 3, name, 10, ht_n);

    // Ta bort en icke tom cart
    ioopm_remove_cart(cart_list, 3, ht_n);
    CU_ASSERT_EQUAL(ioopm_list_size(cart_list->carts), 1);

    free_cart_list(cart_list);
    destructor(ht_n, ht_sl);
}

void test_add()
{
    cart_list_t *cart_list = cart_list_create();
    ioopm_hash_table_t *ht_n = init();
    ioopm_hash_table_t *ht_sl =
        ioopm_hash_table_create(string_knr_hash, string_compare);

    // Den första kundvagnen får ID 1.
    ioopm_create_cart(cart_list);

    char *name = "Bok";
    char *name2 = "Saft";
    char *name3 = "Äpple";

    // Lägg till i en tom cart
    ioopm_add_to_cart(cart_list, 1, name, 10, ht_n);
    int cost1 = ioopm_calc_costs(cart_list, 1, ht_n);
    CU_ASSERT_EQUAL(cost1, 150);

    // Lägg till i en icke tom cart
    ioopm_add_to_cart(cart_list, 1, name2, 5, ht_n);
    int cost2 = ioopm_calc_costs(cart_list, 1, ht_n);
    CU_ASSERT_EQUAL(cost2, 250);

    // Fyll på och använd hela tillgängliga lagret
    ioopm_add_to_cart(cart_list, 1, name, 2, ht_n);
    int cost3 = ioopm_calc_costs(cart_list, 1, ht_n);
    CU_ASSERT_EQUAL(cost3, 280);

    // Lägg till fler än vad som finns i lager
    ioopm_add_to_cart(cart_list, 1, name2, 3, ht_n);
    int cost4 = ioopm_calc_costs(cart_list, 1, ht_n);
    CU_ASSERT_EQUAL(cost3, cost4);

    // Lägg till 0
    ioopm_add_to_cart(cart_list, 1, name2, 0, ht_n);
    int cost5 = ioopm_calc_costs(cart_list, 1, ht_n);
    CU_ASSERT_EQUAL(cost3, cost5);

    // En produkt som inte finns i ht_n
    ioopm_add_to_cart(cart_list, 1, name3, 5, ht_n);
    CU_ASSERT_EQUAL(ioopm_calc_costs(cart_list, 1, ht_n), cost3);

    ioopm_remove_cart(cart_list, 1, ht_n);
    free_cart_list(cart_list);

    destructor(ht_n, ht_sl);
}

void test_remove()
{
    cart_list_t *cart_list = cart_list_create();
    ioopm_hash_table_t *ht_n = init();
    ioopm_hash_table_t *ht_sl =
        ioopm_hash_table_create(string_knr_hash, string_compare);

    // Den första kundvagnen får ID 1.
    ioopm_create_cart(cart_list);

    char *name = "Bok";
    size_t amount = 11;

    char *name2 = "Saft";
    size_t amount2 = 5;

    char *name3 = "Äpple";

    ioopm_add_to_cart(cart_list, 1, name, amount, ht_n);
    int cost1 = ioopm_calc_costs(cart_list, 1, ht_n);
    CU_ASSERT_EQUAL(cost1, 165);

    // Ta bort en produkt
    ioopm_remove_from_cart(cart_list, 1, name, 1, ht_n);
    CU_ASSERT_EQUAL(ioopm_calc_costs(cart_list, 1, ht_n), 150);

    ioopm_add_to_cart(cart_list, 1, name2, amount2, ht_n);
    int cost2 = ioopm_calc_costs(cart_list, 1, ht_n);
    CU_ASSERT_EQUAL(cost2, 250);

    // Ta bort flera produkter
    ioopm_remove_from_cart(cart_list, 1, name, 5, ht_n);
    int cost3 = ioopm_calc_costs(cart_list, 1, ht_n);
    CU_ASSERT_EQUAL(cost3, 175);

    // Ta bort en produkt helt
    ioopm_remove_from_cart(cart_list, 1, name2, 5, ht_n);
    int cost4 = ioopm_calc_costs(cart_list, 1, ht_n);
    CU_ASSERT_EQUAL(cost4, 75);

    // Försök ta bort för många
    ioopm_remove_from_cart(cart_list, 1, name, 7, ht_n);
    int cost5 = ioopm_calc_costs(cart_list, 1, ht_n);
    CU_ASSERT_EQUAL(cost5, 75);

    // Ta bort 0 stycken
    ioopm_remove_from_cart(cart_list, 1, name, 0, ht_n);
    int cost6 = ioopm_calc_costs(cart_list, 1, ht_n);
    CU_ASSERT_EQUAL(cost6, 75);

    // Ta bort en produkt som redan tagits bort från cart
    ioopm_remove_from_cart(cart_list, 1, name2, 6, ht_n);
    CU_ASSERT_EQUAL(ioopm_calc_costs(cart_list, 1, ht_n), 75);

    // Ta bort en produkt som inte finns i cart
    ioopm_remove_from_cart(cart_list, 1, name3, 5, ht_n);
    CU_ASSERT_EQUAL(ioopm_calc_costs(cart_list, 1, ht_n), 75);

    int total_cost = ioopm_checkout(cart_list, 1, ht_n);
    CU_ASSERT_EQUAL(cost6, total_cost);
    printf("%i\n", total_cost);

    free_cart_list(cart_list);
    destructor(ht_n, ht_sl);
}

void test_checkout()
{
    ioopm_hash_table_t *ht_n = init();
    ioopm_hash_table_t *ht_sl =
        ioopm_hash_table_create(string_knr_hash, string_compare);
    cart_list_t *cart_list = cart_list_create();

    // Checkout av en kundvagn som inte finns
    int cost = ioopm_checkout(cart_list, 1, ht_n);
    CU_ASSERT_EQUAL(cost, -2);

    // Skapa två kundvagnar med ID 1 och 2
    ioopm_create_cart(cart_list);
    ioopm_create_cart(cart_list);

    char *name = "Bok";
    char *name2 = "Saft";

    ioopm_add_to_cart(cart_list, 1, name, 4, ht_n);
    ioopm_add_to_cart(cart_list, 1, name2, 7, ht_n);

    // Checka ut kundvagn 1
    cost = ioopm_checkout(cart_list, 1, ht_n);
    CU_ASSERT_EQUAL(cost, 200);
    CU_ASSERT_EQUAL(ioopm_list_size(cart_list->carts), 1);

    // Checka ut kundvagn 2.
    // Dess ID är fortfarande 2, trots att kundvagn 1 tagits bort.
    cost = ioopm_checkout(cart_list, 2, ht_n);
    CU_ASSERT_EQUAL(cost, 0);
    CU_ASSERT_TRUE(ioopm_list_is_empty(cart_list->carts));

    free_cart_list(cart_list);
    destructor(ht_n, ht_sl);
}


int main()
{
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    CU_pSuite my_test_suite =
        CU_add_suite("Hash table iterator test suite", NULL, NULL);

    if (my_test_suite == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // TEST CREATE DESTROY:
    if (CU_add_test(my_test_suite,
                    "test create destroy",
                    test_create_destroy) == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // TEST ADD:
    if (CU_add_test(my_test_suite,
                    "test create remove",
                    test_add) == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // TEST REMOVE:
    if (CU_add_test(my_test_suite,
                    "test remove",
                    test_remove) == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return CU_get_error();
}