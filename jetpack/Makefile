CFLAGS = -Wall -lLabJackM

# Should be equivalent to your list of C files, if you don't build selectively
SRC=$(wildcard *.c)

test: $(SRC)
	gcc -c $^ $(CFLAGS)
	gcc -o test $(SRC) $(CFLAGS)
