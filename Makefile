CC = gcc
CFLAGS = -Wall -Werror

all: wordle

wordle: 
	$(CC) $(CFLAGS) wordle.c -o wordle

clean:
	rm -f wordle