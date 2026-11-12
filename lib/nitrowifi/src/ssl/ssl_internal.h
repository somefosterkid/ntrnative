// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2) at 9a3236bc8, lib/nitrowifi/src/ssl/ssl_internal.h.
#ifndef BWPORT_NITROWIFI_SSL_INTERNAL_H
#define BWPORT_NITROWIFI_SSL_INTERNAL_H

#include <mem.h>
#include <string.h>
#include "types.h"

// SSL's state and helpers, shared by its files


// rc4.c: RC4's state, with pokediamond's field layout (CPSi_rc4_init writes s right after i and j, with no
// padding)
typedef struct CPSRc4Context {
    u8 i;
    u8 j;
    u8 s[256];
} CPSRc4Context;

// pokebw2's CPSi_rc4_init
void CPSi_rc4_init(CPSRc4Context *context, const u8 *key, int keyLength);
// pokebw2's CPSi_rc4_crypt
void CPSi_rc4_crypt(CPSRc4Context *context, void *buf, int length);

// md5.c: MD5 (RFC 1321). No swan name for the object or any of its functions; named for the algorithm.
typedef struct MD5Context {
    u32 state[4];
    u32 count[2]; // The message's length so far, in bits, as a 64-bit little-endian pair
    u8 buffer[64];
} MD5Context;

// pokebw2's MD5Init
void CPSi_md5_init(MD5Context *context);
// pokebw2's MD5Update
void CPSi_md5_calc(MD5Context *context, const u8 *input, u32 inputLen);
// pokebw2's MD5Final
void CPSi_md5_result(MD5Context *context, u8 digest[16]);

// sha1.c: SHA-1 (FIPS 180). No swan name for the object or any of its functions; named for the algorithm.
typedef struct SHA1Context {
    u32 state[5];
    // The total bits hashed so far: count[0] is the high word, count[1] the low one CPSi_sha1_calc adds to
    u32 count[2];
    u8 buffer[64];
} SHA1Context;

// pokebw2's SHA1Init
void CPSi_sha1_init(SHA1Context *context);
// pokebw2's SHA1Update
void CPSi_sha1_calc(SHA1Context *context, const u8 *data, u32 len);
// pokebw2's SHA1Final
void CPSi_sha1_result(SHA1Context *context, u8 *digest);
// Finishes the hash like CPSi_sha1_result, but pads with 44 zero bytes and never appends the bit count. Only ever
// called right after a 20-byte CPSi_sha1_calc, so the zero padding completes exactly one block; used to extend a
// digest into a longer MAC/PRNG output rather than to hash a message (name a guess)
// pokebw2's SHA1FinalNoLength
void CPSi_sha1_result_prng(SHA1Context *context, u8 *digest);

// biginteger.c: a multi-precision integer library, each value a little-endian array of 16-bit digits with a digit
// count given by every call (no header, no sign byte). No swan name for the object or any of its functions; named
// for what each does. A multiplier pointer equal to BIGINT_SQUARE tells CPSi_big_montmult to square its value
// instead of multiplying it by another bignum
#define BIGINT_SQUARE ((const u16 *)1)

// pokebw2's BigInt_GetSign
s32 CPSi_big_sign(const u16 *digits, int n);
// pokebw2's BigInt_Add
void CPSi_big_add(u16 *dst, const u16 *a, const u16 *b, int n);
// pokebw2's BigInt_Negate
void CPSi_big_negate(u16 *digits, int n);
// pokebw2's BigInt_Sub
void CPSi_big_sub(u16 *dst, const u16 *a, const u16 *b, int n);
// pokebw2's BigInt_Mul
void CPSi_big_mult(u16 *dst, const u16 *a, const u16 *b, int n);
// pokebw2's BigInt_Div
void CPSi_big_div(u16 *quotient, const u16 *dividend, const u16 *divisor, u16 *remainder, int n, u16 *workBuf);
// pokebw2's BigInt_ModExp
void CPSi_big_power(u16 *dst, const u16 *base, const u16 *exponent, int n, const u16 *modulus);
// pokebw2's BigInt_ModExpFast
void CPSi_big_montpower(u16 *dst, const u16 *base, const u16 *exponent, int n, const u16 *modulus);
// pokebw2's BigInt_BytesToDigits
void CPSi_big_from_char(u16 *dst, const u8 *src, int nbytes, int n);
// n is the source digit count passed by callers; the original implementation does not use it.
// pokebw2's BigInt_DigitsToBytes
void CPSi_char_from_big(u8 *dst, u16 *src, int nbytes, int n);

#endif // BWPORT_NITROWIFI_SSL_INTERNAL_H
