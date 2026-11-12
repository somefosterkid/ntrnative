// NitroSDK's libmath SHA-1 and the HMAC over it: Black's code at main 0x020812e0-0x02081940. The file split and name
// are pokebw2's (lib/nitro/src/math/math_sha1.c, GPL-3.0, https://github.com/fuddlesworth/pokebw2; see math_md5.c for
// why it is apart from MD5). The compression, MATHi_SHA1ProcessBlock, is hand-written ARM in sha1_block.c; the block
// function is called through a pointer that MATHi_SetOverlayTableMode can swap for a variant that clears words 6 and 14
// of the block. The MATHi_ names are the SDK's, as kh-recoded-decomp has them.

#include "nitro/math.h"
#include "nitro/mi.h"

#define SHA1_BLOCK_SIZE 64

#define BSWAP32(x) (((u8)((x) >> 24)) | (((x) >> 8) & 0x0000ff00) | (((x) << 8) & 0x00ff0000) | (((x) << 24) & 0xff000000))

// pokebw2's sSHA1BlockFunc
static void (*MATHi_SHA1ProcessMessageBlockFunc)(MATHSHA1Context *context) = MATHi_SHA1ProcessBlock;
// pokebw2's sSHA1Masked
static int MATHi_OverlayTableMode;

// The padding that ends a message: a one bit, then zeros
// pokebw2's SHA1_PADDING
static const u8 data_020a5cac[12] = { 0x80 };

void MATHi_SHA1ProcessBlockForOverlay(MATHSHA1Context *context) {
    u32 word6, word14;
    u32 *words = context->block.words;

    word6 = words[6];
    word14 = words[14];
    words[6] = 0;
    words[14] = 0;
    MATHi_SHA1ProcessBlock(context);
    words[6] = word6;
    words[14] = word14;
}

void func_0208130c(MATHSHA1Context *context, u32 value, u32 size) {
    if (size == 0) {
        return;
    }
    do {
        u32 chunk = SHA1_BLOCK_SIZE - context->blockPos;

        if (chunk > size) {
            chunk = size;
        }
        MI_CpuFill8(&context->block.bytes[context->blockPos], value, chunk);
        size -= chunk;
        context->blockPos += chunk;
        if (context->blockPos >= SHA1_BLOCK_SIZE) {
            MATHi_SHA1ProcessMessageBlockFunc(context);
            context->blockPos = 0;
            context->blockCount++;
            if (context->blockCount == 0) {
                context->blockCountHigh++;
            }
        }
    } while (size != 0);
}


int MATHi_SetOverlayTableMode(int flag) {
    int old = MATHi_OverlayTableMode;

    MATHi_OverlayTableMode = flag;
    if (flag) {
        MATHi_SHA1ProcessMessageBlockFunc = MATHi_SHA1ProcessBlockForOverlay;
    } else {
        MATHi_SHA1ProcessMessageBlockFunc = MATHi_SHA1ProcessBlock;
    }
    return old;
}


void MATH_SHA1Init(MATHSHA1Context *context) {
    context->blockCount = 0;
    context->blockCountHigh = 0;
    context->blockPos = 0;
    context->state[0] = 0x67452301;
    context->state[1] = 0xEFCDAB89;
    context->state[2] = 0x98BADCFE;
    context->state[3] = 0x10325476;
    context->state[4] = 0xC3D2E1F0;
}

void MATH_SHA1Update(MATHSHA1Context *context, const void *data, u32 size) {
    const u8 *src = data;

    if (size == 0) {
        return;
    }
    do {
        u32 chunk = SHA1_BLOCK_SIZE - context->blockPos;

        if (chunk > size) {
            chunk = size;
        }
        MI_CpuCopy8(src, &context->block.bytes[context->blockPos], chunk);
        src += chunk;
        size -= chunk;
        context->blockPos += chunk;
        if (context->blockPos >= SHA1_BLOCK_SIZE) {
            MATHi_SHA1ProcessMessageBlockFunc(context);
            context->blockPos = 0;
            context->blockCount++;
            if (context->blockCount == 0) {
                context->blockCountHigh++;
            }
        }
    } while (size != 0);
}

void MATH_SHA1GetHash(MATHSHA1Context *context, void *digest) {
    u32 length[2];
    u32 bits;

    bits = (context->blockCount << 9) + (context->blockPos << 3);
    length[1] = BSWAP32(bits);
    bits = (context->blockCountHigh << 9) + (context->blockCount >> 23);
    length[0] = BSWAP32(bits);
    MATH_SHA1Update(context, &data_020a5cac[0], 1);
    if (SHA1_BLOCK_SIZE - context->blockPos < 8) {
        MATH_SHA1Update(context, &data_020a5cac[1], SHA1_BLOCK_SIZE - context->blockPos);
    }
    func_0208130c(context, 0, 56 - context->blockPos);
    MATH_SHA1Update(context, length, 8);
    context->state[0] = BSWAP32(context->state[0]);
    context->state[1] = BSWAP32(context->state[1]);
    context->state[2] = BSWAP32(context->state[2]);
    context->state[3] = BSWAP32(context->state[3]);
    context->state[4] = BSWAP32(context->state[4]);
    MI_CpuCopy8(context, digest, 20);
}



void MATH_CalcSHA1(u8 *digest, const void *data, u32 size) {
    MATHSHA1Context MStack_78;

    MATH_SHA1Init(&MStack_78);
    MATH_SHA1Update(&MStack_78, data, size);
    MATH_SHA1GetHash(&MStack_78, digest);
}

void MATH_CalcHMACSHA1(void *digest, const void *data, u32 dataSize, const void *key, u32 keySize) {
    MATHSHA1Context context;
    u8 hash[MATH_SHA1_DIGEST_SIZE];
    MATHHashInfo info = { MATH_SHA1_DIGEST_SIZE, SHA1_BLOCK_SIZE };

    info.context = &context;
    info.digest = hash;
    info.init = (void (*)(void *))MATH_SHA1Init;
    info.update = (void (*)(void *, const void *, u32))MATH_SHA1Update;
    info.getHash = (void (*)(void *, void *))MATH_SHA1GetHash;
    MATHi_CalcHMAC(digest, data, dataSize, key, keySize, &info);
}

void MATHi_CalcHMAC(void *out, const void *data, u32 dataSize, const void *key, u32 keySize,
                         const MATHHashInfo *info) {
    u8 keyHash[64];
    u8 innerPad[64];
    u8 outerPad[64];
    const u8 *keyBytes = key;
    u32 i;

    if (out == NULL || data == NULL || dataSize == 0 || key == NULL || keySize == 0 || info == NULL) {
        return;
    }
    if (keySize > info->blockSize) {
        info->init(info->context);
        info->update(info->context, key, keySize);
        keyBytes = keyHash;
        info->getHash(info->context, keyHash);
        keySize = info->digestSize;
    }
    for (i = 0; i < keySize; i++) {
        innerPad[i] = keyBytes[i] ^ 0x36;
    }
    for (; i < info->blockSize; i++) {
        innerPad[i] = 0x36;
    }
    info->init(info->context);
    info->update(info->context, innerPad, info->blockSize);
    info->update(info->context, data, dataSize);
    info->getHash(info->context, info->digest);
    for (i = 0; i < keySize; i++) {
        outerPad[i] = keyBytes[i] ^ 0x5c;
    }
    for (; i < info->blockSize; i++) {
        outerPad[i] = 0x5c;
    }
    info->init(info->context);
    info->update(info->context, outerPad, info->blockSize);
    info->update(info->context, info->digest, info->digestSize);
    info->getHash(info->context, out);
}
