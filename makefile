CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -g

# Alla .c-filer utom backend.c (som har main)
SHARED = linked_list.c list_iterator.c hash_table.c hash_table_iterator.c

all: app tests

app: backend.c $(SHARED) backend.h common.h linked_list.h list_iterator.h hash_table.h hash_table_iterator.h
	$(CC) $(CFLAGS) backend.c $(SHARED) -o app

tests: backend_tests.c $(SHARED) backend.h common.h linked_list.h list_iterator.h hash_table.h hash_table_iterator.h
	$(CC) $(CFLAGS) backend_tests.c $(SHARED) -o tests

clean:
	rm -f app tests

run: app
	./app

test: tests
	./tests