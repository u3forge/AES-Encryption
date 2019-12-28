section .text
; Takes:
;	al: char
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

; Returns:
;	al: value of the char
ReadChar:
	sub esp, 4		;allocate a dword on the stack
   	mov eax, 3		;sys_read systemcall
   	mov ebx, 0		;STDIN file descriptor
   	mov ecx, esp	;move the allocated dword address to ecx
   	mov edx, 1		;Reading one byte
   	int 80h			;Invoke the syscall

	mov al, BYTE [esp] 	;Move the read value into al
	add esp, 4			;Clean up the stack
	ret

; Takes:
;	ecx: String maximum length
;	esi: address of the string
; Returns:
;	eax: number of bytes read
ReadString:
	push esi
	push ecx
	mov eax, 3		;sys_read systemcall
	mov ebx, 0		;STDIN file descriptor
	mov edx, ecx	;Reading n bytes
	mov ecx, esi	;move the allocated dword address to ecx
	int 80h			;Invoke the syscall
	pop ecx
	pop esi

	mov eax, 0
	.count:
	inc eax
	inc esi
	cmp eax, ecx
	je .equal
	cmp BYTE [esi], 0xA
	je .done
	jmp .count
	.equal:
		mov eax, ecx
	.done:
	ret

WriteLine:
	mov al, 0xA
	call WriteChar
	ret

; Takes:
;	esi: address of the string
;	ecx: length of the string
WriteString:
	.print:
	mov eax, [esi]
	call WriteChar
	inc esi
	loop .print
	ret

; Takes:
;	eax: value
; Prints the value in decimal (base-10)
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

; Takes:
;	eax: value
; Prints the value in hexa (base-16)
WriteHex:
    push edx
    push ecx
    push edi
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
    pop edi
    pop ecx
    pop edx
	ret

; Takes:
;	esi: array of length 16
; Prints the array as a 4x4 grid
Print4x4Matrix:
	mov bl, 0
	mov ecx, 16
	.print:
		xor eax, eax
		mov al, [esi]
		call WriteHex
		inc esi
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
	loop .print
	ret

sprintLF:
    call sprint
    push eax
    mov eax, 0xA
    push eax
    mov eax, esp
    call sprint
    pop eax
    pop eax
    ret
	
sprint:
    push edx
    push ecx
    push ebx
    push eax
    call strlen

    mov edx, eax
    pop eax

    mov ecx, eax
    mov ebx, 1
    mov eax, 4
    int 80h

    pop ebx
    pop ecx
    pop edx
    ret

strlen:
    push ebx
    mov ebx, eax
.nextchar:
    cmp BYTE [eax], 0
    jz .finished
    inc eax
    jmp .nextchar
.finished:
    sub eax, ebx
    pop ebx
    ret