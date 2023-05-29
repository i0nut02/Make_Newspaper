CC := gcc

CFLAGS := -Wall -c 

makeNewspaper: main.o utils.o read_file.o newspaper_manager.o make_newspaper.o
	$(CC) main.o utils.o read_file.o newspaper_manager.o make_newspaper.o -o makeNewspaper

main.o: main.c
	$(CC) $(CFLAGS) main.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) utils.c

read_file.o: read_file.c read_file.h
	$(CC) $(CFLAGS) read_file.c

newspaper_manager.o: newspaper_manager.c newspaper_manager.h
	$(CC) $(CFLAGS) newspaper_manager.c

make_newspaper.o: make_newspaper.c make_newspaper.h
	$(CC) $(CFLAGS) make_newspaper.c

.PHONY: clean
clean: 
	rm *.o makeNewspaper