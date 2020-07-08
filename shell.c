#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <unistd.h>
#include <sys/wait.h>
#include "utils.h"

#include <signal.h>
#include <setjmp.h>


static sigjmp_buf env;
static volatile sig_atomic_t jump_active = 0;

void sigint_handler(int signo) {
    if (!jump_active) {
        return;
    }
    siglongjmp(env, 42);
}

int main(){
    char ** command;
    char * input;
    pid_t child_pid;
    int stat_loc;

    //set up SIGINT
    struct sigaction s;
    s.sa_handler = sigint_handler;
    sigemptyset(&s.sa_mask);
    s.sa_flags = SA_RESTART;
    sigaction(SIGINT, &s, NULL);



    while(1){

        if (sigsetjmp(env, 1) == 42) {
            printf("\n");
            continue;
        }

        jump_active = 1;

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

            struct sigaction s_child;
            s_child.sa_handler = sigint_handler;
            sigemptyset(&s_child.sa_mask);
            s_child.sa_flags = SA_RESTART;
            sigaction(SIGINT, &s_child, NULL);

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