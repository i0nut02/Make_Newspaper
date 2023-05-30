#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "newspaper_manager.h"
#include "read_file.h"
#include "utils.h"


void alloc_paragraph(struct newspaper_manager *newspaper_man, char ***src, int *size, int n_paragraph){
    int len_words;
    int remaining_spaces;
    int num_spaces;
    int start_index = 0;
    int finish_index = 0;
    int enable_len = 4 * newspaper_man->column_length;

    /* inserisce una riga della colonna vuota all'interno del giornale che 
            rappresenta la divisione tra paragrafi */
    if (n_paragraph > 0){
        char empty_line[newspaper_man->column_length +1];
        memset_string_to_char(empty_line, ' ', newspaper_man->column_length +1);

        insert_column_space(newspaper_man);
        insert_row(newspaper_man, empty_line);
        update_row_col_index(newspaper_man);
    }
    
    while (start_index < *size){
        len_words = 0;
        /* determina il numero massimo di parole che possiamo inserire in una riga della colonna
            considerando la lunghezza in caratteri reali e lasciando uno spazio tra l'i-esima e (i-1)-esima parola */
        while ( (finish_index < *size) & \
                (len_words + real_len(*(*src + finish_index)) + finish_index - start_index <= newspaper_man->column_length) ) {

            len_words += real_len(*(*src + finish_index));
            finish_index += 1;
    
            if (finish_index == *size){
                break;
            }
        }
        /* spazio necessario per riempiere una riga della colonna colonna avendo inserito le parole e spazi*/
        remaining_spaces = 0;

        /* numero di spazi da distribuire tra le parole*/
        num_spaces = newspaper_man->column_length - len_words;

        /* se stiamo scrivendo l'ultima parola del paragrafo allora lo spazio tra ogni paraola
            dovrà essere uguale a 1*/
        if (finish_index == *size){
            num_spaces = finish_index - start_index -1;

            /* determina gli spazi vuoti che ci devono essere tra l'ultima parola e 
                la fine della riga della colonna */
            remaining_spaces = newspaper_man->column_length - len_words - num_spaces;
        }

        char row_content[enable_len +1];
        memset(row_content, '\0', (enable_len+1) * sizeof(char));
        strcpy(row_content, *(*src + start_index));

        /* inserisce lo spazio che ci dovrebbe essere tra l'i-esima parola e 
            l'(i-1)-esima parola e inserisce l'iesima parola, questo per tutte 
            le parole tranne per la prima */
        for (int i = start_index + 1; i < finish_index; i++){

            char spaces_words[num_spaces / (finish_index - i) +1];
            memset_string_to_char(spaces_words, ' ', num_spaces / (finish_index - i) +1);

            strcat(row_content, spaces_words);
            num_spaces -= num_spaces / (finish_index - i);

            strcat(row_content, *(*src + i));
        }

        /* aggiunge gli spazi vuori che manvano al fine di riempire la riga della colonna
            succede quanto abbiamo inserito l'ultima parola o quanto inseriamo una sola parola*/ 
        char spaces_words[num_spaces + remaining_spaces + 1];
        memset_string_to_char(spaces_words, ' ', num_spaces + remaining_spaces + 1);
        strcat(row_content, spaces_words);
        
        insert_column_space(newspaper_man);
        insert_row(newspaper_man, row_content);
        update_row_col_index(newspaper_man);
        
        start_index = finish_index;

        memset_string_to_char(row_content, '\0', strlen(row_content));
    }
}


void make_newspaper(struct newspaper_manager *newspaper_man, FILE *read_file){
    /* rapresenta se l'ultimo carattere letto è EOF o no */
    int end_file;

    /* numero di parole trovate in un paragrafo*/
    int size;
    int paragraph = 0;
    char **list;
    do {
        size = 0;
        end_file = get_paragraph_words(read_file, &list, newspaper_man->column_length, &size);

        /* se size è uguale a 0 allora abbiamo letto una riga vuota di parole */
        if (size > 0){
            alloc_paragraph(newspaper_man, &list, &size, paragraph);
            paragraph += 1;
        }
        free_list(list, size);
    } while (end_file != -1);

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
        write_row(newspaper_man, i);
    }
}