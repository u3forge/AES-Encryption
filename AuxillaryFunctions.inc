section .text

; Takes:
;   esi: matrix 1
;   edi: matrix 2
; XORs the matrices and places them in matrix 2
XOR4x4Matrices:
    push eax
    push ecx
    mov ecx, 16
    ._xor:
        mov al, BYTE [esi]
        xor BYTE [edi], al
        inc edi
        inc esi
    loop ._xor
    pop ecx
    pop eax
    ret

; Takes:
;   eax: Hex Value
; Returns:
;   edx: Decimal Value with the same digits
GetDecimalValueFromHex:
    xor edx, edx
    xor ebx, ebx
    mov ebx, 1
    .cont:
        mov cl, al
        and cl, 0x0F
        push eax
        push edx
        movzx eax, cl
        mul ebx
        pop edx
        add edx, eax
        pop eax
        shr eax, 4
        push eax
        xchg eax, ebx
        mov ebx, 10
        push edx
        mul ebx
        pop edx
        xchg eax, ebx
        pop eax
        cmp eax, 0
    jne .cont
    ret
    
