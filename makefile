CC = gcc
CFLAGS = -Wall -g

all: ddt 

# ddt
ddt: ddt.o interface.o
	$(CC) $(CFLAGS) $^ -o $@

ddt.o: ddt.c interface.h
	$(CC) $(CFLAGS) -c $< -o $@

interface.o: interface.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	clear
	rm *.o



# $@ reffers to named before :
# $^ reffers to all named after :
# $< reffers to first after :
