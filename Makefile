CC=gcc
CFLAGS=-Wall -g

cs240Elections: main.c elections.h
	$(CC) $(CFLAGS) $< -o $@ -lm

.PHONY: clean

clean:
	rm -f cs240Elections
