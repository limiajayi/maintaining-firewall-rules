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

    //array of strings where the new
    char **newCommand = malloc(50 * sizeof(char *));
    int length = 0;

    char *addressCopy = strdup(address);
    char *token = strtok(addressCopy, " ");

    while (token != NULL) {
        //printf("%s\n", token);
        newCommand[length] = strdup(token);

        token = strtok(NULL, " ");
        length++;
    }
    
    free(addressCopy);
    return newCommand;
}

bool processRCommand(char *letter) {
    return (strcmp("R", letter) == 0);
}

bool processFCommand (char *letter) {
    return (strcmp("F", letter) == 0);
}

char *processRequest (char *request) {

    // from what i understand about static variables, they are preserved during the entire life time of the program
    // so any changes to it persist after concurrent function calls
    // so setting up forthe "R" command
    // we make capacity also static so it persists across function calls

    static int capacity = 50;
    static char *currentCommands = NULL;
    static int currentLength = 0;

    if (currentCommands == NULL) {
        currentCommands = malloc(capacity * sizeof(char));
        currentLength = 0;
    }

    // Check if we need to resize
    int requestLen = strlen(request) + 1; // +1 for the newline
    if (currentLength + requestLen >= capacity) {
        // add as many as needed to make the capacity greater than the current command
        while (capacity <= currentLength + requestLen) {
            capacity *= 2;
        }
        char *temp = realloc(currentCommands, capacity);
        
        // check first before reallocating memory
        if (temp != NULL) {
            currentCommands = temp;
        }
    }
    
    // Append the new request (this runs EVERY call, and not just the first)
    strcat(currentCommands, request);
    strcat(currentCommands, "\n");
    currentLength += requestLen;

    char **commands = processCommand(request);
    // with "R" i want to call all previous commands
    if (processRCommand(commands[0]) == true) {
        printf("Commands: \n%s", currentCommands);
    }

    // with "F" i want to delete all previous commands
    if (processFCommand(commands[0]) == true) {
        // okay changed it to a free so theres no memory leaks
        free(currentCommands);
        capacity = 50;
        currentCommands = calloc(capacity, sizeof(char));
        currentLength = 0;

        printf("All commands deleted. \n");
    }


    // freeing every part of command
    for (int i = 0; commands[i] != NULL; i++) {
        free(commands[i]);
    }
    free(commands);
    

    return NULL;

}
