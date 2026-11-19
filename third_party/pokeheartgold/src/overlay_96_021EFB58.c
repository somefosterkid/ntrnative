#include "global.h"
struct UnkStruct_ov96_021EF610;

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "sprite_system.h"
#include "overlay_96_unk_struct_ov96_021_ef610.h"

s32 ov96_021EF3A8(void *);

s32 ov96_021EF610(void *);

s32 ov96_021EF95C(void *, s32);

void ov96_021EFB58(struct UnkStruct_ov96_021EF610 *arg0);

s32 ov96_021EF430(void *, s32);
s32 ov96_021EF770(void *);
void ov96_021EFC10(struct UnkStruct_ov96_021EF610 *arg0);
s32 ov96_021EF7C4(void *);
void ov96_021EFC8C(struct UnkStruct_ov96_021EF610 *arg0);

void ov96_021EFB58(UnkStruct_ov96_021EF610 *arg0) {
    MtxFx22 mtx;
    s32 value;

    GfGfxLoader_LoadScrnDataFromOpenNarc(*((NARC * *)&arg0->unk_8), 6, *((BgConfig * *)&arg0->unk_4), GF_BG_LYR_MAIN_FIRST, 0U, 0U, 1, *((enum HeapID *)&arg0->unk_C));
    ov96_021EF3A8(arg0);
    ov96_021EF95C(arg0, 0);
    ManagedSprite_SetDrawFlag(*((ManagedSprite * *)&arg0->unk_38), 1);
    ManagedSprite_SetDrawFlag(*((ManagedSprite * *)&arg0->unk_3C), 1);
    ManagedSprite_SetDrawFlag(*((ManagedSprite * *)&arg0->unk_40), 1);
    ManagedSprite_SetDrawFlag(*((ManagedSprite * *)&arg0->unk_44), 1);
    ToggleBgLayer(5U, 1U);
    ToggleBgLayer(6U, 1U);
    ToggleBgLayer(4U, 1U);
    ToggleBgLayer(1U, 1U);
    ov96_021EF610(arg0);
    value = 0;
    if (*((s32 *)&arg0->unk_28) == 0) {
        value = -0x20;
    }
    (*(s32 *)&mtx) = 0x1000;
    (*(s32 *)((u8 *)&mtx + 4)) = 0;
    (*(s32 *)((u8 *)&mtx + 8)) = 0;
    (*(s32 *)((u8 *)&mtx + 12)) = 0x1000;
    OS_WaitVBlankIntr();
    SetBgAffine(*((BgConfig * *)&arg0->unk_4), 7U, (MtxFx22 *)&mtx, 0, 0);
    Bg_SetTextDimAndAffineParams(*((BgConfig * *)&arg0->unk_4), 7U, BG_POS_OP_SET_XSCALE, value, (MtxFx22 *)&mtx, 0, 0);
}

void ov96_021EFC10(UnkStruct_ov96_021EF610 *arg0) {
    ov96_021EF430(arg0, arg0->unk_20);
    GfGfxLoader_LoadScrnDataFromOpenNarc(*((NARC * *)&arg0->unk_8), 7, *((BgConfig * *)&arg0->unk_4), GF_BG_LYR_MAIN_FIRST, 0U, 0U, 1, *((enum HeapID *)&arg0->unk_C));
    ov96_021EF95C(arg0, 1);
    ManagedSprite_SetDrawFlag(*((ManagedSprite * *)&arg0->unk_38), 1);
    ManagedSprite_SetDrawFlag(*((ManagedSprite * *)&arg0->unk_3C), 0);
    ManagedSprite_SetDrawFlag(*((ManagedSprite * *)&arg0->unk_40), 0);
    ManagedSprite_SetDrawFlag(*((ManagedSprite * *)&arg0->unk_44), 0);
    ToggleBgLayer(5U, 0U);
    ToggleBgLayer(6U, 0U);
    ToggleBgLayer(4U, 0U);
    ToggleBgLayer(1U, 0U);
    ov96_021EF770(arg0);
}

void ov96_021EFC8C(UnkStruct_ov96_021EF610 *arg0) {
    ov96_021EF430(arg0, arg0->unk_20);
    GfGfxLoader_LoadScrnDataFromOpenNarc(*((NARC * *)&arg0->unk_8), 7, *((BgConfig * *)&arg0->unk_4), GF_BG_LYR_MAIN_FIRST, 0U, 0U, 1, *((enum HeapID *)&arg0->unk_C));
    ov96_021EF95C(arg0, 1);
    ManagedSprite_SetDrawFlag(*((ManagedSprite * *)&arg0->unk_38), 0);
    ManagedSprite_SetDrawFlag(*((ManagedSprite * *)&arg0->unk_3C), 1);
    ManagedSprite_SetDrawFlag(*((ManagedSprite * *)&arg0->unk_40), 1);
    ManagedSprite_SetDrawFlag(*((ManagedSprite * *)&arg0->unk_44), 0);
    ToggleBgLayer(5U, 0U);
    ToggleBgLayer(6U, 0U);
    ToggleBgLayer(4U, 0U);
    ToggleBgLayer(1U, 0U);
    ov96_021EF7C4(arg0);
}
