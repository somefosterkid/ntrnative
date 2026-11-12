// Black's code at ov009 0x0217cf5c-0x0217d188. GameSpy's RSA signature check, between core and the debug code in the
// library's alphabetical order of files (gsCrypt's role; the name is a guess: lib/dwc/src/crypt/crypt.c has the
// basename): it checks a padded hash against a signature with the large-int functions of largeint.c.

#include "types.h"
#include "string.h"
#include "gs/nonport.h"
#include "gs/largeint.h"

typedef struct {
    LargeIntBuf modulus;
    LargeIntBuf exponent;
} RsaKey;
int func_ov009_0217cf5c(RsaKey *key, const u8 *hash, u32 hashSize, const u8 *signature, u32 signatureSize) {
    LargeIntBuf value;
    u8 *bytes = (u8 *)value.data;
    const u8 md5Prefix[18] = { 0x30, 0x20, 0x30, 0x0c, 0x06, 0x08, 0x2a, 0x86, 0x48,
                               0x86, 0xf7, 0x0d, 0x02, 0x05, 0x05, 0x00, 0x04, 0x10 };
    const u8 sha1Prefix[15] = { 0x30, 0x21, 0x30, 0x09, 0x06, 0x05, 0x2b, 0x0e,
                                0x03, 0x02, 0x1a, 0x05, 0x00, 0x04, 0x14 };
    int start;
    int i;
    u32 size;

    if (hash == NULL || signature == NULL) {
        return -1;
    }
    if (signatureSize != key->modulus.length * sizeof(u32)) {
        return -1;
    }
    if (hashSize != 16 && hashSize != 20) {
        return -1;
    }
    value.length = signatureSize / sizeof(u32);
    memcpy(value.data, signature, signatureSize);
    func_ov009_0217e934(&value);
    func_ov009_0217e078(&value, &key->exponent, &key->modulus, &value);
    func_ov009_0217e934(&value);
    if (bytes[0] != 0 || bytes[1] != 1) {
        return -2;
    }
    for (i = 2; i < 128; i++) {
        if (bytes[i] == 0) {
            break;
        }
        if (bytes[i] != 0xff) {
            return -3;
        }
    }
    size = value.length * sizeof(u32);
    start = i + 1;
    if (start + 0x22 == size) {
        if (memcmp(md5Prefix, &bytes[start], 18) != 0) {
            return -4;
        }
        if (hashSize != 16) {
            return -5;
        }
        if (memcmp(&bytes[i + 1 + 18], hash, 16) != 0) {
            return -5;
        }
    } else if (start + 0x23 == size) {
        if (memcmp(sha1Prefix, &bytes[start], 15) != 0) {
            return -4;
        }
        if (hashSize != 20) {
            return -5;
        }
        if (memcmp(&bytes[i + 1 + 15], hash, 20) != 0) {
            return -5;
        }
    } else {
        return -4;
    }
    return 0;
}

