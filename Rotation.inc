section .text

;Takes:
;   edx: Row Start index
;   esi: Source and Desination message
;   edi: Number of rotations
;   bh : increment 1 for row rot 4 for column rot
Rot:
    push edi
    mov ecx,edi
    push edx
    .rotnum:
        mov edi,ecx
        mov ecx,3
        mov edx,[esp]
        mov bl,[esi+edx] 
        .rot:
            add dl,bh
            mov al,[esi+edx]
            sub dl,bh
            mov [esi+edx],al
            add dl,bh
        loop .rot
        mov [esi+edx],bl
        mov ecx,edi
    loop .rotnum    
    pop edx
    pop edi
    ret

;Takes:
;   esi: Source and Desination message
RotateMessage:

	mov edx,4
	mov edi,1
    mov bh,1
    .rotate:
        call Rot
        add edx, 4
        inc edi
    cmp edi, 4
    jl .rotate
    ret