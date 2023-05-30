#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

#include "newspaper_manager.h"
#include "make_newspaper.h"

int NEWSPAPER_LEN = 9;
char NEWSPAPER_WORD[11] = "_NEWSPAPER";
int DOTTXT_LEN = 4;
char DOTTXT[5] = ".txt";
char DOT = '.';

int errno;

void make_newspaper_name(char *file_name, char *newspaper_name){
    strcpy_while(newspaper_name, file_name, DOT);
    strcat(newspaper_name, NEWSPAPER_WORD);
    strcat(newspaper_name, DOTTXT);
}


int main(int argc, char *argv[]){
    if (argc == 1){
        printf("\"./makeNewspaper\" isn't enough to run this program, please check \"./makeNewspaper --help\"");
        return 0;
    }

    if (strcmp(argv[1], "--help") == 0){
        printf("expected input in order: \
                \n\t-text_file.txt : file with txt extention \
                \n\t-number_of_columns : int \
                \n\t-number_of_lines_of_one_page : int \
                \n\t-number_of_characters_per_column_line : int \
                \n\t-distance_beetween_columns : int \
                \nexample: \"./makeNewspaper example.txt 3 20 25 5\"\n");
        return 0;
    }

    if (argc != 6){
        printf("input doesn't fit the expected input, please check \"./makeNewspaper --help\"");
        return 0;
    }

    while (--argc >= 2){
        if (check_number_grater_zero(argv[argc]) == 0){
            printf("%s isn't a number, please check \"./makeNewspaper --help\"", argv[argc]);
            return 0;
        }
    }

    /* fd_1 è usata per scrivere e leggere le parole dei paragrafi del file di input */
    /* fd_2 è usata per scrivere e leggere le righe del giornale/ file di output */
    int fd_1[2], fd_2[2];
    int pid_1, pid_2, pid_3;

    pipe(fd_1);

    pid_1 = fork();
    check_fork(pid_1);

    if (pid_1 == 0){
        /* figlio che si occupa di leggere e trasferire le parole del file di input */
        close_read_pipe(fd_1);

        read_paragraphs_file(argv[1], atoi(argv[4]), fd_1);
        exit(EXIT_SUCCESS);
    }

    pipe(fd_2);

    pid_2 = fork(); 
    check_fork(pid_2);

    if (pid_2 == 0){
        /* figlio che si occupa di giustificare e inserire le parole e paragrafi nelle 
            righe del giornale 
        */

        close_write_pipe(fd_1);
        close_read_pipe(fd_2);

        struct newspaper_manager newspaper_man;
        newspaper_man.num_columns = atoi(argv[2]);
        newspaper_man.num_rows =  atoi(argv[3]);
        newspaper_man.column_length = atoi(argv[4]);
        newspaper_man.distance_btw_columns = atoi(argv[5]);

        initialize_newspaper(&newspaper_man);
        alloc_paragraph(&newspaper_man, fd_1, fd_2);
        exit(EXIT_SUCCESS);
    }

    pid_3 = fork();
    check_fork(pid_3);

    if (pid_3 == 0){
        /* figlio che srive le righe del giornale nel file di output */
        close_write_pipe(fd_2);
        close_write_pipe(fd_1);
        close_read_pipe(fd_1);

        char newspaper_name[strlen(argv[1]) + NEWSPAPER_LEN +1];
        memset_string_to_char(newspaper_name, '\0', strlen(argv[1]) + NEWSPAPER_LEN +1);
        make_newspaper_name(argv[1], newspaper_name);

        FILE *fp = fopen (newspaper_name, "w");
        int errnum;
        if (fp == NULL ){
            errnum = errno;
            printf("Opening the text file: %s, is happening this error: %s\n", argv[2], strerror(errnum));
            exit(EXIT_FAILURE);
        }

        write_rows(fp, fd_2);

        int close_err = fclose(fp);

        if (close_err == EOF){
            errnum = errno;
            printf("Closing the text file: %s\nerror: %s\n", "ou.txt", strerror(errnum));
            exit(EXIT_FAILURE);
        }

        exit(EXIT_SUCCESS);
    }

    /* processo padre */
    int status_1, status_2, status_3;
    int pid1_res, pid2_res, pid3_res;
    int err_sleep;


    /* controlla se un processo figlio ha smesso di eseguire il programma con un 
        codice di errore, se tutti finiscono con exit code uguale a 0 allora esce dal ciclo
    */
    while(1){
        pid1_res = waitpid(pid_1, &status_1, WNOHANG);
        /* controlla se waitpid ha generato un errore */ 
        if (pid1_res == -1 & errno != ECHILD){
            kill_processes(pid_1, pid_2, pid_3);
            printf("An error occurred using waitpid\n");
            exit(EXIT_FAILURE);
        }

        pid2_res = waitpid(pid_2, &status_2, WNOHANG);
        if (pid2_res == -1 & errno !=ECHILD){
            kill_processes(pid_1, pid_2, pid_3);
            printf("An error occurred using waitpid\n");
            exit(EXIT_FAILURE);
        }

        pid3_res = waitpid(pid_3, &status_3, WNOHANG);
        if (pid3_res == -1 & errno != ECHILD){
            kill_processes(pid_1, pid_2, pid_3);
            printf("An error occurred using waitpid\n");
            exit(EXIT_FAILURE);
        }

        /* controlla se un processo è finito con EXIT_FAILURE */
        if ((pid1_res == -1 & WEXITSTATUS(status_1) != 0) |
            (pid2_res == -1 & WEXITSTATUS(status_2) != 0) |
            (pid3_res == -1 & WEXITSTATUS(status_3) != 0) ){
                kill_processes(pid_1, pid_2, pid_3);
                printf("An error occurred in a child process\n");
                exit(EXIT_FAILURE);
            }
        
        /* tutti i processi sono finiti e nessuna ha avuto un EXIT_FAILURE */
        if (pid1_res == -1 & pid2_res == -1 & pid3_res == -1){
            break;
        }
        
        /* controlla ogni 100 microsecondi*/
        err_sleep = usleep(100);
        if (err_sleep == -1){
            printf("An error occurred with the usleep function\n");
            kill_processes(pid_1, pid_2, pid_3);
            exit(EXIT_FAILURE);
        }
    }
    printf("You can find your newspaper file in the directory of your input file\n");
    return 0;
}