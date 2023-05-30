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
 * @param size dimensione della lista
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
 * @param str 
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

/**
 * @brief 
 * 
 * @param src stringa da cui determinare la len
 * @param break_char carettere al quale ci dobbiamo fermare
 * @return int che rappresenta il numero di caratteri visualizati fino a trovare il break_char
 */
int strlen_while(char *src, char break_char);


/**
 * @brief chiude la lettura di fd e gestisce gli errori
 * 
 * @param fd file descriptors 
 */
void close_read_pipe(int fd[2]);


/**
 * @brief chiude la scrittura di fd e gestisce gli errori
 * 
 * @param fd file descriptors
 */
void close_write_pipe(int fd[2]);


/**
 * @brief contralla il valore del pid e gestisce gli errori
 * 
 * @param pid processo
 */
void check_fork(int pid);


/**
 * @brief uccide i tre processi e gestisce gli errori
 * 
 * @param pid1 processo
 * @param pid2 processo
 * @param pid3 processo
 */
void kill_processes(int pid1, int pid2, int pid3);
#endif