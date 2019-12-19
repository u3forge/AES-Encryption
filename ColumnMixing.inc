section .data
galiosField db  2, 3, 1, 1,
            db  1, 2, 3, 1,
            db  1, 1, 2, 3,
            db  3, 1, 1, 2
galiosIndexI dd 0
galiosIndexJ dd 0
messageIndexI dd 0
messageIndexJ dd 0

section .text

    ; Takes:
    ;   eax: vertical index (i)
    ;   ebx: horizontal index (j)    
    ; Returns:
    ;   dl: value of the byte
    GetGaliosNumber:
        push eax
        push ebx
        mov edx, 4
        mul edx
        xor edx, edx
        mov dl, [galiosField+ebx+eax]
        pop ebx
        pop eax
        ret

    ; Takes: 
    ;   dl: value
    ; Returns:
    ;   dl: result multiplied by 2
    AdvancedMultiplyByTwo:
        shl dl, 1
        jnc .done
        xor dl, 0x1B
        .done: ret
    
    ; Takes: 
    ;   dl: value
    ; Returns:
    ;   dl: result multiplied by 3
    AdvancedMultiplyByThree:
        mov dh, dl
        call AdvancedMultiplyByTwo
        xor dl, dh
        ret

    ; Takes:
    ;   eax: Row
    ;   ebx: Column
    ;   esi: Address of the source message
    ;   edi: Address of the destination message
    ; Multiplies the row from galio's matrix by the column from the message
    MultiplyRowByColumn:
        mov [galiosIndexI], eax
        mov [messageIndexJ], ebx
        mov DWORD [galiosIndexJ], 0
        mov DWORD [messageIndexI], 0
        push eax
        push ebx
        mov ecx, 0
        .rows:
            mov eax, [galiosIndexI]
            mov ebx, [galiosIndexJ]
            call GetGaliosNumber
            inc DWORD [galiosIndexJ]
            mov eax, [messageIndexJ]
            mov ebx, [messageIndexI]
            shl ebx, 2
            add eax, ebx
            cmp dl, 2
            jne .three
            mov dl, [esi + eax]
            call AdvancedMultiplyByTwo
            movzx edx, dl
            xor ecx, edx
            jmp .done

            .three: 
            cmp dl, 3
            jne .one
            mov dl, [esi + eax]
            call AdvancedMultiplyByThree
            movzx edx, dl
            xor ecx, edx
            jmp .done

            .one:
            mov dl, [esi + eax]
            movzx edx, dl
            xor ecx, edx

            .done:
            inc DWORD [messageIndexI]
        cmp DWORD [galiosIndexJ], 4
        jl .rows
        pop ebx
        pop eax
        xchg eax, ebx
        shl ebx, 2
        add eax, ebx
        mov [edi+eax], cl
        ret

    ; Takes:
    ;   esi: Address of the source message
    ;   edi: Address of the destination message
    MixColumns:
        mov eax, 0
        mov ebx, 0
        .nextColumn:
            mov eax, 0
            .nextRow:
                push eax
                push ebx
                call MultiplyRowByColumn
                pop ebx
                pop eax
                inc eax
            cmp eax, 4
            jl .nextRow
            inc ebx
        cmp ebx, 4
        jl .nextColumn
        ret
