#ifndef _ARCH_I386_TTY_H_
#define _ARCH_I386_TTY_H_

#include <stddef.h>

#define STR_ROW_START           "\n> "

#define INT_COLUMN_START_INDEX  2

void terminal_initialize(void);
void terminal_putchar(char c);
void terminal_write(const char *data, size_t size);
void terminal_writestring(const char *data);
void terminal_scroll();
void terminal_backspace();
void terminal_enter();

#endif
