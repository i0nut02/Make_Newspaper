#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

extern int errno;
//TODO: check list and string allocation

int get_paragraph_words(FILE *file_pointer, char ***list, int max_lenght_str, int *size){
    int indx_word = 0;
    char ch = getc(file_pointer);

    *list = (char**)calloc(0, sizeof(char*));
    check_list_allocation(*list);

    while ((ch != '\n') & (ch != EOF)){
        int word_len = 0;
        // in the worst case there will be a word composed of real word character which 
        // ara composed of 3 chars in c notation
        char word[3 * max_lenght_str];
        memset(word, '\0', 3 * max_lenght_str * sizeof(char));

        while ((ch != ' ') & (ch != '\n') & (ch != EOF)){
            word[word_len] = ch;
            word_len++;
            ch = getc(file_pointer);

            if (word_len > 3 * max_lenght_str){
                printf("This \"word\": %s is too long, because column lenght = %d", word, max_lenght_str);
                exit(EXIT_FAILURE);
            }
        }

        // check if we can put the word in a single row of a column
        if (real_len(word) > max_lenght_str){
            printf("This word: %s is too long, because column lenght = %d", word, max_lenght_str);
            exit(EXIT_FAILURE);
        }

        // check if it is an empty word or a sequence of white spaces (only one based on our algorithm)
        if (word_len > 0){
            // resizes the list which contains the words
            *list = (char **)realloc(*list, (indx_word +1) * sizeof(char*));
            check_list_allocation(*list);

            // alloc the space to contain the word
            *(*list +indx_word) = (char *)calloc(word_len, sizeof(char));
            check_string_allocation(*(*list +indx_word));

            memset(*(*list + indx_word), '\0', sizeof(char) * word_len);
            strncpy(*(*list + indx_word), word, word_len+1);
            indx_word++;
        }

        // finishes to read the words in a paragraph
        if (ch == '\n' | ch == EOF){
            break;
        }
        ch = getc(file_pointer);
    }
    // save the number of words founded in input pointer
    *size = indx_word;

    if (ch != EOF){
        if (ferror(file_pointer) != 0){
            printf("There was an error reading the file\n");
            exit(EXIT_FAILURE);
        }
        return 0;
    }
    return -1;
}