AES-Encryption: AES-Encryption.asm SBOX.inc IO.inc Substitution.inc Rotation.inc ColumnMixing.inc RoundKey.inc Encryption.inc SBOXInverse.inc Decryption.inc AuxillaryFunctions.inc Socket.inc
	nasm -f elf AES-Encryption.asm
	ld -m elf_i386 AES-Encryption.o -o AES-Encryption
