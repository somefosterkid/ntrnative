// HMAC-SHA1 for libntr's PC build, which declares these and defines them only in its ARM prebuilt libdgt (or the TWL
// BIOS). libntr's fs_overlay.c checks an overlay's digest with them (FSi_CompareDigest, when its object is linked:
// the game calls FS_LoadOverlay, FS_LoadOverlayInfo, FS_UnloadOverlay), card_hash.c a TWL card's hashes. Over libntr's
// own SHA-1 (libdgt's DGT_Hash2*, which the PC build has).
#include <string.h>

#include "sdk_black.h"
#include <nitro/dgt/dgt.h>

// HMAC (RFC 2104) with SHA-1: a key longer than the 64-byte block is hashed first
void DGT_Hash2CalcHmac(void *digest, void *bin_ptr, int bin_len, void *key_ptr, int key_len)
{
    DGTHash2Context ctx;
    u8 key[64], pad[64], inner[20];
    int i;

    memset(key, 0, sizeof(key));
    if (key_len > (int)sizeof(key)) {
        DGT_Hash2Reset(&ctx);
        DGT_Hash2SetSource(&ctx, (unsigned char *)key_ptr, (unsigned long)key_len);
        DGT_Hash2GetDigest(&ctx, key);
    } else if (key_len > 0) {
        memcpy(key, key_ptr, (size_t)key_len);
    }

    for (i = 0; i < 64; i++) {
        pad[i] = key[i] ^ 0x36;
    }
    DGT_Hash2Reset(&ctx);
    DGT_Hash2SetSource(&ctx, pad, sizeof(pad));
    DGT_Hash2SetSource(&ctx, (unsigned char *)bin_ptr, (unsigned long)bin_len);
    DGT_Hash2GetDigest(&ctx, inner);

    for (i = 0; i < 64; i++) {
        pad[i] = key[i] ^ 0x5c;
    }
    DGT_Hash2Reset(&ctx);
    DGT_Hash2SetSource(&ctx, pad, sizeof(pad));
    DGT_Hash2SetSource(&ctx, inner, sizeof(inner));
    DGT_Hash2GetDigest(&ctx, (unsigned char *)digest);
}

// TWL's BIOS HMAC-SHA1 (swi 0x27 on a DSi; libntr has the ARM call only). fs_overlay.c takes it only when
// OS_IsRunOnTwl(), never on PC, but its object references it
void SVC_CalcHMACSHA1(void *md, const void *data, u32 len, const void *key, u32 keylen)
{
    DGT_Hash2CalcHmac(md, (void *)data, (int)len, (void *)key, (int)keylen);
}

// NitroSDK's math/dgt.c: the SHA-1 code's overlay table mode, which picks the ARM9's smaller hash routine
int MATHi_SetOverlayTableMode(int flag)
{
    return DGT_SetOverlayTableMode(flag);
}
