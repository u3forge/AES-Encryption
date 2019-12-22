%include "Substitution.inc"	; Substitution functions
%include "Rotation.inc"		; Rotation functions
%include "ColumnMixing.inc"	; Column mixing functions
%include "RoundKey.inc"		; Round key generation functions
section .data
mixedMessage TIMES 16 db 0
currentRoundKey TIMES 16 db 0

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
;   esi: message
;   edi: key
; Encrypts the message in esi
Encrypt:
    ; Add first round key
    mov esi, key
	mov edi, message
	call XOR4x4Matrices
    
    ; Create the round keys
	mov esi, key
	call CreateRoundKeys
    mov ecx, 10
    mov ebx, 1
    .NextRound:
        push ebx
        push ecx
        mov ecx, 16
        mov esi, message
        mov edi, message
        call SubstituteMessage
        
        ; mov esi, message
        ; call Print4x4Matrix

        mov esi, message
        call RotateMessage
        ; mov esi, message
        ; call Print4x4Matrix
        pop ecx
        cmp ecx, 1
        push ecx
        je .skip
        mov esi, message
        mov edi, mixedMessage
        call MixColumns
        cld
        mov esi, mixedMessage
        mov edi, message
        mov ecx, 16
        rep movsb

        ; mov esi, message
        ; call Print4x4Matrix
        .skip:
        mov edi, currentRoundKey
        pop ecx
        pop ebx
	    mov eax, ebx
        push ebx
        push ecx
	    call GetRoundKey

        ; mov esi, currentRoundKey
        ; call Print4x4Matrix

        mov esi, currentRoundKey
        mov edi, message
        call XOR4x4Matrices
        ; mov esi, mixedMessage
        ; call Print4x4Matrix
        pop ecx
        pop ebx
        inc ebx
    loop .NextRound
    ret