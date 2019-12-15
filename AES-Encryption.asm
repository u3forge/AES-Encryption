; Assembly with  nasm -f elf AES-Encryption.asm
; Link with ld -m elf_i386 AES-Encryption.o -o AES-Encryption
; Or just run make like a chad
global _start
section .data:
	val db	0x41

section .text:
_start:

	mov eax, val
	call WriteChar

	;Return zero
	mov eax, 0x1
	mov ebx, 0
	int 0x80

WriteChar:
	mov ecx, eax       		;Specifying the thing to be printed
	
	mov eax, 0x4           	;Specifying the write syscall
	mov ebx, 0x1           	;STDOUT file descriptor
	mov edx, 1				;Specifying the count of bytes
	int 0x80               	;Invoke the syscall
	ret