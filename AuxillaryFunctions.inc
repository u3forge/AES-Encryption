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