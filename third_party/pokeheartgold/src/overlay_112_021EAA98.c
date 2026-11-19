#include "global.h"
#include "gf_gfx_loader.h"
#include "pokemon_icon_idx.h"
#include "heap.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021EA9BC(void *, void *);

void ov112_021EAA98(u8 *param_1, u8 *param_2, s32 param_3);

s32 ov112_021EAA10();
s32 ov112_021EAA34(Ov112PokewalkerApp *, s16, s16);
s32 ov112_021EAAE4(Ov112PokewalkerApp *arg0, void *arg1);
void ov112_021EAB58(void);

s32 ov112_021EABE8(void *, u32, u32, s32, s32, void *);
void ov112_021EAB78(Ov112PokewalkerApp *arg0, s32 arg1, u32 arg2, u32 arg3);

void ov112_021EAA98(u8 *param_1, u8 *param_2, s32 param_3) {
    if (param_3 == 0) {
        *(u16 *)(param_2 + 4) = 0x80;
        *(u16 *)(param_2 + 6) = 0x82;
        *(s8 *)(param_2 + 2) = 0xf6;
        *(u16 *)(param_2 + 0xe) = 0;
    } else if (param_3 == 1) {
        *(u16 *)(param_2 + 4) = 0x80;
        *(s16 *)(param_2 + 6) = 0xff92;
        *(u8 *)(param_2 + 2) = 10;
        *(u16 *)(param_2 + 0xe) = 0;
    } else if (param_3 == 2) {
        *(u16 *)(param_2 + 4) = 0x80;
        *(u16 *)(param_2 + 6) = 0x82;
        *(u8 *)(param_2 + 2) = 0;
        *(u16 *)(param_2 + 0xe) = 0;
    }
    *(u16 *)(param_2 + 8) = 0;
    *(u16 *)(param_2 + 10) = 0;
    ov112_021EA9BC(param_1, param_2);
    return;
}

s32 ov112_021EAAE4(Ov112PokewalkerApp *arg0, void *arg1) {
    s8 v0;

    v0 = *(s8 *)((u8 *)arg1 + 2);
    if ((s32)v0 < 0) {
        *(s16 *)((u8 *)arg1 + 6) = (s16)(*(s16 *)((u8 *)arg1 + 6) + v0);
        if ((s32) * (s16 *)((u8 *)arg1 + 6) < -0x6E) {
            *(s8 *)((u8 *)arg1 + 2) = 0;
        }
        ov112_021EAA34(arg0, *(s16 *)((u8 *)arg1 + 4), *(s16 *)((u8 *)arg1 + 6));
        return 0;
    }
    if (((s32)v0 > 0) && ((s32)v0 <= 0x64)) {
        *(s16 *)((u8 *)arg1 + 6) = (s16)(*(s16 *)((u8 *)arg1 + 6) + v0);
        if ((s32) * (s16 *)((u8 *)arg1 + 6) > 0x6C) {
            *(s8 *)((u8 *)arg1 + 2) = 0x65;
        }
        ov112_021EAA34(arg0, *(s16 *)((u8 *)arg1 + 4), *(s16 *)((u8 *)arg1 + 6));
        return 0;
    }
    if (v0 != 0) {
        ov112_021EAA34(arg0, *(s16 *)((u8 *)arg1 + 4), *(s16 *)((u8 *)arg1 + 6));
        return 0;
    }
    ov112_021EAA10();
    return 1;
}

void ov112_021EAB58(void) {
    GfGfxLoader_GXLoadPal(NARC_poketool_icongra_poke_icon, (s32)sub_02074490(), GF_PAL_LOCATION_MAIN_OBJ, GF_PAL_SLOT_5_OFFSET, 0x60U, HEAP_ID_154);
}

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov112_021EAB78(Ov112PokewalkerApp *arg0, s32 arg1, u32 species, u32 form) {
    arg0->unk_1EA60 = GfGfxLoader_GetCharData(NARC_poketool_icongra_poke_icon, (s32)GetMonIconNaixEx(species, 0, form), 0, &arg0->unk_1EA64, HEAP_ID_154);
    DC_FlushRange(arg0->unk_1EA64->pRawData, 0x400U);
    ov112_021EABE8(arg0->unk_1EA64, species, form, 0, arg1, arg0->sprites[arg1 + 9]);
    Heap_Free(arg0->unk_1EA60);
}
#else
void ov112_021EAB78(Ov112PokewalkerApp *arg0, s32 arg1, u32 species, u32 form) {
    *(void **)((u8 *)arg0 + 0x1EA60) = GfGfxLoader_GetCharData(NARC_poketool_icongra_poke_icon, (s32)GetMonIconNaixEx(species, 0, form), 0, (void *)((u8 *)arg0 + 0x1EA64), HEAP_ID_154);
    DC_FlushRange(*(void **)((u8 *)(*(void **)((u8 *)arg0 + 0x1EA64)) + 0x14), 0x400U);
    ov112_021EABE8(*(void **)((u8 *)arg0 + 0x1EA64), species, form, 0, arg1, *(void **)((u8 *)((u8 *)arg0 + ((arg1 + 9) * 4)) + 0x1E530));
    Heap_Free(*(void **)((u8 *)arg0 + 0x1EA60));
}
#endif
