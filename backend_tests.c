#include <CUnit/Basic.h>
#include <stdlib.h>
#include <string.h>
#include "backend.h"
#include "hash_table.h"

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

/* ---------- Hjälpfunktioner ---------- */

static ioopm_hash_table_t *make_ht(void)
{
  return ioopm_hash_table_create(string_knr_hash, string_compare);
}

/* Skapar ett standardpar av tabeller */
static void make_tables(ioopm_hash_table_t **ht_n, ioopm_hash_table_t **ht_sl)
{
  *ht_n  = make_ht();
  *ht_sl = make_ht();
}

/* ---------- Befintliga tester (behåll) ---------- */

void add_remove(void)
{
  ioopm_hash_table_t *ht_n, *ht_sl;
  make_tables(&ht_n, &ht_sl);
  add_merchandise(ht_n, "Bok", "En intressant bok", 67);
  CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht_n));
  remove_merchandise(ht_n, ht_sl, "Bok");
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht_n));
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht_sl));
  destructor(ht_n, ht_sl);
}

void list(void)
{
  ioopm_hash_table_t *ht_n, *ht_sl;
  make_tables(&ht_n, &ht_sl);
  add_merchandise(ht_n, "Bok", "En intressant bok", 67);
  add_merchandise(ht_n, "En intressant bok", "Bok", 67);
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht_n), 2);
  char **results = list_merchandise(ht_n);
  CU_ASSERT_STRING_EQUAL(results[0], "Bok");
  CU_ASSERT_STRING_EQUAL(results[1], "En intressant bok");
  free(results);
  destructor(ht_n, ht_sl);
}

void edit(void)
{
  ioopm_hash_table_t *ht_n, *ht_sl;
  make_tables(&ht_n, &ht_sl);
  add_merchandise(ht_n, "Bok", "En intressant bok", 67);
  edit_merchandise(ht_n, ht_sl, "Bok", "Bock", "En bockig bock", 76);
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht_n), 1);
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht_n, string_elem("Bock")));
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht_n, string_elem("Bok")));

  /* Verifiera att nya fält är satta korrekt */
  elem_t out;
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht_n, string_elem("Bock"), &out));
  s_t *S = out.p;
  CU_ASSERT_STRING_EQUAL(S->item->name, "Bock");
  CU_ASSERT_STRING_EQUAL(S->item->desc, "En bockig bock");
  CU_ASSERT_EQUAL(S->item->price, 76);

  destructor(ht_n, ht_sl);
}

void replenish_show_stock(void)
{
  ioopm_hash_table_t *ht_n, *ht_sl;
  make_tables(&ht_n, &ht_sl);
  add_merchandise(ht_n, "Bok", "En intressant bok", 67);
  replenish(ht_sl, ht_n, "Bok", "C23", 3);
  replenish(ht_sl, ht_n, "Bok", "D22", 0);
  replenish(ht_sl, ht_n, "Hej", "C23", 3);   /* okänt item – ska ignoreras */
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht_sl), 2);
  remove_merchandise(ht_n, ht_sl, "Bok");
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht_n));
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht_sl));
  destructor(ht_n, ht_sl);
}

/* ---------- NYA TESTER ---------- */

/* 1. add_merchandise med dubblett – ska inte skapa ny post */
void add_duplicate(void)
{
  ioopm_hash_table_t *ht_n, *ht_sl;
  make_tables(&ht_n, &ht_sl);
  add_merchandise(ht_n, "Bok", "desc1", 10);
  add_merchandise(ht_n, "Bok", "desc2", 20);
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht_n), 1);

  /* Originalets desc ska vara kvar */
  elem_t out;
  ioopm_hash_table_lookup(ht_n, string_elem("Bok"), &out);
  s_t *S = out.p;
  CU_ASSERT_STRING_EQUAL(S->item->desc, "desc1");
  CU_ASSERT_EQUAL(S->item->price, 10);

  destructor(ht_n, ht_sl);
}

/* 2. remove_merchandise på okänt item – ska inte krascha */
void remove_nonexistent(void)
{
  ioopm_hash_table_t *ht_n, *ht_sl;
  make_tables(&ht_n, &ht_sl);
  add_merchandise(ht_n, "Bok", "desc", 10);
  remove_merchandise(ht_n, ht_sl, "FinnsEj");
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht_n), 1);
  destructor(ht_n, ht_sl);
}

