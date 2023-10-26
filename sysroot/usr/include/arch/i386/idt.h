#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define INT_IDT_NUM_ENTRIES 256

#define STR_IDT_INSTALLED       "Installed IDT"
#define STR_ERROR_IDT_INSTALL   "Error installing IDT!"

void idt_install();

#endif