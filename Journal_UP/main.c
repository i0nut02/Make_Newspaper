#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "journal_manager.h"
#include "make_journal.h"

int JOURNAL_LEN = 7;
char JOURNAL_WORD[9] = "_JOURNAL";
int DOTTXT_LEN = 4;
char DOTTXT[5] = ".txt";
char DOT = '.';

int errno;

void make_journal_name(char *file_name, char *journal_name){
    strcpy_while(journal_name, file_name, DOT);
    strcat(journal_name, JOURNAL_WORD);
    strcat(journal_name, DOTTXT);
}


int main(int argc, char *argv[]){
    if (argc == 1){
        printf("\"./makeJournal\" isn't enough to run this program, please check \"./makeJournal --help\"");
        return 0;
    }

    if (strcmp(argv[1], "--help") == 0){
        printf("expected input in order: \
                \n\t-text_file.txt : file with txt extention \
                \n\t-number_of_columns : int \
                \n\t-number_of_lines_of_one_page : int \
                \n\t-number_of_characters_per_column_line : int \
                \n\t-distance_beetween_columns : int \
                \nexample: \"./makeJournal example.txt 3 20 25 5\"\n");
        return 0;
    }

    if (argc != 6){
        printf("input doesn't fit the expected input, please check \"./makeJournal --help\"");
        return 0;
    }

    while (--argc >= 2){
        if (check_number_grater_zero(argv[argc]) == 0){
            printf("%s isn't a number, please check \"./makeJournal --help\"", argv[argc]);
            return 0;
        }
    }
  
    struct journal_manager journal_man;
    journal_man.num_columns = atoi(argv[2]);
    journal_man.num_rows =  atoi(argv[3]);
    journal_man.column_length = atoi(argv[4]);
    journal_man.distance_btw_columns = atoi(argv[5]);
    
    FILE *fp = fopen(argv[1], "r");
    int errnum;
    if (fp == NULL ){
        errnum = errno;
        printf("Opening the text file: %s\nerror: %s\n", argv[1], strerror(errnum));
        exit(EXIT_FAILURE);
    }

    // make Journal name 
    char journal_name[strlen(argv[1]) + JOURNAL_LEN +1];
    memset_string_to_char(journal_name, '\0', strlen(argv[1]) + JOURNAL_LEN +1);
    make_journal_name(argv[1], journal_name);

    initialize_journal(journal_name, &journal_man);

    make_journal(&journal_man, fp);
    
    // closing the read file (passed in input)
    int close_err = fclose(fp);

    if (close_err == EOF){
        errnum = errno;
        printf("Closing the text file: %s\nerror: %s\n", "ou.txt", strerror(errnum));
        exit(EXIT_FAILURE);
    }

    close_journal(&journal_man);
    printf("You can find your Journal file in the directory of your input file");
    memset_string_to_char(journal_name, '\0', strlen(argv[1]) + JOURNAL_LEN +1);
    return 0;
}