#ifndef MAKE_NEWSPAPER_H
#define MAKE_NEWSPAPER_H

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "newspaper_manager.h"
#include "read_file.h"
#include "utils.h"


/**
 * @brief alloca e giustifica le parole del paragrafo in src all'interno della pagina di 
 *          giornale 
 * 
 * @param newspaper_man struttura contenente il formato del giornale
 * @param src rapresenta le parole di un paragrafo
 * @param size numero di parole in src
 * @param n_paragraph il numero di paragrafi che abbiamo gestito (numero di paragrafo)
 */
void alloc_paragraph(struct newspaper_manager *newspaper_man, char ***src, int *size, int n_paragraph);


/**
 * @brief legge le parole di ogni paragrafo all'interno del read_file e chiama
 *          alloc_paragraph per ogni paragrafo
 * 
 * @param newspaper_man struttura contenente il formato del giornale
 * @param read_file file pointer al file da leggere
 */
void make_newspaper(struct newspaper_manager *newspaper_man, FILE *read_file);

#endif