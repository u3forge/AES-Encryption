%include "SBOX.inc" ; File contining SBOX and its functions

section .text
    ;Takes:
    ;   cl: value
    ;Returns:
    ;   bl: lowernibble
    GetLowerNibble:
        mov bl, cl
        and bl, 0b00001111
        ret
    
    ;Takes:
    ;   cl: value
    ;Returns:
    ;   bh: highernibble
    GetHigherNibble:
        mov bh, cl
        and bh, 0b11110000
        shr bh, 4
        ret
    
    ;Takes:
    ;   esi: source message
    ;   edi: destination message
    ;   ecx: length of the message
    ;Returns:
    ;   edi: source message substituted by SBOX
    SubstituteMessage:
        .substitute:
            push ecx
            mov cl, [esi]
            call GetLowerNibble
            call GetHigherNibble
            movzx eax, bh
            movzx ebx, bl
            call GetSBOXByte
            mov BYTE [edi], dl
            inc esi
            inc edi
            pop ecx
	    loop .substitute
        ret