#include <arch/i386/idt.h>
#include <stdio.h>
#include <string.h>
#include <asm.h>

#define SET_IDT_ENTRY(idx) set_idt_entry(idx, (uint32_t) &interrupt_handler_##idx, 0x08, 0x8E);

#define DECLARE_INTERRUPT_HANDLER(i) void interrupt_handler_##i(void);

// IDT entry
typedef struct idt_entry {  // 64-bits
    uint16_t base_lo;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_hi;
} __attribute__((packed)) idt_entry_t;

typedef struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr_t;

// Declare an IDT of 256 entries
static idt_entry_t idt[INT_IDT_NUM_ENTRIES];
static idt_ptr_t idtp;

// External assembly routines
extern void idt_load(struct idt_ptr* idt_ptr_addr);

// Declare interrupt handlers in assembly code
// ISRs
DECLARE_INTERRUPT_HANDLER(0);
DECLARE_INTERRUPT_HANDLER(1);
DECLARE_INTERRUPT_HANDLER(2);
DECLARE_INTERRUPT_HANDLER(3);
DECLARE_INTERRUPT_HANDLER(4);
DECLARE_INTERRUPT_HANDLER(5);
DECLARE_INTERRUPT_HANDLER(6);
DECLARE_INTERRUPT_HANDLER(7);
DECLARE_INTERRUPT_HANDLER(8);
DECLARE_INTERRUPT_HANDLER(9);
DECLARE_INTERRUPT_HANDLER(10);
DECLARE_INTERRUPT_HANDLER(11);
DECLARE_INTERRUPT_HANDLER(12);
DECLARE_INTERRUPT_HANDLER(13);
DECLARE_INTERRUPT_HANDLER(14);
DECLARE_INTERRUPT_HANDLER(15);
DECLARE_INTERRUPT_HANDLER(16);
DECLARE_INTERRUPT_HANDLER(17);
DECLARE_INTERRUPT_HANDLER(18);
DECLARE_INTERRUPT_HANDLER(19);

// IRQs
DECLARE_INTERRUPT_HANDLER(32);
DECLARE_INTERRUPT_HANDLER(33);

void set_idt_entry(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_lo = base & 0xFFFF;
    idt[num].base_hi = (base >> 16) & 0xFFFF;
    idt[num].always0 = 0;
    idt[num].flags = flags;
    idt[num].sel = sel;
}

void idt_install() {
    // Set the base and limit of the IDT 
    idtp.limit = (sizeof(struct idt_entry) * INT_IDT_NUM_ENTRIES) - 1;
    idtp.base = (uint32_t) &idt;

    // Clear the entire IDT, initializing to 0's
    memset(&idt, 0, sizeof(struct idt_entry) * INT_IDT_NUM_ENTRIES);

    // Set IDT entries 
    // ISRs
    SET_IDT_ENTRY(0);
    SET_IDT_ENTRY(1);
    SET_IDT_ENTRY(2);
    SET_IDT_ENTRY(3);
    SET_IDT_ENTRY(4);
    SET_IDT_ENTRY(5);
    SET_IDT_ENTRY(6);
    SET_IDT_ENTRY(7);
    SET_IDT_ENTRY(8);
    SET_IDT_ENTRY(9);
    SET_IDT_ENTRY(10);
    SET_IDT_ENTRY(11);
    SET_IDT_ENTRY(12);
    SET_IDT_ENTRY(13);
    SET_IDT_ENTRY(14);
    SET_IDT_ENTRY(15);
    SET_IDT_ENTRY(16);
    SET_IDT_ENTRY(17);
    SET_IDT_ENTRY(18);
    SET_IDT_ENTRY(19);

    // IRQs
    SET_IDT_ENTRY(32);
    SET_IDT_ENTRY(33);

    // Remap PIC 
    outb(0x20, 0x10);
    outb(0xA0, 0x10);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    // Points the processor's internal register to the new IDT
    idt_load(&idtp);
    printf("%s\n", STR_IDT_INSTALLED);
}

/*
            Base    Limit 
    IDT = 00107080 000007ff
    (&base + limit = range)

    To dump memory to file 
    1. Open QEMU console (ctrl + alt + 2)
    2. 'dump-guest-memory <file name>'

    idtp.base = 0x00107080, which is what shows in the IDT register,
    so register is being loaded with a value, but interrupts are not being
    triggered. 

    Adding the limit of 7ff to the base revealed nothing in memory (0x0010787f)

    Outputting address of div by 0 interrupt function, converting to hex,
    and dumping memory reveals nothing stored in that address
*/