CC = gcc
CFLAGS = -Wall -g

all: ddt 

# ddt
ddt: ddt.o configuration.o
	$(CC) $(CFLAGS) $^ -o $@

ddt.o: ddt.c configuration.h
	$(CC) $(CFLAGS) -c $< -o $@

configuration.o: configuration.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	clear
	rm *.o



# $@ reffers to named before :
# $^ reffers to all named after :
# $< reffers to first after :
