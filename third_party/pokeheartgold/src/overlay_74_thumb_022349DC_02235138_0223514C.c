#include "global.h"

#include "heap.h"

void CRYPTO_SetAllocator(void *(*alloc)(u32), void (*free)(void *));

int CRYPTO_VerifySignature(const void *data, int dataLen, const void *sign, const void *pubkey);

void *ov74_02235138(u32 size);

extern const u8 _0223B690[];

static u32 ov74_0223CE9C[] = {
    'B5BE', 'B5CE', 'B5DE', 'B5EE', 'B5FE', 'B5GE', 'B5HE', 'B5IE', 'B5JE', 'B5KE', 'B5LE', 'B5ME', 'B5PE', 'B5QE', 'B5RE', 'B5SE', 'B5TE', 'B5UE', 'B5VE', 'B5WE', 0x0
};

static inline u32 FlipEndianness(u32 val) {
    return ((val >> 0) & 255) << 24 | ((val >> 8) & 255) << 16 | ((val >> 16) & 255) << 8 | ((val >> 24) & 255) << 0;
}

BOOL ov74_0223514C(void);

BOOL ov74_0223514C(void) {
    u32 gameCode = CTRDG_GetAgbGameCode();

    gameCode = FlipEndianness(gameCode);

    for (int i = 0; i < ov74_0223CE9C[i]; i++) {
        if (ov74_0223CE9C[i] == gameCode) {
            u8 cartridgeData[1192];
            u8 signature[128];

            CTRDG_Enable(TRUE);
            CTRDG_CpuCopy8((void *)0x8100000, cartridgeData, 1192);
            CTRDG_CpuCopy8((void *)0x8020000, signature, 128);
            CTRDG_Enable(FALSE);

            CRYPTO_SetAllocator(ov74_02235138, Heap_Free);

            if (CRYPTO_VerifySignature(cartridgeData, 1192, signature, _0223B690)) {
                return TRUE;
            }
        }
    }

    return FALSE;
}
