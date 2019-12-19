section .data
tmp dd 0

section .text

;edx : array
;esi : row start index
;edi : rotation number

RowRot:
    
    mov ecx,edi
    mov [tmp],esi
    .rotnum
        mov edi,ecx
        mov ecx,3
        mov esi,[tmp]
        mov bl,[edx+esi] 
        .rot:
            mov al,[edx+esi+1]
            mov [esi+edx],al
            inc esi
        loop .rot
        mov [edx+esi],bl
        mov ecx,edi
    loop .rotnum    
    ret

RotationMessage:
    ;second row rotate 1
	mov esi,4
	mov edi,1
    call RowRot
    
	;third row rotate 2
	mov esi,8
	mov edi,2
	call RowRot

	;fourth row rotate 3
	mov esi,12
	mov edi,3
	call RowRot

    ret