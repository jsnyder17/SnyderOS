#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <arch/i386/gdt.h>
#include <arch/i386/idt.h>
#include <arch/i386/tty.h>
#include <asm.h>
#include <kernel/keyboard.h>
#include <kernel/timer.h>
#include <external/multiboot.h>

void kernel_early(struct multiboot_info* mb) {
	terminal_initialize();
	gdt_install();
	idt_install();
	timer_install();
	phys_memory_init(mb);
	virt_memory_init();
	kernel_heap_init();
	keyboard_install();
	enable_interrupts();
}

void kernel_main(void) {
	print_logo();
	terminal_writestring("> ");

	for(;;) __asm__ ("hlt");
}