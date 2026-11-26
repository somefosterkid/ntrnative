#include "global.h"
struct UnkStruct_ov114_0225E1A4;

#include "bg_window.h"
#include "error_handling.h"
#include "gf_gfx_loader.h"
#include "overlay_90_unk_struct_ov114_0225_e1_a4.h"

void ov90_0225A5BC(struct UnkStruct_ov114_0225E1A4 *arg0, BgConfig **arg1, s32 arg2);

void ov90_0225A5BC(UnkStruct_ov114_0225E1A4 *arg0, BgConfig **arg1, s32 val) {
    s32 v0;

    v0 = *((s32 *)&arg0->unk_10);
    switch (v0) {
    case 0:
    case 1:
        BgSetPosTextAndCommit(*arg1, 0U, BG_POS_OP_SET_ROT, val);
        BgSetPosTextAndCommit(*arg1, 1U, BG_POS_OP_SET_ROT, val);
        return;
    case 2:
        BgSetPosTextAndCommit(*arg1, 0U, BG_POS_OP_SET_XSCALE, val);
        BgSetPosTextAndCommit(*arg1, 1U, BG_POS_OP_SET_XSCALE, val);
        return;
    default:
        GF_AssertFail();
        return;
    }
}

void ov90_0225A60C(UnkStruct_ov114_0225E1A4 *arg0, BgConfig **arg1, s32 arg2);

void ov90_0225A60C(UnkStruct_ov114_0225E1A4 *arg0, BgConfig **arg1, s32 value) {
    s32 v0;

    v0 = (s32)arg0->unk_10;
    switch (v0) {
    case 0:
    case 1:
        ScheduleSetBgPosText(*arg1, 0U, BG_POS_OP_SET_ROT, value);
        ScheduleSetBgPosText(*arg1, 1U, BG_POS_OP_SET_ROT, value);
        return;
    case 2:
        ScheduleSetBgPosText(*arg1, 0U, BG_POS_OP_SET_XSCALE, value);
        ScheduleSetBgPosText(*arg1, 1U, BG_POS_OP_SET_XSCALE, value);
        return;
    default:
        GF_AssertFail();
        return;
    }
}

void ov90_0225A65C(NARC *arg0, s32 arg1, enum HeapID arg2);

void ov90_0225A65C(NARC *narc, s32 arg1, enum HeapID heapID) {
    s32 v0;
    u32 srcOffset;

    switch (arg1) {
    case 0:
        v0 = 1;
        break;
    case 1:
        v0 = 2;
        break;
    case 2:
        v0 = 0;
        break;
    default:
        v0 = 1;
        break;
    }
    srcOffset = v0 << 5;
    GfGfxLoader_GXLoadPalWithSrcOffsetFromOpenNarc(narc, 5, GF_PAL_LOCATION_MAIN_BG, srcOffset, GF_PAL_SLOT_5_OFFSET, 0x20U, heapID);
    GfGfxLoader_GXLoadPalWithSrcOffsetFromOpenNarc(narc, 5, GF_PAL_LOCATION_SUB_BG, srcOffset, GF_PAL_SLOT_5_OFFSET, 0x20U, heapID);
}
