#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* To be written. This file needs to be sumitted to canvas */

extern char *processRequest (char *);

// i need a function called process rule
// which turns a rule: <IP Address>-<IP Address> <Port>-<Port> into "IP-In-Base-10-range Port Range"

char* processIPAddress(char* IPAddress) {
    char *containsDash = strstr(IPAddress, "-");

    if (containsDash) printf("what\n");

    return "whatever man";
}

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
    
    //newCommand[length] = NULL;
    free(addressCopy);
    return newCommand;
}

char* processRCommand(char *request) {
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

    return currentCommands;
}

// global variables for commands A, F, D, L to access
static int rulesCapacity = 50;
static char **rules = NULL;
static int allRulesLength = 0;

char* processACommand (char *rule) {
    if (rules == NULL) {
        rules = malloc(rulesCapacity * sizeof(char *));
    }

    int ruleLength = strlen(rule);

    if (ruleLength + allRulesLength >= rulesCapacity) {
        while (rulesCapacity <= ruleLength + allRulesLength) {
            rulesCapacity *= 2;
        }

        char **temp = realloc(rules, rulesCapacity);
        if (temp != NULL) rules = temp;
    }

    rules[allRulesLength] = strdup(rule);
    allRulesLength += 1;

    // printf("\nNew rule added\n===============\n");
    // for (int i = 0; i < allRulesLength; i++) {
    //     printf("%s\n", rules[i]);
    // }
    
    return strdup("Rule added");
}

char* processFCommand() {
    free(rules);

    rulesCapacity = 50;
    rules = calloc(rulesCapacity, sizeof(char *)); // i want rules to be NULL
    allRulesLength = 0;

    return strdup("All rules deleted");
}

char *processRequest (char *request) {
    char *previousCommands = processRCommand(request);
    char **commands = processCommand(request);

    // with "R" i want to call all previous commands
    if (strcmp("R", commands[0]) == 0) {
        printf("\nPrevious Commands:\n%s", previousCommands);

        return previousCommands;
    }

    // with "A" I want to add all rules to an array of strings
    if (strcmp("A", commands[0]) == 0) {
        char *newRule = malloc(strlen(commands[1]) + strlen(commands[2]) + 2);
        strcpy(newRule, commands[1]);
        strcat(newRule, " ");
        strcat(newRule, commands[2]);

        char *response = processACommand(newRule);

        free(newRule);
        return response;
    }

    // with "F" i want to delete all rules
    if (strcmp("F", commands[0]) == 0) {
        // printf("\nClearing all rules...\n");
        char *response = processFCommand();

        return response;
    }


    // freeing every part of a command
    for (int i = 0; commands[i] != NULL; i++) {
        free(commands[i]);
    }
    free(commands);   

    return NULL;

}
