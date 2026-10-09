#include "common.h"
#include "hash_table_iterator.h"
#include "list_iterator.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


//Hjälpfunktion som friar upp alla pekare tillhörande locationslistan
static void destroy_loc_pairs(ioopm_list_t *list)
{
    if(ioopm_list_is_empty(list)){ //Nullcheck för att undvika errors
        return;
    }

    ioopm_list_iterator_t *it = ioopm_list_iterator_create(list);
    while (!ioopm_list_iterator_at_end(it)) 
    {
        elem_t tmp = ioopm_list_iterator_current(it);
        loc_pair_t *pair = tmp.p; //Hämtar och castar om pekaren till loc_pair_t

        free(pair->shelf); //friar strängen inuti loc_pairet först
        free(pair); //sedan resten
        ioopm_list_iterator_advance(it);
    }
    ioopm_list_iterator_destroy(it);
}

static void destroy_locations(ioopm_list_t *l){ //Tar bort hela locations listan i två steg
    destroy_loc_pairs(l); // rensar upp alla pekare som list_destroy inte når
    ioopm_list_destroy(l);
} 

// Hjälpfunktion för att förstöra en S från htn, tar också in en bool som säger huruvida listan ska förstöras också
static void destroy_entry_htn(s_t *S, bool destroy_list)
{
    if (S->item) //Extra checks för att undvika segfaults
    {
        if (S->item->desc) //friar djupaste strängen först, lämnar namnet kvar till den yttre funktionen att fria
            free(S->item->desc);
        free(S->item);
    }

    if(destroy_list){ //Förstör listan om hela S:n ska tas bort
        destroy_locations(S->locations);
        free(S);
    }
}


//Hashfunktionen som används i alla hashningar
size_t string_knr_hash(elem_t key)
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
 
// Våran strängjämförelsefunktion som används i hashtabellen
bool string_compare(elem_t str1, elem_t str2)
{
    const char *string1 = str1.s;
    const char *string2 = str2.s;

    return strcmp(string1, string2) == 0;
}

// Friar upp allt minne använt av htn och htsl
void destructor(ioopm_hash_table_t *htn, ioopm_hash_table_t *htsl)
{
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(htn);
    while (!ioopm_hash_table_iterator_at_end(it))
    {
        elem_t tmp = ioopm_hash_table_iterator_current_value(it);
        s_t *current = tmp.p;
        char *to_free = current->item->name;
        destroy_entry_htn(current, true);
        free(to_free);
        ioopm_hash_table_iterator_advance(it);
    }
    ioopm_hash_table_iterator_destroy(it);
    ioopm_hash_table_destroy(htn);
    ioopm_hash_table_destroy(htsl);
}

// Todo, fixa en alfabetisk sorteringsfunktion
ioopm_list_t *sort(ioopm_list_t *list)
{
    // STUB
    return list;
}



// skapar en S:
static s_t *S_create(char *name, char *desc_in, size_t price)
{   
    char *desc = strdup(desc_in);

    // skapar merch
    merch_t *merch = calloc(1, sizeof(merch_t));
    merch->name = strdup(name); // behöver strdupas för att destructorn ska funka som jag vill med nyckeln
    merch->desc = desc; // behöver inte strdupas eftersom vi inte använder den för något annat
    merch->price = price;

    // skapar S med merch och null
    s_t *S = calloc(1, sizeof(s_t));
    S->item = merch;
    S->locations = ioopm_list_create();
    return S;
}

void add_merchandise(ioopm_hash_table_t *ht_n, char *name, char *desc, size_t price)
{
    // skapa en S
    s_t *item = S_create(name, desc, price);

    // lägg in den i ht_n med name som nyckel ger felmeddelande om den redan finns
    bool exists = ioopm_hash_table_has_key(ht_n, string_elem(name));
    if (exists)
    {
        printf("%s already exists\n", name);
    }
    else
    {
        ioopm_hash_table_insert(ht_n, string_elem(item->item->name), st_elem(item));
    }
}

char *list_merchandise(ioopm_hash_table_t *ht_n)
{
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht_n);
    char *result[ht_n->ht_size];
    int i = 0;
    while (!ioopm_hash_table_iterator_at_end(it)) //Itererar över alla element i htn
    {
        //sparar varje element i resultat-arrayen och lägger till det i countern
        result[i] = it->current_entry->key.s; 
        i++;
        ioopm_hash_table_iterator_advance(it);
    }
    ioopm_hash_table_iterator_destroy(it);
    return result;
}

void print_merchandise(char *to_print, size_t i){
    int n = 0;
    while (n < i) //Utprintningsfunktionen, måste brytas ut för att kunna köra tester
    {
        printf("%s \n", to_print[n]);
        n++;
        if (n % 20 == 0)
        {
            char *ans = ask_question_string("Vill du fortsätta? (Y/N)\n");
            to_upper_case(ans);
            if (ans[0] != 'Y')
            {
                free(ans);
                return;
            }
        }
    }
}

//Hjälpfunktion som castar om pekarelementet från elem_t till en s_t 
static s_t *lookup_htn(ioopm_hash_table_t *ht_n, char *name)
{
    elem_t item;
    if(ioopm_hash_table_lookup(ht_n, string_elem(name), &item)){
        return item.p;
    }
    return NULL;
}

