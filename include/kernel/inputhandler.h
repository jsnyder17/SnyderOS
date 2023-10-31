#ifndef _KERNEL_INPUT_HANDLER_H_
#define _KERNEL_INPUT_HANDLER_H_

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <arch/i386/vga.h>
#include <kernel/kernel.h>
#include <arch/i386/tty.h>

#define STR_CMD_ECHO                "echo"
#define STR_CMD_REGISTERS           "registers"
#define STR_CMD_PRINT_LOGO          "logo"
#define STR_CMD_CLEAR               "clear"
#define STR_CMD_HIST                "hist"
#define STR_CMD_EXIT                "exit"
#define STR_CMD_HELP                "help"

#define STR_CMD_HELP_OUTPUT         "====== SnyderOS Commands ======\necho <input> - Outputs the value <input> to the screen\nregisters - Display register values\nlogo - Display the SnyderOS logo!\nclear - Clears the screen\nhist - Show list of previous inputs\nexit - Shutdown the OS\nhelp - Show this prompt"
#define STR_CMD_HIST_HEADER         "====== History ======"

#define STR_ERROR_INVALID_COMMAND   "Invalid command. Type 'help' for a list fo commands"

#define INT_ECHO_CMD_LEN            5
#define INT_REGISTER_CMD_LEN        10
#define INT_PRINT_LOGO_CMD_LEN      5
#define INT_CLEAR_CMD_LEN           6
#define INT_HIST_CMD_LEN            5
#define INT_HELP_CMD_LEN            5
#define INT_EXIT_CMD_LEN            5

#define INT_INPUT_START_OFFSET      2
#define INT_ECHO_CMD_OFFSET         6
#define INT_HIST_LIMIT              8

static unsigned char *hist_buffer[INT_HIST_LIMIT][INT_VGA_WIDTH - INT_COLUMN_START_INDEX];
static int hist_buffer_index = 0;

void process_input(const unsigned char *input_str);
void manage_hist_buffer(const unsigned char *input_str);

void echo(const unsigned char *input_str);
void registers();
void print_logo();
void clear();
void history();
void div_by_zero();
void help();
void exit();

#endif