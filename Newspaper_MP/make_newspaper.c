#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "newspaper_manager.h"
#include "read_file.h"
#include "utils.h"


void alloc_paragraph(struct newspaper_manager *newspaper_man, int fd_1[2], int fd_2[2]){
    int len_words;
    int start_index;
    int finish_index;
    int enable_len = newspaper_man->num_columns * newspaper_man->column_length;
    char **list;
    int size;
    int n_paragraph;

    
    if (read(fd_1[0], &size, sizeof(int)) == -1){
        printf("There was a problem reading the data in the pipe\n");
        exit(EXIT_FAILURE);
    }
    while(size != -1){

        start_index = 0;
        finish_index = 0;

        if (read(fd_1[0], &n_paragraph, sizeof(int)) == -1){
            printf("There was a problem reading the data in the pipe\n");
            exit(EXIT_FAILURE);
        }
        
        // list of word in the paragraph
        list = (char**)calloc(size, sizeof(char*));
        for(int i = 0; i < size; i++){
            int word_len;
            if (read(fd_1[0], &word_len, sizeof(int)) == -1){
                printf("There was a problem reading the data in the pipe\n");
                exit(1);
            }
            list[i] = (char*)calloc(word_len, sizeof(char));
            if (read(fd_1[0], list[i], sizeof(char) * word_len) == -1){
                printf("There was a problem reading the data in the pipe\n");
                exit(EXIT_FAILURE);
            }
        }

        // puts an empty row of a column between the i-th and i-1-th paragraph
        if (n_paragraph > 0){
            char empty_line[newspaper_man->column_length +1];
            memset_string_to_char(empty_line, ' ', newspaper_man->column_length +1);

            insert_column_space(newspaper_man);
            insert_row(newspaper_man, empty_line);
            update_row_col_index(newspaper_man, fd_2);
        }
        
        while (start_index < size){
            len_words = 0;
            // check if the real len of words from start_index to finish_index + (1 white space per word) -1
            // is lower than the number of characters in a row of a column of our newspaper
            while ((finish_index < size) & \
                  ((len_words + real_len(list[finish_index]) + finish_index - start_index) <= newspaper_man->column_length) ) {
        
                len_words += real_len(list[finish_index]);
                finish_index += 1;
        
                if (finish_index == size){
                    break;
                }
            }

            // space to fulfill of white spaces after the last word
            int remaining_spaces = 0;

            // number of white spaces to insert in the row of a column
            int num_spaces = newspaper_man->column_length - len_words;

            // is writing the last words in a paragraph so num_
            if (finish_index == size){
                // take one white space per word -1
                num_spaces = finish_index - start_index -1;
                // add the last remaing spece to fulfill the entire row of a column after the words
                remaining_spaces = newspaper_man->column_length - len_words - num_spaces;
            }

            // make a string whit length equal to row of a column length +1, and copy the first word;
            char row_content[enable_len +1];
            memset(row_content, '\0', enable_len +1);
            strcpy(row_content, list[start_index]);

            for (int i = start_index + 1; i < finish_index; i++){

                // make a string which represent the space between the i-1-th and the i-th word
                char spaces_words[num_spaces / (finish_index - i) +1];
                memset_string_to_char(spaces_words, ' ', num_spaces / (finish_index - i) +1);

                strcat(row_content, spaces_words);
                num_spaces -= num_spaces / (finish_index - i);

                // add the i-th word in the string which represent the row of a column string
                strcat(row_content, list[i]);
            }

            // add white spaces to compleate an entire row of a column
            // it add something if and only if finish = size
            char spaces_words[num_spaces + remaining_spaces + 1];
            memset_string_to_char(spaces_words, ' ', num_spaces + remaining_spaces + 1);
            
            strcat(row_content, spaces_words);
            
            insert_column_space(newspaper_man);
            insert_row(newspaper_man, row_content);
            update_row_col_index(newspaper_man, fd_2);
            start_index = finish_index;

            memset_string_to_char(row_content, '\0', strlen(row_content));
        }
        free_list(list, size);

        // read the new number of words of next paragraph
        if (read(fd_1[0], &size, sizeof(int)) == -1){
            printf("There was a problem reading the data in the pipe\n");
            exit(EXIT_FAILURE);
        }
    }

    int len;
    int start = 0;
    int finish = newspaper_man->row_index;


    if (newspaper_man->col_index == newspaper_man->num_columns -1){
        // writes only the rows which have the row of the last column 
        // empty, if num_columns = one then will not print anything, otherwise 
        // will enter in the else if
        start = newspaper_man->row_index;

    }
    if (newspaper_man->col_index != 0){
        // write until the last row because all the rows of the col-index -1 column
        // aren't empty 
        finish = newspaper_man->num_rows;
    } 

    for (int i = start; i < finish; i++){
        len = strlen(newspaper_man->newspaper_page[i]) +1;
        if (len == 0){
            break;
        }

        if (write(fd_2[1], &len, sizeof(int)) == -1){
            printf("There was a problem writing the data in the pipe\n");
            exit(1);
        }
        if (write(fd_2[1], newspaper_man->newspaper_page[i], len * sizeof(char)) == -1){
            printf("There was a problem writing the data in the pipe\n");
            exit(1);
        }
    }

    if (write(fd_2[1], &size, sizeof(int)) == -1){
        printf("There was a problem writing the data in the pipe\n");
        exit(1);
    }

}


void read_paragraphs_file(char file_name[], int max_word_len, int fd[2]){

    FILE *fp = fopen(file_name, "r");
    int errnum;
    if (fp == NULL ){
        errnum = errno;
        printf("Opening the text file: %s\nerror: %s\n", file_name, strerror(errnum));
        exit(EXIT_FAILURE);
    }
    // represent the last char found in read_file
    int end_file;

    // number of words founded during get_paragraph_words
    int size;
    int paragraph = 0;
    do {
        char **list;
        size = 0;
        end_file = get_paragraph_words(fp, &list, max_word_len, &size);

        // if size = 0 then we found a \n or EOF like first 
        // char (after deleting the white speces)
        if (size > 0){
            if (write(fd[1], &size, sizeof(int)) == -1){
                printf("There was a problem writing the data in the pipe\n");
                exit(EXIT_FAILURE);
            }

            if (write(fd[1], &paragraph, sizeof(int)) == -1){
                printf("There was a problem writing the data in the pipe\n");
                exit(EXIT_FAILURE);
            }

            for(int i= 0; i < size; i++){
                int word_len = strlen(list[i]) +1;
                
                if (write(fd[1], &word_len, sizeof(int)) == -1){
                    printf("There was a problem writing the data in the pipe\n");
                    exit(EXIT_FAILURE);
                }
                //printf("read process, i send : %d\n", word_len);
                if (write(fd[1], list[i], sizeof(char) * word_len) == -1){
                    printf("There was a problem writing the data in the pipe\n");
                    exit(EXIT_FAILURE);
                }
                //printf("read process, i send : %s\n", list[i]);
                free(list[i]);
            }
            paragraph += 1;
        }
        free(list);
    } while (end_file != -1);
    size = -1;
    if (write(fd[1], &size, sizeof(int)) == -1){
        printf("There was a problem writing the data in the pipe\n");
        exit(EXIT_FAILURE);
    }

    int close_err = fclose(fp);

    if (close_err == EOF){
        errnum = errno;
        printf("Closing the text file: %s\nerror: %s\n", "ou.txt", strerror(errnum));
        exit(EXIT_FAILURE);
    }
}