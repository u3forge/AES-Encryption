AES-Encryption: AES-Encryption.asm
	nasm -f elf AES-Encryption.asm
	ld -m elf_i386 AES-Encryption.o -o AES-Encryption
