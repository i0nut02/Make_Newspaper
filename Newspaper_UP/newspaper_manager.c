#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"

int errno;


struct newspaper_manager{
    int num_columns;
    int column_length;
    int num_rows;
    int distance_btw_columns;
    int row_index;
    int col_index;
    int newspaper_row_size;
    char **newspaper_page;
    FILE *newspaper_pointer;
};


void initialize_newspaper(char *file_name, struct newspaper_manager *newspaper_man){

    newspaper_man->newspaper_pointer = fopen (file_name, "w");

    int errnum;
    if (newspaper_man->newspaper_pointer == NULL ){
        errnum = errno;
        printf("Opening the text file: %s, is happening this error: %s\n", "ou.txt", strerror(errnum));
        exit(EXIT_FAILURE);
    }

    newspaper_man->col_index = 0;
    newspaper_man->row_index = 0;

    int size_newspaper_row  = ((newspaper_man->num_columns) * newspaper_man->column_length) * 3 +
                            (newspaper_man->num_columns-1) * newspaper_man->distance_btw_columns;

    newspaper_man->newspaper_page = (char **)calloc(newspaper_man->num_rows, sizeof(char*));
    check_list_allocation(newspaper_man->newspaper_page);

    for (int i = 0; i < newspaper_man->num_rows; i++){
        *(newspaper_man->newspaper_page +i) = (char*)calloc( size_newspaper_row , sizeof(char*)); 
        check_string_allocation(*(newspaper_man->newspaper_page +i));
    }
}


void write_row(struct newspaper_manager *newspaper_man, int row_index){
    int err;

    err = fputs(newspaper_man->newspaper_page[row_index], newspaper_man->newspaper_pointer);
    if (err == EOF){
        printf("Trere was an error writing the newspaper file");
        exit(EXIT_FAILURE);
    }

    err = putc('\n', newspaper_man->newspaper_pointer);
    if (err == EOF){
        printf("Trere was an error writing the newspaper file");
        exit(EXIT_FAILURE);
    }

    memset(newspaper_man->newspaper_page[row_index], '\0', sizeof(char));
}


void next_page(FILE *file_pointer){
    int err;
    err = fputs("\n%%%\n\n", file_pointer);
    if (err == EOF){
        printf("Trere was an error writing the newspaper file");
        exit(EXIT_FAILURE);
    }
}


void insert_row(struct newspaper_manager *newspaper_man, char *src){
    if (newspaper_man->col_index == 0){
        strcpy(*(newspaper_man->newspaper_page + newspaper_man->row_index), src);

    } else {
        strcat(*(newspaper_man->newspaper_page + newspaper_man->row_index), src);
    }
}


void insert_column_space(struct newspaper_manager *newspaper_man){
    if (newspaper_man->col_index != 0){

        char spaces[newspaper_man->distance_btw_columns + 1];
        memset_string_to_char(spaces, ' ', newspaper_man->distance_btw_columns + 1);

        strcat(*(newspaper_man->newspaper_page + newspaper_man->row_index), spaces);
    }
}


void update_row_col_index(struct newspaper_manager *newspaper_man){

    if (newspaper_man->col_index == newspaper_man->num_columns -1){
        write_row(newspaper_man, newspaper_man->row_index);
    }

    if (newspaper_man->row_index == newspaper_man->num_rows -1){
        newspaper_man->row_index = 0;
        newspaper_man->col_index = (newspaper_man->col_index + 1) % newspaper_man->num_columns;

        /* se ci troviamo nella prima riga della prima colonna allora stiamo strivendo sull'inizio
            di una nuova pagina */
        if (newspaper_man->col_index == 0){
            next_page(newspaper_man->newspaper_pointer);
        }

    } else{
        newspaper_man->row_index += 1;
    }
}


void close_newspaper(struct newspaper_manager *newspaper_man){
    free_list(newspaper_man->newspaper_page, newspaper_man->num_rows);
    
    int errnum;
    int close_err = fclose(newspaper_man->newspaper_pointer);

    if (close_err == EOF){
        errnum = errno;
        printf("Closing the text file: %s, is happening this error: %s\n", "ou.txt", strerror(errnum));
        exit(EXIT_FAILURE);
    }
}