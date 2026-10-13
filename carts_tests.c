#include <CUnit/Basic.h>
#include <stdlib.h>
#include "backend.h"
#include "carts.h"
#include "hash_table.h"


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

void test_add() {
    cart_list_t *cart_list = cart_list_create();
    cart_t *cart = ioopm_create_cart(cart_list);
    ioopm_hash_table_t *ht_n = init();
    ioopm_hash_table_t *ht_sl = ioopm_hash_table_create(string_knr_hash, string_compare);
    

    char *name = "Bok";
    size_t amount = 10;

    char *name2 = "Saft";
    size_t amount2 = 5;

    size_t amount3 = 2;
    size_t amount4 = 3;

    ioopm_add_to_cart(cart, ht_n, name, amount);
    int cost1 = ioopm_calc_costs(cart, ht_n);
    CU_ASSERT_EQUAL(cost1, 150);

    ioopm_add_to_cart(cart, ht_n, name2, amount2);
    int cost2 = ioopm_calc_costs(cart, ht_n);
    CU_ASSERT_EQUAL(cost2, 250);

    ioopm_add_to_cart(cart, ht_n, name, amount3);
    int cost3 = ioopm_calc_costs(cart, ht_n);
    CU_ASSERT_EQUAL(cost3, 280);

    ioopm_add_to_cart(cart, ht_n, name2, amount4);
    int cost4 = ioopm_calc_costs(cart, ht_n);
    CU_ASSERT_EQUAL(cost3, cost4);

    ioopm_add_to_cart(cart, ht_n, name2, 0);
    int cost5 = ioopm_calc_costs(cart, ht_n);
    CU_ASSERT_EQUAL(cost3, cost5);

    //ioopm_remove_cart(cart_list, 1, ht_n);
    free_cart_list(cart_list);

    destructor(ht_n, ht_sl);
}


void test_remove() {
    cart_list_t *cart_list = cart_list_create();
    cart_t *cart = ioopm_create_cart(cart_list);
    ioopm_hash_table_t *ht_n = init();
    ioopm_hash_table_t *ht_sl = ioopm_hash_table_create(string_knr_hash, string_compare);
    

    char *name = "Bok";
    size_t amount = 10;

    char *name2 = "Saft";
    size_t amount2 = 5;


    ioopm_add_to_cart(cart, ht_n, name, amount);
    int cost1 = ioopm_calc_costs(cart, ht_n);
    CU_ASSERT_EQUAL(cost1, 150);

    ioopm_add_to_cart(cart, ht_n, name2, amount2);
    int cost2 = ioopm_calc_costs(cart, ht_n);
    CU_ASSERT_EQUAL(cost2, 250);

    ioopm_remove_from_cart(cart, name, 5, ht_n);
    int cost3 = ioopm_calc_costs(cart, ht_n);
    CU_ASSERT_EQUAL(cost3, 175);

    ioopm_remove_from_cart(cart, name2, 5, ht_n);
    int cost4 = ioopm_calc_costs(cart, ht_n);
    CU_ASSERT_EQUAL(cost4, 75);

    ioopm_remove_from_cart(cart, name, 7, ht_n);
    int cost5 = ioopm_calc_costs(cart, ht_n);
    CU_ASSERT_EQUAL(cost5, 75);

    ioopm_remove_from_cart(cart, name, 0, ht_n);
    int cost6 = ioopm_calc_costs(cart, ht_n);
    CU_ASSERT_EQUAL(cost6, 75);

    //int total_cost = ioopm_checkout(cart_list, 1, ht_n);
    //CU_ASSERT_EQUAL(cost6, total_cost);
    //printf("%i", total_cost);

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