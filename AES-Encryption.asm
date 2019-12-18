; Assembly with  nasm -f elf AES-Encryption.asm
; Link with ld -m elf_i386 AES-Encryption.o -o AES-Encryption
; Or just run make like a chad
%include "IO.inc" ; Custom Written IO library
%include "SBOX.inc" ; File contining SBOX

global _start
section .data:
	val db	0x41
	st db "TEST2", 0x0
section .text:
_start:
	mov eax, 5
	mov ebx, 5
	call GetSBOXByte
	movzx eax, dl
	call WriteHex
	;call PrintSBOX
	;Return zero
	mov eax, 0x1
	mov ebx, 0
	int 0x80