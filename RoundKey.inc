section .data
RConstantMatrix db 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
                TIMES 30 db 0

RoundKeys TIMES 176 db 0
; 4*4*11 Matrix for all of the round keys.
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
    push ebx
    mov ecx, 4
    .next:
        mov bl, BYTE [esi + eax]
        mov BYTE [edi], bl
        inc edi
        add eax, edx
    loop .next
    pop ebx
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
;   ecx: value of eax before the division
CreateFirstColumn:
    push ecx
    push ecx
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
    pop ecx
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
    mov ecx, 40
    .schedule:
        push ecx
        mov ebx, 4
        mov edx, 0
        mov ecx, eax
        div ebx
        cmp edx, 0
        jne .NotAFirstColumn
        call CreateFirstColumn 
        jmp .next
        .NotAFirstColumn:
            mov eax, ecx
            mov ebx, 0
            mov ecx, 2
            .GetTheNextColumns:
                push ecx
                cmp ecx, 2
                je .sub1
                    sub eax, 3
                    jmp .done
                .sub1:
                    sub eax, 1
                .done:
                mov esi, RoundKeys
                mov edi, column1
                mov edx, 44
                call GetColumn
                xor ebx, [column1]
                pop ecx
            loop .GetTheNextColumns
            mov [column1], ebx

            add eax, 4
            ; Place the column at W[i]
            mov esi, column1
            mov edi, RoundKeys
            mov edx, 44
            call PlaceColumn
        .next:
        inc eax
        pop ecx
    loop .schedule
    ret

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

; Takes:
;   edi: variable to store the round in
;   eax: the number of the round
GetRoundKey:
    push edi
    mov ebx, 4
    mul ebx
    mov edx, eax
    mov ecx, 4
    .GetNextColumn:
        push ecx
        mov esi, RoundKeys
        mov edi, column1
        push edx
        mov edx, 44
        call GetColumn
        pop edx
        push eax
        sub eax, edx
        mov esi, column1
        mov edi, [esp+0x8]
        mov ebx, 4
        push edx
        mov edx, 4
        call PlaceColumn
        pop edx
        pop eax
        inc eax
        pop ecx
    loop .GetNextColumn
    pop edi
    ret