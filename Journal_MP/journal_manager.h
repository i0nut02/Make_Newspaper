#ifndef JOURNAL_MANAGER_H
#define JOURNAL_MANAGER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils.h"

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

/**
 * @brief create and memset a list of string which rapresent a Journal page
 *        
 * 
 * @param journal_man the struct which will contain the pointer to the journal file
 */
void initialize_journal(struct journal_manager *journal_man);


/**
 * @brief writes in fp the rows which are in the file descriptor fd
 * 
 * @param fp file pointer to Journal file
 * @param fd file descriptor read only
 */
void write_row(FILE *fp, int fd[2]);


/**
 * @brief inserts on the journal_page the src string and if we are 
 *          adding the last column text on the i-th row, write the i-th
 *          row on the journal file
 *          
 * 
 * @param journal_man 
 * @param src a string which represents a row of a column
 */
void insert_row(struct journal_manager *journal_man, char *src);


/**
 * @brief insert the space between the i-th column and the 
 *          i-1-th column if the j-th row, i and j are the column_index
 *          and row_index of the journal_man
 * 
 * @param journal_man 
 */
void insert_column_space(struct journal_manager *journal_man);


/**
 * @brief if column_index = num_columns than write in the file descriptor
 *          row_index-th row of Journal page, than increment row_index and
 *          handle the different cases of indexes
 *          
 * @param journal_man 
 * @param fd file descriptor write
 */
void update_row_col_index(struct journal_manager *journal_man, int fd[2]);

#endif
