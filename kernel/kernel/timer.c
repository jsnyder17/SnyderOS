#include <arch/i386/interrupts.h>
#include <kernel/timer.h>
#include <asm.h>
#include <stdio.h>

int timer_ticks = 0;
int tick_tock = 0;
char *output = "";

void timer_handler(struct regs *r) {
    timer_ticks++;
    if (!(timer_ticks % 100)) {
        output = (tick_tock) ? STR_TOCK: STR_TICK;
        //printf("%s\n", output);
        tick_tock = ~tick_tock;
    
    }
}

void timer_phase(int hz) {
    int divisor = 1193180 / hz;  // Calculates the divisor
    outb(0x43, 0x36);            // Set our command byte 0x36
    outb(0x40, divisor & 0xFF);  // Set low byte of divisor
    outb(0x40, divisor >> 8);    // Set high byte of divisor
}

void timer_install() {
    if (!register_interrupt_handler(INT_TIMER_IDT_INDEX, timer_handler)) {
        printf("%s\n", STR_ERROR_TIMER_INSTALL);

    } else {
        timer_phase(INT_TICKS_PER_SECOND);
        printf("%s\n", STR_TIMER_INSTALLED);
    }
}