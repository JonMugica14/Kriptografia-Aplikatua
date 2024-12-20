#ifndef WORD_H_
#define WORD_H_

#include <stdint.h>
#include <string.h>

#include "forceinline.h"
#include "lendian.h"

typedef union {
  uint64_t x;
  uint32_t w[2];
  uint8_t b[8];
} word_t;

#define U64TOWORD(x) U64LE(x)
#define WORDTOU64(x) U64LE(x)
#define LOAD(b, n) LOADBYTES(b, n)
#define STORE(b, w, n) STOREBYTES(b, w, n)

#define XMUL(i, x)                               \
  do {                                           \
    tmp = (uint16_t)a.b[i] * (1 << (x));         \
    b.b[(byte_rol + (i)) & 0x7] ^= (uint8_t)tmp; \
    b.b[(byte_rol + (i) + 1) & 0x7] ^= tmp >> 8; \
  } while (0)

forceinline uint64_t ROR(uint64_t x, int n) {
  word_t a = {.x = x}, b = {.x = 0ull};
  int bit_rol = (64 - n) & 0x7;
  int byte_rol = (64 - n) >> 3;
  uint16_t tmp;
  XMUL(0, bit_rol);
  XMUL(1, bit_rol);
  XMUL(2, bit_rol);
  XMUL(3, bit_rol);
  XMUL(4, bit_rol);
  XMUL(5, bit_rol);
  XMUL(6, bit_rol);
  XMUL(7, bit_rol);
  return b.x;
}

forceinline uint8_t NOT8(uint8_t a) { return ~a; }

forceinline uint8_t XOR8(uint8_t a, uint8_t b) { return a ^ b; }

forceinline uint8_t AND8(uint8_t a, uint8_t b) { return a & b; }

forceinline uint8_t OR8(uint8_t a, uint8_t b) { return a | b; }

forceinline uint64_t KEYROT(uint64_t hi2lo, uint64_t lo2hi) {
  return lo2hi << 32 | hi2lo >> 32;
}

forceinline int NOTZERO(uint64_t a, uint64_t b) {
  uint64_t result = a | b;
  result |= result >> 32;
  result |= result >> 16;
  result |= result >> 8;
  return ((((int)(result & 0xff) - 1) >> 8) & 1) - 1;
}

forceinline uint64_t PAD() { return 0x01; }

forceinline uint64_t DSEP() { return 0x80; }

forceinline uint64_t PRFS_MLEN(uint64_t len) { return len << 51; }

forceinline uint64_t CLEAR(uint64_t w, int n) {
  /* undefined for n == 0 */
  uint64_t mask = ~0ull << (8 * n);
  return w & mask;
}

forceinline uint64_t MASK(int n) {
  /* undefined for n == 0 */
  return ~0ull << (64 - 8 * n);
}

forceinline uint64_t LOADBYTES(const uint8_t* bytes, int n) {
  uint64_t x = 0;
  memcpy(&x, bytes, n);
  return U64TOWORD(x);
}

forceinline void STOREBYTES(uint8_t* bytes, uint64_t w, int n) {
  uint64_t x = WORDTOU64(w);
  memcpy(bytes, &x, n);
}

forceinline void memxor(uint8_t* dst, const uint8_t* src, int n) {
  while (n > 0) {
    *dst++ ^= *src++;
    --n;
  }
}

forceinline void INSERT(uint8_t* s, const uint8_t* d, int n) {
  while (n > 0) {
    *s++ = *d++;
    --n;
  }
}

forceinline void SQUEEZE(uint8_t* d, const uint8_t* s, int n) {
  while (n > 0) {
    *d++ = *s++;
    --n;
  }
}

forceinline void ABSORB(uint8_t* s, const uint8_t* d, int n) {
  while (n > 0) {
    *s++ ^= *d++;
    --n;
  }
}

forceinline void ENCRYPT(uint8_t* s, uint8_t* c, const uint8_t* m, int n) {
  while (n > 0) {
    *c++ = (*s++ ^= *m++);
    --n;
  }
}

forceinline void DECRYPT(uint8_t* s, uint8_t* m, const uint8_t* c, int n) {
  while (n > 0) {
    uint8_t t = *c++;
    *m++ = *s ^ t;
    *s++ = t;
    --n;
  }
}

forceinline uint8_t VERIFY(const uint8_t* s, const uint8_t* d, int n) {
  uint8_t r = 0;
  while (n > 0) {
    r |= *s++ ^ *d++;
    --n;
  }
  return r;
}

#endif /* WORD_H_ */
