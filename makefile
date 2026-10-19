CC     = gcc
CFLAGS = -Wall -Wextra -g -fsanitize=address,undefined

# Flaggor för coverage (ersätter sanitizers eftersom de inte fungerar ihop med gcov)
COVFLAGS = -Wall -Wextra -g --coverage

# Alla .c-filer utom frontend.c och backend_tests.c (båda har main)
SHARED = backend.c linked_list.c list_iterator.c hash_table.c hash_table_iterator.c utils.c

# Alla .h-filer (för beroenden)
HDRS = backend.h common.h linked_list.h list_iterator.h hash_table.h hash_table_iterator.h utils.h carts.h

# Programmet
app: frontend.c $(SHARED) $(HDRS)
	$(CC) $(CFLAGS) frontend.c $(SHARED) -o app

# Testerna 
tests: backend_tests.c $(SHARED) $(HDRS)
	$(CC) $(CFLAGS) backend_tests.c $(SHARED) -o tests -lcunit

# Carts-tester
carts_tests: carts_tests.c carts.c $(SHARED) $(HDRS)
	$(CC) $(CFLAGS) carts_tests.c carts.c $(SHARED) -o carts_tests -lcunit

# Coverage
coverage: clean
	@echo "--- Kompilerar med coverage-flaggor ---"
	$(CC) $(COVFLAGS) backend_tests.c $(SHARED) -o tests -lcunit
	$(CC) $(COVFLAGS) carts_tests.c carts.c $(SHARED) -o carts_tests -lcunit

	@echo "--- Kör testerna ---"
	./tests
	./carts_tests

	@echo "--- gcov: backend täcks av backend_tests ---"
	@gcov -b -c tests-backend.gcda

	@echo "--- gcov: carts täcks av carts_tests ---"
	@gcov -b -c carts_tests-carts.gcda

	@rm -f *.gcda *.gcno *-*.gcda *-*.gcno
	@echo "Klart! Öppna backend.c.gcov och carts.c.gcov."

clean:
	rm -f app tests carts_tests test_carts
	rm -f *.gcda *.gcno *.gcov *-*.gcda *-*.gcno

run: app
	./app < test.txt

test: tests carts_tests
	./tests
	./carts_tests