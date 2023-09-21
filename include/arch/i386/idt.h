#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define IDT_NUM_ENTRIES 256

void idt_install();

#endif