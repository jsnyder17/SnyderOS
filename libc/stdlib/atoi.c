#include <stdlib.h>

int atoi(const unsigned char *str) {
    int num, i;
    num = i = 0;
    
    for (i = 0; str[i] != '\0'; ++i) {
        num = num * 10 + str[i] - '0';
    }

    return num;
}