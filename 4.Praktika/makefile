all:	decrypt_AESNI decrypt_AES

decrypt_AESNI: decrypt.c libaesni/libaes_lin64.so
	gcc	-DAESNI	-o	decrypt_AESNI	decrypt.c	libaesni/libaes_lin64.so

decrypt_AES: decrypt.c tiny_aes/aes.c
	gcc	-o	decrypt_AES	decrypt.c	tiny_aes/aes.c