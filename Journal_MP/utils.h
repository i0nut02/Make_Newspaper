#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <ctype.h> 
#include <stdlib.h>
#include <string.h> 

/**
 * 
 * @param s
 * @return int wich rapresent the number of character in real word
 */
int real_len(char *s);


/**
 * @brief act like memset chit ch but assign '\0' as last char in dest
 * 
 * @param dest 
 * @param ch char to place in dest
 * @param size size of dest / number of character -1 to replace with ch in dest
 */
void memset_string_to_char(char *dest, char ch, int size);


/**
 * @brief free and memset the strings in the list and free itself
 * 
 * @param list list of strings
 * @param size size of list
 */
void free_list(char **list, int size);


/**
 * @brief check if list is NULL, if true than print the error type and make exit call
 * 
 * @param list 
 */
void check_list_allocation(char **list);


/**
 * @brief check if string is NULL, if true than print the error type and make exit call
 * 
 * @param str allocated with malloc/alloc
 */
void check_string_allocation(char *str);


/**
 * @brief check if str is composed of integer numbers
 * 
 * @param str 
 * @return 1 if str is composed of integer numbers otherwise 0
 */
int check_number(char *str);


/**
 * @brief copy in dest the src content while we don't encounted the break_char
 * 
 * @param dest save the characters from src
 * @param src read the single characters
 * @param break_char 
 */
void strcpy_while(char *dest, char *src, char break_char);


/**
 * @brief 
 * 
 * @param src 
 * @param break_char 
 * @return len of the src string while we find the break_char
 */
int strlen_while(char *src, char break_char);


/**
 * @brief close the fd[0] and manage the errors
 * 
 * @param fd file descriptors of a pipe
 */
void close_read_pipe(int fd[2]);


/**
 * @brief close the fd[1] and manage the errors
 * 
 * @param fd file descriptors of a pipe
 */
void close_write_pipe(int fd[2]);


/**
 * @brief check if pid < 0 than call exit(1) 
 * 
 * @param pid process
 */
void check_fork(int pid);


/**
 * @brief kill the 3 process and manage the errors
 * 
 * @param pid1 process
 * @param pid2 process
 * @param pid3 process
 */
void kill_processes(int pid1, int pid2, int pid3);
#endif