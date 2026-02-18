#include <stdio.h>
#include <stdlib.h>

/* To be written. Do not submit this file to canvas */

extern char *processRequest (char *);

int main (int argc, char **argv) {

    

    char *req0 = "A 147.188.192.43 22";
    char *req1 = "C 147.188.192.41 443";
    char *req2 = "R";
    char *req3 = "F";
    processRequest(req0);
    processRequest(req1);
    processRequest(req2);
    processRequest(req3);
    processRequest(req2);

    // printf("%s\n", response);
    // free(response);

    return 0;
}
