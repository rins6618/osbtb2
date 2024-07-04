#include "../drivers/screen.h"
#include "../cpu/isr.h"
#include "kernel.h"
#include "../libc/string.h"
#include "../libc/mem.h"

void main() {
    clear_screen();
    isr_install(); 
    irq_install();

    
    kprint("echo: type anything\n"
        "Type END to exit and end execution\n> ");
}

void user_input(char* input) {
    if (strcmp("END", input) == 0) {
        kprint("Ending execution. exec asm volatile(\"hlt\");");
        asm volatile("hlt");
    } else if (strcmp("MALLOC", input) == 0) {
        u32 phys_addr;
        u32 page = kmalloc(1000, 1, &phys_addr);

        // NOTE ON CODE BELOW: Since you just called a big function with lots of stack variables (kmalloc), these following values will have garbage.
        // Zero them out or you'll have weird characters on the address strings.
        
        char page_str[16] = {0};
        hex_to_ascii(page, page_str);
        char addr_str[16] = {0};
        hex_to_ascii(phys_addr, addr_str);
        kprint("Page: ");
        kprint(page_str);
        kprint(", physical addr: ");
        kprint(addr_str);
        kprint("\n");
    }
    kprint("echo: ");
    kprint(input);
    kprint("\n> ");
}
