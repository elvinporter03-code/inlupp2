#include "backend.h"
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

void main_loop(ioopm_hash_table_t *ht_sl, ioopm_hash_table_t *ht_n)
{
    char *name;
    char *desc;
    char *shelf;
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
            char *name = ask_question_string("Vilket item vill du lägga till? \n");
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
            char *name_old = ask_question_string("Vilket item vill du ta ändra? \n");
            char *name_new = ask_question_string("Nytt namn? \n");
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
    }
    (void)name;
}

int main()
{
    ioopm_hash_table_t *ht_n = ioopm_hash_table_create(string_knr_hash, string_compare);
    ioopm_hash_table_t *ht_sl = ioopm_hash_table_create(string_knr_hash, string_compare);

    main_loop(ht_n, ht_sl);

    destructor(ht_n, ht_sl); // shoppingcarts
    return 0;
}