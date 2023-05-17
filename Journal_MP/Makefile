CC := gcc

CFLAGS := -Wall -c 

makeJournal: main.o utils.o read_file.o journal_manager.o make_journal.o
	$(CC) main.o utils.o read_file.o journal_manager.o make_journal.o -o makeJournal

main.o: main.c
	$(CC) $(CFLAGS) main.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) utils.c

read_file.o: read_file.c read_file.h
	$(CC) $(CFLAGS) read_file.c

journal_manager.o: journal_manager.c journal_manager.h
	$(CC) $(CFLAGS) journal_manager.c

make_journal.o: make_journal.c make_journal.h
	$(CC) $(CFLAGS) make_journal.c

.PHONY: clean
clean: 
	rm *.o makeJournal