#pragma once
#include <stdlib.h>
#include <string.h>

char ** get_input(char * input){
    char ** command = malloc(8 * sizeof(char *));
    if( command == NULL){
        perror("malloc failed");
        exit(1);
    }
    char * separator = " ";
    char * parsed;
    int index = 0;

    parsed = strtok(input, separator);
    while(parsed != NULL){
        command[index] = parsed;
        index++;

        parsed = strtok(NULL, separator);
    }

    command[index] = NULL; // last index must be null for exc
    return command;
}

int cd(char * path){
    return chdir(path);
}