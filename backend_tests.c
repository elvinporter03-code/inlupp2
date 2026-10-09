#include <CUnit/Basic.h>
#include <stdlib.h>
#include "backend.h"
#include "hash_table.h"

int init_suite(void) {
  // Change this function if you want to do something *before* you
  // run a test suite
  return 0;
}

int clean_suite(void) {
  // Change this function if you want to do something *after* you
  // run a test suite
  return 0;
}


void add_remove(void){
  ioopm_hash_table_t *ht_n = ioopm_hash_table_create(string_knr_hash, string_compare);
  ioopm_hash_table_t *ht_sl = ioopm_hash_table_create(string_knr_hash, string_compare);
  char *name = "Bok";
  char *desc = "En intressant bok";
  size_t price = 67;
  add_merchandise(ht_n, name, desc, price);
  CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht_n));
  remove_merchandise(ht_n, ht_sl, name);
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht_n));
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht_sl));
  destructor(ht_n, ht_sl);
}

void list(void){
  ioopm_hash_table_t *ht_n = ioopm_hash_table_create(string_knr_hash, string_compare);
  ioopm_hash_table_t *ht_sl = ioopm_hash_table_create(string_knr_hash, string_compare);
  char *name = "Bok";
  char *desc = "En intressant bok";
  char **results;
  char *expected_results[2];
  expected_results[1]="Bok";
  expected_results[0]="En intressant bok";
  size_t price = 67;
  add_merchandise(ht_n, name, desc, price);
  CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht_n));
  add_merchandise(ht_n, desc, name, price);
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht_n), 2);
  results = list_merchandise(ht_n);
  for(int i = 0; i < 2; i++){
    CU_ASSERT_TRUE(string_compare(string_elem(results[i]), string_elem(expected_results[i])));
  }
  free(results);
  destructor(ht_n, ht_sl);
}

void edit(void){
  ioopm_hash_table_t *ht_n = ioopm_hash_table_create(string_knr_hash, string_compare);
  ioopm_hash_table_t *ht_sl = ioopm_hash_table_create(string_knr_hash, string_compare);
  char *name = "Bok";
  char *desc = "En intressant bok";
  size_t price = 67;
  add_merchandise(ht_n, name, desc, price);
  CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht_n));
  edit_merchandise(ht_n, ht_sl, name, "Bock", desc, price);
  edit_merchandise(ht_n, ht_sl, name, "Bock", "En bockig bock", price);
  edit_merchandise(ht_n, ht_sl, name, "Bock", "En bockig bock", price);
  edit_merchandise(ht_n, ht_sl, name, "Bock", "En bockig bock", 76);
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht_n), 1);
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht_n, string_elem("Bock")));
  //Behöver fler asserts här!!
  destructor(ht_n, ht_sl);
}

void replenish_show_stock(void){
  ioopm_hash_table_t *ht_n = ioopm_hash_table_create(string_knr_hash, string_compare);
  ioopm_hash_table_t *ht_sl = ioopm_hash_table_create(string_knr_hash, string_compare);
  char *name = "Bok";
  char *desc = "En intressant bok";
  size_t price = 67;
  add_merchandise(ht_n, name, desc, price);
  replenish(ht_sl, ht_n, name, "C23", 3);
  replenish(ht_sl, ht_n, name, "D22", 0); // Flera av samma men med invalid siffra, ska ändå lägga till 0 där
  replenish(ht_sl, ht_n, "Hej", "C23", 3); // Invalid element
  CU_ASSERT_EQUAL(ht_sl->ht_size, 2);
  CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht_n));
  remove_merchandise(ht_n, ht_sl, name);
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht_n));
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht_sl));
  destructor(ht_n, ht_sl);
}

int main() {
  // First we try to set up CUnit, and exit if we fail
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  // We then create an empty test suite and specify the name and
  // the init and cleanup functions
  CU_pSuite my_test_suite = CU_add_suite("My awesome test suite", init_suite, clean_suite);
  if (my_test_suite == NULL) {
      // If the test suite could not be added, tear down CUnit and exit
      CU_cleanup_registry();
      return CU_get_error();
  }

  // This is where we add the test functions to our test suite.
  // For each call to CU_add_test we specify the test suite, the
  // name or description of the test, and the function that runs
  // the test in question. If you want to add another test, just
  // copy a line below and change the information'
  if (CU_add_test(my_test_suite, "Lägger till ett item och tar bort det", add_remove) == NULL ||
      CU_add_test(my_test_suite, "Listtest", list) == NULL ||
      CU_add_test(my_test_suite, "Listtest", replenish_show_stock) == NULL)
    {
      // If adding any of the tests fails, we tear down CUnit and exit
      CU_cleanup_registry();
      return CU_get_error();
    }

  // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
  // Use CU_BRM_NORMAL to only print errors and a summary
  CU_basic_set_mode(CU_BRM_NORMAL);
  
  // This is where the tests are actually run!
  CU_basic_run_tests();

  // Tear down CUnit before exiting
  CU_cleanup_registry();
  return CU_get_error();
}
