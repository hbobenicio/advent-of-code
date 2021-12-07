CC = clang
CFLAGS = -Wall -Wextra -Wpedantic -std=c17 -g -ggdb -fsanitize=address,undefined -fno-omit-frame-pointer

all: a b

a: a.c
	$(CC) $(CFLAGS) -o $@ $^

b: b.c
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: distclean
distclean:
	$(RM) -rfv a b

