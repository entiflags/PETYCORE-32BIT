#ifndef CPU32_H // ТОЧКА РАБОТЫ PETYPROC 32BIT
#define CPU32_H

#include <stdint.h>

#define MEM_SIZE 4*1024*1024
#define REG_COUNT 8
// флаги EFLAGS
#define FLAG_CF     (1 << 0)
#define FLAG_ZF     (1 << 6)
#define FLAG_SF     (1 << 7)
#define ZERO_FLAG   (1 << 6)
#define SIGN_FLAG   (1 << 7)

// состояние процессора
typedef struct {    
    // основные регистры
    uint32_t eax, ebx, ecx, edx;
    uint32_t esi, edi, esp, ebp;
    uint32_t eip;
    uint32_t eflags;

    // сегментные регистры
    uint16_t cs, ds, es, fs, gs, ss;

    // системные регистры
    uint32_t cr0, cr2, cr3;
    uint32_t idtr, gdtr;

    // память
    uint8_t* mem;
} cpu_context;

void cpu_init(cpu_context* ctx);
int cpu_step(cpu_context* ctx);
void cpu_interrupt(cpu_context* ctx, uint8_t int_num);

#endif // CPU32_H НУ ИЛИ PETYPROC 32BIT
