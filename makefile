CC     = gcc
CFLAGS =  -Wall -Wextra -g -fsanitize=address,undefined

# Alla .c-filer utom frontend.c och backend_tests.c (båda har main)
SHARED = backend.c linked_list.c list_iterator.c hash_table.c hash_table_iterator.c utils.c

# Alla .h-filer (för beroenden)
HDRS = backend.h common.h linked_list.h list_iterator.h hash_table.h hash_table_iterator.h utils.h

# Programmet
app: frontend.c $(SHARED) $(HDRS)
	$(CC) $(CFLAGS) frontend.c $(SHARED) -o app

# Testerna (egen main i backend_tests.c)
tests: backend_tests.c $(SHARED) $(HDRS)
	$(CC) $(CFLAGS) backend_tests.c $(SHARED) -o tests -lcunit

clean:
	rm -f app tests

run: app
	./app < test.txt

test: tests
	./tests