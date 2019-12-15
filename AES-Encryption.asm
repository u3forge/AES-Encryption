; Assembly with  nasm -f elf AES-Encryption.asm
; Link with ld -m elf_i386 AES-Encryption.o -o AES-Encryption
; Or just run make like a chad
global _start
section .data:
	val db	0x41

section .text:
_start:

	mov eax, 123456
	call WriteDec

	mov al, 0xA
	call WriteChar
	
	mov eax, 0x190
	call WriteHex

	mov al, 0xA
	call WriteChar

	;Return zero
	mov eax, 0x1
	mov ebx, 0
	int 0x80

WriteChar:
	push eax       	;Store parameter on the stack
	mov ecx, esp	;Get address of the parameter
	mov eax, 0x4	;Specifying the write syscall
	mov ebx, 0x1	;STDOUT file descriptor
	mov edx, 1		;Printing one byte
	int 0x80		;Invoke the syscall
	pop eax			;Clean up the stack
	ret

WriteDec:
	push 1337
	.next:
		mov ecx, 10
		mov edx, 0
		div ecx
		add dl, 0x30
		mov edi, eax
		xor eax, eax
		mov al, dl
		push eax
		mov eax, edi
	cmp eax, 0
	jne .next
	.print:
		pop eax
		cmp eax, 1337
		je .done
		call WriteChar
		jmp .print
	.done:
	ret

WriteHex:
	push eax
	mov al, '0'
	call WriteChar
	mov al, 'x'
	call WriteChar
	pop eax
	push 1337
	.next:
		mov ecx, 16
		mov edx, 0
		div ecx
		add dl, 0x30
		mov edi, eax
		xor eax, eax
		mov al, dl
		push eax
		mov eax, edi
	cmp eax, 0
	jne .next
	.print:
		pop eax
		cmp eax, 1337
		je .done
		call WriteChar
		jmp .print
	.done:
	ret
