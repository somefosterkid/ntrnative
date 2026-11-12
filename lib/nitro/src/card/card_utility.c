// TwlSDK's CARD library, card_utility.c: Black's main 0x020762f4-0x020763bc and the LTD autoload's .ltdmain
// 0x0272e9e0-0x0272e9e8. The file is libntr's (MIT, https://github.com/cybervisi0n/libntr) for SDK 5; the names are
// kh-recoded-decomp's twins' (MIT) or libntr's in the same place.

#include "card_internal.h"
#include "nitro/mi.h"

#define MI_DMA_USING_NEW 0x10

void MIi_CardDmaCopy32(u32 dmaNo, const void *src, void *dest, u32 size);

static const CARDDmaInterface CARDiDmaUsingFormer = {
    MIi_CardDmaCopy32,
    MI_StopDma,
};

void MI_Card_NDmaCopy(u32 ndmaNo, const void *src, void *dest, u32 size);
void MI_StopNDma(u32 ndmaNo);

// TwlSDK's CARDiDmaUsingNew (MI_Card_A_NDmaCopy and MI_StopNDma) in its twl/ltdmain_begin.h block: the first object of
// the LTD autoload's .ltdmain (0x0272e9e0-0x0272e9e8), which ltd_autoload_0's delinks.txt names card_utility@ltd.c
// (tools/two_modules.py: one object, two modules)
#include "twl/ltdmain_begin.h"

static CARDDmaInterface CARDiDmaUsingNew = {
    MI_Card_NDmaCopy,
    MI_StopNDma,
};

#include "twl/ltdmain_end.h"

const CARDDmaInterface *CARDi_GetDmaInterface(u32 channel) {
    const CARDDmaInterface *retval = NULL;

    BOOL isNew = ((channel & MI_DMA_USING_NEW) != 0);
    channel &= ~MI_DMA_USING_NEW;
    if (channel <= MI_DMA_MAX_NUM) {
        if (!isNew) {
            retval = &CARDiDmaUsingFormer;
        } else if (!OS_IsRunOnTwl()) {
            OS_Terminate();
        } else {
            retval = &CARDiDmaUsingNew;
        }
    }
    return retval;
}

void CARDi_ICInvalidateSmart(void *buffer, u32 length, u32 threshold) {
    if (length >= threshold) {
        IC_InvalidateAll();
    } else {
        IC_InvalidateRange((void *)buffer, length);
    }
}

void CARDi_DCInvalidateSmart(void *buffer, u32 length, u32 threshold) {
    if (length >= threshold) {
        DC_FlushAll();
    } else {
        u32 pos = (u32)buffer;
        u32 mod = (pos & (HW_CACHE_LINE_SIZE - 1));
        if (mod) {
            pos -= mod;
            DC_StoreRange((void *)(pos), HW_CACHE_LINE_SIZE);
            DC_StoreRange((void *)(pos + length), HW_CACHE_LINE_SIZE);
            length += HW_CACHE_LINE_SIZE;
        }
        DC_InvalidateRange((void *)pos, length);
        DC_WaitWriteBufferEmpty();
    }
}
