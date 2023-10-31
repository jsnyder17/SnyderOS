#ifndef _KERNEL_KERNEL_H
#define _KERNEL_KERNEL_H

#include <stdio.h>
#include <stdint.h>

#include <arch/i386/tty.h>
#include <arch/i386/gdt.h>
#include <arch/i386/idt.h>
#include <arch/i386/interrupts.h>
#include <arch/i386/multiboot.h>
#include <kernel/timer.h>
#include <kernel/keyboard.h>
#include <kernel/inputhandler.h>
#include <asm.h>

#define INT_MIN         0
#define INT_MAX         16

#define STR_VERSION     "v0.1"
#define STR_AUTHOR      "Joshua Snyder"

void load_module(uint32_t ebx);

#endif