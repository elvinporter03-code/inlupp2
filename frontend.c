#include "backend.h"
#include "common.h"
#include "hash_table_iterator.h"
#include "list_iterator.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

bool confirmation()
{
    char *ans = ask_question_string("Säker? (Y/N) \n");
    to_upper_case(ans);
    if (ans[0] == 'Y')
    {
        return true;
    }
    return false;
}

void main_loop(ioopm_hash_table_t *ht_sl, ioopm_hash_table_t *ht_n)
{
    char *name;
    char *desc;
    char *shelf;
    char *name_old;
    char *name_new;
    size_t price;
    bool running = true;
    while (running)
    {
        char *ans = ask_question_string("Välj ett menyalternativ \n");
        printf("%s \n", ans);
        to_upper_case(ans);
        switch (ans[0])
        {
        case 'A':
            puts("Du tryckte a");
            name = ask_question_string("Vilket item vill du lägga till? \n");
            desc = ask_question_string("Description? \n");
            price = ask_question_int("Hur mycket kostar itemet? \n");
            add_merchandise(ht_n, name, desc, price);
            break;

        case 'L':
            list_merchandise(ht_n);
            break;

        case 'D':
            name = ask_question_string("Vilket item vill du ta bort? \n");
            if (confirmation())
                remove_merchandise(ht_n, ht_sl, name);
            break;

        case 'E':
            name_old = ask_question_string("Vilket item vill du ta ändra? \n");
            name_new = ask_question_string("Nytt namn? \n");
            desc = ask_question_string("Ny description? \n");
            price = ask_question_int("Hur mycket kostar itemet? \n");
            if (confirmation())
                edit_merchandise(ht_n, ht_sl, name_old, name_new, desc, price);
            break;

        case 'S':
            name = ask_question_string("Vilket item vill du visa stock för? \n");
            show_stock(ht_n, name);
            break;

        case 'P':
            shelf = ask_question_shelf("Vilken hylla vill du lägga till på \n");
            name = ask_question_string("Vilket item vill du lägga till fler av? \n");
            size_t amount = ask_question_int("Hur många vill du fylla på med? \n");
            replenish(ht_sl, ht_n, name, shelf, amount);
            break;

        case 'C':
            break;

        case 'R':
            break;

        case '+':
            break;

        case '-':
            break;

        case '=':
            break;

        case 'O':
            break;

        case 'Q':
            running = false;
            break;

        default:
            running = false;
            break;
        }
        free(ans);
    }
    free(shelf);
    free(desc);
    free(name);
    free(name_old);
    free(name_new);
}

int main()
{
    ioopm_hash_table_t *ht_n = ioopm_hash_table_create(string_knr_hash, string_compare);
    ioopm_hash_table_t *ht_sl = ioopm_hash_table_create(string_knr_hash, string_compare);

    main_loop(ht_sl, ht_n);

    destructor(ht_n, ht_sl); // shoppingcarts
    return 0;
}