#include "timer.h"
#include "isr.h"
#include "ports.h"
#include "../drivers/screen.h"
#include "../libc/string.h"
#include "../libc/function.h"

u32 tick = 0;

static void timer_callback(registers_t reg) {
    tick++;
    UNUSED(reg);
}

void init_timer(u32 freq) {
    register_interrupt_handler(IRQ0, timer_callback);

    u32 divisor = 1193180 / freq;
    u8 lo = (u8) (divisor & 0xFF);
    u8 hi = (u8) ( (divisor >> 8) & 0xFF);

    port_byte_out(0x43, 0x36);
    port_byte_out(0x40, lo);
    port_byte_out(0x40, hi);
}   