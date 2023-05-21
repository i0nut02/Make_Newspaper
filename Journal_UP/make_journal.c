#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "journal_manager.h"
#include "read_file.h"
#include "utils.h"


void alloc_paragraph(struct journal_manager *journal_man, char ***src, int *size, int n_paragraph){
    int len_words;
    int remaining_spaces;
    int num_spaces;
    int start_index = 0;
    int finish_index = 0;
    int enable_len = journal_man->num_columns * journal_man->column_length;

    // make an empty row of column which represent the division between 
    // the n-1-th paragraph to the n-th paragraph
    if (n_paragraph > 0){
        char empty_line[journal_man->column_length +1];
        memset_string_to_char(empty_line, ' ', journal_man->column_length +1);

        insert_column_space(journal_man);
        insert_row(journal_man, empty_line);
        update_row_col_index(journal_man);
    }
    
    while (start_index < *size){
        len_words = 0;
        // check if the real len of words from start_index to finish_index + (1 white space per word) -1
        // is lower than the number of characters in a row of a column of our journal
        while ( (finish_index < *size) & \
                (len_words + real_len(*(*src + finish_index)) + finish_index - start_index <= journal_man->column_length) ) {

            len_words += real_len(*(*src + finish_index));
            finish_index += 1;
    
            if (finish_index == *size){
                break;
            }
        }
        // space to fulfill of white spaces after the last word
        remaining_spaces = 0;
        // number of white spaces to insert in the row of a column
        num_spaces = journal_man->column_length - len_words;

        // is writing the last words in a paragraph so num_
        if (finish_index == *size){
            // take one white space per word -1
            num_spaces = finish_index - start_index -1;
            // add the last remaing spece to fulfill the entire row of a column after the words
            remaining_spaces = journal_man->column_length - len_words - num_spaces;
        }

        // make a string whit length equal to row of a column length +1, and copy the first word;
        char row_content[enable_len +1];
        memset(row_content, '\0', (enable_len+1) * sizeof(char));

        strcpy(row_content, *(*src + start_index));

        for (int i = start_index + 1; i < finish_index; i++){

            // make a string which represent the space between the i-1-th and the i-th word
            char spaces_words[num_spaces / (finish_index - i) +1];
            memset_string_to_char(spaces_words, ' ', num_spaces / (finish_index - i) +1);

            strcat(row_content, spaces_words);
            num_spaces -= num_spaces / (finish_index - i);

            // add the i-th word in the string which represent the row of a column string
            strcat(row_content, *(*src + i));
        }

        // add white spaces to compleate an entire row of a column
        // it add something if and only if finish = *size or *size = 1 (insert only one word)
        char spaces_words[num_spaces + remaining_spaces + 1];
        memset_string_to_char(spaces_words, ' ', num_spaces + remaining_spaces + 1);
        strcat(row_content, spaces_words);
        
        insert_column_space(journal_man);
        insert_row(journal_man, row_content);
        update_row_col_index(journal_man);
        
        start_index = finish_index;

        memset_string_to_char(row_content, '\0', strlen(row_content));
    }
}


void make_journal(struct journal_manager *journal_man, FILE *read_file){
    // represent the last char found in read_file
    int end_file;

    // number of words founded during get_paragraph_words
    int size;
    int paragraph = 0;
    char **list;
    do {
        size = 0;
        end_file = get_paragraph_words(read_file, &list, journal_man->column_length, &size);

        // if size = 0 then we found a \n or EOF like first char (after deleting the white speces)
        if (size > 0){
            alloc_paragraph(journal_man, &list, &size, paragraph);
            paragraph += 1;
        }
        free_list(list, size);
    } while (end_file != -1);

    int start = 0;
    int finish = journal_man->row_index;

    if (journal_man->col_index == journal_man->num_columns -1){
        // writes only the rows which have the row of the last column 
        // empty, if num_columns = one then will not print anything, otherwise 
        // will enter in the else if
        start = journal_man->row_index;

    }
    if (journal_man->col_index != 0){
        // write until the last row because all the rows of the col-index -1 column
        // aren't empty 
        finish = journal_man->num_rows;
    } 

    for (int i = start; i < finish; i++){
        write_row(journal_man, i);
    }
}