#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "newspaper_manager.h"
#include "make_newspaper.h"

int NEWSPAPER_LEN = 9;
char NEWSPAPER_WORD[11] = "_NEWSPAPER";
int DOTTXT_LEN = 4;
char DOTTXT[5] = ".txt";
char DOT = '.';

int errno;

void make_newspaper_name(char *file_name, char *newspaper_name){
    strcpy_while(newspaper_name, file_name, DOT);
    strcat(newspaper_name, NEWSPAPER_WORD);
    strcat(newspaper_name, DOTTXT);
}


int main(int argc, char *argv[]){
    if (argc == 1){
        printf("\"./makeNewspaper\" isn't enough to run this program, please check \"./makeNewspaper --help\"");
        return 0;
    }

    if (strcmp(argv[1], "--help") == 0){
        printf("expected input in order: \
                \n\t-text_file.txt : file with txt extention \
                \n\t-number_of_columns : int \
                \n\t-number_of_lines_of_one_page : int \
                \n\t-number_of_characters_per_column_line : int \
                \n\t-distance_beetween_columns : int \
                \nexample: \"./makeNewspaper example.txt 3 20 25 5\"\n");
        return 0;
    }

    if (argc != 6){
        printf("input doesn't fit the expected input, please check \"./makeNewspaper --help\"");
        return 0;
    }

    while (--argc >= 2){
        if (check_number_grater_zero(argv[argc]) == 0){
            printf("%s isn't a number, please check \"./makenewspaper --help\"", argv[argc]);
            return 0;
        }
    }
  
    struct newspaper_manager newspaper_man;
    newspaper_man.num_columns = atoi(argv[2]);
    newspaper_man.num_rows =  atoi(argv[3]);
    newspaper_man.column_length = atoi(argv[4]);
    newspaper_man.distance_btw_columns = atoi(argv[5]);
    
    FILE *fp = fopen(argv[1], "r");
    int errnum;
    if (fp == NULL ){
        errnum = errno;
        printf("Opening the text file: %s\nerror: %s\n", argv[1], strerror(errnum));
        exit(EXIT_FAILURE);
    }

    // make newspaper name 
    char newspaper_name[strlen(argv[1]) + NEWSPAPER_LEN +1];
    memset_string_to_char(newspaper_name, '\0', strlen(argv[1]) + NEWSPAPER_LEN +1);
    make_newspaper_name(argv[1], newspaper_name);

    initialize_newspaper(newspaper_name, &newspaper_man);

    make_newspaper(&newspaper_man, fp);
    
    // closing the read file (passed in input)
    int close_err = fclose(fp);

    if (close_err == EOF){
        errnum = errno;
        printf("Closing the text file: %s\nerror: %s\n", "ou.txt", strerror(errnum));
        exit(EXIT_FAILURE);
    }

    close_newspaper(&newspaper_man);
    printf("You can find your newspaper file in the directory of your input file\n");
    memset_string_to_char(newspaper_name, '\0', strlen(argv[1]) + NEWSPAPER_LEN +1);
    return 0;
}