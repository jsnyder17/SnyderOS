#ifndef _KERNEL_GDT_H_
#define _KERNEL_GDT_H_

#define STR_GDT_INSTALLED       "GDT installed"
#define STR_ERROR_GDT_INSTALL   "Error installing GDT!"

// Sets up the GDT, should be called on early initialization
void gdt_install();

#endif  // _KERNEL_GDT_H_