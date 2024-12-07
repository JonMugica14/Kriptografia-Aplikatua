#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "api.h"
#include "ascon.h"
#include "../ascon-c-main/ascon-c-main/tests/crypto_aead.h"

#define BLOCK_SIZE (512 * 1024) // 512KiB

int main(int argc, char *argv[])
{

     if (argc < 2)
    {
        printf("Sartu parametroa\n");
        return 1;
    }
    if (atoi(argv[1]) <= 100)
    {
        printf("Gutxienez 101 mesedez\n");
        return 1;
    }

    struct timespec begin, end; // Structures to store the start and end times for timing

    // Define a 16-byte key for encryption, initialized with example values
    uint8_t key[CRYPTO_KEYBYTES] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

    // Define a nonce (initialization vector) of required size for encryption, initialized with example values
    uint8_t nonce[CRYPTO_NPUBBYTES] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    int result;
    /*
    // Define the plaintext message
    unsigned char plaintext[] = "Your message here"; // 17B, 1KiB ,32KiB ,128KiB ,256KiB , 512KiB
    unsigned long long plaintext_len = strlen((char *)plaintext);
    */

    // Define associated data (optional additional data) for authenticated encryption
    uint8_t associated_data[] = "Optional AD";
    uint64_t ad_len = strlen((char *)associated_data);

    // Allocate memory for the ciphertext with additional bytes for the authentication tag
    uint64_t ciphertext_len;
    uint8_t *ciphertext;

    // Allocate memory for decrypted text
    uint64_t decrypted_len;        // Will store the length of decrypted text
    uint8_t*decrypted; // Allocate memory for decrypted data

    FILE *file = fopen("archivo_512KiB.txt", "rb");
    if (!file)
    {
        perror("Errorea fitxategia irekitzerako orduan.");
        return 1;
    }

    uint8_t plaintext[BLOCK_SIZE];
    size_t plaintext_len = fread(plaintext, 1, BLOCK_SIZE, file);

    if (plaintext == 0)
    {
        perror("Errorea fitxategia irakurtzerako orduan.");
        fclose(file);
        return 1;
    }
    fclose(file);

    int n = atoi(argv[1]);

    int tartea;

    float encr_denborak[6];
    float decr_denborak[6];
    uint8_t *tag;
    tag = malloc(CRYPTO_ABYTES * sizeof(uint8_t));
    ciphertext = malloc((BLOCK_SIZE + CRYPTO_ABYTES) * sizeof(uint8_t));
    decrypted = malloc(BLOCK_SIZE * sizeof(uint8_t));

    float sum_enc = 0, sum_dec = 0;
    for (int j = 0; j < 6; j++)
    {
        if(j == 0) tartea = 17;
        else {
            tartea = 104857 * j;
        }
       
        ciphertext_len = tartea + CRYPTO_ABYTES;
        
        ciphertext = realloc(ciphertext, ciphertext_len * sizeof(unsigned char));
        
        decrypted_len = ciphertext_len - CRYPTO_ABYTES;
        decrypted = realloc(decrypted, decrypted_len * sizeof(unsigned char));
      

        for (int i = 0; i < n; i++)
        {
            // Measure encryption time
            clock_gettime(CLOCK_MONOTONIC_RAW, &begin);                                                                            // 17B, 1KiB, 32KiB, 64KiB, 128KiB, 512KiB                                                                            // Start time for encryption
           
          crypto_aead_encrypt(ciphertext, &ciphertext_len, plaintext, tartea, associated_data, ad_len, NULL, nonce, key); // Encrypt the plaintext
          clock_gettime(CLOCK_MONOTONIC_RAW, &end);                                                                              // End time for encryption
            
            if (i >= 100)
                sum_enc += (end.tv_nsec - begin.tv_nsec) / 1000.0 + (end.tv_sec - begin.tv_sec) * 1e6;

            // Measure decryption time
            clock_gettime(CLOCK_MONOTONIC_RAW, &begin);                                                                                  // Start time for decryption
             result=  crypto_aead_decrypt(decrypted, &decrypted_len, NULL, ciphertext, ciphertext_len, associated_data, ad_len, nonce, key); // Decrypt the ciphertext
               clock_gettime(CLOCK_MONOTONIC_RAW, &end);                                                                                       // End time for decryption

            if (i >= 100)
                sum_dec += (end.tv_nsec - begin.tv_nsec) / 1000.0 + (end.tv_sec - begin.tv_sec) * 1e6;
                
                
        }
        
        


        // Adierazi tartea
        if (j == 0) printf("Plaintext %dB izanik: \n", tartea);
        else printf("Plaintext %dKiB izanik: \n", tartea/1024);

        encr_denborak[j] = sum_enc / (atoi(argv[1]) - 100);
        decr_denborak[j] = sum_dec / (atoi(argv[1]) - 100);
    
        // Calculate and print the encryption time
        printf("Encryption time = %.3lf us\n", sum_enc / (atoi(argv[1]) - 100));

        // Calculate and print the decryption time
        printf("Decryption time = %.3lf us\n", sum_dec / (atoi(argv[1]) - 100));

        printf("\n");

        if (result != 0)
        {
            printf("Errorea gertatu da desenkriptatzean\n");
            
        }
        

        // Free allocated memory
        
    }
free(ciphertext);
        free(decrypted);
        free(tag);
    return 0;
}
