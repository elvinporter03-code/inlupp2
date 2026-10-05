#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "utils.h"

// kollar så att en inskickad sträng är ett tal, check-funktion för is_question_int.
// konverterar negativa tal till positiva. returnerar ett positivt tal.
bool is_number(char *str)
{
    int length = strlen(str);
    int i = 0;
    if (str[0] == '-' && length > 1)
    {
        i = 1;
    }

    for (; i < length; i++)
    {
        if (!isdigit(str[i]))
        {
            return false;
        }
    }
    
    return true;
}

// hjälp-funktion för read_string, rensar input_buffer.
void clear_input_buffer()
{
    int c;
    do
    {
        c = getchar();
    }
    while (c != '\n' && c != EOF);
}

// hjälpfunktion som kollar om en sträng är tom.
bool not_empty(char *str)
{
  return strlen(str) > 0;
}

// skickar ut en fråga i terminalen och returnerar ett positivt heltal den får som input.
int ask_question_int(char *question)
{
  answer_t answer = ask_question(question, is_number, (convert_func *) atoi);
  return answer.int_value;
}

// kopierar in ett ord från input i terminalen till en string buf. rensar input_buf när den fyllt 
// sin storlek.
int read_string(char *buf, int buf_siz)
{
    int c = 0;
    int counter = 0;
   
    while (c != '\n' && c != EOF && counter < buf_siz - 1)
    {
        c = getchar();
        if (c != '\n' && c != EOF)
        {
            buf[counter] = c;
            counter ++;
        }
    }
    
    if (c != '\n' && c != EOF)
    {
        clear_input_buffer();
    }
    
    buf[counter] = '\0';
    return counter;
}

// skickar ut en fråga i terminalen och returnerar den sträng den får som input.
char *ask_question_string(char *question)
{
  return ask_question(question, not_empty, (convert_func *) strdup).string_value;
}

// skalfunktion som genererar en fråga, tar in input, 
// checkar mha en check_func, konverterar mha en convert_func.
answer_t ask_question(char *question, check_func *check, convert_func *convert)
{
    int buf_siz = 100;
    char buf[buf_siz];

    do
    {
        printf("%s\n", question);
        read_string(buf, buf_siz);
      
    } while (!check(buf));

    answer_t result = convert(buf);
    return result;
}

// hjälpfunktion till ask_question_shelf, kollar om en sträng är ett shelf 
// dvs har formen "bokstav ett antal siffror"
bool is_shelf(char *str)
{
    int length = strlen(str);
    if (strlen(str) <= 1) return false;
    if (!isalpha(str[0])) return false;

    for (int i = 1; i < length; i++)
    {
        if (!isdigit(str[i]))
        {
            return false;
        }
    }
    
    return true;
}

// skickar ut en fråga i terminalen och returnerar ett shelf i form av en sträng den får som input.
char *ask_question_shelf(char *question)
{
    return ask_question(question, is_shelf, (convert_func *) strdup).string_value;
}

// check-funktion till ask_question_menu som kollar att inputsträngen innehåller:
// ett tecken som ingår i "LlTtRrGgHhAa".
bool is_menu_letter(char *str)
{
    if (strlen(str) != 1) return false;

    char *menu_letters = "LlTtRrGgHhAa";
    int length = strlen(menu_letters);

    for (int i = 0; i < length; i++)
    {
        if (str[0] == menu_letters[i]) return true;
    }

    return false;
}

// konverterings-funktion till ask_question_menu. konverterar små bokstäver till stora
answer_t to_upper_case(char *letter)
{
    answer_t result;
    result.character = toupper(letter[0]);
    return result;
}




// printfunktioner från str.c lab3

int print(char *str)
{
    int counter = 0;
    while (str[counter] != '\0')
    {
        putchar(str[counter]);
        counter++;
    }
    return 0;
}

int println(char *str)
{
    print(str);
    putchar('\n');
    return 0;
}

// lab 4 db
/*
void list_db(item_t *items, int db_siz)
{
    for (int i = 0; i < db_siz; i++)
    {
      printf("%d. %s\n", (i + 1), items[i].name);
    }
}

void print_item(item_t *itm)
{
    printf("Name: %s\n"
            "Desc: %s\n"
            "Price: %d.%d%d SEK\n"
            "Shelf: %s\n", 
            itm->name, 
            itm->desc, 
            (itm->price)/100, 
            ((itm->price) % 100)/10, 
            (itm->price) % 10, 
            itm->shelf);
}

item_t make_item(char *item_name, char *item_desc, int item_price, char *item_shelf)
{
    item_t itm = {.name = item_name, .desc = item_desc, .price = item_price, .shelf = item_shelf};
    return itm;
}

item_t input_item(void)
{
    char *name = ask_question_string("Skriv in namn:\n");
    char *desc = ask_question_string("Skriv beskrivning:\n");
    int price = ask_question_int("Skriv pris:\n");
    char *shelf = ask_question_shelf("Skriv lagerhylla:\n");

    item_t itm = make_item(name, desc, price, shelf);
    return itm;
}

void edit_db(item_t *items, int db_siz)
{
    int itm_nmr;
    do 
    {
        itm_nmr = ask_question_int("Write the number of the item: ");
    }
    while (!(0 < itm_nmr && itm_nmr <= db_siz));

    item_t itm = items[itm_nmr - 1];
    print_item(&itm);

    item_t itm_update = input_item();

    free(items[itm_nmr - 1].name);
    free(items[itm_nmr - 1].desc);
    free(items[itm_nmr - 1].shelf);

    items[itm_nmr -1] = itm_update;
}
*/






