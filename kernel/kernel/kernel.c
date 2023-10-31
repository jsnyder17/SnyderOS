#include <kernel/kernel.h>

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

void kernel_main(unsigned long ebx) {
	terminal_initialize();
	gdt_install();
	idt_install();
	timer_install();
	keyboard_install();
	enable_interrupts();

	print_logo();

	load_module(ebx);

	terminal_writestring("> ");

	for(;;) __asm__ ("hlt");	// We don't want the kernel to return 
}

void load_module(unsigned long ebx) {
	multiboot_info_t *mbinfo = (multiboot_info_t *) ebx;
	unsigned int address_of_module = mbinfo->mods_addr;

	printf("address: %x\n", address_of_module);
	printf("flags: %x\n", (unsigned) mbinfo->flags);
	printf("mods_count: %x\n", mbinfo->mods_count);

	
	if (CHECK_FLAG (mbinfo->flags, 0)) {
		printf("mem_* valid\n");
	} else {
		printf("!!! mem_* invalid\n");
	}
	if (CHECK_FLAG (mbinfo->flags, 1)) {
		printf("boot_device valid\n");
	}
	else {
		printf("!!! boot_device invalid\n");
	}
	if (CHECK_FLAG (mbinfo->flags, 2)) {
		printf("command line passed\n");
	} else {
		printf("!!! command line not passed\n");
	}
	if (CHECK_FLAG (mbinfo->flags, 3)) {
		printf("mods_* valid\n");
	} else {
		printf("!!! mods_* invalid\n");
	}
	if (CHECK_FLAG (mbinfo->flags, 4) && CHECK_FLAG (mbinfo->flags, 5)) {
		printf("both bits 4 and 5 are set\n");
	} else {
		printf("!!! bit(s) 4 or 5 are not set\n");
	}
	if (CHECK_FLAG (mbinfo->flags, 4)) {
		printf("symbol table of a.out valid\n");
	} else {
		printf("!!! symbol table of a.out invalid\n");
	}
	if (CHECK_FLAG (mbinfo->flags, 5)) {
		printf("section header table of ELF valid\n");
	} else {
		printf("!!! section header table of ELF invalid\n");
	}
	if (CHECK_FLAG (mbinfo->flags, 6)) {
		printf("mmap_* valid\n");
	} else {
		printf("!!! mmap_* invalid\n");
	}

	typedef void (*call_module_t)(void);
	call_module_t start_program = (call_module_t) address_of_module;
	start_program();

	registers();
}