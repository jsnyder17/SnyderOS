#include <kernel/inputhandler.h>

void process_input(const unsigned char *input_str) {
    // Get the command entered by the user
    unsigned char command_buffer[INT_VGA_WIDTH - INT_INPUT_START_OFFSET];

    unsigned char curr_char = 0x00;
    size_t i = 0;
    while (((curr_char = input_str[i + 1]) != ' ') && curr_char != '\0') {
        command_buffer[i] = curr_char;
        i++;
    }
    command_buffer[i] = '\0';

    // Determine which command to run based on user input
    if (!memcmp(command_buffer, STR_CMD_ECHO, INT_ECHO_CMD_LEN)) {
        echo(input_str);

    } else if (!memcmp(command_buffer, STR_CMD_REGISTERS, INT_REGISTER_CMD_LEN)) {
        registers();

    } else if (!memcmp(command_buffer, STR_CMD_HELP, INT_HELP_CMD_LEN)) {
        help();

    } else if (!memcmp(command_buffer, STR_CMD_EXIT, INT_EXIT_CMD_LEN)) {
        exit();

    } else {
        printf("\n%s", STR_ERROR_INVALID_COMMAND);
    }
}

void echo(const unsigned char *input_str) {
    // Get the argument from the command
    unsigned char arg_buffer[INT_VGA_WIDTH - INT_ECHO_CMD_OFFSET];

    unsigned char curr_char = 0x00;
    size_t i = 0;
    while ((curr_char = input_str[i + INT_ECHO_CMD_OFFSET]) != '\0') {
        arg_buffer[i] = curr_char;
        i++;
    }
    arg_buffer[i] = '\0';

    printf("\n%s", arg_buffer);
}

void registers() {
    register int *eax asm("eax");
	register int *ebx asm("ebx");
	register int *ecx asm("ecx");
	register int *edx asm("edx"); 
    register int *esi asm("esi");
    register int *edi asm("edi");
    register int *ebp asm("ebp");
    register int *esp asm("esp");

	printf("\n$eax: %d", eax);
	printf("\n$ebx: %d", ebx);
	printf("\n$ecx: %d", ecx);
	printf("\n$edx: %d", edx);
    printf("\n$esi: %d", esi);
    printf("\n$edi: %d", edi);
    printf("\n$ebp: %d", ebp);
    printf("\n$esp: %d", esp);
}

void help() {
    printf("\n%s", STR_CMD_HELP_OUTPUT);
}

void exit() {
    outb(0x604, 0x2000);    // Shutdown apparently?
    printf("\nSorry, but for some reason this doesn't work yet. Just force close QEMU");
}