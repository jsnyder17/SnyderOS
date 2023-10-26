#include <stdio.h>

#include <kernel/kernel.h>
#include <arch/i386/tty.h>
#include <arch/i386/gdt.h>
#include <arch/i386/idt.h>
#include <arch/i386/interrupts.h>
#include <kernel/timer.h>
#include <kernel/keyboard.h>
#include <kernel/inputhandler.h>
#include <asm.h>

void print_logo() {
	printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n", " _____                 _           _____ _____ ",
							   				   "/  ___|               | |         |  _  /  ___|",
							   				   "\\ `--. _ __  _   _  __| | ___ _ __| | | \\ `--. ",
							   				   " `--. \\ '_ \\| | | |/ _` |/ _ \\ '__| | | |`--. \\",
							   				   "/\\__/ / | | | |_| | (_| |  __/ |  \\ \\_/ /\\__/ /",
							   				   "\\____/|_| |_|\\__, |\\__,_|\\___|_|   \\___/\\____/ ",
							   				   "              __/ |                            ",
							   				   "             |___/                             ");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t\t%s\t%s\n", STR_VERSION, STR_AUTHOR);
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

void div_by_zero() {
	int x = 5;
	int y = 5;
	printf("1 / 0 is ... %d\n", 1 / (x - y));	// bypasses crash somehow??? runtime generated 0
	//__asm__ volatile("div %0" :: "r"(0));
}

void kernel_main(void) {
	terminal_initialize();
	gdt_install();
	idt_install();
	timer_install();
	keyboard_install();
	enable_interrupts();

	print_logo();

	terminal_writestring("> ");
	//div_by_zero();

	for(;;) __asm__ ("hlt");	// We don't want the kernel to return 
}

/*

IDT is used to show the processor what interrupt service routine (ISR) to call to handle either an exception or an 'int' opcode
	- Entries are also called by interrupt requests whenever a device has completed a request or needs to be serviced 

IDT Entry 
	- Base address (address of the ISR the processor should call)
	- Access flag
		* Field to say is descriptor is present
		* Descriptor Priviledge Level to say which ring is teh highest number that is allowed to use the given interrupt
		* Lower 5-bits always 01110. 
	- 64-bits long 

	- Doens't have a limit, instead had a segment that you must specify (must be same segment that the given ISR is loacted in)


ISRs are used to save the current processor state and set up the appropriate segment registers needed for kernel mode before the kernel's C-level interrupt handler is called
	- Also need to point to the correct entry in the IDT to the correct ISR 

Some exceptions push an error code onto the stack. A dummy error code of 0 is pushed onto the stack for any ISR that doens't already push an error code
	- Helps to keep a uniform stack frame

We push the interrupt number onto the stack to keep track of which exception is firing

*/
