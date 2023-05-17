#ifndef READ_FILE_H
#define READ_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// return -1 if we finish to read the file
// hendling of error allocation
/**
 * @brief read from file_pointer while we encounter a "\n", save the words founded
 *          in list and increment size every word founded, raise an error if exist a
 *          word with lenght > max_lenght_str
 * 
 * @param file_pointer in read mode
 * @param list a memory address of a double-pointed char 
 * @param max_lenght_str max real len of a word
 * @param size a memory address where will be saved the number of words founded
 * @return 0 if found EOF like last char otherwise -1
 */
int get_paragraph_words(FILE *file_pointer, char ***list, int max_lenght_str, int *size);

#endif


