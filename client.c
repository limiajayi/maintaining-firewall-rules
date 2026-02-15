#include <stdio.h>

/* To be written. Do not submit this file to canvas */

extern char *processRequest (char *);

int main (int argc, char **argv) {

    char *request, *response;

    request = "A 147.188.192.43 22";
    response = processRequest(request);

    printf(response);

    return 0;
}
