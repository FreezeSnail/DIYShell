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

    while(1){
        input = readline("unixsh> ");
        command = get_input(input);

        child_pid = fork();
        if(child_pid < 0) {
            printf("Fork failed\n");
            exit(1);
        }
        if(child_pid == 0){
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