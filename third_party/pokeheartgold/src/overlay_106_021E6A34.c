#include "global.h"

#include "overlay_106.h"
#include "error_handling.h"
#include "overlay_106_arceus_cinematic.h"
#include "overlay_106_legendary_cinematic.h"



void ov106_021E6A34(ArceusCinematic *arceus) {
#if defined(SDK_PORT) || defined(NONMATCHING)
    u32 destSlotAddr;
    u32 i;

    i = 0;
    destSlotAddr = 0;
    do {
        if (arceus->bgPalettes[i].size != 0) {
            GX_BeginLoadBGExtPltt();
            GX_LoadBGExtPltt(arceus->bgPalettes[i].data, destSlotAddr, arceus->bgPalettes[i].size);
            GX_EndLoadBGExtPltt();
            arceus->bgPalettes[i].size = 0U;
        }
        destSlotAddr += 0x2000;
        i += 1;
    } while (i < 3U);
#else
    u32 destSlotAddr;
    u32 v1;
    void *pSrc;
    void *v0;

    v0 = arceus;
    v1 = 0;
    destSlotAddr = 0;
    pSrc = arceus->bgPalettes;
    do {
        if (*(u32 *)((u8 *)v0 + offsetof(ArceusCinematic, bgPalettes[0].size)) != 0) {
            GX_BeginLoadBGExtPltt();
            GX_LoadBGExtPltt(pSrc, destSlotAddr, *(u32 *)((u8 *)v0 + offsetof(ArceusCinematic, bgPalettes[0].size)));
            GX_EndLoadBGExtPltt();
            *(u32 *)((u8 *)v0 + offsetof(ArceusCinematic, bgPalettes[0].size)) = 0U;
        }
        v0 += sizeof(ArceusBgPalette);
        destSlotAddr += 0x2000;
        pSrc += sizeof(ArceusBgPalette);
        v1 += 1;
    } while (v1 < 3U);
#endif
}

s32 ov106_021E6A80(LegendaryCinematic *arg0, s32 *arg1) {
    s32 v0;

    if (arg1[0] != 0) {
        GF_AssertFail();
    }
    v0 = arg0->frame;
    if (v0 == arg1[1]) {
        arg0->frame = 0;
        return 2;
    }
    arg0->frame = (s32)(v0 + 1);
    return 0;
}

s32 ov106_021E6AB0(LegendaryCinematic *arg0, s32 *arg1) {
    if (arg1[0] != 1) {
        GF_AssertFail();
    }
    ov106_021E5954(arg0, arg1[1], arg1[2], 4);
    return 1;
}

s32 ov106_021E6AD0(LegendaryCinematic *arg0, s32 *arg1) {
    if (arg1[0] != 2) {
        GF_AssertFail();
    }
    ov106_021E597C(arg0, arg1[1], arg1[2], 4);
    return 1;
}

s32 ov106_021E6AF0(LegendaryCinematic *arg0, s32 *arg1) {
    if (arg1[0] != 3) {
        GF_AssertFail();
    }
    ov106_021E59A4(arg0, arg1[1], arg1[2], 4);
    return 1;
}

s32 ov106_021E6B10(LegendaryCinematic *arg0, s32 *arg1) {
    if (arg1[0] != 4) {
        GF_AssertFail();
    }
    ov106_021E59D0(arg0, arg1[1], arg1[2], 4);
    return 1;
}
