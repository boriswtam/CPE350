CFLAGS = -Wall -lLabJackM -pthread -lrt

# Should be equivalent to your list of C files, if you don't build selectively
SRC=$(wildcard *.c)

test: $(SRC)
	gcc -c $^ $(CFLAGS)
	gcc -o run_all_sensors $(SRC) $(CFLAGS)
