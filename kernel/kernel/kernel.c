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

void kernel_main(void) {
	terminal_initialize();
	gdt_install();
	idt_install();
	timer_install();
	keyboard_install();
	enable_interrupts();

	print_logo();

	terminal_writestring("> ");

	for(;;) __asm__ ("hlt");	// We don't want the kernel to return 
}