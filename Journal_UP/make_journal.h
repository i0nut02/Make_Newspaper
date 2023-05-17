#ifndef MAKE_JOURNAL_H
#define MAKE_JOURNAL_H

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "journal_manager.h"
#include "read_file.h"
#include "utils.h"


/**
 * @brief alloc and manage the words from src to Journal page of journal_man and add a 
 *          empty row of colomn if n_paragraph > 0 before managing the src list
 * 
 * @param journal_man 
 * @param src represent the words of a paragraph
 * @param size number of words in src
 * @param n_paragraph the paragraph which we are managing
 */
void alloc_paragraph(struct journal_manager *journal_man, char ***src, int *size, int n_paragraph);


/**
 * @brief read read_file and write the content as a Journal
 *          based on journal_man values
 * 
 * @param journal_man 
 * @param read_file file from where we need to read from
 */
void make_journal(struct journal_manager *journal_man, FILE *read_file);

#endif