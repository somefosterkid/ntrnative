#ifndef POKEHEARTGOLD_CRYPTO_RC4_H
#define POKEHEARTGOLD_CRYPTO_RC4_H

#include "global.h"

// The SDK's RC4 context (0x104 bytes on the DS), as overlay 74 uses it.
typedef struct CRYPTORC4Context {
    u8 i, j;
    u8 padd[2];
    u8 s[256];
} CRYPTORC4Context;

void CRYPTO_RC4Init(CRYPTORC4Context *ctx, const void *key, u32 keyLength);
void CRYPTO_RC4Encrypt(CRYPTORC4Context *ctx, const void *src, u32 length, void *dst);

#endif // POKEHEARTGOLD_CRYPTO_RC4_H