// samma som ovan fast hämtar loc_pair från locationslistan
static loc_pair_t *list_fetch(ioopm_list_iterator_t *it)
{
    elem_t current = ioopm_list_iterator_current(it);
    loc_pair_t *tmp = current.p;
    return tmp;
}

void remove_merchandise(ioopm_hash_table_t *ht_n, ioopm_hash_table_t *ht_sl, char *name)
{
    elem_t tmp;
    s_t *item_s = lookup_htn(ht_n, name);
    if(item_s == NULL){ // nullcheck för att undvika segfaults
        puts("Hittade inte itemet som söks!");
        return;
    }

    ioopm_list_t *l = item_s->locations;
    if (!ioopm_list_is_empty(l))
    {
        ioopm_list_iterator_t *it = ioopm_list_iterator_create(l);
        while (!ioopm_list_iterator_at_end(it)) // Hämtar varje shelf från locationslistan
        {                                       // Tar sedan bort dessa shelves från htsl
            elem_t to_cast = ioopm_list_iterator_current(it);
            loc_pair_t *casted = to_cast.p;
            ioopm_hash_table_remove(ht_sl, string_elem(casted->shelf), &tmp);
            ioopm_list_iterator_advance(it);
        }
        ioopm_list_iterator_destroy(it);
    }

    char *to_free = item_s->item->name; //Sparar undan nyckeln och namn-elemntet i S för att fria manuellt efteråt
    char *name_copy = strdup(name); // Inte elegant men det funkar
    destroy_entry_htn(item_s, true); //Tar bort och friar allt utrymme 
    ioopm_hash_table_remove(ht_n, string_elem(name_copy), &tmp);
    free(name_copy);
    free(to_free);
}

void edit_merchandise(ioopm_hash_table_t *ht_n, ioopm_hash_table_t *ht_sl, char *name_old, char *name_new_in, char *desc_in, size_t price)
{
    char *name_new = strdup(name_new_in);
    char *desc = strdup(desc_in);
    // Hämta all tidigare info om itemet
    s_t *item_s = lookup_htn(ht_n, name_old);
    char *to_free = item_s->item->name; //namn för att fria separat på rätt plats

    // skapar ny(?) info om  itemet
    merch_t *tmp = calloc(1, sizeof(merch_t));
    tmp->desc = desc;
    tmp->name = name_new;
    tmp->price = price;
    tmp->stock = item_s->item->stock;

    // skapar nya itemet
    ioopm_list_t *locs = item_s->locations;
    s_t *to_insert = calloc(1, sizeof(s_t));
    to_insert->item = tmp;
    to_insert->locations = locs;

    if (!string_compare(string_elem(name_new), string_elem(name_old)))
    {
        // tar bort alla instanser av den gamla varan utifall namnet ändrats
        elem_t tmp;
        destroy_entry_htn(item_s, false);
        free(item_s);
        ioopm_hash_table_remove(ht_n, string_elem(name_old), &tmp);
        if (!ioopm_list_is_empty(locs))
        {
            // Uppdaterar varje shelf med den gamla varan med den nya utifall namnet ändrats
            ioopm_list_iterator_t *it = ioopm_list_iterator_create(locs);
            while (!ioopm_list_iterator_at_end(it))
            {
                elem_t to_cast = ioopm_list_iterator_current(it);
                loc_pair_t *casted = to_cast.p;
                ioopm_hash_table_insert(ht_sl, string_elem(casted->shelf), string_elem(name_new));
                ioopm_list_iterator_advance(it);
            }
            ioopm_list_iterator_destroy(it);
        }
        free(to_free);
    }
    // Uppdaterar eller sätter in nya beroende på om namnet ändrats
    ioopm_hash_table_insert(ht_n, string_elem(name_new), ptr_elem(to_insert));
}

void show_stock(ioopm_hash_table_t *ht_n, char *name)
{
    s_t *item_s = lookup_htn(ht_n, name);
    if (item_s->item->stock != 0)
    {
        ioopm_list_iterator_t *it = ioopm_list_iterator_create(sort(item_s->locations));
        while (!ioopm_list_iterator_at_end(it))
        {
            loc_pair_t *current = list_fetch(it);
            printf("Hylla: %s innehåller %ld %s \n", current->shelf, current->stock, name);
            ioopm_list_iterator_advance(it);
        }
        ioopm_list_iterator_destroy(it);
    }
}

void replenish(ioopm_hash_table_t *ht_sl, ioopm_hash_table_t *ht_n, char *name, char *shelf_in, size_t amount)
{
    char *shelf = strdup(shelf_in);
    ioopm_hash_table_insert(ht_sl, string_elem(shelf), string_elem(name));
    s_t *item = lookup_htn(ht_n, name);
    loc_pair_t *tmp = calloc(1, sizeof(loc_pair_t));
    tmp->shelf = shelf;
    item->item->stock += amount;
    tmp->stock = amount;
    item->item->available_stock += amount;
    ioopm_list_append(item->locations, ptr_elem(tmp));
}