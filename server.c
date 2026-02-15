#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* To be written. This file needs to be sumitted to canvas */

extern char *processRequest (char *);

bool processPortNums(int num) {
    return (num >= 0) && (num <= 65535);
}

char** processCommand(char *address) {
    // first i want to split by spaces, so split into
    // [command] [IP] [port]

    char **newCommand = malloc(12 * sizeof(char *));
    int length = 0;

    char *addressCopy = strdup(address);
    char *token = strtok(addressCopy, " ");

    while (token != NULL) {
        printf("%s\n", token);
        newCommand[length] = strdup(token);

        token = strtok(NULL, " ");
        length++;
    }
    
    free(addressCopy);
    return newCommand;
}

char *processRequest (char *request) {
    
    char **commands = processCommand(request);
    printf("Is our port valid? %s\n", processPortNums(atoi(commands[2])) ? "true" : "false");


    for (int i = 0; commands[i] != NULL; i++) {
        free(commands[i]);
    }
    free(commands);
    return NULL;

}
