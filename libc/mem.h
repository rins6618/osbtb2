#ifndef MEM_H
#define MEM_H

#include "../cpu/type.h"

void memory_copy(u8* source, u8* dest, int nbytes);
void memory_set(void* dest, u8 val, u32 len);
u32 kmalloc(u32 size, int align, u32 *phys_addr);

#endif // MEM_H
