; Assembly with  nasm -f elf AES-Encryption.asm
; Link with ld -m elf_i386 AES-Encryption.o -o AES-Encryption
; Or just run make like a chad
%include "IO.inc" 			; Custom Written IO library
%include "Substitution.inc"	; Substitution functions
%include "Rotation.inc"		; Rotation functions
%include "ColumnMixing.inc"	; Column mixing functions
global _start

section .data
message db 0x19, 0xa0, 0x9a, 0xe9, 0x3d, 0xf4, 0xc6, 0xf8, 0xe3, 0xe2, 0x8d, 0x48, 0xbe, 0x2b, 0x2a, 0x08
encrypted TIMES 16 db 0

section .text
_start:

	mov ecx, 16
	mov esi, message
	mov edi, encrypted
	call SubstituteMessage
	mov esi, encrypted
	call RotateMessage
	mov esi, encrypted
	call MixColumns

	;Print the substituted message
	mov edi, encrypted
	mov bl, 0
	mov ecx, 16
	.L1:
		xor eax, eax
		mov al, [edi]
		call WriteHex
		inc edi
		inc bl
		cmp bl, 4
		jne .next
		mov al, 0xA
		call WriteChar
		mov bl, 0
		jmp .done
		.next:
		mov al, ' '
		call WriteChar
		.done:
	loop .L1

	;Return zero
	mov eax, 0x1
	mov ebx, 0
	int 0x80