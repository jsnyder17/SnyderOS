#include <stdlib.h>

int rand(long seed) {
    return (((seed = seed * 214013L + 2531011L) >> 16) & 0x7FFFF);
}