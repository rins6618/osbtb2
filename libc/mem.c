#include "mem.h"
#include "../cpu/type.h"

void memory_copy(u8* source, u8* dest, int nbytes) {
    for (int i = 0; i < nbytes; i++)
        *(dest + i) = *(source + i);
}

void memory_set(void* dest, u8 val, u32 len) {
    u8* temp = (u8*) dest;
    for (; len != 0; len--) *temp++ = val;
}

// The base point for dynamically allocated memory:
u32 free_mem_address = 0x10000;


u32 kmalloc(u32 size, int align, u32* phys_addr) {
    if (align == 1 && (free_mem_address & 0xFFFFF000)) {
        free_mem_address &= 0xFFFFF000;
        free_mem_address += 0x1000;
    }

    if (phys_addr) *phys_addr = free_mem_address;
    
    u32 ret = free_mem_address;
    free_mem_address += size;
    return ret;
}