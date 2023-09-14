#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include "vga.h"

static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLUE);
	terminal_buffer = VGA_MEMORY;
	for (size_t y = 0; y < INT_VGA_HEIGHT; y++) {
		for (size_t x = 0; x < INT_VGA_WIDTH; x++) {
			const size_t index = y * INT_VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * INT_VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c) {
	unsigned char uc = c;

	if (uc != '\n') {
		if (uc == '\t') {
			terminal_column++;

		} else {
			terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
		}
	}

	if (++terminal_column == INT_VGA_WIDTH || uc == '\n') {
		terminal_column = 0;

		if (++terminal_row == INT_VGA_HEIGHT) {
			terminal_scroll();
		}
	}
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}

void terminal_scroll() {
	terminal_row--;

	for (size_t y = 0; y < INT_VGA_HEIGHT - 1; y++) {
		for (size_t x = 0; x < INT_VGA_WIDTH; x++) {
			const size_t src_index = y * INT_VGA_WIDTH + x;	// 25 apart each 
			const size_t dest_index = (y + 1) * INT_VGA_WIDTH + x;
			terminal_buffer[src_index] = terminal_buffer[dest_index];
		}
	}

	for (size_t x = 0; x < INT_VGA_WIDTH; x++) { 
		const size_t index = (INT_VGA_HEIGHT - 1) * INT_VGA_WIDTH + x;
		terminal_buffer[index] = vga_entry(' ', terminal_color);
	}
}
