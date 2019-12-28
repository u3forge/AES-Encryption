; Assembly with  nasm -f elf AES-Encryption.asm
; Link with ld -m elf_i386 AES-Encryption.o -o AES-Encryption
; Or just run make like a chad
%include "IO.inc" 			; Custom Written IO library
%include "Encryption.inc"	; Encryption functions
%include "Decryption.inc"	; Decryption functions
%include "Socket.inc"		; Socket functions
global _start

section .data
message db 0x32, 0x88, 0x31, 0xe0, 0x43, 0x5a, 0x31, 0x37, 0xf6, 0x30, 0x98, 0x07, 0xa8, 0x8d, 0xa2, 0x34
key		db 0x2b, 0x28, 0xab, 0x09, 0x7e, 0xae, 0xf7, 0xcf, 0x15, 0xd2, 0x15, 0x4f, 0x16, 0xa6, 0x88, 0x3c

section .text
_start:
	; mov     eax, [esp+8]
    ; call    SetSocketFromArg
    ; mov     esi, prompt
    ; mov     ecx, 30
    ; call    WriteString
    ; mov     bx, [socket]
    ; xchg    bl, bh
    ; movzx   eax, bx
    ; call    WriteDec
    ; call    WriteLine
    
    ; call InitialiseServer

	mov esi, message
	mov edi, key
	call Encrypt

	call Decrypt
	mov esi, message
	mov ecx, 16
	call Print4x4Matrix
	
	;Return zero
	mov eax, 0x1
	mov ebx, 0
	int 0x80