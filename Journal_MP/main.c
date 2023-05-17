#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

#include "journal_manager.h"
#include "make_journal.h"

int JOURNAL_LEN = 7;
char JOURNAL_WORD[9] = "_JOURNAL";
int DOTTXT_LEN = 4;
char DOTTXT[5] = ".txt";
char DOT = '.';

int errno;

void make_journal_name(char *file_name, char *journal_name){
    strcpy_while(journal_name, file_name, DOT);
    strcat(journal_name, JOURNAL_WORD);
    strcat(journal_name, DOTTXT);
}


int main(int argc, char *argv[]){
    if (argc == 1){
        printf("\"./makeJournal\" isn't enough to run this program, please check \"./makeJournal --help\"");
        return 0;
    }

    if (strcmp(argv[1], "--help") == 0){
        printf("expected input in order: \
                \n-text_file.txt : file with txt extention \
                \n-number_of_columns : int \
                \n-number_of_lines_of_one_page : int \
                \n-number_of_characters_per_column_line : int \
                \n-distance_beetween_columns : int \
                \nexample: \"./makeJournal example.txt 3 20 25 5\"\n");
        return 0;
    }

    if (argc != 6){
        printf("input doesn't fit the expected input, please check \"./makeJournal --help\"");
        return 0;
    }

    while (--argc >= 2){
        if (check_number(argv[argc]) == 0){
            printf("%s isn't a number, please check \"./makeJournal --help\"", argv[argc]);
            return 0;
        }
    }

    // fd_1 is used to transferring the paragraphs read from the input file to another process
    // fd_2 is used to transferring the row of the journal to another process
    int fd_1[2], fd_2[2];
    int pid_1, pid_2, pid_3;

    pipe(fd_1);

    pid_1 = fork();
    check_fork(pid_1);

    if (pid_1 == 0){
        // child process who read the input file and send the data to pid_2
        close_read_pipe(fd_1);

        read_paragraphs_file(argv[1], atoi(argv[4]), fd_1);
        exit(EXIT_SUCCESS);
    }

    pipe(fd_2);

    pid_2 = fork(); // check -1 val
    check_fork(pid_2);

    if (pid_2 == 0){
        // child process who read the paragraphs from pid_1 and write the 
        // rows of journal page to pid_3
        close_write_pipe(fd_1);
        close_read_pipe(fd_2);

        struct journal_manager journal_man;
        journal_man.num_columns = atoi(argv[2]);
        journal_man.num_rows =  atoi(argv[3]);
        journal_man.column_length = atoi(argv[4]);
        journal_man.distance_btw_columns = atoi(argv[5]);

        initialize_journal(&journal_man);

        alloc_paragraph(&journal_man, fd_1, fd_2);
        exit(EXIT_SUCCESS);
    }

    pid_3 = fork();
    check_fork(pid_3);

    if (pid_3 == 0){
        // child process who read the rows of Journal page from pid_2
        close_write_pipe(fd_2);
        close_write_pipe(fd_1);
        close_read_pipe(fd_1);

        char journal_name[strlen(argv[1]) + JOURNAL_LEN +1];
        memset_string_to_char(journal_name, '\0', strlen(argv[1]) + JOURNAL_LEN +1);
        make_journal_name(argv[1], journal_name);

        FILE *fp = fopen (journal_name, "w");
        int errnum;
        if (fp == NULL ){
            errnum = errno;
            printf("Opening the text file: %s, is happening this error: %s\n", "ou.txt", strerror(errnum));
            exit(EXIT_FAILURE);
        }

        write_row(fp, fd_2);

        int close_err = fclose(fp);

        if (close_err == EOF){
            errnum = errno;
            printf("Closing the text file: %s\nerror: %s\n", "ou.txt", strerror(errnum));
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }

    // father process
    int status_1, status_2, status_3;
    int pid1_res, pid2_res, pid3_res;
    int err_sleep;

    // check if child process have an exit failure, if all are finished with
    // exit sucess than exit from the while
    while(1){
        pid1_res = waitpid(pid_1, &status_1, WNOHANG);
        // check if there is an error with waitpid 
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

        // checks if a process finishes the execution with an error
        if ((pid1_res == -1 & WEXITSTATUS(status_1) != 0) |
            (pid2_res == -1 & WEXITSTATUS(status_2) != 0) |
            (pid3_res == -1 & WEXITSTATUS(status_3) != 0) ){
                kill_processes(pid_1, pid_2, pid_3);
                printf("An error occurred in a child process\n");
                exit(EXIT_FAILURE);
            }
        
        // there isn't any one process with exit status != 0 then 
        // we can check only if all the processes are finished, which 
        // means that all the child processes finish with success
        if (pid1_res == -1 & pid2_res == -1 & pid3_res == -1){
            break;
        }
        // checks every 100 microseconds
        err_sleep = usleep(100);
        if (err_sleep == -1){
            printf("An error occurred with the usleep function\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("You can find your Journal file in the directory of your input file");
    return 0;
}