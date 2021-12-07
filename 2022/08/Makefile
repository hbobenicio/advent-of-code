CC = clang-15
CFLAGS = -Wall -Wextra -Wpedantic -std=c18 -g -O0 -fsanitize=address,undefined -fno-omit-frame-pointer

all: a

a: a.c
	$(CC) $(CFLAGS) -o $@ $^


.PHONY: distclean
distclean:
	$(RM) -rfv a

