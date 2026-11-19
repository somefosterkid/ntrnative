// The SDK functions Platinum calls that libntr doesn't define for the PC build (docs/platinum.md): the BIOS's square
// root, which libntr has as an ARM call only, and RC4's encryption, which pret's lib/crypto has as ARM assembly only
// (its CRYPTO_RC4Init is C, in pl.o).
#include "sdk_black.h"

// The BIOS's Sqrt (swi 0x0d): the integer square root, rounded down
u16 SVC_Sqrt(u32 src)
{
    u32 root = 0;
    u32 bit = 1u << 30;

    while (bit > src) {
        bit >>= 2;
    }
    while (bit != 0) {
        if (src >= root + bit) {
            src -= root + bit;
            root = (root >> 1) + bit;
        } else {
            root >>= 1;
        }
        bit >>= 2;
    }
    return (u16)root;
}

// pret's CRYPTORC4Context (include/crypto/rc4.h, which the port's files don't include): the two indices, then the state
typedef struct {
    u8 i, j;
    u8 padd[2];
    u8 s[256];
} PlatRC4Context;

// RC4's output (the state advanced a byte at a time, XORed with the input) to out, which may be the input
void CRYPTO_RC4Encrypt(PlatRC4Context *context, const void *in, u32 length, void *out)
{
    const u8 *src = in;
    u8 *dest = out;
    u8 i = context->i;
    u8 j = context->j;
    u8 *s = context->s;

    while (length-- != 0) {
        u8 x, y;

        i++;
        x = s[i];
        j += x;
        y = s[j];
        s[i] = y;
        s[j] = x;
        *dest++ = *src++ ^ s[(u8)(x + y)];
    }
    context->i = i;
    context->j = j;
}
