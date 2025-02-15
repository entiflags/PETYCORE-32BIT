#include "cpu32.h"
#include <stdlib.h>
#include <string.h>


void cpu_init(cpu_context* ctx) {
    memset(ctx, 0, sizeof(cpu_context));
    ctx->mem = malloc(MEM_SIZE);
    if(!ctx->mem) {
        exit(1); // обработка ошибки выделения памяти
    }
    ctx->eip = 0x000FFF0; // стартовый адрес
    ctx->esp = MEM_SIZE - 4; // верх стека
    ctx->cs = 0x8; // базовый сегмент кода
    ctx->gdtr = 0x1000; // пример адреса GDT 
}

void cpu_interrupt(cpu_context* ctx, uint8_t int_num) {
    // сохраняем состояние процессора
    ctx->mem[ctx->esp -= 4] = ctx->eip;
    ctx->mem[ctx->esp -= 4] = ctx->eflags;

    // переход к обработчику прерываний
    ctx->eip  = (ctx->idtr & 0xFFFF0000) + (int_num * 8);
}

int cpu_step(cpu_context* ctx) {
    uint8_t opcode = ctx->mem[ctx->eip];

    switch(opcode) {
        case 0x90: // NOP
            ctx->eip++;
            break;
        case 0x50: // PUSH eax
            ctx->esp -= 4;
            *(uint32_t*)&ctx->mem[ctx->esp] = ctx->eax;
            ctx->eip++;
            break;
        case 0x58: // POP eax
            ctx->eax = *(uint32_t*)&ctx->mem[ctx->esp];
            ctx->esp += 4;
            ctx->eip++;
            break;
        case 0xB8: // MOV eax, imm32
            ctx->eax = *(uint32_t*)&ctx->mem[ctx->eip+1];
            ctx->eip += 5;
            break;
        case 0x01: // ADD
            ctx->eax += *(uint32_t*)&ctx->mem[ctx->eip+1];
            update_flags(ctx, ctx->eax);
            ctx->eip += 5;
        case 0x29: // SUB
            ctx->eax -= *(uint32_t*)&ctx->mem[ctx->eip+1];
            update_flags(ctx, ctx->eax);
            ctx->eip += 5;
            break;
        case 0x74: // JE
            if (ctx->eflags & ZERO_FLAG)
                ctx->eip += *(int32_t*)&ctx->mem[ctx->eip+1];
            else
                ctx->eip += 5;
            break;
        default:
            cpu_interrupt(ctx, 0x06); // генерация исключения #UD
    }
    return 0;
}

// обновление флагов
static void update_flags(cpu_context* ctx, uint32_t result) {
    ctx->eflags = 0;
    if(result == 0) ctx->eflags |= ZERO_FLAG;
    if(result & 0x80000000) ctx->eflags |= SIGN_FLAG;
}