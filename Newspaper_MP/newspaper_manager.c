#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils.h"

extern int errno;

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


void initialize_newspaper(struct newspaper_manager *newspaper_man){

    newspaper_man->col_index = 0;
    newspaper_man->row_index = 0;

    int size_newspaper_row  = ((newspaper_man->num_columns) * newspaper_man->column_length) * 4 +
                            (newspaper_man->num_columns-1) * newspaper_man->distance_btw_columns;

    newspaper_man->newspaper_page = (char **)calloc(newspaper_man->num_rows, sizeof(char*));
    check_list_allocation(newspaper_man->newspaper_page);

    for (int i = 0; i < newspaper_man->num_rows; i++){
        *(newspaper_man->newspaper_page +i) = (char*)calloc( size_newspaper_row , sizeof(char)); 
        check_string_allocation(*(newspaper_man->newspaper_page +i));
    }
}


void write_rows(FILE *fp, int fd[2]){
    int size;
    int err;
    int first_row = 0;

    if (read(fd[0], &size, sizeof(int)) == -1){
        printf("There was an error reading the data in the pipe\n");
        exit(EXIT_FAILURE);
    }

    while (size != -1){

        if (first_row != 0){
            err = putc('\n', fp);
            if (err == EOF){
                printf("Trere was an error writing the newspaper file\n");
                exit(EXIT_FAILURE);
            }
        }

        char row[size];
        if (read(fd[0], row, size * sizeof(char)) == -1) {
            printf("There was an error reading the data in the pipe\n");
            exit(EXIT_FAILURE);
        }

        err = fputs(row, fp);
        if (err == EOF){
            printf("Trere was an error writing the newspaper file\n");
            exit(EXIT_FAILURE);
        }

        if (read(fd[0], &size, sizeof(int)) == -1){
            printf("There was an error reading the data in the pipe\n");
            exit(EXIT_FAILURE);
        }
        first_row += 1;
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


void update_row_col_index(struct newspaper_manager *newspaper_man, int fd[2]){

    if (newspaper_man->col_index == newspaper_man->num_columns -1){
        int row_indx =  newspaper_man->row_index;
        int size = strlen_while(newspaper_man->newspaper_page[row_indx], '\0');

        if (write(fd[1], &size, sizeof(int)) == -1){
            printf("There was an error writing the data in the pipe\n");
            exit(EXIT_FAILURE);
        }
        if (write(fd[1], newspaper_man->newspaper_page[row_indx], size * sizeof(char)) == -1){
            printf("There was an error writing the data in the pipe\n");
            exit(EXIT_FAILURE);
        }
        memset_string_to_char(newspaper_man->newspaper_page[row_indx], '\0', strlen(newspaper_man->newspaper_page[row_indx]) * sizeof(char));
    }

    if (newspaper_man->row_index == newspaper_man->num_rows -1){
        newspaper_man->row_index = 0;
        newspaper_man->col_index = (newspaper_man->col_index + 1) % newspaper_man->num_columns;

        if (newspaper_man->col_index == 0){
            int size = 7;
            char next_p[7] = "\n%%%\n";
            
            if (write(fd[1], &size, sizeof(int)) == -1){
                printf("There was a problem writing the data in the pipe\n");
                exit(EXIT_FAILURE);
            }
            if (write(fd[1], next_p, size * sizeof(char)) == -1){
                printf("There was a problem writing the data in the pipe\n");
                exit(EXIT_FAILURE);
            }
        }

    } else{
        newspaper_man->row_index += 1;
    }
}