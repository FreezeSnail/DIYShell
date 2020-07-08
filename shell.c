#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <unistd.h>
#include <sys/wait.h>
#include "utils.h"

int main(){
    char ** command;
    char * input;
    pid_t child_pid;
    int stat_loc;

    signal(SIGINT, SIG_IGN);

    while(1){
        input = readline("unixsh> ");
        if(input == NULL){ // ctrl-d exit
            printf("\n");
            exit(0);
        }
        command = get_input(input);


        if(strcmp(command[0], "cd") == 0){
            if(cd(command[1]) < 0)
                perror(command[1]);

            continue;  //dont fork
        }

        if(strcmp(command[0], "exit") == 0){
            exit(0);
        }


        child_pid = fork();
        if(child_pid < 0) {
            printf("Fork failed\n");
            exit(1);
        }
        if(child_pid == 0){
            signal(SIGINT, SIG_DFL);
            // should never return this is sucseeded
            if(execvp(command[0], command) < 0){
                perror(command[0]);
                exit(1);
            }
            
        } else {
            waitpid(child_pid, &stat_loc, WUNTRACED);
        }

        free(input);
        free(command);
    }

    return 0;
}