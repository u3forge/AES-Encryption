section .text:
WriteChar:
    push ecx
    push edx
    push ebx
	push eax       	;Store parameter on the stack
	mov ecx, esp	;Get address of the parameter
	mov eax, 0x4	;Specifying the write syscall
	mov ebx, 0x1	;STDOUT file descriptor
	mov edx, 1		;Printing one byte
	int 0x80		;Invoke the syscall
	pop eax			;Clean up the stack
    pop ebx
    pop edx
    pop ecx
	ret

WriteString:
	.print:
	mov eax, [esi]
	push ecx
	call WriteChar
	pop ecx
	inc esi
	loop .print
	ret

WriteDec:
	push 0xf1a9
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
		cmp eax, 0xf1a9
		je .done
		call WriteChar
		jmp .print
	.done:
	ret

WriteHex:
    push edx
	push eax
	mov al, '0'
	call WriteChar
	mov al, 'x'
	call WriteChar
	pop eax
	push 0xf1a9
	.next:
		mov ecx, 16
		mov edx, 0
		div ecx
        cmp dl, 10
        jl  .zeroto9
            add dl, 0x7
        .zeroto9:
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
		cmp eax, 0xf1a9
		je .done
		call WriteChar
		jmp .print
	.done:
    pop edx
	ret
