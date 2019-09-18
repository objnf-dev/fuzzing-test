#include <stdio.h>

int main() {
    printf("Hello!\nWhat\'s Your Name?\n");
    char s[15];
    gets(s);
    printf("Welcome %s!", s);
    return 0;
}

