#include "keyboard.h"
#include "screen.h"

#include "../cpu/ports.h"
#include "../cpu/isr.h"

#include "../libc/string.h"
#include "../libc/function.h"

#include "../kernel/kernel.h"

#define BACKSPACE 0x0E
#define ENTER 0x1C

static char input_buffer[256];

#define SC_MAX 57
const char *sc_name[] = { "ERROR", "Esc", "1", "2", "3", "4", "5", "6", 
    "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E", 
        "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl", 
        "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`", 
        "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".", 
        "/", "RShift", "Keypad *", "LAlt", "Spacebar"};
const char sc_ascii[] = { '?', '*', '1', '2', '3', '4', '5', '6',     
    '7', '8', '9', '0', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y', 
        'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G', 
        'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V', 
        'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};


static void keyboard_callback(registers_t reg) {
    UNUSED(reg);
    u8 scancode = port_byte_in(0x60);
    
    if (scancode > SC_MAX) return;
    if (scancode == BACKSPACE) {
        backspace(input_buffer);
        kprint_backspace();
    } else if (scancode == ENTER) {
        kprint("\n");
        user_input(input_buffer);
        input_buffer[0] = '\0';
    } else {
        char letter = sc_ascii[(int) scancode];
        char str[2] = {letter, '\0'};
        append(input_buffer, letter);
        kprint(str);
    }
    

}

void init_keyboard() {
    register_interrupt_handler(IRQ1, keyboard_callback);
}
