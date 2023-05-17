#include <stdio.h>
#include <ctype.h> 
#include <stdlib.h>
#include <string.h> 


int real_len(char *s){
    int len = 0;    
    int index = 0;

    while (s[index] != '\0'){
        len += 1;
        char ch = s[index];

        // real word chars composed of 3 chars
        if ('\xe0' <= ch & ch <= '\xef'){
            index += 3;
        }
        // real word chars composed of 2 chars
        else if ('\xc0' <= ch & ch <= '\xcf'){
            index += 2;
        }
        else {
            index += 1;
        } 
    }
    return len;
}


void memset_string_to_char(char *dest, char ch, int size){
    memset(dest, ch, sizeof(char) * size);
    // add NULL terminator
    dest[size-1] = '\0';
}


void free_list(char **list, int size){
    for (int i = 0; i < size; i++){
        memset_string_to_char(*(list + i), '\0', strlen(*(list + i)));
        free(*(list + i));
    }
    free(list);
}


void check_list_allocation(char **list){
    if (list == NULL){
        printf("There was a problem with the allocation of a list in the heap\n");
        exit(EXIT_FAILURE);
    }
}


void check_string_allocation(char *str){
    if (str == NULL){
        printf("There was a problem with the allocation of a string in the heap\n");
        exit(EXIT_FAILURE);
    }
}


int check_number(char *str){
    int i = 0;
    while (str[i] != '\0'){
        if (str[i] >= '0' & str[i] <= '9'){
            i -= 1;
        }
        else{
            return 0;
        }
    }
    return 1;
}


void strcpy_while(char *dest, char *src, char break_char){
    int i = 0;
    while (*(src +i) != break_char){
        *(dest + i) = *(src +i);
        i++;
    }
}