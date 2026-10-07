CC     = gcc
CFLAGS = -Wall -Wextra -g

# Alla .c-filer utom backend.c (som har main)
SHARED = linked_list.c list_iterator.c hash_table.c hash_table_iterator.c utils.c backend.c 

all: app tests

app: backend.c $(SHARED) backend.h common.h linked_list.h list_iterator.h hash_table.h hash_table_iterator.h utils.h
	$(CC) $(CFLAGS) frontend.c $(SHARED) -o app

tests: backend_tests.c backend.c linked_list.c list_iterator.c hash_table.c hash_table_iterator.c utils.c
	$(CC) $(CFLAGS) $^ -o tests -lcunit
clean:
	rm -f app tests

run: app
	./app < test.txt

test: tests
	./tests