/* 3. remove_merchandise med flera hyllor – verifierar att alla raderas ur ht_sl */
void remove_with_locations(void)
{
  ioopm_hash_table_t *ht_n, *ht_sl;
  make_tables(&ht_n, &ht_sl);
  add_merchandise(ht_n, "Bok", "desc", 10);
  replenish(ht_sl, ht_n, "Bok", "A1", 5);
  replenish(ht_sl, ht_n, "Bok", "B2", 7);
  replenish(ht_sl, ht_n, "Bok", "C3", 2);
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht_sl), 3);

  remove_merchandise(ht_n, ht_sl, "Bok");
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht_n));
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht_sl));
  destructor(ht_n, ht_sl);
}

/* 4. replenish uppdaterar stock/available_stock korrekt */
void replenish_updates_stock(void)
{
  ioopm_hash_table_t *ht_n, *ht_sl;
  make_tables(&ht_n, &ht_sl);
  add_merchandise(ht_n, "Bok", "desc", 10);

  elem_t out;
  ioopm_hash_table_lookup(ht_n, string_elem("Bok"), &out);
  s_t *S = out.p;
  CU_ASSERT_EQUAL(S->item->stock, 0);
  CU_ASSERT_EQUAL(S->item->available_stock, 0);

  replenish(ht_sl, ht_n, "Bok", "A1", 5);
  CU_ASSERT_EQUAL(S->item->stock, 5);
  CU_ASSERT_EQUAL(S->item->available_stock, 5);

  replenish(ht_sl, ht_n, "Bok", "B2", 3);
  CU_ASSERT_EQUAL(S->item->stock, 8);
  CU_ASSERT_EQUAL(S->item->available_stock, 8);

  /* locations-listan ska ha två poster */
  CU_ASSERT_EQUAL(S->locations->size, 2);

  destructor(ht_n, ht_sl);
}

/* 5. edit_merchandise utan namnbyte – ska INTE röra ht_sl */
void edit_no_rename(void)
{
  ioopm_hash_table_t *ht_n, *ht_sl;
  make_tables(&ht_n, &ht_sl);
  add_merchandise(ht_n, "Bok", "gammal beskrivning", 10);
  replenish(ht_sl, ht_n, "Bok", "A1", 4);

  edit_merchandise(ht_n, ht_sl, "Bok", "Bok", "ny beskrivning", 99);

  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht_n, string_elem("Bok")));
  elem_t out;
  ioopm_hash_table_lookup(ht_n, string_elem("Bok"), &out);
  s_t *S = out.p;
  CU_ASSERT_STRING_EQUAL(S->item->desc, "ny beskrivning");
  CU_ASSERT_EQUAL(S->item->price, 99);
  CU_ASSERT_EQUAL(S->item->stock, 4);          /* stock ska bevaras */

  /* ht_sl ska fortfarande peka på "Bok" */
  elem_t sl_out;
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht_sl, string_elem("A1"), &sl_out));
  CU_ASSERT_STRING_EQUAL(sl_out.s, "Bok");

  destructor(ht_n, ht_sl);
}

/* 6. edit_merchandise med namnbyte – ht_sl ska uppdateras */
void edit_with_rename_updates_shelves(void)
{
  ioopm_hash_table_t *ht_n, *ht_sl;
  make_tables(&ht_n, &ht_sl);
  add_merchandise(ht_n, "Bok", "desc", 10);
  replenish(ht_sl, ht_n, "Bok", "A1", 2);
  replenish(ht_sl, ht_n, "Bok", "B2", 3);

  edit_merchandise(ht_n, ht_sl, "Bok", "Roman", "ny desc", 42);

  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht_n, string_elem("Bok")));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht_n, string_elem("Roman")));
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht_n), 1);

  /* Båda hyllorna ska nu mappa till "Roman" */
  elem_t out;
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht_sl, string_elem("A1"), &out));
  CU_ASSERT_STRING_EQUAL(out.s, "Roman");
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht_sl, string_elem("B2"), &out));
  CU_ASSERT_STRING_EQUAL(out.s, "Roman");

  /* locations-listan ska följa med */
  ioopm_hash_table_lookup(ht_n, string_elem("Roman"), &out);
  s_t *S = out.p;
  CU_ASSERT_EQUAL(S->locations->size, 2);
  CU_ASSERT_EQUAL(S->item->stock, 5);

  destructor(ht_n, ht_sl);
}

/* 7. edit_merchandise på okänt item – dokumentera beteende (kraschar idag?) */
/* Hoppa över om backend inte nullcheckar. Aktivera om du lägger till nullcheck. */
/*
void edit_nonexistent(void)
{
  ...
}
*/

