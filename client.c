#include <stdio.h>
#include <stdlib.h>

/* To be written. Do not submit this file to canvas */

extern char *processRequest (char *);

int main (int argc, char **argv) {

    

    char *req0 = "A 147.188.192.43 22";
    processRequest(req0);
    
    char *req1 = "A 147.188.192.41 443";
    processRequest(req1);

    char *req2 = "A 147.188.192.41 441";
    processRequest(req2);

    char *req5 = "C 147.188.192.41 441";
    processRequest(req5);

    char *req11 = "D 147.188.192.41 441";
    processRequest(req11);

    char *req12 = "C 147.188.192.41 441";
    processRequest(req12);

    char *req4 = "F";
    processRequest(req4);

    char *req3 = "R";
    processRequest(req3);

    char *req9 = "A";
    processRequest(req9);

    


    return 0;
}
