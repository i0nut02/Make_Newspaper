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
 * @brief crea e azzera una lista di stringhe che rappresenta una pagina di giornale, e 
 *        inzializza gli indici di scrittura riga e colonna su 0, 0 
 * 
 * @param newspaper_man la struttura che conterrà la pagina di giornale e le varie informazioni
 */
void initialize_newspaper(struct newspaper_manager *newspaper_man);


/**
 * @brief scrive su fp le righe che riceve tramite fd
 * 
 * @param fp file pointer su file di output
 * @param fd file descriptor solo lettura
 */
void write_rows(FILE *fp, int fd[2]);


/**
 * @brief aggiunge src alla pagina di giornale nelli indice di riga e colonna
 *          che trova in newspaper_man
 * 
 * @param newspaper_man struttura che contiene il formato del giornala e la pagina di giornale
 * @param src una stringa che rappresenta una riga di una colonna
 */
void insert_row(struct newspaper_manager *newspaper_man, char *src);


/**
 * @brief inserisce lo spazio tra colonne tra la j-esima riga della i-esima colonna
 *          e la j-esima riga della (i-1)-esima colonna
 * 
 * @param newspaper_man struttura che contiene il formato del giornala e la pagina di giornale
 */
void insert_column_space(struct newspaper_manager *newspaper_man);


/**
 * @brief aggiorna gli indici delle righe e colonne dove scrivere, e se
 *          si accorge che il vecchio indice di colonna è "uguale -1" al
 *          numero di colonne allora, invia tutta la riga precedente (rispetto a quella aggiornata)
 *          in fd
 *          
 * @param newspaper_man struttura che contiene il formato del giornala e la pagina di giornale
 * @param fd file descriptor solo scrittura
 */
void update_row_col_index(struct newspaper_manager *newspaper_man, int fd[2]);

#endif
