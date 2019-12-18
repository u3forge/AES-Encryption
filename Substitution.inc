%include "SBOX.inc" ; File contining SBOX and its functions

section .text:
    ;Takes:
    ;   cl: value
    ;   bl: lowernibble
    GetLowerNibble:
        mov bl, cl
        and bl, 0b00001111
        ret
    
    ;Takes:
    ;   cl: value
    ;   bh: highernibble
    GetHigherNibble:
        mov bh, cl
        and bh, 0b11110000
        shr bh, 4
        ret