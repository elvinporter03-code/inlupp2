#include "backend.h"
#include "common.h"
#include "hash_table_iterator.h"
#include "list_iterator.h"
#include "utils.h"
#include "carts.h"
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
        free(ans);
        return true;
    }
    free(ans);
    return false;
}

void print_merchandise(char **to_print, size_t i){
    size_t n = 0;
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

void main_loop(ioopm_hash_table_t *ht_sl, ioopm_hash_table_t *ht_n, cart_list_t *cart_list)
{
    char *name;
    char *desc;
    char *shelf;
    char *name_old;
    char *name_new;
    size_t price;
    size_t cart_id;
    char **results;
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
            free(desc);
            free(name);

            break;

        case 'L':
            results = list_merchandise(ht_n);
            print_merchandise(results,ht_n->ht_size);
            free(results);
            break;

        case 'D':
            name = ask_question_string("Vilket item vill du ta bort? \n");
            if (confirmation())remove_merchandise(ht_n, ht_sl, name);
            free(name);

            break;

        case 'E':
            name_old = ask_question_string("Vilket item vill du ta ändra? \n");
            name_new = ask_question_string("Nytt namn? \n");
            desc = ask_question_string("Ny description? \n");
            price = ask_question_int("Hur mycket kostar itemet? \n");
            if (confirmation()) edit_merchandise(ht_n, ht_sl, name_old, name_new, desc, price);
            free(desc);
            free(name_old);
            free(name_new);

            break;

        case 'S':
            name = ask_question_string("Vilket item vill du visa stock för? \n");
            show_stock(ht_n, name);
            free(name);
            break;

        case 'P':
            shelf = ask_question_shelf("Vilken hylla vill du lägga till på \n");
            name = ask_question_string("Vilket item vill du lägga till fler av? \n");
            size_t amount = ask_question_int("Hur många vill du fylla på med? \n");
            replenish(ht_sl, ht_n, name, shelf, amount);
            free(name);
            free(shelf);

            break;

        case 'C':
            ioopm_create_cart(cart_list);
            break;

        case 'R':
            size_t index = ask_question_int("Vilken kundvagn vill du ta bort? \n");
            ioopm_remove_cart(cart_list, index, ht_n);
            printf("The cart with id %zu has now been removed \n");
            break;

        case '+':
            size_t index = ask_question_int("Vilken kundvagn vill du lägga till varor i? \n");
            name = ask_question_string("Vilken vara vill du lägga till? \n");
            size_t amount = ask_question_int("Hur många varor vill du lägga till? \n");
            ioopm_add_to_cart(cart_list, index, name, amount, ht_n);
            printf("%zu items of %s has now been added to cart %zu \n", amount, name, index);
            free(name);
            break;

        case '-':
            size_t index = ask_question_int("Vilken kundvagn vill du ta bort varor från? \n");
            name = ask_question_string("Vilken vara vill du ta bort? \n");
            size_t amount = ask_question_int("Hur många varor vill du ta bort? \n");
            ioopm_remove_from_cart(cart_list, index, name, amount, ht_n);
            printf("%zu items of %s has now been removed from cart %zu \n", amount, name, index);
            free(name);
            break;

        case '=':
            size_t index = ask_question_int("Vilken kundvagn vill beräkna kostnaden av? \n");
            int cost = ioopm_calc_costs(cart_list, index, ht_n);
            printf("the total cost of the items in cart %zu is: %d \n", index, cost);
            break;

        case 'O':
            size_t index = ask_question_int("Vilken kundvagn vill du checka ut? \n");
            ioopm_checkout(cart_list, index, ht_n);
            printf("the cart %zu is now checked out, the total cost is: %d \n", index, cost);
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

}

int main()
{
    ioopm_hash_table_t *ht_n = ioopm_hash_table_create(string_knr_hash, string_compare);
    ioopm_hash_table_t *ht_sl = ioopm_hash_table_create(string_knr_hash, string_compare);
    cart_list_t *cart_list = cart_list_create();

    main_loop(ht_sl, ht_n, cart_list);

    destructor(ht_n, ht_sl); // shoppingcarts
    return 0;
}