# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Reserve a stack for the initial thread.
# .section .bss
# .align 16
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:


# This is the virtual base address of kernel space. It must be used to convert virtual
# addresses into physical addresses until paging is enabled. Note that this is not
# the virtual address where the kernel image itself is loaded -- just the amount that must
# be subtracted from a virtual address to get a physical address.
.set KERNEL_VIRTUAL_BASE, 	0xC0000000					# 3GB
.set KERNEL_PAGE_NUMBER, 	(KERNEL_VIRTUAL_BASE >> 22)	# Page directory index of kernel's 4MB PTE

# Declare the boot Paging directory to load a virtual higher half kernel
.section .data
.align 0x1000
.global _boot_page_directory
_boot_page_directory:
	.long 0x00000083
	.fill (KERNEL_PAGE_NUMBER - 1), 4, 0x00000000
	.long 0x00000083
	.fill (1024 - KERNEL_PAGE_NUMBER - 1), 4, 0x00000000

# The kernel entry point.
.section .text
.global _start
_start:
	# Load page directory base register
	mov $(_boot_page_directory - KERNEL_VIRTUAL_BASE), %ecx
	mov %ecx, %cr3			# ebx has the address of the page directory

	mov %cr4, %ecx			# Read current cr4
	or $0x00000010, %ecx		# Set PSE
	mov %ecx, %cr4			# Update cr4
	mov %cr0, %ecx			# Read current cr0
	or $0x80000000, %ecx		# Set PG
	mov %ecx, %cr0			# Update cr0

	# Transfer control to the main kernel.
	movl $_higher_half, %ecx	# Load the address of the label in ebx
	jmp %ecx					# Jump to the label

.global _higher_half
.type _higher_half, @function
_higher_half:
	# Unmap the identity-mapped first 4MB of physcial address space
	invlpg (0)

	# Enter protection mode
	mov %cr0, %eax
	or $0x00000001, %al
	mov %eax, %cr0

	# Set stack top
	movl $stack_top, %esp
	push %ebx

	# Call kernel_early
	call kernel_early

	# Call the global constructors.
	call _init

	# Transfer control to C kernel
	call kernel_main

	# Hang if kernel_main unexpectedly returns.
	cli

.hang:	
	hlt
	jmp .hang
.size _start, . - _start
