#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int Function1(char a[], int size) {
    char b = a[rand() % (size + 1)]; 
    printf("it's %c\n",b);
    return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if(size >= 4)
        Function1((char *)data, size);
    return 0;
}

