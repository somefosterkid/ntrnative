#include "global.h"

#include "gf_gfx_loader.h"
#include "sprite.h"
#include "heap.h"
#include "overlay_90.h"
#include "bg_window.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_90_unk_struct_ov114_0225cdb4.h"
#include "overlay_90_unk_struct_ov114_0225f578.h"
#include "overlay_90_unk_struct_ov114_02260284.h"
#include "overlay_90_unk_struct_ov114_0225cef0.h"

void *ov90_02258E54(void *, NARC *, s32, s32, s32, s32, s32, s32, enum HeapID);

Sprite *ov90_02258EB4(void *, s32, s16, s32, s32, enum HeapID);

void ov90_0225B59C(void *arg0, BgConfig **arg1, s32 *arg2, void *arg3, NARC *arg4, enum HeapID arg5);

void ov90_0225B690(UnkStruct_ov114_0225F578 *param0);

void ov90_02258DB0(NNSG2dScreenData *param0, u32 param1, s32);
BOOL ov90_0225B6C4(UnkStruct_ov114_0225F578 *param0, UnkStruct_ov114_0225CEF0 *param1, s32 arg2);

void ov90_0225B59C(void *arg0, BgConfig **arg1, s32 *arg2, void *arg3, NARC *narc, enum HeapID heapID) {
    s32 v3;
    s32 v2;
    s32 v1;
    s32 seq;
    s32 *v0;
    Sprite *sprite;
    s32 v7;
    s32 v4;
    s32 v6;
    void *v5;

    v0 = arg2;
    memset(arg0, 0, 0x4CU);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0x18, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_14_OFFSET, 0x20U, heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0x19, *arg1, GF_BG_LYR_SUB_1_F, 0x8BU, 0U, 0, heapID);
    *(void **)arg0 = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 0x1A, 0, arg0 + 4, heapID);
    *(void **)((u8 *)arg0 + 0x44) = ov90_02258E54(arg3, narc, 0xF, 1, 0x10, 0x11, 0x12, 0x1389, heapID);
    v2 = 0x28;
    seq = 0;
    v1 = 0;
    do {
        v6 = 0x4A;
        v7 = 0;
        v3 = (s32)(s16)v2;
loop_2:
        v5 = arg0 + ((v7 + v1) * 4);
        sprite = ov90_02258EB4(*(void **)((u8 *)arg0 + 0x44), *v0, v6, v3, 0, heapID);
        *(Sprite **)((u8 *)v5 + 8) = sprite;
        Sprite_SetDrawFlag(sprite, 0);
        Sprite_SetAnimCtrlSeq(*(Sprite **)((u8 *)v5 + 8), seq);
        Sprite_SetPriority(*(Sprite **)((u8 *)v5 + 8), 0U);
        v7 += 1;
        v6 += 0x18;
        if (v7 < 5) {
            goto loop_2;
        }
        v2 += 0x24;
        v1 += 5;
        v4 = seq + 1;
        seq = v4;
    } while (v4 < 3);
}

void ov90_0225B690(UnkStruct_ov114_0225F578 *param0) {
    {
        int v0;

        for (v0 = 0; v0 < (5 * 3); v0++) {
            Sprite_Delete(param0->unk_08[v0]);
        }
    }

    {
        Heap_Free(param0->unk_00);
    }
}

void ov90_0225B6B0(UnkStruct_ov114_0225F578 *arg0, s8 arg1) {
    arg0->unk_4B = arg1;
    arg0->unk_48 = 0;
    arg0->unk_4A = 0;
}

BOOL ov90_0225B6C4(UnkStruct_ov114_0225F578 *param0, UnkStruct_ov114_0225CEF0 *param1, s32 arg2) {
    switch (param0->unk_48) {
    case 0:

        ov90_02258DB0(param0->unk_04, (1 + (18 + 12)) + (27 * 4), arg2);
        LoadRectToBgTilemapRect(param1->unk_00, 2, param0->unk_04->rawData, 0, 0, 32, 20);
        BgTilemapRectChangePalette(param1->unk_00, 2, 0, 0, 32, 20, 13 + 1);
        ScheduleBgTilemapBufferTransfer(param1->unk_00, 2);
        param0->unk_48++;
        break;
    case 1:
        if (param0->unk_4A == param0->unk_4B) {
            param0->unk_48 = 3;
            break;
        }

        PlaySE(1428);
        Sprite_SetDrawFlag(param0->unk_08[(param0->unk_4A % (5 * 3))], 1);
        param0->unk_49 = 8;
        param0->unk_48++;
        break;
    case 2:
        param0->unk_49--;

        if (param0->unk_49 == 0) {
            param0->unk_4A++;

            if ((param0->unk_4A % (5 * 3)) == 0) {
                int v0;

                if (param0->unk_4A != param0->unk_4B) {
                    for (v0 = 0; v0 < (5 * 3); v0++) {
                        Sprite_SetDrawFlag(param0->unk_08[v0], 0);
                        Sprite_SetAnimCtrlSeq(param0->unk_08[v0], 2);
                    }
                }
            }

            param0->unk_48 = 1;
        }
        break;
    case 3:
        return 1;
    }

    return 0;
}
