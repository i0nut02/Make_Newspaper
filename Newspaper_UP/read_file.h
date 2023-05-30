#ifndef READ_FILE_H
#define READ_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * @brief legge le parole che trova nel file pointer e le salva in list, fino a quando
 *          non incontra un '\n' o EOF che rappresenta la fine di un paragrafo, e si salva in 
 *          size il numero di parole trovate, se una parola contiene più caratteri di max_lenght_str
 *          viene ritornato un errore
 * 
 * @param file_pointer input file da cui leggere le parole
 * @param list un indirizzo di memoria dove è salvata una lista di strinnghe (non allocata)
 * @param max_lenght_str lunghezza massima di una stringa (non utf-8)
 * @param size un indirizzo di memoria dove si può salvare un intero
 * @return 0 se ha trovato come ultimo carattere EOF altrimenti -1
 */
int get_paragraph_words(FILE *file_pointer, char ***list, int max_lenght_str, int *size);

#endif


