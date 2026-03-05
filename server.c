#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

/* To be written. This file needs to be sumitted to canvas */

extern char *processRequest (char *);

// i need a function called process rule
// which turns a rule: <IP Address>-<IP Address> <Port>-<Port> into "IP-In-Base-10-range Port Range"

struct Rule {
    long startIPAddress;
    long endIPAddress;
    int startPort;
    int endPort;

    char *ruleString;

    char **queries;
    int queryCount;
    int queryCapacity;
};

// global variables for commands A, F, D, L to access
static int rulesCapacity = 50;
static int allRulesLength = 0;
static struct Rule **rules = NULL;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

bool validIP(int IP) {
    return (IP >= 0) && (IP <= 255);
}

bool validPort (int port) {
    return (port >= 0) && (port <= 65535); 
}

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


// IP Address
// 147.188.192.43
// 147 188 192 43
// check inbetween 0 - 255
int* breakUpDots(char *address) {
    int *newArr = malloc(4 * sizeof(int));
    int length = 0;
    char *copy = strdup(address);
    char *token = strtok(copy, ".");

    while (token != NULL) {
        newArr[length] = atoi(token);

        token = strtok(NULL, ".");
        length += 1;
    }
    
    free(copy);
    return newArr;
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

//This function turns IP Addresses into a number of type long
//for example "147.188.192.41" becomes 2478620713
// enables IP Addresses to be compared
long* turnIPIntoInt(char *IPaddress) {
    long *num = malloc(sizeof(long));
    *num = 0;

    char *copy = strdup(IPaddress);
    char *token = strtok(copy, ".");
    int length = 3;

    long multipliers[] = {1, 256, 65536, 16777216};


    while (token != NULL && length >= 0) {

        *num += (long)atoi(token) * multipliers[length];

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

bool checkValidRule(char *rule) {
    char** jointRule = processCommand(rule);
    char** IPs = breakUp(jointRule[0]);
    char** ports = breakUp(jointRule[1]);

    bool verdictIP;
    bool verdictPort;

    if (IPs[0]) {
        int len = 0;
        int* IPnums = breakUpDots(IPs[0]);
        while (len < 4) {
            
            verdictIP = validIP(IPnums[len]);

            if (verdictIP == false)  break;
            len += 1;
        }

        free(IPnums);
    }

    if (IPs[1]) {
        int len = 0;
        int* IPnums = breakUpDots(IPs[1]);
        while (len < 4) {
            verdictIP = validIP(IPnums[len]);

            if (verdictIP == false)  break;

            len += 1;
        }

        free(IPnums);
    }

    if (ports[0]) {
        verdictPort = validPort(atoi(ports[0]));
    }

    if (ports[1]) {
        verdictPort = validPort(atoi(ports[1]));
    }
    

    freeCommands(jointRule);
    freeCommands(IPs);
    freeCommands(ports);

    return verdictIP && verdictPort;
}

char* concatIPAndPort(char *IP, char *Port) {
    char *result = malloc(strlen(IP) + strlen(Port) + 2);
    strcpy(result, IP);
    strcat(result, " ");
    strcat(result, Port);

    return result;
}

char* processRCommand(char *request) {
    static int capacity = 50;
    static char *currentCommands = NULL;
    static int currentLength = 0;

    // for the F command
    // so there's nothing on heap after F is called
    if (request == NULL) {
        if (currentCommands != NULL) {
            free(currentCommands);
            currentCommands = NULL;
        }

        currentLength = 0;
        capacity = 0;
        return NULL;
    }

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

char* processACommand (char *rule, char *IP, char *Port) {

    struct Rule *newRule = turnIPandPortToRule(IP, Port);
    newRule->ruleString = strdup(rule);
    newRule->queryCount = 0;
    newRule->queryCapacity = 10;

    if (rules == NULL) {
        rules = calloc(rulesCapacity, sizeof(struct Rule *));
    }

    int ruleLength = sizeof(newRule);

    if (ruleLength + allRulesLength >= rulesCapacity) {
        while (rulesCapacity <= ruleLength + allRulesLength) {
            rulesCapacity *= 2;
        }

        struct Rule **temp = realloc(rules, rulesCapacity);
        if (temp != NULL) rules = temp;
    }

    rules[allRulesLength] = newRule;
    allRulesLength += 1;
    
    return strdup("Rule added");
}

char* processCCommand(char *IP, char *port) {
    struct Rule *r1 = turnIPandPortToRule(IP, port);
    char *result = NULL;
    
    int length = 0;
    
    while (rules[length] != NULL) {
        
        if (isIPInRange(r1->startIPAddress, rules[length]->startIPAddress, rules[length]->endIPAddress) && 
            isPortInRange(r1->startPort, rules[length]->startPort, rules[length]->endPort)) {
            result = strdup("Connection accepted");

            // initialising queries array
            if (rules[length]->queries == NULL) {
                rules[length]->queryCapacity = 10;
                rules[length]->queries = malloc(rules[length]->queryCapacity * sizeof(char *));
                rules[length]->queryCount = 0;
            }

            // check if we need to rezise the queries array
            // before adding a query to it
            if (rules[length]->queryCount >= rules[length]->queryCapacity) {
                rules[length]->queryCapacity *= 2;
                char **temp = realloc(rules[length]->queries, rules[length]->queryCapacity * sizeof(char *));

                if (temp != NULL) rules[length]->queries = temp;
            }

            char *query = concatIPAndPort(IP, port);
            rules[length]->queries[rules[length]->queryCount] = query;
            rules[length]->queryCount += 1;

        }
        
        
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
    // clear EVERYTHING
    printf("Freeing %d rules\n", allRulesLength);

    for (int i = 0; i < allRulesLength; i++) {

        printf("Freeing %d rules\n", allRulesLength);

        if (rules[i] != NULL) {

            // free the rule string
            if (rules[i]->ruleString != NULL) {
                free(rules[i]->ruleString);
            }

            // free the queries
            if (rules[i]->queries != NULL) {

                for (int j = 0; j < rules[i]->queryCount; j++) {
                    printf("Freeing rule %d\n", i);
                    free(rules[i]->queries[j]);
                }
                
                free(rules[i]->queries);
            }


            free(rules[i]);
        }
    }

    free(rules);
    rules = NULL;

    // clear commands history
    processRCommand(NULL); 

    rulesCapacity = 50;
    allRulesLength = 0;

    return strdup("All rules deleted");
}

char* processDCommand(char *unwantedRule) {
    char *result = NULL;

    // loop through all rules
    for (int i = 0; i < allRulesLength; i++) {
        if (strcmp(rules[i]->ruleString, unwantedRule) == 0) {
            // free queries
            freeCommands(rules[i]->queries);
            //free the unwanted rule
            free(rules[i]);
        }

        //shift the remaining rules down
        for (int j = i; rules[j + 1] != NULL; j++) {
            rules[j] = rules[j + 1];
        }

        result = strdup("Rule deleted");
        allRulesLength -= 1;
        break; // no need to loop through all the ruls anymore
    }
    
    if (result == NULL) result = strdup("Rule not found");
    
    return result;
}

char* processLCommand() {
    int totalSize = 1;

    for (int rLen = 0; rLen < allRulesLength; rLen++) {
        totalSize += strlen("Rule: ") + strlen(rules[rLen]->ruleString) + strlen("\n\n");

        for (int qLen = 0; qLen < rules[rLen]->queryCount; qLen++) {
            totalSize += strlen("Query ") + strlen(rules[rLen]->queries[qLen]) + strlen("\n");
        }

    }

    char *result = malloc(totalSize);
    result[0] = '\0';

    for (int len = 0; len < allRulesLength; len++) {
        strcat(result, "Rule: ");
        strcat(result, rules[len]->ruleString);
        strcat(result, "\n");

        for (int queryLen = 0; queryLen < rules[len]->queryCount; queryLen++) {
            
            strcat(result, "Query: ");
            strcat(result, rules[len]->queries[queryLen]);
            strcat(result, "\n");
        }

        strcat(result, "\n");
    }

    return result;
}

char *processRequest (char *request) {
    //makes processRequest thread safe
    pthread_mutex_lock(&mutex);

    //concatenation of all previous requests
    char *previousCommands = processRCommand(request);

    //the current request broken up into an array of strings
    char **commands = processCommand(request);

    if (strcmp("R", commands[0]) == 0) {
        // with "R" i want to call all previous commands

        freeCommands(commands);
        pthread_mutex_unlock(&mutex);

        printf("%s", previousCommands);

        return previousCommands;

    } else if (strcmp("A", commands[0]) == 0) {
        // with "A" I want to add all rules to an array of strings

        // checks IP and PORT parameters exist
        if (commands[1] == NULL) {
            freeCommands(commands);

            pthread_mutex_unlock(&mutex);

            return strdup("Illegal response");
        }

        // construct a rule from the incoming request
        char *newRule = concatIPAndPort(commands[1], commands[2]);

        //checks if the rule is valid
        if (checkValidRule(newRule) == false) {
            free(newRule);
            freeCommands(commands);

            pthread_mutex_unlock(&mutex);
            return strdup("Invalid Rule");
        }

        char *response = processACommand(newRule, commands[1], commands[2]);
        
        free(newRule);
        freeCommands(commands);

        printf("%s\n", response);

        pthread_mutex_unlock(&mutex);
        return response;
    } else if (strcmp("C", commands[0]) == 0) {

        // checks if IP Address and Port are entered
        if (commands[1] == NULL) {
            freeCommands(commands);

            pthread_mutex_unlock(&mutex);
            return strdup("Illegal response");
        }

        char *newRule = malloc(strlen(commands[1]) + strlen(commands[2]) + 2);
        strcpy(newRule, commands[1]);
        strcat(newRule, " ");
        strcat(newRule, commands[2]);

        //checks if IP Address and Port are valid
        if (checkValidRule(newRule) == false) {
            free(newRule);
            freeCommands(commands);

            pthread_mutex_unlock(&mutex);
            return strdup("Illegal IP address or port specified");
        }

        char *response = processCCommand(commands[1], commands[2]);
        printf("%s\n", response);
        free(newRule);
        freeCommands(commands);

        pthread_mutex_unlock(&mutex);
        return response;
    } else if (strcmp("L", commands[0]) == 0) {
        
        char *response = processLCommand();
        freeCommands(commands);

        printf("%s\n", response);

        pthread_mutex_unlock(&mutex);
        return response;

    } else if (strcmp("D", commands[0]) == 0) {

        if (commands[1] == NULL) {
            freeCommands(commands);

            pthread_mutex_unlock(&mutex);
            return strdup("Illegal response");
        }

        char *newRule = malloc(strlen(commands[1]) + strlen(commands[2]) + 2);
        strcpy(newRule, commands[1]);
        strcat(newRule, " ");
        strcat(newRule, commands[2]);

        if (checkValidRule(newRule) == false) {
            free(newRule);
            freeCommands(commands);
            printf("Invalid Rule\n");

            pthread_mutex_unlock(&mutex);
            return strdup("Invalid rule");
        }

        char *response = processDCommand(newRule);

        free(newRule);
        freeCommands(commands);
        printf("%s\n", response);

        pthread_mutex_unlock(&mutex);
        return response;

    } else if (strcmp("F", commands[0]) == 0) {
        // with "F" i want to delete all rules

        //RESULTS
        printf("\nClearing all rules...\n");
        char *response = processFCommand();

        printf("%s\n", response);

        freeCommands(commands);

        pthread_mutex_unlock(&mutex);
        return response;
    }


    freeCommands(commands);

    pthread_mutex_unlock(&mutex);
    return strdup("Illegal request");
}
