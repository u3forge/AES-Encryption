section .data
RConstantMatrix db 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
                TIMES 30 db 0

RoundKeys TIMES 176 db 0
; 4*4*11 Matrix for all of the round keys.
; TODO: Optimise this shit :D 
column1 db 1,1,1,1
column2 db 2,2,2,2
section .text

; Takes:
;   esi: matrix
;   edi: destination
;   eax: column
;   edx: size of matrix row
GetColumn:
    push edi
    push eax
    mov ecx, 4
    .next:
        mov bl, BYTE [esi + eax]
        mov BYTE [edi], bl
        inc edi
        add eax, edx
    loop .next
    pop eax
    pop edi
    ret

; Takes: 
;   esi: column
;   edi: matrix
;   eax: column to be replaced
;   edx: size of the matrix row
PlaceColumn:
    push esi
    push eax
    mov ecx, 4
    .next:
        mov bl, BYTE [esi]
        mov BYTE [edi+eax], bl
        inc esi
        add eax, edx
    loop .next
    pop eax
    pop esi
    ret
; Takes:
;   esi: key
CreateRoundKeys:
    ; First copy the cipher key for Round 0
    push esi ;save esi
    xor eax, eax
    .nextColumn:
        mov edi, column1
        mov esi, [esp]
        mov edx, 4
        call GetColumn

        mov esi, column1
        mov edi, RoundKeys
        mov edx, 44
        call PlaceColumn

        inc eax
    cmp eax, 4
    jne .nextColumn
    pop esi ;stack cleanup

    ; Calculate the rest of the Round keys
    mov eax, 4 ; This is W
    mov ebx, 4
    mov edx, 0
    push eax
    div ebx
    ; cmp edx, 0
    ; jne .NotAFirstColumn

    mov esi, RConstantMatrix
    mov edi, column1
    mov edx, 10
    sub eax, 1
    call GetColumn

    pop eax
    mov esi, RoundKeys
    mov edi, column2
    mov edx, 44
    sub eax, 1
    call GetColumn
    add eax, 1
    push eax

    ; Shift the column by one byte
    mov ebx, [column2]
    ror ebx, 8
    mov [column2], ebx

    ; Substitute the bytes
    mov ecx, 4
    mov esi, column2
    mov edi, column2
    call SubstituteMessage

    ; XOR W[i] and RCON
    mov ebx, [column2]
    mov eax, [column1]
    xor eax, ebx
    mov [column1], eax

    ; Get W[i-4]
    pop eax
    sub eax, 4
    mov esi, RoundKeys
    mov edi, column2
    mov edx, 44
    call GetColumn
    add eax, 4
    push eax

    ; XOR W[i] XOR RCON XOR W[i-4]
    mov ebx, [column2]
    mov eax, [column1]
    xor eax, ebx
    
    ; Save it 
    mov [column1], eax

    ; Place the column at W[i]
    pop eax
    mov esi, column1
    mov edi, RoundKeys
    mov edx, 44
    call PlaceColumn
    inc eax

    ret



  ;  ret

PrintRoundKeys:
    mov bl, 0
    mov dl, 0
	mov ecx, 176
	.print:
		xor eax, eax
		mov al, [esi]
		call WriteHex
		inc esi
		inc bl
        inc dl
        cmp bl, 44
		jne .next
		mov al, 0xA
		call WriteChar
		mov bl, 0
        mov dl, 0
		jmp .done
		.next:
            cmp dl, 4
            je .tab
            mov al, ' '
            call WriteChar
            jmp .done
        .tab:
        mov al, ' '
        call WriteChar
        mov al, ' '
        call WriteChar
        mov dl, 0
		.done:
	loop .print
	ret