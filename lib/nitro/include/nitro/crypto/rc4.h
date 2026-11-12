// NitroSDK's RC4 stream cipher (libcrypto), with the SDK's names. From pokebw2 (GPL-3.0,
// https://github.com/fuddlesworth/pokebw2), lib/nitro/include/nitro/crypto/rc4.h.
#ifndef BWPORT_NITRO_CRYPTO_RC4_H
#define BWPORT_NITRO_CRYPTO_RC4_H

#include "types.h"

typedef struct CRYPTORC4Context {
    u8 i;
    u8 j;
    u8 padd[2];
    u8 s[256];
} CRYPTORC4Context;

// Sets up the cipher's state from a key of up to 16 bytes; a longer key is cut to 16
void CRYPTO_RC4Init(CRYPTORC4Context *context, const void *key, u32 key_len);
// Encrypts or decrypts length bytes of in into out, which may be the same buffer
void CRYPTO_RC4Encrypt(CRYPTORC4Context *context, const void *in, u32 length, void *out);

#endif // BWPORT_NITRO_CRYPTO_RC4_H
