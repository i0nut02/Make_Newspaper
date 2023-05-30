#ifndef MAKE_NEWSPAPER_H
#define MAKE_NEWSPAPER_H

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "newspaper_manager.h"
#include "read_file.h"
#include "utils.h"


/**
 * @brief alloca e giustifica i paragrafi ottenuti da fd_1 nel giornale, inoltre
 *          inivia su fd_2 le righe del giornale da scrivere 
 * 
 * @param newspaper_man struttura contenente il formato del giornale
 * @param fd_1 file descriptor solo lettura
 * @param fd_2 file ddescriptor solo scrittura
 */
void alloc_paragraph(struct newspaper_manager *newspaper_man, int fd_1[2], int fd_2[2]);


/**
 * @brief apre file_name e si salva le parole di ogni paragrafo per poi
 *          inviarle tramite fd
 *
 * @param file_name nome del file da leggere
 * @param max_word_len massima lunghezza di caratteri "reali" da poter inserire in una riga della colonna
 * @param fd file descriptore solo scrittura
 */
void read_paragraphs_file(char file_name[], int max_word_len, int fd[2]);

#endif