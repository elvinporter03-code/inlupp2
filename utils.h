#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdbool.h>

typedef union 
{
    int int_value;
    float float_value;
    char *string_value;
    char character;
} answer_t;

typedef struct item 
{
    char *name;
    char *desc;
    int price;
    char *shelf;
} item_t;


typedef bool check_func(char *);
typedef answer_t convert_func(char *);

int read_string(char *buf, int buf_siz);
bool is_number(char *str);
int ask_question_int(char *question);
char *ask_question_string(char *question);
int print(char *str);
int println(char *str);
bool not_empty(char *str);
bool is_shelf(char *str);
char *ask_question_shelf(char *question);
bool is_menu_letter(char *str);
answer_t to_upper_case(char *letter);
answer_t ask_question(char *question, check_func *check, convert_func *convert);

// lab 4 db
/*
void list_db(item_t *items, int db_siz);
void print_item(item_t *itm);
void edit_db(item_t *items, int db_siz);
item_t make_item(char *item_name, char *item_desc, int item_price, char *item_shelf);
item_t input_item(void);
*/

extern char *strdup(const char *);


#endif 