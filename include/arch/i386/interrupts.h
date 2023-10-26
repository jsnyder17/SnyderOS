#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdbool.h>
#include <stdint.h>

#define INT_SYSCALL_IDT_INDEX       128
#define INT_TIMER_IDT_INDEX         32
#define INT_KEYBOARD_IDT_INDEX      33

#define STR_EXCEPTION                               "exception"
#define STR_SYSTEM_HALTED                           "System halted"

#define STR_EXCEPTION_DIV_BY_ZERO                   "Divison by Zero"
#define STR_EXCEPTION_DEBUG                         "Debug"
#define STR_EXCEPTION_NON_MASKABLE_INTERRUPT        "Non Maskable Interrupt"
#define STR_EXCEPTION_BREAKPOINT                    "Breakpoint"
#define STR_EXCEPTION_INTO_DETECTED_OVERFLOW        "Into Detected Overflow"
#define STR_EXCEPTION_OUT_OF_BOUNDS                 "Out of Bounds"
#define STR_EXCEPTION_INVALID_OPCODE                "Invalid Opcode"
#define STR_EXCEPTION_NO_COPROCESSOR                "No Coprocessor"
#define STR_EXCEPTION_DOUBLE_FAULT                  "Double Fault"
#define STR_EXCEPTION_COPROCESSOR_SEGMENT_OVERRUN   "Coprocessor Segment Overrun"
#define STR_EXCEPTION_BAD_TSS                       "Bad TSS"
#define STR_EXCEPTION_SEGMENT_NOT_PRESENT           "Segment Not Present"
#define STR_EXCEPTION_STACK_FAULT                   "Stack Fault"
#define STR_EXCEPTION_GENERAL_PROTECTION_FAULT      "General Protection Fault"
#define STR_EXCEPTION_PAGE_FAULT                    "Page Fault Exception"
#define STR_EXCEPTION_UNKNOWN_INTERRUPT             "Unknown Interrupt"
#define STR_EXCEPTION_COPROCESSOR_FAULT             "Coprocessor Fault"

// Holds the registers at the time of the interrupt
struct regs
{
    uint32_t gs, fs, es, ds;      /* pushed the segs last */
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    uint32_t idt_index, err_code;    /* our 'push byte #' and ecodes do this */
    uint32_t eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};

typedef void (*interrupt_handler_t)(struct regs *r);

bool register_interrupt_handler(uint32_t idt_index, interrupt_handler_t handler);

#endif