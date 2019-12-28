section .data
rkey TIMES 16 db 0
ImixedMessage TIMES 16 db 0
section .text

; Takes:
;   esi: encrypted message
;   edi: input key
; decrypts the message in esi
Decrypt:
    mov esi, edi
    call CreateRoundKeys
    ;First round
    ; Add last round key
    mov eax,10
    mov edi,rkey
    call GetRoundKey
    mov esi, rkey
	mov edi, message
	call XOR4x4Matrices

    mov ecx,9

    .rounds:
    push ecx 
    ;inverse shift
    mov esi,message
    mov bh,2
    call RotateMessage


    ;inverse sub
    mov edi, message
    mov esi, message
    mov ecx,16
    call SubstituteMessageInverse

    ;inverse Add round key
    pop ecx
    mov eax,ecx
    push ecx
    mov edi,rkey
    call GetRoundKey

    mov esi,rkey
    mov edi,message
    call XOR4x4Matrices
    
    ;inverse mix coulumns
    call GaliosDecryption
    mov esi, message
    mov edi, ImixedMessage
    call MixColumns
    cld
    mov esi, ImixedMessage
    mov edi, message
    mov ecx, 16
    rep movsb

    pop ecx  

    loop .rounds

    ;inverse shift
    mov esi,message
    mov bh,2
    call RotateMessage

    ;inverse sub
    mov edi, esi
    mov ecx,16
    call SubstituteMessageInverse

    mov esi, key
	mov edi, message
	call XOR4x4Matrices    

    ret