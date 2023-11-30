#include <stdlib.h>

int atoi(const char *str) {
    int num, i, sign;
    num = i = 0; sign = 1;

    if (str[0] == '-') {
        sign = -1;
        i++;
    }
    
    while (str[i] != '\0') {
        num = num * 10 + str[i] - '0';
        i++;
    }

    return num * sign;
}