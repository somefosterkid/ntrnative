#include "global.h"

enum HTTPB64EncodeResult {
    HTTP_B64_ENCODE_SUCCESS = 0,
    HTTP_B64_ENCODE_ERROR_ALLOC_FAIL,
    HTTP_B64_ENCORE_ERROR_INSUFFICIENT_BUFFER_SIZE,
};

extern u32 ov38_0221BB44(u32 decodedSize);

typedef enum {
    DWC_ALLOCTYPE_AUTH,
    DWC_ALLOCTYPE_AC,
    DWC_ALLOCTYPE_BM,
    DWC_ALLOCTYPE_UTIL,
    DWC_ALLOCTYPE_BASE,
    DWC_ALLOCTYPE_GS,
    DWC_ALLOCTYPE_ND,

    DWC_ALLOCTYPE_ENC,
    DWC_ALLOCTYPE_MAIL,

    DWC_ALLOCTYPE_LAST
} DWCAllocType;

void *DWC_Alloc(DWCAllocType name, u32 size);

void DWC_Free(DWCAllocType name, void *ptr, u32 size);

void B64Encode(
    const char *input,
    char *output,
    int inlen,
    int encodingType);

void ov38_0221BA00(u32 seed);

u8 ov38_0221BA10(void);

enum HTTPB64EncodeResult ov38_0221BA38(u32 dwcProfileID, const u8 *in, int length, u8 *out, int outSize);

enum HTTPB64EncodeResult ov38_0221BA38(u32 dwcProfileID, const u8 *in, int length, u8 *out, int outSize) {
    u8 *buffer;
    u32 encryptionSeed = 0;
    int i;

    if (outSize < (int)ov38_0221BB44(length + 8) + 1) {
        return HTTP_B64_ENCORE_ERROR_INSUFFICIENT_BUFFER_SIZE;
    }

    buffer = DWC_Alloc((DWCAllocType)(10), length + 8);

    if (buffer == 0L) {
        return HTTP_B64_ENCODE_ERROR_ALLOC_FAIL;
    }

    encryptionSeed += (u8)((dwcProfileID >> 24) & 0xff);
    encryptionSeed += (u8)((dwcProfileID >> 16) & 0xff);
    encryptionSeed += (u8)((dwcProfileID >> 8) & 0xff);
    encryptionSeed += (u8)((dwcProfileID >> 0) & 0xff);

    for (i = 0; i < length; i++) {
        encryptionSeed += in[i];
    }

    ov38_0221BA00(encryptionSeed);

    buffer[4] = ((dwcProfileID >> 0) & 0xff) ^ ov38_0221BA10();
    buffer[5] = ((dwcProfileID >> 8) & 0xff) ^ ov38_0221BA10();
    buffer[6] = ((dwcProfileID >> 16) & 0xff) ^ ov38_0221BA10();
    buffer[7] = ((dwcProfileID >> 24) & 0xff) ^ ov38_0221BA10();

    for (i = 0; i < length; i++) {
        buffer[8 + i] = in[i] ^ ov38_0221BA10();
    }

    encryptionSeed ^= 0x4A3B2C1D;

    buffer[0] = (encryptionSeed >> 24) & 0xff;
    buffer[1] = (encryptionSeed >> 16) & 0xff;
    buffer[2] = (encryptionSeed >> 8) & 0xff;
    buffer[3] = (encryptionSeed >> 0) & 0xff;

    B64Encode(buffer, out, length + 8, 2);

    out[ov38_0221BB44(length + 8)] = '\0';

    DWC_Free((DWCAllocType)(10), buffer, 0);

    return HTTP_B64_ENCODE_SUCCESS;
}
