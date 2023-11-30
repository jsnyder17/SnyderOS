#include <kernel/inputhandler.h>

void process_input(const char *input_str) {
    manage_hist_buffer(input_str);

    // Get the command entered by the user
    char command_buffer[INT_VGA_WIDTH - INT_INPUT_START_OFFSET];

    char curr_char = 0x00;
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

    } else if (!memcmp(command_buffer, STR_CMD_PRINT_LOGO, INT_PRINT_LOGO_CMD_LEN)) {
        print_logo();

    } else if (!memcmp(command_buffer, STR_CMD_CLEAR, INT_CLEAR_CMD_LEN)) {
        clear();

    } else if (!memcmp(command_buffer, STR_CMD_HIST, INT_HIST_CMD_LEN)) {
        history();

    } else if (!memcmp(command_buffer, STR_CMD_PRINT_HEX, INT_PRINT_HEX_CMD_LEN)) {
        print_hex(input_str);

    } else if (!memcmp(command_buffer, STR_CMD_TEST_DYNAMIC_ALLOCATION, INT_TEST_DYNAMIC_CMD_LEN)) {
        test_dynamic_allocation();

    } else if (!memcmp(command_buffer, STR_CMD_HELP, INT_HELP_CMD_LEN)) {
        help();

    } else if (!memcmp(command_buffer, STR_CMD_EXIT, INT_EXIT_CMD_LEN)) {
        exit();

    } else {
        printf("\n%s", STR_ERROR_INVALID_COMMAND);
    }
}

void manage_hist_buffer(const char *input_str) {
    int i, not_empty;
    i = not_empty = 0;
    char curr_char = 0x00;
    while ((curr_char = input_str[i]) != '\0') {
        if (curr_char != ' ') {
            not_empty = 1;
            break;
        }
        i++;
    }

    if (not_empty) {
        if (hist_buffer_index == INT_HIST_LIMIT) {
            hist_buffer_index = 0;

        } else {
            hist_buffer_index++;
        }

        memcpy(hist_buffer[hist_buffer_index], input_str, INT_VGA_WIDTH - INT_COLUMN_START_INDEX);
    }
}

char* get_arg(const char *input_str, int offset) {
    char *arg = (char*) kmalloc(sizeof(char*) * INT_ARG_LEN);

    char curr_char;
    size_t i = 0;
    while ((curr_char = input_str[i + offset]) != '\0') {
        arg[i] = curr_char;
        i++;
    }
    arg[i] = '\0';

    return arg;
}

void echo(const char *input_str) {
    char *arg = get_arg(input_str, INT_ECHO_CMD_OFFSET);
    printf("\n%s", arg);
    kfree(arg);
}

void registers() {
    //__asm__ volatile("mov $1, %eax\n\tmov $2, %ebx\n\tmov $3, %ecx\n\tmov $4, %edx");

    register int *eax __asm__ ("%eax");
	register int *ebx __asm__ ("%ebx");
	register int *ecx __asm__ ("%ecx");
	register int *edx __asm__ ("%edx"); 
    register int *esi __asm__ ("%esi");
    register int *edi __asm__ ("%edi");
    register int *ebp __asm__ ("%ebp");
    register int *esp __asm__ ("%esp");

	printf("\n$eax: %x -> %d", eax, eax);
	printf("\n$ebx: %x -> %d", ebx, ebx);
	printf("\n$ecx: %x -> %d", ecx, ecx);
	printf("\n$edx: %x -> %d", edx, edx);
    printf("\n$esi: %x -> %d", esi, esi);
    printf("\n$edi: %x -> %d", edi, edi);
    printf("\n$ebp: %x -> %d", ebp, ebp);
    printf("\n$esp: %x -> %d", esp, esp);
}

void print_logo() {
	printf("\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n", " _____                 _           _____ _____ ",
							   				   "/  ___|               | |         |  _  /  ___|",
							   				   "\\ `--. _ __  _   _  __| | ___ _ __| | | \\ `--. ",
							   				   " `--. \\ '_ \\| | | |/ _` |/ _ \\ '__| | | |`--. \\",
							   				   "/\\__/ / | | | |_| | (_| |  __/ |  \\ \\_/ /\\__/ /",
							   				   "\\____/|_| |_|\\__, |\\__,_|\\___|_|   \\___/\\____/ ",
							   				   "              __/ |                            ",
							   				   "             |___/                             ");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t\t%s\t%s\n", STR_VERSION, STR_AUTHOR);
}

void clear() {
    terminal_clear();
}

void history() {
    printf("\n%s", STR_CMD_HIST_HEADER);

    int i;
    for (i = 0; i < hist_buffer_index; i++) {
        printf("\n%s", hist_buffer[i]);
    }
}

void print_hex(const char *input_str) {
    char *arg = get_arg(input_str, INT_PRINT_HEX_CMD_OFFSET);
    int int_arg = atoi(arg);
    printf("\n%d -> %x", int_arg, int_arg);
    kfree(arg);
}

char* get_test_str() {
    char *str = (char*) kmalloc(sizeof(char*) * 4);
    str[0] = 'a';
    str[1] = 'b';
    str[2] = 'c';
    str[3] = '\0';

    return str;
}

void test_dynamic_allocation() {
    char *str = get_test_str();
    printf("\nDynamically allocated string: %s", str);
    printf("\n&str: %x", &str);
    kfree(str);
}

void div_by_zero() {
	int x = 5;
	int y = 5;
	printf("1 / 0 is ... %d\n", 1 / (x - y));	// bypasses crash somehow??? runtime generated 0
	//__asm__ volatile("div %0" :: "r"(0));
}

void help() {
    printf("\n%s", STR_CMD_HELP_OUTPUT);
}

void exit() {
    outw(0x604, 0x2000);
}