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
 * @brief alloc and manage the words from fd_1 to newspaper page of newspaper_man, and send
 *          in fd_2 the rows of newspaper page
 *          
 * 
 * @param newspaper_man 
 * @param fd_1 file descriptor only reading
 * @param fd_2 file ddescriptor only writing
 */
void alloc_paragraph(struct newspaper_manager *newspaper_man, int fd_1[2], int fd_2[2]);


/**
 * @brief open and read file_name and write the content as a list of 
 *          row in fd
 * 
 * @param file_name name of a file to read 
 * @param read_file file from where we need to read from
 * @param fd file descriptore writing only
 */
void read_paragraphs_file(char file_name[], int max_word_len, int fd[2]);

#endif