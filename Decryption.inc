section .data
rkey TIMES 16 db 0
section .text

; Takes:
;   esi: encrypted message
;   edi: input key
; decrypts the message in esi
Decrypt:
    ; Add last round key
    mov eax,10
    mov edi,rkey
    call GetRoundKey
    mov esi, rkey
	mov edi, message
	call XOR4x4Matrices


    mov esi,message
    mov bh,2
    call RotateMessage

    mov edi, esi
    mov ecx,16
    call SubstituteMessageInverse
    ret