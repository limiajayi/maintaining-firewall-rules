#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

extern char *processRequest(char *);

// Thread function - each thread runs this
void *clientThread(void *arg) {
    char *request = (char *)arg;
    char *response = processRequest(request);
    
    printf("Thread sent: %s\n", request);
    printf("Thread got: %s\n", response);
    
    free(response);
    return NULL;
}

int main() {
    pthread_t threads[5];  // Create 7 threads
    
    // Different requests each thread will make
    char *requests[] = {
        "R",
        "A 147.188.192.43 22",
        "A 10.0.0.1 80",
        "C 147.188.192.43 22",
        "C 10.0.0.1 80",
        "A 147.188.192.48 22",
        "D 147.188.192.48 22",
        "L"
    };
    
    // Start all threads at once
    for (int i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, clientThread, requests[i]);
    }
    
    // Wait for all threads to finish
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // Clean up
    char *response = processRequest("F");
    free(response);
    
    printf("All threads completed\n");
    return 0;
}