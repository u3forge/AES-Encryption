; Assembly with  nasm -f elf AES-Encryption.asm
; Link with ld -m elf_i386 AES-Encryption.o -o AES-Encryption
; Or just run make like a chad
%include "IO.inc" ; Custom Written IO library
%include "Substitution.inc"; Substitution functions
global _start
section .data:

section .text:
_start:
	mov cl, 0x76
	call GetLowerNibble
	movzx eax, bl
	call WriteHex
	call GetHigherNibble
	movzx eax, bh
	call WriteHex

	;Return zero
	mov eax, 0x1
	mov ebx, 0
	int 0x80