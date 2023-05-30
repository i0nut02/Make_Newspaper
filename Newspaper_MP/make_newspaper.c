#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "newspaper_manager.h"
#include "read_file.h"
#include "utils.h"

MAX_LEN_REAL_CHAR = 4

void alloc_paragraph(struct newspaper_manager *newspaper_man, int fd_1[2], int fd_2[2]){
    int len_words;
    int start_index;
    int finish_index;
    int enable_len = MAX_LEN_REAL_CHAR * newspaper_man->column_length;
    char **list;
    int size;
    int n_paragraph;

    
    if (read(fd_1[0], &size, sizeof(int)) == -1){
        printf("There was an error reading the data in the pipe\n");
        exit(EXIT_FAILURE);
    }
    
    /* si salva tutte le parole inviate da fd_1 e cerca di inserirle nel giornale, size
        rappresenta il numero di parole all'interno di un paragrafo */
    while(size != -1){

        start_index = 0;
        finish_index = 0;

        if (read(fd_1[0], &n_paragraph, sizeof(int)) == -1){
            printf("There was an error reading the data in the pipe\n");
            exit(EXIT_FAILURE);
        }
        
        // list of word in the paragraph
        list = (char**)calloc(size, sizeof(char*));
        for(int i = 0; i < size; i++){
            int word_len;
            if (read(fd_1[0], &word_len, sizeof(int)) == -1){
                printf("There was an error reading the data in the pipe\n");
                exit(EXIT_FAILURE);
            }
            list[i] = (char*)calloc(word_len, sizeof(char));
            if (read(fd_1[0], list[i], sizeof(char) * word_len) == -1){
                printf("There was an error reading the data in the pipe\n");
                exit(EXIT_FAILURE);
            }
        }

        /* inserisce una riga della colonna vuota all'interno del giornale che 
            rappresenta la divisione tra paragrafi */
        if (n_paragraph > 0){
            char empty_line[newspaper_man->column_length +1];
            memset_string_to_char(empty_line, ' ', newspaper_man->column_length +1);

            insert_column_space(newspaper_man);
            insert_row(newspaper_man, empty_line);
            update_row_col_index(newspaper_man, fd_2);
        }
        
        while (start_index < size){
            len_words = 0;
            /* determina il numero massimo di parole che possiamo inserire in una riga della colonna
                considerando la lunghezza in caratteri reali e lasciando uno spazio tra l'i-esima e (i-1)-esima parola */
            while ((finish_index < size) & \
                  ((len_words + real_len(list[finish_index]) + finish_index - start_index) <= newspaper_man->column_length) ) {
        
                len_words += real_len(list[finish_index]);
                finish_index += 1;
        
                if (finish_index == size){
                    break;
                }
            }

            /* spazio necessario per riempiere una riga della colonna colonna avendo inserito le parole e spazi*/
            int remaining_spaces = 0;

            /* numero di spazi da distribuire tra le parole*/
            int num_spaces = newspaper_man->column_length - len_words;

            /* se stiamo scrivendo l'ultima parola del paragrafo allora lo spazio tra ogni paraola
                dovrà essere uguale a 1*/
            if (finish_index == size){
                num_spaces = finish_index - start_index -1;
                /* determina gli spazi vuoti che ci devono essere tra la fine della riga della colonna
                    e l'ultima parola */
                remaining_spaces = newspaper_man->column_length - len_words - num_spaces;
            }

            
            char row_content[enable_len +1];
            memset(row_content, '\0', enable_len +1);
            strcpy(row_content, list[start_index]);

            /* inserisce lo spazio che ci dovrebbe essere tra l'i-esima parola e 
                l'(i-1)-esima parola e inserisce l'iesima parola, questo per tutte 
                le parole tranne per la prima */
            for (int i = start_index + 1; i < finish_index; i++){

                char spaces_words[num_spaces / (finish_index - i) +1];
                memset_string_to_char(spaces_words, ' ', num_spaces / (finish_index - i) +1);

                strcat(row_content, spaces_words);
                num_spaces -= num_spaces / (finish_index - i);

                strcat(row_content, list[i]);
            }

            /* aggiunge gli spazi vuori che manvano al fine di riempire la riga della colonna
                succede quanto abbiamo inserito l'ultima parola o quanto inseriamo una sola parola*/ 
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

        /* legge e si salva il numero di parole del prossimo paragrafo */
        if (read(fd_1[0], &size, sizeof(int)) == -1){
            printf("There was a problem reading the data in the pipe\n");
            exit(EXIT_FAILURE);
        }
    }

    int len;
    int start = 0;
    int finish = newspaper_man->row_index;


    /* se la colonna in cui dovremmo scrivere è uguale all'ultima disponibile
        allora dovremmo iniziare a scrivere dall'indice della riga della colonna 
        in cui dovevamo scrivere in giù
    */
    if (newspaper_man->col_index == newspaper_man->num_columns -1){ 
        start = newspaper_man->row_index;
    }

    /* se l'indice della colonna è diversa da zero allora vuol dire che abbiamo 
        sicuramente scritto la colonna con indice 0 e quindi dovremmo scrivere fino
        alla fine della riga
    */
    if (newspaper_man->col_index != 0){
        finish = newspaper_man->num_rows;
    } 

    for (int i = start; i < finish; i++){
        len = strlen(newspaper_man->newspaper_page[i]) +1;

        if (write(fd_2[1], &len, sizeof(int)) == -1){
            printf("There was an error writing the data in the pipe\n");
            exit(1);
        }
        if (write(fd_2[1], newspaper_man->newspaper_page[i], len * sizeof(char)) == -1){
            printf("There was an error writing the data in the pipe\n");
            exit(1);
        }
    }

    if (write(fd_2[1], &size, sizeof(int)) == -1){
        printf("There was an error writing the data in the pipe\n");
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
    /* rapresenta se l'ultimo carattere letto è EOF o no */
    int end_file;

    /* numero di parole trovate in un paragrafo*/
    int size;
    int paragraph = 0;
    do {
        char **list;
        size = 0;
        end_file = get_paragraph_words(fp, &list, max_word_len, &size);

        // se size è uguale a 0 allora abbiamo letto una riga vuota di parole
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

                if (write(fd[1], list[i], sizeof(char) * word_len) == -1){
                    printf("There was a problem writing the data in the pipe\n");
                    exit(EXIT_FAILURE);
                }

                free(list[i]);
            }
            paragraph += 1;
        }
        free(list);

    } while (end_file != -1);

    size = -1;
    if (write(fd[1], &size, sizeof(int)) == -1){
        printf("There was an error writing the data in the pipe\n");
        exit(EXIT_FAILURE);
    }

    int close_err = fclose(fp);

    if (close_err == EOF){
        errnum = errno;
        printf("Closing the text file: %s\nerror: %s\n", file_name, strerror(errnum));
        exit(EXIT_FAILURE);
    }
}