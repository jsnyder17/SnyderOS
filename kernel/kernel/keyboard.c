#include <arch/i386/interrupts.h>
#include <arch/i386/tty.h>
#include <kernel/keyboard.h>
#include <stdint.h>
#include <stdio.h>
#include <asm.h>

unsigned char kbdus[128][4] = {
    {0, 0, 0, 0},
    {27, 27, 27, 27},
    {'1', '!', '1', '!'},
    {'2', '@', '2', '@'},
    {'3', '#', '3', '#'},
    {'4', '$', '4', '$'},
    {'5', '%', '5', '%'},
    {'6', '^', '6', '^'},
    {'7', '&', '7', '&'},
    {'8', '*', '8', '*'},  // index 9
    {'9', '(', '9', '('},
    {'0', ')', '0', ')'},
    {'-', '_', '-', '_'},
    {'=', '+', '=', '+'},
    {'\b', '\b', '\b', '\b'},  // Backspace
    {'\t', '\t', '\t', '\t'},  // Tab
    {'q', 'Q', 'Q', 'q'},
    {'w', 'W', 'W', 'w'},
    {'e', 'E', 'E', 'e'},
    {'r', 'R', 'R', 'r'},  // index 19
    {'t', 'T', 'T', 't'},
    {'y', 'Y', 'Y', 'y'},
    {'u', 'U', 'U', 'u'},
    {'i', 'I', 'I', 'i'},
    {'o', 'O', 'O', 'o'},
    {'p', 'P', 'P', 'p'},
    {'[', '{', '[', '{'},
    {']', '}', ']', '}'},
    {'\n', '\n', '\n', '\n'},  // Enter key
    {0, 0, 0, 0},              // Control key, index 29
    {'a', 'A', 'A', 'a'},
    {'s', 'S', 'S', 's'},
    {'d', 'D', 'D', 'd'},
    {'f', 'F', 'F', 'f'},
    {'g', 'G', 'G', 'g'},
    {'h', 'H', 'H', 'h'},
    {'j', 'J', 'J', 'j'},
    {'k', 'K', 'K', 'k'},
    {'l', 'L', 'L', 'l'},
    {';', ':', ';', ':'},  // index 39
    {'\'', '\"', '\'', '\"'},
    {'`', '~', '`', '~'},
    {0, 0, 0, 0},  // Left Shift
    {'\\', '|', '\\', '|'},
    {'z', 'Z', 'Z', 'z'},
    {'x', 'X', 'X', 'x'},
    {'c', 'C', 'C', 'c'},
    {'v', 'V', 'V', 'v'},
    {'b', 'B', 'B', 'b'},
    {'n', 'N', 'N', 'n'},  // index 49
    {'m', 'M', 'M', 'm'},
    {',', '<', ',', '<'},
    {'.', '>', '.', '>'},
    {'/', '?', '/', '?'},
    {0, 0, 0, 0},  // Right Shift
    {'*', '*', '*', '*'},
    {0, 0, 0, 0},          // Alt
    {' ', ' ', ' ', ' '},  // Space
    {0, 0, 0, 0},          // Caps Lock
    {0, 0, 0, 0},          // F1 key, index 59
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},  // F10
    {0, 0, 0, 0},  // Num Lock, index 69
    {0, 0, 0, 0},  // Scroll Lock
    {0, 0, 0, 0},  // Home Key
    {0, 0, 0, 0},  // Up Arrow
    {0, 0, 0, 0},  // Page Up
    {'-', '-', '-', '-'},
    {0, 0, 0, 0},  // Left Arrow
    {0, 0, 0, 0},
    {0, 0, 0, 0},  // Right Arrow
    {'+', '+', '+', '+'},
    {0, 0, 0, 0},  // End Key, index 79
    {0, 0, 0, 0},  // Down Arrow
    {0, 0, 0, 0},  // Page Down
    {0, 0, 0, 0},  // Insert Key
    {0, 0, 0, 0},  // Delete Key
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},  // F11 Key
    {0, 0, 0, 0},  // F12 Key
    {0, 0, 0, 0},  // All other keys are undefined
};

typedef struct {
    int caps_lock;
    int shift_held;
} keyboard_state_t;

keyboard_state_t keyboard_state;

void keyboard_handler(struct regs *r) {
    unsigned char scan_code;
    int column = 0;
    char clicked = 0;

    // Read from the keyboard's data buffer
    scan_code = inb(0x60);

    // If the top bit of the byte we read from the keyboard
    // is set, that means that a key has just been released
    if (scan_code & 0x80) {
        if (scan_code >> 2 == 42 || scan_code >> 2 == 54) {
            keyboard_state.shift_held = 0;
        }

    } else {
        switch (scan_code) {
            case 14:    // backspace
                terminal_backspace();
                break;
            case 28:    // enter
                terminal_enter();
                break;

            case 42:    // shift cases
            case 54:
                keyboard_state.shift_held = 1;
                break;
            
            case 58:    // caps lock
                keyboard_state.caps_lock = ~keyboard_state.caps_lock;
                break;

            default:
                column = keyboard_state.shift_held * 1 + keyboard_state.caps_lock * 2;
                clicked = kbdus[scan_code][column];
                if (clicked != 0 && clicked != 27) {
                    terminal_putchar(clicked);
                }
                break;

        }
    }
}

void keyboard_install() {
    if (!register_interrupt_handler(INT_KEYBOARD_IDT_INDEX, keyboard_handler)) {
        printf("%s\n", STR_ERROR_KEYBOARD_INSTALL);
        
    } else {
        keyboard_state.caps_lock = 0;
        keyboard_state.shift_held = 0;
        printf("%s\n", STR_KEYBOARD_INSTALLED);
    }
}