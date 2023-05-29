#ifndef MAKE_NEWSPAPER_H
#define MAKE_NEWSPAPER_H

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "newspaper_manager.h"
#include "read_file.h"
#include "utils.h"


/**
 * @brief alloc and manage the words from src to newspaper page of newspaper_man and add a 
 *          empty row of colomn if n_paragraph > 0 before managing the src list
 * 
 * @param newspaper_man 
 * @param src represent the words of a paragraph
 * @param size number of words in src
 * @param n_paragraph the paragraph which we are managing
 */
void alloc_paragraph(struct newspaper_manager *newspaper_man, char ***src, int *size, int n_paragraph);


/**
 * @brief read read_file and write the content as a newspaper
 *          based on newspaper_man values
 * 
 * @param newspaper_man 
 * @param read_file file from where we need to read from
 */
void make_newspaper(struct newspaper_manager *newspaper_man, FILE *read_file);

#endif