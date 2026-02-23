#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* To be written. This file needs to be sumitted to canvas */

extern char *processRequest (char *);

// i need a function called process rule
// which turns a rule: <IP Address>-<IP Address> <Port>-<Port> into "IP-In-Base-10-range Port Range"

struct Rule {
    long startIPAddress;
    long endIPAddress;
    int startPort;
    int endPort;
};

void freeCommands(char **commands) {
    if (commands == NULL) return;
    
    for (int i = 0; commands[i] != NULL; i++) {
        free(commands[i]);
    }
    free(commands);
}

int* arrOfPorts(char *port1, char *port2) {
    int *ports = malloc(2 * sizeof(int));
    ports[0] = atoi(port1);
    ports[1] = atoi(port2);

    return ports;
}

// breaks up <IPAddress>-<IPAddress> and <port>-<port>
char** breakUp(char *address) {
    char **newArr = malloc(50 * sizeof(char *));
    int length = 0;
    char *copy = strdup(address);
    char *token = strtok(copy, "-");

    while (token != NULL) {
        newArr[length] = strdup(token);

        token = strtok(NULL, "-");
        length += 1;
    }
    
    newArr[length] = NULL;
    free(copy);
    return newArr;
}

long* turnIPIntoInt(char *IPaddress) {
    long *num = malloc(sizeof(long));
    *num = 0;

    char *copy = strdup(IPaddress);
    char *token = strtok(copy, ".");
    int length = 3;

    int multipliers[] = {1, 256, 65536, 16777216};


    while (token != NULL && length >= 0) {

        *num += (long)atoi(token) * (long)multipliers[length];

        token = strtok(NULL, ".");
        length -= 1;
    }

    free(copy);
    return num;
}

long** arrOfIPs(long *IP1, long *IP2) {
    long **IPAddresses = malloc(2 * sizeof(long));
    IPAddresses[0] = IP1;
    IPAddresses[1] = IP2;

    printf("here are the IPs");

    for (int i = 0; i < 2; i++) {
        printf("\n %ld \n", *IPAddresses[i]);
    }
    return IPAddresses;
}

// takes in something like "10.10.10.10-255.255.255.255" "22-33"

struct Rule* turnIPandPortToRule(char *IP, char *port) {
    struct Rule *r = calloc(1, sizeof(struct Rule));
    char **IPs = breakUp(IP);
    char **Ports = breakUp(port);

    if (IPs[0]) {
        long *startIP = turnIPIntoInt(IPs[0]);
        r->startIPAddress = *startIP;
        free(startIP);
        
        if (IPs[1]) {
            long *endIP = turnIPIntoInt(IPs[1]);
            r->endIPAddress = *endIP;
            free(endIP);
        } else {
            r->endIPAddress = r->startIPAddress;
        }
    }

    if (Ports[0]) {
        r->startPort = atoi(Ports[0]);
        if (Ports[1]) {
            r->endPort = atoi(Ports[1]);
        } else {
            r->endPort = r->startPort;
        }
    }
    
    freeCommands(IPs);
    freeCommands(Ports);

    return r;
}

bool isIPInRange(long ip, long startIP, long endIP) {
    return (ip >= startIP) && (ip <= endIP);
}

bool isPortInRange(int port, int startPort, int endPort) {
    return (port >= startPort) && (port <= endPort);
}

char** processCommand(char *address) {
    // first i want to split by spaces, so split into
    // [command] [IP] [port]

    //array of strings where the new command will be
    char **newCommand = malloc(60 * sizeof(char *));
    int length = 0;

    char *addressCopy = strdup(address);
    char *token = strtok(addressCopy, " ");

    while (token != NULL) {
        //printf("%s\n", token);
        newCommand[length] = strdup(token);

        token = strtok(NULL, " ");
        length++;
    }
    
    newCommand[length] = NULL;
    free(addressCopy);
    return newCommand;
}

