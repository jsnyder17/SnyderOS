#ifndef LIBC_ASM_H
#define LIBC_ASM_H 1

#include <stdint.h>
#include <stdio.h>

inline void outb(uint16_t port, uint8_t val) {
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

inline void outw(uint16_t port, uint16_t val) {
    asm volatile("outw %0, %1" : : "a"(val), "Nd"(port));
}

inline void enable_interrupts(void) { 
    asm volatile("sti"); 
    printf("Enabled interrupts!\n");
}

inline void disable_interrupts(void) { asm volatile("cli"); }

inline void invlpg(void *m) {
    asm volatile("invlpg (%0)" : : "b"(m) : "memory");
}

#endif