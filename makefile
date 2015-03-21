SOURCES = $(shell ls source/*.c)
TEMP = $(subst source/, obj/, $(SOURCES))
OBJECTS = $(subst .c,.o, $(TEMP))
CFLAGS = -Wall -ansi -std=gnu99 -g


all: makefolders ddt

ddt: $(OBJECTS)
	$(CC) $^ -o $@

obj/%.o: source/%.c
	$(CC) $(CFLAGS) -c $< -o $@

#makefolders
makefolders:
	mkdir -p obj

clean:
	clear
	rm -f $(OBJECTS) ddt
	rm -f obj/*.o

# $@ reffers to named before the :
# $^ reffers to all named after the :
# $< reffers to first after the :
# $(variable) : declared at the top of the file
