//
// NIST-developed software is provided by NIST as a public service.
// You may use, copy and distribute copies of the software in any medium,
// provided that you keep intact this entire notice. You may improve,
// modify and create derivative works of the software or any portion of
// the software, and you may copy and distribute such modifications or
// works. Modified works should carry a notice stating that you changed
// the software and should note the date and nature of any such change.
// Please explicitly acknowledge the National Institute of Standards and
// Technology as the source of the software.
//
// NIST-developed software is expressly provided "AS IS." NIST MAKES NO
// WARRANTY OF ANY KIND, EXPRESS, IMPLIED, IN FACT OR ARISING BY OPERATION
// OF LAW, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTY OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT AND DATA
// ACCURACY. NIST NEITHER REPRESENTS NOR WARRANTS THAT THE OPERATION OF THE
// SOFTWARE WILL BE UNINTERRUPTED OR ERROR-FREE, OR THAT ANY DEFECTS WILL BE
// CORRECTED. NIST DOES NOT WARRANT OR MAKE ANY REPRESENTATIONS REGARDING THE
// USE OF THE SOFTWARE OR THE RESULTS THEREOF, INCLUDING BUT NOT LIMITED TO THE
// CORRECTNESS, ACCURACY, RELIABILITY, OR USEFULNESS OF THE SOFTWARE.
//
// You are solely responsible for determining the appropriateness of using and
// distributing the software and you assume all risks associated with its use,
// including but not limited to the risks and costs of program errors,
// compliance with applicable laws, damage to or loss of data, programs or
// equipment, and the unavailability or interruption of operation. This software
// is not intended to be used in any situation where a failure could cause risk
// of injury or damage to property. The software developed by NIST employees is
// not subject to copyright protection within the United States.
//

// This software is modified to work for MACs (05 May 2021)
//
// This file has been modified. The history of changes can be found at:
// https://github.com/ascon/ascon-c/commits/main/tests/genkat_auth.c

// disable deprecation for sprintf and fopen
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"
#include "crypto_auth.h"

#if defined(AVR_UART)
#include "avr_uart.h"
#endif

#define KAT_SUCCESS 0
#define KAT_FILE_OPEN_ERROR -1
#define KAT_DATA_ERROR -3
#define KAT_CRYPTO_FAILURE -4

#define MAX_FILE_NAME 256

#ifndef MAX_DATA_LENGTH
#define MAX_DATA_LENGTH 1024
#endif

void init_buffer(unsigned char* buffer, unsigned long long numbytes);

void fprint_bstr(FILE* fp, const char* label, const unsigned char* data,
                 unsigned long long length);

int generate_test_vectors();

int main() {
  int ret = generate_test_vectors();
  if (ret != KAT_SUCCESS) {
    fprintf(stderr, "test vector generation failed with code %d\n", ret);
  }
  exit(ret);
}

int generate_test_vectors() {
  FILE* fp;
#if !defined(AVR_UART)
  char fileName[MAX_FILE_NAME];
#endif
  unsigned char key[CRYPTO_KEYBYTES];
  unsigned char* d;
  unsigned char t[CRYPTO_BYTES];
  unsigned long long dlen;
  int count = 1;
  int func_ret, ret_val = KAT_SUCCESS;

  init_buffer(key, sizeof(key));

#if !defined(AVR_UART)
  sprintf(fileName, "LWC_AUTH_KAT_%d_%d.txt", (CRYPTO_KEYBYTES * 8),
          (CRYPTO_BYTES * 8));
  if ((fp = fopen(fileName, "w")) == NULL) {
    fprintf(stderr, "Couldn't open <%s> for write\n", fileName);
    return KAT_FILE_OPEN_ERROR;
  }
#else
  avr_uart_init();
  stdout = &avr_uart_output;
  stdin = &avr_uart_input_echo;
  fp = stdout;
#endif

  for (dlen = 0; dlen <= MAX_DATA_LENGTH; dlen++) {
    d = malloc(dlen);
    init_buffer(d, dlen);

    fprintf(fp, "Count = %d\n", count++);

    fprint_bstr(fp, "Key = ", key, CRYPTO_KEYBYTES);

    fprint_bstr(fp, "Msg = ", d, dlen);

    if ((func_ret = crypto_auth(t, d, dlen, key)) != 0) {
      fprintf(fp, "crypto_auth returned <%d>\n", func_ret);
      ret_val = KAT_CRYPTO_FAILURE;
      free(d);
      break;
    }

    fprint_bstr(fp, "Tag = ", t, CRYPTO_BYTES);

    fprintf(fp, "\n");

    if ((func_ret = crypto_auth_verify(t, d, dlen, key)) != 0) {
      fprintf(fp, "crypto_auth_verify returned <%d>\n", func_ret);
      ret_val = KAT_CRYPTO_FAILURE;
      free(d);
      break;
    }

    // test failing verification
    t[0] ^= 1;
    if ((func_ret = crypto_auth_verify(t, d, dlen, key)) == 0) {
      fprintf(fp, "crypto_auth_verify should have failed\n");
      ret_val = KAT_CRYPTO_FAILURE;
      free(d);
      break;
    }
    free(d);
  }

#if !defined(AVR_UART)
  fclose(fp);
#else
  fprintf(stderr, "Press Ctrl-C to quit\n");
#endif

  return ret_val;
}

void fprint_bstr(FILE* fp, const char* label, const unsigned char* data,
                 unsigned long long length) {
  unsigned long long i;
  fprintf(fp, "%s", label);

  for (i = 0; i < length; i++) fprintf(fp, "%02X", data[i]);

  fprintf(fp, "\n");
}

void init_buffer(unsigned char* buffer, unsigned long long numbytes) {
  unsigned long long i;
  for (i = 0; i < numbytes; i++) buffer[i] = (unsigned char)i;
}
