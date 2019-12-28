section .text


;Takes:
;   edx: Row Start index
;   esi: Source and Desination message
;   edi: Number of rotations
;   bh : 1 for encryption  2 for decryption
RotateMessage:

	mov edx,4
	mov edi,1
    mov cl,8
    push eax
    .rotate:
         ;call Rot
        mov eax,[esi+edx]
        cmp bh,1
        je .encr
        cmp bh,2
        je .decr
        .encr:
            ror eax,cl
            jmp .cnt
        .decr:
            rol eax,cl
        .cnt:
            mov [esi+edx],eax
            add edx, 4
            inc edi
            add cl,8
        cmp edi, 4
        jl .rotate
        pop eax
    ret


