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
PromptKey		db	"Enter the key: ", 0
PromptDec		db 	"1.Encrypt", 0xA, "2.Decrypt", 0xA
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
	mov esi, PromptMessage
	mov ecx, 20
	call WriteString

	mov esi, message
	mov ecx, 18
	call ReadString

	mov esi, PromptKey
	mov ecx, 15
	call WriteString

	mov esi, key
	mov ecx, 17
	call ReadString

	mov esi, PromptDec
	mov ecx, 20
	call WriteString
	
	call ReadChar
	cmp al, 0x31
	jne .decrypt
	mov esi, message
	mov edi, key
	call Encrypt
	jmp .done
	.decrypt:
	mov esi, message
	mov edi, key
	call Decrypt
	.done:
	mov esi, message
	mov ecx, 16
	call WriteString
	;Return zero
	mov eax, 0x1
	mov ebx, 0
	int 0x80