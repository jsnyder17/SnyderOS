#ifndef _KERNEL_IDT_H_
#define _KERNEL_IDT_H_

#include <stdint.h>

#define IDT_NUM_ENTRIES 256

void idt_install();

#endif