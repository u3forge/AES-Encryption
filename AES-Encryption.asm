; Assembly with  nasm -f elf AES-Encryption.asm
; Link with ld -m elf_i386 AES-Encryption.o -o AES-Encryption
; Or just run make like a chad
%include "IO.inc" 			; Custom Written IO library
%include "Encryption.inc"	; Encryption functions
%include "Decryption.inc"	; Decryption functions
%include "Socket.inc"		; Socket functions
global _start

section .data
message  TIMES 16 db 0
key		 TIMES 16 db 0
PromptMessage 	db 	"Enter the message: ",0
PromptHex		db 	"Enter the 16 hex bytes without leading '0x', pad the bytes to be 2 digits wide:", 0xA
PromptKey		db	"Enter the key: ", 0
PromptDec		db 	"1.Encrypt", 0xA, "2.Decrypt", 0xA
section .text
_start:
	cmp DWORD  [esp], 1
	ja .server
	;Ask whether to encrypt or decrypt
	mov esi, PromptDec
	mov ecx, 20
	call WriteString
	call ReadChar
	cmp al, 0x31
	jne .decrypt

	;Print prompt for message
	mov esi, PromptMessage
	mov ecx, 20
	call WriteString
	;Read message
	mov esi, message
	mov ecx, 18
	call ReadString
	;Print prompt for key
	mov esi, PromptKey
	mov ecx, 15
	call WriteString
	;Read key
	mov esi, key
	mov ecx, 17
	call ReadString
	;Encrypt the message
	mov esi, message
	mov edi, key
	call Encrypt

	;Print the encrypted message
	mov esi, message
	call Print4x4Matrix
	jmp .done

	.decrypt:
	;Print promp for encrypted message
	mov esi, PromptHex
	mov ecx, 80
	call WriteString
	;Read 16 bytes
	mov esi, message
	mov ecx, 16
	.input:
		call ReadHexByte
		mov BYTE[esi], al
		inc esi
	loop .input
	;Print prompt for key
	mov esi, PromptKey
	mov ecx, 15
	call WriteString
	;Read key
	mov esi, key
	mov ecx, 17
	call ReadString
	;Decrypt the string
	mov esi, message
	mov edi, key
	call Decrypt
	;Print decrypted string
	mov esi, message
	mov ecx, 16
	call WriteString
	jmp .done
	.server:
	mov     eax, [esp+8]
    call    SetSocketFromArg
    mov     esi, prompt
    mov     ecx, 30
    call    WriteString
    mov     bx, [socket]
    xchg    bl, bh
    movzx   eax, bx
    call    WriteDec
    call    WriteLine
    
    call InitialiseServer
	.done:
	;Return zero
	mov eax, 0x1
	mov ebx, 0
	int 0x80