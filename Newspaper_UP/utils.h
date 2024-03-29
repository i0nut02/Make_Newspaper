#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <ctype.h> 
#include <stdlib.h>
#include <string.h> 

/**
 * 
 * @param s
 * @return intero che rappresenta la lunghezza in caratteri reali anziché utf-8
 */
int real_len(char *s);


/**
 * @brief funziona come memset ma aggiunge come ultimo carattere '\0'
 * 
 * @param dest 
 * @param ch carattere da inserire in dest
 * @param size numero di caratteri da rimpiazzare con ch in dest (lunghezza dest -1)
 */
void memset_string_to_char(char *dest, char ch, int size);


/**
 * @brief libera e memsetta a '\0' le stringhe della lista, e la lista stessa
 * 
 * @param list lista di stringhe
 * @param size lunghezza della lista
 */
void free_list(char **list, int size);


/**
 * @brief controlla se list è NULL, se si chiude il programma con errore
 * 
 * @param list 
 */
void check_list_allocation(char **list);


/**
 * @brief controlla se str è NULL, se si chiude il programma con errore
 * 
 * @param str allocated with malloc/alloc
 */
void check_string_allocation(char *str);


/**
 * @brief controlla se str è composta da numeri, se si controlla se è maggiore di 0
 * 
 * @param str 
 * @return 1 se str è un numero maggiore di 0, altrimenti 0
 */
int check_number_grater_zero(char *str);


/**
 * @brief copia in dest i caratteri di src fino a quando non incontra il break_char in src
 * 
 * @param dest scrittura dei caratteri di src
 * @param src lettura dei caratteri
 * @param break_char 
 */
void strcpy_while(char *dest, char *src, char break_char);

#endif