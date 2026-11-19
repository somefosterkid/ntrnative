#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_0200A090.h"
#include "unk_020932E0.h"
#include "yes_no_prompt.h"

void sub_02093354(UnkStruct_020932E0 *ptr) {
    s32 v1;

    sub_02095D2C(ptr->unk_46B8);
    sub_0209515C(ptr);
    sub_020950F8(&ptr->unk_4650, 1);
    RemoveWindow(&ptr->unk_4670);
    sub_020950D4(ptr);
    Heap_Free(ptr->unk_46C8);
    Heap_Free(ptr->unk_46CC);
    SpriteTransfer_DeleteCharTransferTask(ptr->unk_15C);
    SpriteTransfer_DeletePlttTransferTask(ptr->unk_160);
    v1 = 0;
    do {
        Destroy2DGfxResObjMan(ptr->unk_144[v1]);
        v1 += 1;
    } while (v1 < 6);
    SpriteList_Delete(ptr->unk_18);
    Heap_Free(ptr->unk_46A4);
    YesNoPrompt_Destroy(ptr->unk_8);
    Heap_Free(ptr->unk_7E4);
    Heap_Free(ptr->unk_8D0);
    FontID_Release(FONT_UNK_4);
    gSystem.screensFlipped = 0;
    GfGfx_SwapDisplay();
    reg_G2_BLDCNT = 0;
    FreeBgTilemapBuffer(ptr->unk_0, 0U);
    FreeBgTilemapBuffer(ptr->unk_0, 1U);
    FreeBgTilemapBuffer(ptr->unk_0, 2U);
    FreeBgTilemapBuffer(ptr->unk_0, 3U);
    Heap_Free(ptr);
}

void sub_02093440(UnkStruct_020932E0 *arg0, BgConfig *bgConfig, struct PCStorage *pcStorage, Party *party, s32 arg4, s32 arg5, s32 arg6, s32 arg7, UnkStruct_020932E0_GetInfoFunc arg8, UnkStruct_020932E0_BoxNameFunc arg9, UnkStruct_020932E0_Pair *argA, UnkStruct_020932E0_SelectFunc argB, void *argC) {
    arg0->unk_8 = YesNoPrompt_Create(arg0->unk_4);
    arg0->unk_463C = pcStorage;
    arg0->unk_4640 = party;
    arg0->unk_0 = bgConfig;
    arg0->unk_46DC = argC;
    arg0->unk_46D8 = argB;
    arg0->unk_F = (s8)arg7;
    arg0->unk_46E4 = -1;
    arg0->unk_46E0 = -1;
    arg0->unk_13 = (s8)arg6;
    sub_02094D9C(arg0->unk_0, arg0->unk_4);
    sub_02093A50(arg0);
    sub_02093B40(arg0);
    sub_02094004(arg0);
    GfGfx_EngineATogglePlanes(0x10U, 1U);
    FontID_Alloc(FONT_UNK_4, arg0->unk_4);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, arg0->unk_4);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, arg0->unk_4);
    arg0->unk_11 = (s8)arg4;
    arg0->unk_12 = (s8)arg5;
    sub_02095D1C(arg0->unk_46B8, (u8)arg6);
    sub_02095D40(arg0->unk_46B8, 2, 0);
    sub_02093B84(arg0);
    arg0->unk_4648 = arg8;
    arg0->unk_464C = arg9;
    AddWindowParameterized(arg0->unk_0, &arg0->unk_4650, 1U, 4U, 1U, 0xDU, 2U, 0xDU, 0xA0U);
    AddWindowParameterized(arg0->unk_0, &arg0->unk_4670, 1U, 0x19U, 0x15U, 6U, 2U, 0xCU, 0xBAU);
    sub_02094D1C(arg0);
    sub_020941CC(arg0, arg0->unk_0, arg0->unk_4);
    sub_020942B0(arg0);
    sub_020943EC(arg0);
    sub_020950C0(arg0);
    sub_02095794();
    arg0->unk_46B4 = argA;
    arg0->unk_14 = 0;
    arg0->unk_46BC = 0;
    arg0->unk_46C0 = 0;
    arg0->unk_46C4 = 0;
}
