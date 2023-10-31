#include <arch/i386/idt.h>
#include <arch/i386/interrupts.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <asm.h>

static interrupt_handler_t interrupt_handlers[INT_IDT_NUM_ENTRIES];

char *exception_msgs[] = {
    STR_EXCEPTION_DIV_BY_ZERO,
    STR_EXCEPTION_DEBUG,
    STR_EXCEPTION_NON_MASKABLE_INTERRUPT,
    STR_EXCEPTION_BREAKPOINT,
    STR_EXCEPTION_INTO_DETECTED_OVERFLOW,
    STR_EXCEPTION_OUT_OF_BOUNDS,
    STR_EXCEPTION_INVALID_OPCODE,
    STR_EXCEPTION_NO_COPROCESSOR,
    STR_EXCEPTION_DOUBLE_FAULT,
    STR_EXCEPTION_COPROCESSOR_SEGMENT_OVERRUN,
    STR_EXCEPTION_BAD_TSS,
    STR_EXCEPTION_SEGMENT_NOT_PRESENT,
    STR_EXCEPTION_STACK_FAULT,
    STR_EXCEPTION_GENERAL_PROTECTION_FAULT,
    STR_EXCEPTION_PAGE_FAULT,
    STR_EXCEPTION_UNKNOWN_INTERRUPT,
    STR_EXCEPTION_COPROCESSOR_FAULT
};

bool register_interrupt_handler(uint32_t idt_index, interrupt_handler_t handler) {
    if (idt_index >= INT_IDT_NUM_ENTRIES) {
        return false;
    }

    if (interrupt_handlers[idt_index] != NULL) {
        return false;
    }

    interrupt_handlers[idt_index] = handler;
    return true;
}

void fault_handler(int idt_index, struct regs *r) {
    printf("(%x) %s %s. %s", idt_index, exception_msgs[idt_index], STR_EXCEPTION, STR_SYSTEM_HALTED);
    printf("\n$eax: %x", r->eax);
    printf("\n$ebx: %x", r->ebx);
    printf("\n$ecx: %x", r->ecx);
    printf("\n$edx: %x", r->edx);
    printf("\n$esp: %x", r->esp);

    for(;;);
}

void run_irq_handler(struct regs *r) {
    void (*handler)(struct regs *r);

    handler = interrupt_handlers[r->idt_index];
    if (handler) {
        handler(r);
    }

    if (r->idt_index >= 40) {
        outb(0xA0, 0x20);
    }

    outb(0x20, 0x20);
}

void run_interrupt_handler(struct regs *r) {
    size_t idt_index = r->idt_index;
    if (idt_index < 32) {
        fault_handler(idt_index, r);
        return;
    }

    if (idt_index >= 32 && idt_index <= 47) {
        run_irq_handler(r);
        return;
    }

    if (interrupt_handlers[r->idt_index] != NULL) {
        interrupt_handlers[r->idt_index](r);
    }
}