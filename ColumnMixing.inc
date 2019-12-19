section .data
galiosField db  2, 3, 1, 1,
            db  1, 2, 3, 1,
            db  1, 1, 2, 3,
            db  3, 1, 1, 2

section .text

    ; Takes:
    ;   eax: vertical index (i)
    ;   ebx: horizontal index (j)    
    ; Returns:
    ;   dl: value of the byte
    GetGaliosNumber:
        mov edx, 4
        mul edx
        xor edx, edx
        mov dl, [galiosField+ebx+eax]
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
    ;   esi: address of the message
    ; Mixed the columns of the message in place
    MixColumns:
        ret