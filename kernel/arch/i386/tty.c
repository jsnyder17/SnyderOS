#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <asm.h>

#include <arch/i386/vga.h>
#include <kernel/inputhandler.h>

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

	update_cursor(terminal_row, terminal_column);
}

void terminal_write(const char *data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char *data) {
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

void terminal_backspace() {
	/*
	if (terminal_column == 2) {
		terminal_column = INT_VGA_WIDTH - 1;
		terminal_row--;

	} else {
		terminal_column--;
	}
	*/

	// For now, we will only allow the user to type on one line at a time
	if (terminal_column != INT_COLUMN_START_INDEX) {
		terminal_column--;
	}

	terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
	update_cursor(terminal_row, terminal_column);
}

void terminal_clear() {
	terminal_row = 0;
	terminal_column = INT_COLUMN_START_INDEX;
	
	for (size_t y = 0; y < INT_VGA_HEIGHT; y++) {
		for (size_t x = 0; x < INT_VGA_WIDTH; x++) {
			const size_t index = y * INT_VGA_WIDTH + x;	// 25 apart each 
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_enter() {
	char input_str[INT_VGA_WIDTH - INT_COLUMN_START_INDEX];

	// Get the input typed
	size_t i, j;
	j = 0;
	for (i = terminal_column; i >= INT_COLUMN_START_INDEX; i--) {
		input_str[j] = terminal_buffer[(terminal_row * INT_VGA_WIDTH + terminal_column) - (i - 1)];
		j++;
	}
	input_str[j] = '\0';

	// Handle the command entered by the user
	process_input(input_str);

	// Print the starting cursor
	printf(STR_ROW_START);
}