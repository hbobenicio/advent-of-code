CC = clang
CFLAGS = -Wall -Wextra -Wpedantic -std=c18 -g -O0 -fsanitize=address,undefined -fno-omit-frame-pointer

all: ab

ab: ab.c
	$(CC) $(CFLAGS) -o $@ $^


.PHONY: distclean
distclean:
	$(RM) -rfv ab

