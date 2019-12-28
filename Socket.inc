section .data
response db 'HTTP/1.1 200 OK', 0Dh, 0Ah, 'Content-Type: text/html', 0Dh, 0Ah, 'Content-Length: 16', 0Dh, 0Ah, 0Dh, 0Ah, 'ABCDABCDABCDABCD', 0Dh, 0Ah, 0h
prompt   db 'Running on localhost at port: '
section .bss
buffer resb 255,    ; variable to store request headers
socket resw 0       ; variable to store the socket number in hex, reverse bytes

section .text
InitialiseServer:
    xor eax, eax
    xor ebx, ebx
    xor edi, edi
    xor esi, esi
    ;Initiliase the socket
    push BYTE 6              ; push 6 onto the stack (IPPROTO_TCP)
    push BYTE 1              ; push 1 onto the stack (SOCK_STREAM)
    push BYTE 2              ; push 2 onto the stack (PF_INET)
    mov ecx, esp            ; move address of arguments into ecx
    mov ebx, 1              ; invoke subroutine SOCKET (1)
    mov eax, 102            ; invoke SYS_SOCKETCALL (kernel opcode 102)
    int 80h                 ; call the kernel
    
    ;Bind the server to the socket at localhost
    mov edi, eax            ; move return value of SYS_SOCKETCALL into edi (file descriptor for new socket, or -1 on error)
    push DWORD 0x00000000    ; move 0 dec onto the stack IP ADDRESS
    push WORD [socket]       ; move 9001 dec onto stack PORT
    push WORD 2              ; move 2 dec onto stack AF_INET
    mov ecx, esp            ; move address of stack pointer into ecx
    push BYTE 16             ; move 16 dec onto stack (arguments length)
    push ecx                 ; push the address of arguments onto stack
    push edi                 ; push the file descriptor onto stack
    mov ecx, esp            ; move address of arguments into ecx
    mov ebx, 2              ; invoke subroutine BIND (2)
    mov eax, 102            ; invoke SYS_SOCKETCALL (kernel opcode 102)
    int 80h                 ; call the kernel
    
    ;Listen on the port for an encoming request
    push BYTE 1              ; move 1 onto stack (max queue length argument)
    push edi                 ; push the file descriptor onto stack
    mov ecx, esp            ; move address of arguments into ecx
    mov ebx, 4              ; invoke subroutine LISTEN (4)
    mov eax, 102            ; invoke SYS_SOCKETCALL (kernel opcode 102)
    int 80h                 ; call the kernel

    ;Accept an incoming request
    .accept:
    push BYTE 0              ; push 0 dec onto stack (address length argument)
    push BYTE 0              ; push 0 dec onto stack (address argument)
    push edi                 ; push the file descriptor onto stack
    mov ecx, esp            ; move address of arguments into ecx
    mov ebx, 5              ; invoke subroutine ACCEPT (5)
    mov eax, 102            ; invoke SYS_SOCKETCALL (kernel opcode 102)
    int 80h                 ; call the kernel
    
    ;Fork the server into 2 child processes, one to read the buffer, one to accept the request
    mov esi, eax            ; move return value of SYS_SOCKETCALL into esi (file descriptor for accepted socket, or -1 on error)
    mov eax, 2              ; invoke SYS_FORK (kernel opcode 2)
    int 80h                 ; call the kernel

    cmp eax, 0              ; if return value of SYS_FORK in eax is zero we are in the child process
    jz .read               ; jmp in child process to _read

    jmp .accept             ; jmp in parent process to _accept
    
    ;Read the sent request
    .read:
    mov edx, 255            ; number of bytes to read (we will only read the first 255 bytes for simplicity)
    mov ecx, buffer         ; move the memory address of our buffer variable into ecx
    mov ebx, esi            ; move esi into ebx (accepted socket file descriptor)
    mov eax, 3              ; invoke SYS_READ (kernel opcode 3)
    int 80h                 ; call the kernel
    
    mov  eax, buffer         ; move the memory address of our buffer variable into eax for printing
    call sprintLF            ; call our string printing function

    ;Send the value of the request to the socket
    mov edx, 80             ; move 80 dec into edx (length in bytes to write)
    mov ecx, response       ; move address of our response variable into ecx
    mov ebx, esi            ; move file descriptor into ebx (accepted socket id)
    mov eax, 4              ; invoke SYS_WRITE (kernel opcode 4)
    int 80h                 ; call the kernel
    
    ;Close the socket
    mov ebx, esi            ; move esi into ebx (accepted socket file descriptor)
    mov eax, 6              ; invoke SYS_CLOSE (kernel opcode 6)
    int 80h                 ; call the kernel
    ret

; Takes:
;   eax: address of argv[1]
; Sets the value of the socket varibale to argv[1] in reverse byte order
SetSocketFromArg:
    mov eax, [eax]
    push eax
    sub al, 0x30
    sub ah, 0x30
    shl al, 4
    add al, ah
    movzx bx, al
    pop eax
    shr eax, 16
    sub al, 0x30
    sub ah, 0x30
    shl al, 4
    add al, ah
    shl bx, 8
    and ax, 0xFF
    add bx, ax
    movzx eax, bx
    call GetDecimalValueFromHex
    xchg dh, dl
    mov [socket], dx
    ret