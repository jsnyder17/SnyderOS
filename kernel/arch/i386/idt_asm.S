# Loads the IDT defiend in "_idtp" into the processor
.global idt_load
idt_load:
    mov 4(%esp), %eax
    lidt (%eax)
    ret