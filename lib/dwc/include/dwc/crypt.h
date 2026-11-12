// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/crypt.h: its types and
// constants, and the prototypes of Black's named functions.
#ifndef BWPORT_DWC_CRYPT_H
#define BWPORT_DWC_CRYPT_H

#include "types.h"

typedef void *(*CryptAllocFunc)(u32 size);
typedef void (*CryptFreeFunc)(void *ptr);

// pokebw2's Crypt_VerifySignature
// Digests size bytes and checks that the signature, of 128 bytes, holds the digest when decrypted with the key, whose
// modulus is 128 bytes; TRUE when it does
int func_ov114_021caa70(const void *data, u32 size, void *signature, const u8 *key);
// pokebw2's Crypt_SetAllocator
// Gives the library the allocator that it takes its memory from; the third argument is stored and not used
void func_ov114_021cab1c(CryptAllocFunc alloc, CryptFreeFunc free, u32 arg);

#endif // BWPORT_DWC_CRYPT_H
