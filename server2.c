#include <stdio.h>

int main() {
    int sum = 45;
    int *ptr = NULL;
    *ptr = 42;

    printf("Hello: %d", *ptr);
    return 0;
}