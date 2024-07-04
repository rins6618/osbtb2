#ifndef IDT_H
#define IDT_H

#include "type.h"

#define KERNEL_CS 0x08

typedef struct {
    u16 low_offset;     /* bits 0-15 of function address */
    u16 sel;            /* kernel segment selector */
    u8  always0;        /* won't change the stack */
    u8  flags;          /* RTFM */
    u16 high_offset;    /* bits 16-31 of function address */
} __attribute__((packed)) idt_gate_t;

typedef struct {
    u16 limit;
    u32 base;
} __attribute__((packed)) idt_register_t;

#define IDT_ENTRIES 256
extern idt_gate_t idt[IDT_ENTRIES];
extern idt_register_t idt_reg;

void set_idt_gate(int n, u32 handler);
void set_idt();

#endif // IDT_H