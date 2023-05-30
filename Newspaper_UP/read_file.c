#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

extern int errno;

int get_paragraph_words(FILE *file_pointer, char ***list, int max_lenght_str, int *size){
    int indx_word = 0;
    int word_len = 0;
    char ch = getc(file_pointer);
    char word[4 * max_lenght_str];

    *list = (char**)calloc(0, sizeof(char*));
    check_list_allocation(*list);

    while ((ch != '\n') & (ch != EOF)){
        word_len = 0; 

        memset(word, '\0', 4 * max_lenght_str * sizeof(char));
        
        while ((ch != ' ') & (ch != '\n') & (ch != EOF) & (ch != '\t')){
            word[word_len] = ch;
            word_len++;
            ch = getc(file_pointer);

            if (word_len > 4 * max_lenght_str){
                printf("This \"word\": %s is too long, because column lenght = %d", word, max_lenght_str);
                exit(EXIT_FAILURE);
            }
        }
        
        /* controlla se la parola riesce ad esser inserita in una riga di una colonna del giornale */
        if (real_len(word) > max_lenght_str){
            printf("This word: %s is too long, because column lenght = %d", word, max_lenght_str);
            exit(EXIT_FAILURE);
        }

        /* controlla se è stata letta una seguenza di caratteri non appartenente solo a [\n, ' ', EOF, \t] */
        if (word_len > 0){

            /* dobbiamo salvarci una parola in più allora aumentiamo la capacità della lista di parole */
            *list = (char **)realloc(*list, (indx_word+1) * sizeof(char*));
            check_list_allocation(*list);
            
            *(*list +indx_word) = (char*)calloc(word_len +1, sizeof(char));
            check_string_allocation(*(*list +indx_word));
            
            strncpy(*(*list + indx_word), word, word_len);
            indx_word++;
        }

        /* smette di leggere le parole di un paragrafo */
        if (ch == '\n' | ch == EOF){
            break;
        }
        ch = getc(file_pointer);
    }
    
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