#include <stdio.h>

#include <kernel/kernel.h>
#include <arch/i386/tty.h>
#include <arch/i386/idt.h>
#include <arch/i386/interrupts.h>

void print_logo() {
	printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n", " _____                 _           _____ _____ ",
							   				   "/  ___|               | |         |  _  /  ___|",
							   				   "\\ `--. _ __  _   _  __| | ___ _ __| | | \\ `--. ",
							   				   " `--. \\ '_ \\| | | |/ _` |/ _ \\ '__| | | |`--. \\",
							   				   "/\\__/ / | | | |_| | (_| |  __/ |  \\ \\_/ /\\__/ /",
							   				   "\\____/|_| |_|\\__, |\\__,_|\\___|_|   \\___/\\____/ ",
							   				   "              __/ |                            ",
							   				   "             |___/                             ");
	printf("\t\t\t\t\t\t%s\t%s\n", STR_VERSION, STR_AUTHOR);
}

void print_nums(int min, int max) {
	printf("\nPrinting numbers %i through %i ...\n", min, max);

	int i;
	if (min > max) {
		for (i = min; i >= max; i--) {
			printf("%i\n", i);
		}

	} else {
		for (i = min; i < max + 1; i++) {
			printf("%i\n", i);
		}
	}

	printf("\n");
}

void print_registers() {
	printf("\nPrinting registers ...\n");

	register int *eax asm("eax");
	register int *ebx asm("ebx");
	register int *ecx asm("ecx");
	register int *edx asm("edx");
	
	printf("$eax: %d\n", eax);
	printf("$ebx: %d\n", ebx);
	printf("$ecx: %d\n", ecx);
	printf("$edx: %d\n", edx);
}

void kernel_main(void) {
	terminal_initialize();
	idt_install();

	print_logo();
	printf("%d\n", 3 / 0);
	//print_nums(INT_MIN, INT_MAX);
}
