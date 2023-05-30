#ifndef NEWSPAPER_MANAGER_H
#define NEWSPAPER_MANAGER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
 * @brief crea un file pointer su file_name e inizializza una lista di
 *          stringhe che rappresenta la pagina di giornale, e vengono valvati nella struttura
 * 
 * @param file_name nome del file di output
 * @param newspaper_man struttura che nonterrà il file pointer e la lista di stringhe (pagina di giornale)
 */
void initialize_newspaper(char *file_name, struct newspaper_manager *newspaper_man);


/**
 * @brief scrive la row_index-esima riga della pagina di giornale nel file 
 *          che contiene il giornale
 * 
 * @param newspaper_man strttura contenente le informazioni del giornale e il suo formato
 * @param row_index indice della riga da scrivere
 */
void write_row(struct newspaper_manager *newspaper_man, int row_index);


/**
 * @brief scrive la divisione tra due pagine all'interno del file
 * 
 * @param file_pointer file in modalità di scrittura
 */
void next_page(FILE *file_pointer);


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
 *          numero di colonne allora, chiama la funzione "write_row" con
 *          indice non aggiornato della riga
 *          
 * @param newspaper_man struttura che contiene il formato del giornala e la pagina di giornale 
 */
void update_row_col_index(struct newspaper_manager *newspaper_man);


/**
 * @brief chiude il file pointer che rappresenta il giornale
 * 
 * @param newspaper_man struttura che contiene il formato del giornala e la pagina di giornale 
 */
void close_newspaper(struct newspaper_manager *newspaper_man);


#endif
