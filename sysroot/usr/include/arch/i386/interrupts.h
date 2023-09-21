#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdbool.h>
#include <stdint.h>

#define TIMER_IDT_INDEX     32
#define KEYBOARD_IDT_INDEX  33
#define SYSCALL_IDT_INDEX   128

// Holds the registers at the time of the interrupt
struct regs {
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t idt_index, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};

typedef void (*interrupt_handler_t)(struct regs *r);

bool register_interrupt_handler(uint32_t idt_index, interrupt_handler_t handler);

#endif