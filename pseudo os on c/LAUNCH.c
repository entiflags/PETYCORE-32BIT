#include "PETYCORE 32BIT\cpu32.h"
#include <stdio.h>

#define MEM_SIZE 0x100000 // 1 mb памяти

int main() {
    cpu_context ctx;
    cpu_init(&ctx);

    // пример программы в памяти
    uint8_t program[] = {
        0xB8, 0x01, 0x00, 0x00, 0x00, // MOV eax, 1
        0x29, 0x00, 0x00, 0x00, 0x00, // SUB eax, 2
        0x74, 0xFB, 0xFF, 0xFF, 0xFF // JE (условный период)
    };

    // загрузка программы в память
    memcpy(ctx.mem + 0x1000, program, sizeof(program));
    ctx.eip = 0x1000;

    // выполнение
    for(int i = 0; i < 100; i++) {
        if(cpu_step(&ctx)) break;
    }

    printf("EAX: 0x%08X\n", ctx.eax);
    free(ctx.mem);
    return 0;
}

// ВСЕ ЭТО КОНЕЦ ЛАУНЧА ЧТОБЫ ЗАПУСКАТЬ ЭМУЛЯТОР ВВОДИТЕ КОМАНДЫ ДЛЯ КОМПИЛЯЦИИ В ТЕРМИНАЛЕ ДИРЕКТОРИИ!
// КОМАНДА:
// gcc -o emulator LAUNCH.c cpu32.c
// ./emulator
// ДЛЯ РАБОТЫ ТАКЖЕ НУЖЕН gcc