/* 8. show_stock – smoke-test (output verifieras ej, men koden körs) */
void show_stock_smoke(void)
{
  ioopm_hash_table_t *ht_n, *ht_sl;
  make_tables(&ht_n, &ht_sl);
  add_merchandise(ht_n, "Bok", "desc", 10);

  /* Inget lager – ska inte skriva ut något, men inte krascha */
  show_stock(ht_n, "Bok");

  replenish(ht_sl, ht_n, "Bok", "A1", 5);
  replenish(ht_sl, ht_n, "Bok", "B2", 7);
  show_stock(ht_n, "Bok");   /* ska skriva ut två rader */

  destructor(ht_n, ht_sl);
}

/* 9. list_merchandise med tom tabell */
void list_empty(void)
{
  ioopm_hash_table_t *ht_n, *ht_sl;
  make_tables(&ht_n, &ht_sl);
  char **results = list_merchandise(ht_n);
  CU_ASSERT_PTR_NOT_NULL(results);
  free(results);
  destructor(ht_n, ht_sl);
}

/* 10. list_merchandise sorterar skiftlägesokänsligt */
void list_sorts_case_insensitive(void)
{
  ioopm_hash_table_t *ht_n, *ht_sl;
  make_tables(&ht_n, &ht_sl);
  add_merchandise(ht_n, "banan", "d", 1);
  add_merchandise(ht_n, "Apple", "d", 1);
  add_merchandise(ht_n, "cherry", "d", 1);

  char **results = list_merchandise(ht_n);
  CU_ASSERT_STRING_EQUAL(results[0], "Apple");
  CU_ASSERT_STRING_EQUAL(results[1], "banan");
  CU_ASSERT_STRING_EQUAL(results[2], "cherry");
  free(results);
  destructor(ht_n, ht_sl);
}

/* 11. Flera varor oberoende av varandra */
void multiple_items_independent(void)
{
  ioopm_hash_table_t *ht_n, *ht_sl;
  make_tables(&ht_n, &ht_sl);
  add_merchandise(ht_n, "Bok", "d", 10);
  add_merchandise(ht_n, "Penna", "d", 5);
  add_merchandise(ht_n, "Suddgummi", "d", 2);
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht_n), 3);

  replenish(ht_sl, ht_n, "Bok", "A1", 5);
  replenish(ht_sl, ht_n, "Penna", "B1", 10);
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht_sl), 2);

  remove_merchandise(ht_n, ht_sl, "Bok");
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht_n), 2);
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht_sl), 1);
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht_n, string_elem("Penna")));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht_n, string_elem("Suddgummi")));

  destructor(ht_n, ht_sl);
}

/* 12. replenish med befintlig hylla (samma shelf-sträng två gånger) */
void replenish_same_shelf(void)
{
  ioopm_hash_table_t *ht_n, *ht_sl;
  make_tables(&ht_n, &ht_sl);
  add_merchandise(ht_n, "Bok", "d", 10);
  replenish(ht_sl, ht_n, "Bok", "A1", 3);
  replenish(ht_sl, ht_n, "Bok", "A1", 4);   /* samma hylla igen */

  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht_sl), 1);   /* bara en shelf-nyckel */
  elem_t out;
  ioopm_hash_table_lookup(ht_n, string_elem("Bok"), &out);
  s_t *S = out.p;
  CU_ASSERT_EQUAL(S->item->stock, 7);
  CU_ASSERT_EQUAL(S->locations->size, 2); /* men två loc_pair */

  destructor(ht_n, ht_sl);
}

/* ---------- main ---------- */

int main(void)
{
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  CU_pSuite suite = CU_add_suite("Backend tests", init_suite, clean_suite);
  if (suite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (CU_add_test(suite, "add/remove", add_remove) == NULL ||
      CU_add_test(suite, "list", list) == NULL ||
      CU_add_test(suite, "edit", edit) == NULL ||
      CU_add_test(suite, "replenish/show_stock", replenish_show_stock) == NULL ||
      CU_add_test(suite, "add duplicate", add_duplicate) == NULL ||
      CU_add_test(suite, "remove nonexistent", remove_nonexistent) == NULL ||
      CU_add_test(suite, "remove with locations", remove_with_locations) == NULL ||
      CU_add_test(suite, "replenish updates stock", replenish_updates_stock) == NULL ||
      CU_add_test(suite, "edit no rename", edit_no_rename) == NULL ||
      CU_add_test(suite, "edit with rename updates shelves", edit_with_rename_updates_shelves) == NULL ||
      CU_add_test(suite, "show_stock smoke", show_stock_smoke) == NULL ||
      CU_add_test(suite, "list empty", list_empty) == NULL ||
      CU_add_test(suite, "list sorts case-insensitive", list_sorts_case_insensitive) == NULL ||
      CU_add_test(suite, "multiple items independent", multiple_items_independent) == NULL ||
      CU_add_test(suite, "replenish same shelf", replenish_same_shelf) == NULL)
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_NORMAL);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}