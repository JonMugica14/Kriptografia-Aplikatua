#include "AES_CBC.h"
#include <math.h>

int valid(uint8_t *plaintext, uint8_t *ciphertext, uint32_t clen)
{
	uint32_t plen = decipher_AES_CBC_PO(plaintext, ciphertext, clen);
	if (plen != 0) // valid pad when plen!=0
	{
		printf("Plaintext (plen = %d): ", plen);
		print_c(plaintext, plen);
		return 1;
	}
	else // invalid pad when plen=0
	{
		//printf("Padding error, plen = %d.\n", plen);
		return 0;
	}
}

int main(void)
{
	uint8_t cor;
	uint32_t clen, plen;

	uint8_t *ciphertext_hex = "60592ff65e192e29a29be678fc8873cd0aabea229e2d4521568b1fa32712a1fd8037b482bbc8f3bc523ad5e2e2fd0868";
	clen = strlen(ciphertext_hex) / 2;

	uint8_t *ciphertext = malloc(clen * sizeof(uint8_t));
	uint8_t *intermediate = malloc(16 * sizeof(uint8_t));
	uint8_t *probakoa = malloc(clen * sizeof(uint8_t));
	uint8_t *new_plaintext = malloc(16 * sizeof(uint8_t));
	
	parse(clen, ciphertext_hex, ciphertext);
	printf("Ciphertext (hex): ");
	print_hex(ciphertext, clen);

	plen = clen;

	uint8_t *plaintext = malloc(plen * sizeof(uint8_t));
	plen = decipher_AES_CBC_PO(plaintext, ciphertext, clen);
	/*if (plen != 0) // valid pad when plen!=0
	{
		printf("Plaintext (plen = %d): ", plen);
		print_c(plaintext, plen);
	}
	else // invalid pad when plen=0
	{
		printf("Padding error, plen = %d.\n", plen);
	}*/

	// INSERT YOUR CODE HERE

	int balizkoa;
	int i;
	memcpy(probakoa, ciphertext, clen * sizeof(uint8_t));

	for (int j = 0; j < 16; j++)
	{
		balizkoa = 0;
		i = 1;
		probakoa[31-j] = 0;
		while (balizkoa == 0 && i < 256)
		{
			probakoa[31-j]++;
			balizkoa = valid(plaintext, probakoa, clen);
			i++;
		}
		if(balizkoa == 0) printf("Ez da padding egokia lortzeko C[%d] balioa aurkitu\n", 31-j);

		intermediate[15-j] = probakoa[31-j] ^ (j+1);
		for (int z = 0; z < j+1; z++)
		{
			probakoa[31-z] = intermediate[15-z] ^ (j+2);
		}
		printf("\nIntermediate: ");
		print_hex(intermediate, 16);
	}

	// The new plaintext "JON mUGICA" (11 characters, fill the rest with spaces or padding)
    new_plaintext = "JON mUGICA      ";  // Total 16 bytes

    // Modify the second-to-last ciphertext block
    // Second-to-last block is located at index 16 to 31 (because block size is 16 bytes)
    for (int i = 0; i < 16; i++) {
        ciphertext[16+i] = intermediate[i] ^ new_plaintext[i];
    }

	
	plen = decipher_AES_CBC_PO(plaintext, ciphertext, clen);
	if (plen != 0) // valid pad when plen!=0
	{
		printf("Plaintext (plen = %d): ", plen);
		print_c(plaintext, plen);
	}
	else // invalid pad when plen=0
	{
		printf("Padding error, plen = %d.\n", plen);
	}

	//printf("Ciphertext (hex): ");
	//print_hex(ciphertext, clen);

	uint32_t offset = 32;
	printf("Your name: ");
	print_c(plaintext + offset, plen - offset);

	free(ciphertext);
	free(plaintext);
	free(probakoa);


	return (0);
}