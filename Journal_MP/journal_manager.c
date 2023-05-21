#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils.h"

extern int errno;


struct {
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


void initialize_journal(struct journal_manager *journal_man){

    journal_man->col_index = 0;
    journal_man->row_index = 0;

    // every row of the journal have in the worst case:
    //  - num_columns - 1  * journal_man->distance_btw_columns empty spaces 
    //  - there can be real chars that need 3 chars to be represented
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


void write_row(FILE *fp, int fd[2]){
    int size;
    int err;
    int first_row = 0;

    if (read(fd[0], &size, sizeof(int)) == -1){
        printf("There was a problem reading the data in the pipe\n");
        exit(1);
    }

    while (size != -1){

        if (first_row != 0){
            err = putc('\n', fp);
            if (err == EOF){
                printf("Trere was an error writing the Journal file");
                exit(EXIT_FAILURE);
            }
        }

        char row[size];
        read(fd[0], row, size * sizeof(char));

        err = fputs(row, fp);
        if (err == EOF){
            printf("Trere was an error writing the Journal file");
            exit(EXIT_FAILURE);
        }

        if (read(fd[0], &size, sizeof(int)) == -1){
            printf("There was a problem reading the data in the pipe\n");
            exit(1);
        }
        first_row += 1;
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


void update_row_col_index(struct journal_manager *journal_man, int fd[2]){

    if (journal_man->col_index == journal_man->num_columns -1){
        int row_indx =  journal_man->row_index;
        int size = strlen_while(journal_man->journal_page[row_indx], '\0');
        if (write(fd[1], &size, sizeof(int)) == -1){
            printf("There was a problem writing the data in the pipe\n");
            exit(1);
        }
        if (write(fd[1], journal_man->journal_page[row_indx], size * sizeof(char)) == -1){
            printf("There was a problem writing the data in the pipe\n");
            exit(1);
        }
        memset_string_to_char(journal_man->journal_page[row_indx], '\0', strlen(journal_man->journal_page[row_indx]) * sizeof(char));
    }

    if (journal_man->row_index == journal_man->num_rows -1){
        journal_man->row_index = 0;
        journal_man->col_index = (journal_man->col_index + 1) % journal_man->num_columns;
        if (journal_man->col_index == 0){
            int size = 7;
            char next_p[7] = "\n %%% \n";
            if (write(fd[1], &size, sizeof(int)) == -1){
                printf("There was a problem writing the data in the pipe\n");
                exit(1);
            }
            if (write(fd[1], next_p, size * sizeof(char)) == -1){
                printf("There was a problem writing the data in the pipe\n");
                exit(1);
            }
        }

    } else{
        journal_man->row_index += 1;
    }
}