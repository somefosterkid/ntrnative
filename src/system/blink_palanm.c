// Black's code at main 0x0202e930-0x0202eb0c. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "app/comm_tvt/ctvt_comm.h"
#include "gfl/arc_util.h"
#include "gfl/heap.h"
#include "nitro/fs.h"
#include "nitro/mi.h"
#include "nnsys/g2d.h"
#include "save/player_info.h"
#include "struct_decls.h"
#include "system/blink_palanm.h"
#include "system/ctvt_beacon.h"
#include "nnsys/gfd.h"
#include "nitro/fx/fx_trig.h"

struct ArcTool {
    FSFile file;
    u32 fimgOffset;
    u16 fatbOffset;
    u16 fileCount;
};
typedef struct endPos endPos;
typedef struct startPos startPos;

u16 *func_0202e930(CtvtCommBeacon *beacon) {
    return GetPlayerName(&beacon->player);
}

u16 func_0202e938(CtvtCommBeacon *beacon) {
    return getTrainerID(&beacon->player);
}

BlinkPalAnm *func_0202e940(u16 offset, u16 count, u16 palette, HeapID heapId) {
    BlinkPalAnm *anm = GFL_HeapAllocate(heapId, sizeof(BlinkPalAnm), FALSE, "blink_palanm.c", 60);

    anm->unk_08 = GFL_HeapAllocate(heapId, count * 2, FALSE, "blink_palanm.c", 61);
    anm->unk_0C = offset;
    anm->unk_0E = count;
    anm->unk_10 = 0;
    anm->unk_12 = 0;
    anm->unk_18 = heapId;
    if (palette == 0xfffe) {
        anm->unk_14 = 0xe;
    } else if (palette == 0xffff) {
        anm->unk_14 = 0x1e;
    } else if (palette <= 3) {
        anm->unk_14 = 0xf;
    } else {
        anm->unk_14 = 0x1f;
    }
    return anm;
}


void func_0202e9ac(BlinkPalAnm *anm, ArcTool *arc, u32 fileId, u32 startPos, u32 endPos) {
    void *ptr;
    void *rawData;
    NNSG2dPaletteData *pNStack_1c;
    u32 uStack_18;

    uStack_18 = startPos;
    ptr = GFL_HeapAllocate(anm->unk_18,(u32)anm->unk_0E << 1,0,"blink_palanm.c",0x83);
    anm->unk_00 = ptr;
    ptr = GFL_HeapAllocate(anm->unk_18,(u32)anm->unk_0E << 1,0,"blink_palanm.c",0x84);
    anm->unk_04 = ptr;
    ptr = GFL_G2DIOReadNCLRArc(arc, fileId, &pNStack_1c, anm->unk_18);
    rawData = pNStack_1c->pRawData;
    MI_CpuCopy8((u8 *)rawData + startPos * 2, anm->unk_00, (u32)anm->unk_0E << 1);
    MI_CpuCopy8((u8 *)rawData + endPos * 2, anm->unk_04, (u32)anm->unk_0E << 1);
    GFL_HeapFree(ptr);
}

void func_0202ea20(BlinkPalAnm *anm) {
    GFL_HeapFree(anm->unk_04);
    GFL_HeapFree(anm->unk_00);
    GFL_HeapFree(anm->unk_08);
    GFL_HeapFree(anm);
}

void func_0202ea40(BlinkPalAnm *anm) {
    fx32 ratio;
    u32 i;
    s32 next;

    next = anm->unk_10 + 0x400;
    if (next >= 0x10000) {
        anm->unk_10 = anm->unk_10 - 0xfc00;
    } else {
        anm->unk_10 = next;
    }
    ratio = (FX_CosIdx(anm->unk_10) + FX32_ONE) / 2;
    for (i = 0; i < anm->unk_0E; i++) {
        u16 start = anm->unk_00[i];
        u16 end = anm->unk_04[i];
        u8 r = (start & 0x1f) + ((((end & 0x1f) - (start & 0x1f)) * ratio) >> FX32_SHIFT);
        u8 g = ((start >> 5) & 0x1f) + ((((end >> 5) & 0x1f) - ((start >> 5) & 0x1f)) * ratio >> FX32_SHIFT);
        u8 b = ((start >> 10) & 0x1f) + ((((end >> 10) & 0x1f) - ((start >> 10) & 0x1f)) * ratio >> FX32_SHIFT);

        anm->unk_08[i] = GX_RGB(r, g, b);
    }
    NNS_GfdRegisterNewVramTransferTask(anm->unk_14, anm->unk_0C * 2, anm->unk_08, anm->unk_0E * 2);
}


void func_0202eb04(BlinkPalAnm *anm) {
    anm->unk_10 = 0;
}

