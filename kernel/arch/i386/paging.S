.global enable_paging
enable_paging:
    # Move directory into CR3
    mov 4(%esp), %eax
    mov %cr3, %eax

    # Disable 4MB pages
    mov %ecx, %cr4
    and %ecx, ~0x00000010
    mov %cr4, %ecx
    
    # Enable paging
    mov %eax, %cr0
    or %eax, 0x80000000
    mov %cr0, %eax
    ret