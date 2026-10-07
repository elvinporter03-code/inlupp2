#include <CUnit/Basic.h>
#include "backend.h"

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

void add_remove(void){
  ioopm_hash_table_t *ht_n = ioopm_hash_table_create(string_knr_hash, string_compare);
  ioopm_hash_table_t *ht_sl = ioopm_hash_table_create(string_knr_hash, string_compare);
  char *name = "Bok";
  char *desc = "En intressant bok";
  size_t price = 67;
  add_merchandise(ht_n, name, desc, price);
  remove_merchandise(ht_n, ht_sl, name);
  
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
  if (CU_add_test(my_test_suite, "Lägger till ett item och tar bort det", add_remove) == NULL)
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