char* processRCommand(char *request) {
    static int capacity = 50;
    static char *currentCommands = NULL;
    static int currentLength = 0;

    if (currentCommands == NULL) {
        currentCommands = malloc(capacity * sizeof(char));
        currentCommands[0] = '\0';
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

    //RESULTS
    // printf("\nNew rule added\n===============\n");
    // for (int i = 0; i < allRulesLength; i++) {
    //     printf("%s\n", rules[i]);
    // }
    
    return strdup("Rule added");
}

char* processCCommand(char *IP, char *port) {
    struct Rule *r1 = turnIPandPortToRule(IP, port);
    char *result = NULL;
    
    int length = 0;
    
    while (rules[length] != NULL) {
        char **currentRule = processCommand(rules[length]);
        struct Rule *r2 = turnIPandPortToRule(currentRule[0], currentRule[1]);
        
        if (isIPInRange(r1->startIPAddress, r2->startIPAddress, r2->endIPAddress) && 
            isPortInRange(r1->startPort, r2->startPort, r2->endPort)) {
            result = strdup("Connection accepted");
        }
        
        freeCommands(currentRule);
        free(r2);
        
        if (result != NULL) break;
        length += 1;
    }
    
    if (result == NULL) {
        result = strdup("Connection rejected");
    }
    
    free(r1);
    return result;
}

char* processFCommand() {
    freeCommands(rules);

    rulesCapacity = 50;
    rules = calloc(rulesCapacity, sizeof(char *)); // i want rules to be NULL
    allRulesLength = 0;

    return strdup("All rules deleted");
}

char* processDCommand(char *unwantedRule) {
    char *result = NULL;

    int length = 0;
    while (rules[length] != NULL) {

        if (strcmp(rules[length], unwantedRule) == 0) {
            free(rules[length]);

            // shift remaining rules down
            for (int i = length; rules[i + 1] != NULL; i++) {
                rules[i] = rules[i + 1];
            }

            // mark end of array after shifting all items down
            rules[length + 1] = NULL;

            result = strdup("Rule deleted");
            allRulesLength -= 1;
            break; // no need to loop through anymore
        }

        length += 1;
    }

    if (result == NULL) result = strdup("Rule not found");
    
    return result;
}

char *processRequest (char *request) {

    char *previousCommands = processRCommand(request);
    char **commands = processCommand(request);

    // with "R" i want to call all previous commands
    if (strcmp("R", commands[0]) == 0) {
        //RESULTS
        //printf("\nPrevious Commands:\n%s", previousCommands);

        freeCommands(commands);
        return previousCommands;
    } else if (strcmp("A", commands[0]) == 0) {

        // with "A" I want to add all rules to an array of strings

        if (commands[1] == NULL) {
            freeCommands(commands);
            return strdup("Illegal response");
        }

        char *newRule = malloc(strlen(commands[1]) + strlen(commands[2]) + 2);
        strcpy(newRule, commands[1]);
        strcat(newRule, " ");
        strcat(newRule, commands[2]);

        char *response = processACommand(newRule);

        free(newRule);
        freeCommands(commands);
        return response;
    } else if (strcmp("C", commands[0]) == 0) {

        if (commands[1] == NULL) {
            freeCommands(commands);
            return strdup("Illegal response");
        }

        char *response = processCCommand(commands[1], commands[2]);
        printf("%s\n", response);
        freeCommands(commands);
        return response;
    } else if (strcmp("F", commands[0]) == 0) {

        // with "F" i want to delete all rules

        //RESULTS
        //printf("\nClearing all rules...\n");
        char *response = processFCommand();

        freeCommands(commands);
        return response;
    } else if (strcmp("D", commands[0]) == 0) {

        if (commands[1] == NULL) {
            freeCommands(commands);
            return strdup("Illegal response");
        }

        char *newRule = malloc(strlen(commands[1]) + strlen(commands[2]) + 2);
        strcpy(newRule, commands[1]);
        strcat(newRule, " ");
        strcat(newRule, commands[2]);

        char *response = processDCommand(newRule);

        free(newRule);
        freeCommands(commands);
        printf("%s\n", response);
        return response;
    } else {
        freeCommands(commands);
        return strdup("Illegal request");
    }

    return NULL;

}
