#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"

extern int errno;


struct journal_manager{
    int num_columns;
    int column_length;
    int num_rows;
    int distance_btw_columns;
    int row_index;
    int col_index;
    int journal_row_size;
    char **journal_page;
    FILE *journal_pointer;
};


void initialize_journal(char *name, struct journal_manager *journal_man){
    // x make something if the file already exist
    journal_man->journal_pointer = fopen (name, "w");

    int errnum;
    if (journal_man->journal_pointer == NULL ){
        errnum = errno;
        printf("Opening the text file: %s, is happening this error: %s\n", "ou.txt", strerror(errnum));
        exit(EXIT_FAILURE);
    }

    journal_man->col_index = 0;
    journal_man->row_index = 0;

    // every row of the journal have in the worst case:
    //  - num_columns - 1  * journal_man->distance_btw_columns empty spaces 
    //  - there can be chars that need 3 bytes to be represented
    int size_journal_row  = ((journal_man->num_columns) * journal_man->column_length) * 3 +
                            (journal_man->num_columns-1) * journal_man->distance_btw_columns;

    journal_man->journal_row_size = size_journal_row;

    journal_man->journal_page = (char **)calloc(journal_man->num_rows, sizeof(char*));
    check_list_allocation(journal_man->journal_page);

    for (int i = 0; i < journal_man->num_rows; i++){
        *(journal_man->journal_page +i) = (char*)calloc( size_journal_row , sizeof(char*)); 
        check_string_allocation(*(journal_man->journal_page +i));

        memset(*(journal_man->journal_page +i), '\0', size_journal_row * sizeof(char));
    }
}


void write_row(struct journal_manager *journal_man, int row_index){
    int err;
    err = fputs(journal_man->journal_page[row_index], journal_man->journal_pointer);
    if (err == EOF){
        printf("Trere was an error writing the Journal file");
        exit(EXIT_FAILURE);
    }

    err = putc('\n', journal_man->journal_pointer);
    if (err == EOF){
        printf("Trere was an error writing the Journal file");
        exit(EXIT_FAILURE);
    }
    memset(journal_man->journal_page[row_index], '\0', sizeof(char));
}


void next_page(FILE *file_pointer){
    int err;
    err = fputs("\n%%% \n\n", file_pointer);
    if (err == EOF){
        printf("Trere was an error writing the Journal file");
        exit(EXIT_FAILURE);
    }
}


void insert_row(struct journal_manager *journal_man, char *src){
    if (journal_man->col_index == 0){
        strcpy(*(journal_man->journal_page + journal_man->row_index), src);

    } else {
        strcat(*(journal_man->journal_page + journal_man->row_index), src);
    }
}


void insert_column_space(struct journal_manager *journal_man){
    if (journal_man->col_index != 0){

        char spaces[journal_man->distance_btw_columns + 1];
        memset_string_to_char(spaces, ' ', journal_man->distance_btw_columns + 1);

        strcat(*(journal_man->journal_page + journal_man->row_index), spaces);
    }
}


void update_row_col_index(struct journal_manager *journal_man){

    if (journal_man->col_index == journal_man->num_columns -1){
        int row_indx =  journal_man->row_index;
        write_row(journal_man, row_indx);
    }

    if (journal_man->row_index == journal_man->num_rows -1){
        journal_man->row_index = 0;
        journal_man->col_index = (journal_man->col_index + 1) % journal_man->num_columns;
        if (journal_man->col_index == 0){
            next_page(journal_man->journal_pointer);
        }

    } else{
        journal_man->row_index += 1;
    }
}


void close_journal(struct journal_manager *journal_man){
    free_list(journal_man->journal_page, journal_man->num_rows);
    
    int errnum;
    int close_err = fclose(journal_man->journal_pointer);

    if (close_err == EOF){
        errnum = errno;
        printf("Closing the text file: %s, is happening this error: %s\n", "ou.txt", strerror(errnum));
        exit(EXIT_FAILURE);
    }
}