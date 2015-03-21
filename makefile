CC 				= gcc
CFLAGS 			= -Wall -g
RMFLAGS			= -f
EXECUTABLES		= ddt testTejo
S_DIR 			= source
O_DIR 			= obj


all: makefolders ddt

# ddt
ddt: ddt.o interface.o network.o ringOps.o responses.o triggers.o
	$(CC) $(CFLAGS) $(addprefix $(O_DIR)/,$^) -o $@

ddt.o: $(S_DIR)/ddt.c $(S_DIR)/interface.h $(S_DIR)/network.h $(S_DIR)/ringOps.h $(S_DIR)/responses.h $(S_DIR)/triggers.h
	$(CC) $(CFLAGS) -c $< -o $(O_DIR)/$@

interface.o: $(S_DIR)/interface.c
	$(CC) $(CFLAGS) -c $< -o $(O_DIR)/$@

network.o: $(S_DIR)/network.c
	$(CC) $(CFLAGS) -c $< -o $(O_DIR)/$@

ringOps.o: $(S_DIR)/ringOps.c
	$(CC) $(CFLAGS) -c $< -o $(O_DIR)/$@

responses.o: $(S_DIR)/responses.c
	$(CC) $(CFLAGS) -c $< -o $(O_DIR)/$@

triggers.o: $(S_DIR)/triggers.c
	$(CC) $(CFLAGS) -c $< -o $(O_DIR)/$@

#testTejo
testTejo: testTejo.o network.o
	$(CC) $(CFLAGS) $(addprefix $(O_DIR)/,$^) -o $@

testTejo.o: $(S_DIR)/testTejo.c
	$(CC) $(CFLAGS) -c $< -o $(O_DIR)/$@

#makefolders
makefolders:
	mkdir -p $(O_DIR)

#clean
clean:
	clear
	rm $(RMFLAGS) $(O_DIR)/*.o
	rm $(RMFLAGS) $(EXECUTABLES)


# $@ reffers to named before the :
# $^ reffers to all named after the :
# $< reffers to first after the :
# $(variable) : declared at the top of the file
