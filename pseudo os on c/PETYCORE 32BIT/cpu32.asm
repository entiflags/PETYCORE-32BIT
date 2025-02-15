section .text
global cpu_handle_interrupt

cpu_handle_interrupt:
    pusha
    mov eax, [esp + 36] 
    mov [eax + 0], ebx 
    call [esp + 40]
    popa
    ret