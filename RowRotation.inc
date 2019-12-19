section .text

;Takes:
;   edx: Row Start index
;   esi: Source and Desination message
;   edi: Number of rotations
RowRot:
    push edi
    mov ecx,edi
    push edx
    .rotnum:
        mov edi,ecx
        mov ecx,3
        mov edx,[esp]
        mov bl,[esi+edx] 
        .rot:
            mov al,[esi+edx+1]
            mov [esi+edx],al
            inc edx
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
    .rotate:
        call RowRot
        add edx, 4
        inc edi
    cmp edi, 4
    jl .rotate
    ; ;second row rotate 1

    ; call RowRot
    
	; ;third row rotate 2
	; mov edx,8
	; mov edi,2
	; call RowRot

	; ;fourth row rotate 3
	; mov edx,12
	; mov edi,3
	; call RowRot

    ret