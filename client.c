#include <stdio.h>
#include <stdlib.h>

/* To be written. Do not submit this file to canvas */

extern char *processRequest (char *);

int main (int argc, char **argv) {

    char *request;

    request = "A 147.188.192.43 22";
    processRequest(request);

    // printf("%s\n", response);
    // free(response);

    return 0;
}
