#include <stdio.h>
#include <stdlib.h>

/* To be written. Do not submit this file to canvas */

extern char *processRequest (char *);

int main (int argc, char **argv) {

    

    // char *req0 = "A 147.188.192.43 22";
    // processRequest(req0);
    
    char *req1 = "A 147.188.192.41 443";
    processRequest(req1);

    char *req2 = "A 147.188.192.41 441";
    processRequest(req2);

    char *req5 = "C 147.188.192.41 441";
    processRequest(req5);

    char *req18 = "L";
    processRequest(req18);

    char *req11 = "D 147.188.192.41 443";
    processRequest(req11);

    // char *req14 = "D 147.655.192.41 441";
    // processRequest(req14);

    char *req15 = "A 147.655.192.41 441";
    processRequest(req15);

    // char *req12 = "C 147.188.192.41 441";
    // processRequest(req12);

    char *req13 = "L";
    processRequest(req13);

    char *req4 = "F";
    processRequest(req4);

    

    


    return 0;
}
