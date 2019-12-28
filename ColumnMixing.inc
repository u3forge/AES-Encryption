section .data
galiosField db  2, 3, 1, 1,
            db  1, 2, 3, 1,
            db  1, 1, 2, 3,
            db  3, 1, 1, 2

galiosFieldInverse db 0x0E, 0x0B, 0x0D, 0x09,
                   db 0x09, 0x0E, 0x0B, 0x0D,
                   db 0x0D, 0x09, 0x0E, 0x0B,
                   db 0x0B, 0x0D, 0x09, 0x0E
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
    ;   dl: value
    ; Returns:
    ;   dl: result multiplied by 9
    AdvancedMultiplyByNine:
        mov dh, dl ;x0
        call AdvancedMultiplyByTwo
        ; dl now contains x1=x0*2
        call AdvancedMultiplyByTwo
        ; dl now contains x2=x1*2
        call AdvancedMultiplyByTwo
        ; dl now contains x3=x2*2
        xor dl, dh
        ret

    ; Takes: 
    ;   dl: value
    ; Returns:
    ;   dl: result multiplied by 11
    AdvancedMultiplyBy0B:
        push eax
        mov dh, dl      ;x0
        call AdvancedMultiplyByTwo
        ;dl now contains x1=x0*2
        mov al, dl
        call AdvancedMultiplyByTwo
        ;dl now contains x2=x1*2
        call AdvancedMultiplyByTwo
        ;dl now contains x3=x2*2
        xor dl, dh
        xor dl, al
        pop eax
        ret

    ; Takes: 
    ;   dl: value
    ; Returns:
    ;   dl: result multiplied by 13
    AdvancedMultiplyBy0D:
        push eax
        mov dh, dl  ;save x0 in dh
        call AdvancedMultiplyByTwo
        ;dl now contains x1=x0*2
        mov al, dl  ;save x1 in al
        call AdvancedMultiplyByTwo
        ;dl now contains x2=x1*2
        mov ah, dl  ;save x2 in ah
        call AdvancedMultiplyByTwo
        ;dl now contains x3=x2*2
        xor dl, dh
        xor dl, ah
        pop eax
        ret

    ; Takes: 
    ;   dl: value
    ; Returns:
    ;   dl: result multiplied by 11
    AdvancedMultiplyBy0E:
        push eax
        call AdvancedMultiplyByTwo
        ;dl now contains x1=x0*2
        mov al, dl  ;save x1 in al
        call AdvancedMultiplyByTwo
        ;dl now contains x2=x1*2
        mov ah, dl  ;save x2 in ah
        call AdvancedMultiplyByTwo
        ;dl now contains x3=x2*2
        xor dl, al
        xor dl, ah
        pop eax
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
            jne .nine
            mov dl, [esi + eax]
            call AdvancedMultiplyByThree
            movzx edx, dl
            xor ecx, edx
            jmp .done

            .nine:
            cmp dl,9
            jne .zeroB
            mov dl, [esi + eax]
            call AdvancedMultiplyByNine
            movzx edx,dl
            xor ecx,edx
            jmp .done

            .zeroB:
            cmp dl,0x0B
            jne .zeroD
            mov dl, [esi + eax]
            call AdvancedMultiplyBy0B
            movzx edx,dl
            xor ecx,edx
            jmp .done
            
            .zeroD:
            cmp dl,0x0D
            jne .zeroE
            mov dl, [esi + eax]
            call AdvancedMultiplyBy0D
            movzx edx,dl
            xor ecx,edx
            jmp .done

            .zeroE:
            cmp dl,0x0E
            jne .one
            mov dl, [esi + eax]
            call AdvancedMultiplyBy0E
            movzx edx,dl
            xor ecx,edx
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


    GaliosDecryption:
    push eax 
    mov eax,galiosFieldInverse
    mov [galiosField],eax 
    pop eax

    mov DWORD [galiosIndexI],0
    mov DWORD [galiosIndexJ],0
    mov DWORD [messageIndexI],0
    mov DWORD [messageIndexJ],0

    ret