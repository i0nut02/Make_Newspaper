#ifndef NEWSPAPER_MANAGER_H
#define NEWSPAPER_MANAGER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils.h"

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

/**
 * @brief create and memset a list of string which rapresent a newspaper page
 *        
 * 
 * @param newspaper_man the struct which will contain the pointer to the newspaper file
 */
void initialize_newspaper(struct newspaper_manager *newspaper_man);


/**
 * @brief writes in fp the rows which are in the file descriptor fd
 * 
 * @param fp file pointer to newspaper file
 * @param fd file descriptor read only
 */
void write_row(FILE *fp, int fd[2]);


/**
 * @brief inserts on the newspaper_page the src string and if we are 
 *          adding the last column text on the i-th row, write the i-th
 *          row on the newspaper file
 *          
 * 
 * @param newspaper_man 
 * @param src a string which represents a row of a column
 */
void insert_row(struct newspaper_manager *newspaper_man, char *src);


/**
 * @brief insert the space between the i-th column and the 
 *          i-1-th column if the j-th row, i and j are the column_index
 *          and row_index of the newspaper_man
 * 
 * @param newspaper_man 
 */
void insert_column_space(struct newspaper_manager *newspaper_man);


/**
 * @brief if column_index = num_columns than write in the file descriptor
 *          row_index-th row of newspaper page, than increment row_index and
 *          handle the different cases of indexes
 *          
 * @param newspaper_man 
 * @param fd file descriptor write
 */
void update_row_col_index(struct newspaper_manager *newspaper_man, int fd[2]);

#endif
