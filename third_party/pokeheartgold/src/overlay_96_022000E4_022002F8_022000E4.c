#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "error_handling.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "text.h"
#include "unk_0200A090.h"
#include "unk_02013FDC.h"
#include "overlay_96_ov96_mon_icon_work.h"
#include "overlay_96_unk_struct_ov96_021e9a78.h"

typedef struct Words2 {
    u32 w[2];
} Words2;

typedef struct Ov96MonSpec {
    u16 species;
    u16 form;
} Ov96MonSpec;

void ov96_021EB2BC(void *, u32, u32, u32, u32);

void ov96_021EB2F4(void *, u32, u32, u32, u32, u8);

void ov96_021EB334(void *, u32, u32, u32);

void ov96_021EB36C(void *, u32, u32, u32);

static const u8 ov96_0221C718[6] = { 0x90, 0x98, 0xA8, 0xB8, 0xC0, 0xD0 };

static const u8 ov96_0221C720[6] = { 1, 0xC, 1, 1, 0xB, 1 };

static const VecFx32 ov96_0221C748 = { 0, 0x50000, 0 };

static const VecFx32 ov96_0221C754 = { 0xA0000, 0x260000, 0 };

void ov96_022000E4(u32 param_1, void *param_2, u32 param_3, u32 param_4);

void ov96_022000E4(u32 param_1, void *param_2, u32 param_3, u32 param_4) {
    u32 *puVar1;
    s32 iVar2;
    u32 auStack_20[2] = { 0x66, 0x67 };

    puVar1 = auStack_20;
    iVar2 = 0;
    do {
        ov96_021EB2BC(param_2, 0x5d, 9, *puVar1, 2);
        ov96_021EB2F4(param_2, 0x5d, 6, *puVar1, 2, 1);
        ov96_021EB334(param_2, 0x5d, 10, *puVar1);
        ov96_021EB36C(param_2, 0x5d, 10, *puVar1);
        iVar2 = iVar2 + 1;
        puVar1 = puVar1 + 1;
    } while (iVar2 < 2);
    ov96_021EB2BC(param_2, 0xb2, 0xd, 0x65, 2);
    ov96_021EB2F4(param_2, 0xb2, 10, 0x65, 2, 5);
    ov96_021EB334(param_2, 0xb2, 0xc, 0x65);
    ov96_021EB36C(param_2, 0xb2, 0xb, 0x65);
    return;
}

Sprite *ov96_021EA2C4(UnkStruct_ov96_021E9A78 *, SpriteList *, s32, enum HeapID);

Ov96Obj *ov96_021EB3E4(void *, s32, s32, u8, u8);

Sprite *ov96_021EB408(void *, s32, s32, u8, u8);

Ov96Obj *ov96_021EB4F4(void *, s32, s32);

void ov96_021EB52C(Ov96Obj *, s32, s32);

void ov96_021EB564(Ov96Obj *, s32);

void ov96_021EB588(Ov96Obj *, VecFx32 *);

SpriteList *ov96_021EB5E8(void *);

void ov96_02200C8C(void *arg0, Ov96MonIconWork *arg1);

void ov96_02200180(Ov96MonIconWork *arg0, void *arg1, UnkStruct_ov96_021E9A78 *arg2);

void ov96_02200180(Ov96MonIconWork *arg0, void *arg1, UnkStruct_ov96_021E9A78 *arg2) {
    VecFx32 sp10[2];
    s32 spC;
    UnkStruct_ov96_021E9A78 *sp8;
    Ov96MonIconWork *sp4;
    Ov96Obj *temp_r0;
    s32 temp_r0_2;
    s32 var_r4;
    s32 var_r4_2;
    s32 var_r6;
    const u8 *var_r7;
    const u8 *var_r6_2;
#if !(defined(SDK_PORT) || defined(NONMATCHING))
    Sprite **var_r4_4;
#endif

    sp4 = arg0;
    sp8 = arg2;
    var_r4 = 0;
    do {
        ov96_021EB408(arg1, 3, 2, 0x65, 2);
        ov96_021EB408(arg1, 3, 2, 0x65, 3);
        ov96_021EB408(arg1, 3, 2, 0x65, 8);
        ov96_021EB408(arg1, 3, 2, 0x65, 0xA);
        var_r4 += 1;
    } while (var_r4 < 4);
    var_r4_2 = 0;
    do {
        ov96_021EB408(arg1, 3, 2, 0x65, 0x10);
        var_r4_2 += 1;
    } while (var_r4_2 < 2);
    var_r6 = 0;
    do {
        sp4->slots[var_r6].obj0C = ov96_021EB4F4(arg1, 0x65, 2);
        sp4->slots[var_r6].obj18 = ov96_021EB4F4(arg1, 0x65, 3);
        sp4->slots[var_r6].obj14 = ov96_021EB4F4(arg1, 0x65, 8);
        sp4->slots[var_r6].obj10 = ov96_021EB4F4(arg1, 0x65, 0xA);
        ov96_021EB564(sp4->slots[var_r6].obj0C, var_r6);
        ov96_021EB564(sp4->slots[var_r6].obj18, var_r6 + 4);
        ov96_021EB564(sp4->slots[var_r6].obj14, 0xA);
        var_r6 += 1;
    } while (var_r6 < 4);
    temp_r0 = ov96_021EB3E4(arg1, 3, 2, 0x65, 0x11);
    ov96_021EB52C(temp_r0, 1, 1);
    ov96_021EB564(temp_r0, 0xB);
    sp10[0].z = 0;
    sp10[0].x = 0x80000;
    sp10[0].y = 0x238000;
    ov96_021EB588(temp_r0, &sp10[0]);
    ov96_02200C8C(arg1, sp4);
#if defined(SDK_PORT) || defined(NONMATCHING)
    var_r7 = ov96_0221C718;
    var_r6_2 = ov96_0221C720;
    spC = 0;
    do {
        sp4->timeSprites[spC] = ov96_021EA2C4(sp8, ov96_021EB5E8(arg1), 3, sp4->heapID);
        Sprite_SetDrawFlag(sp4->timeSprites[spC], 1);
        sp10[1].x = *var_r7 << 0xC;
        sp10[1].y = 0x238000;
        sp10[1].z = 0;
        Sprite_SetMatrix(sp4->timeSprites[spC], &sp10[1]);
        Sprite_SetAnimCtrlSeq(sp4->timeSprites[spC], (s32)*var_r6_2);
        var_r7 += 1;
        temp_r0_2 = spC + 1;
        var_r6_2 += 1;
        spC = temp_r0_2;
    } while (temp_r0_2 < 6);
#else
    var_r4_4 = (Sprite **)sp4;
    var_r7 = ov96_0221C718;
    var_r6_2 = ov96_0221C720;
    spC = 0;
    do {
        *(Sprite **)((u8 *)var_r4_4 + offsetof(Ov96MonIconWork, timeSprites)) = ov96_021EA2C4(sp8, ov96_021EB5E8(arg1), 3, sp4->heapID);
        Sprite_SetDrawFlag(*(Sprite **)((u8 *)var_r4_4 + offsetof(Ov96MonIconWork, timeSprites)), 1);
        sp10[1].x = *var_r7 << 0xC;
        sp10[1].y = 0x238000;
        sp10[1].z = 0;
        Sprite_SetMatrix(*(Sprite **)((u8 *)var_r4_4 + offsetof(Ov96MonIconWork, timeSprites)), &sp10[1]);
        Sprite_SetAnimCtrlSeq(*(Sprite **)((u8 *)var_r4_4 + offsetof(Ov96MonIconWork, timeSprites)), (s32)*var_r6_2);
        var_r4_4 += 1;
        var_r7 += 1;
        temp_r0_2 = spC + 1;
        var_r6_2 += 1;
        spC = temp_r0_2;
    } while (temp_r0_2 < 6);
#endif
}

void ov96_022004B4(Ov96MonIconWork *work, s32 slot, s32 variant);

void ov96_022002F8(Ov96MonIconWork *work, Ov96MonSpec *mons);

void ov96_022002F8(Ov96MonIconWork *work, Ov96MonSpec *mons) {
    NARC *narc;
    int i;
    u16 species;
    u8 form;
    VecFx32 pos;
    narc = NARC_New(NARC_poketool_icongra_poke_icon, work->heapID);
    GfGfxLoader_GXLoadPal(NARC_poketool_icongra_poke_icon, sub_02074490(), GF_PAL_LOCATION_SUB_OBJ, GF_PAL_SLOT_1_OFFSET, 0x60, work->heapID);
    for (i = 0; i < 12; i++) {
        work->charBuf[i] = Heap_AllocAtEnd(work->heapID, 0x1000);
        form = mons[i].form;
        species = mons[i].species;
        NARC_ReadWholeMember(narc, GetMonIconNaixEx(species, FALSE, form), work->charBuf[i]);
        NNS_G2dGetUnpackedBGCharacterData(work->charBuf[i], &work->charData[i]);
        work->palette[i] = GetMonIconPaletteEx(species, form, FALSE) + 1;
    }
    NARC_Delete(narc);
    for (i = 0; i < 4; i++) {
        ov96_022004B4(work, (u8)i, 0);
        ov96_021EB52C(work->slots[i].obj0C, 1, 1);
        ov96_021EB52C(work->slots[i].obj18, 1, 1);
        pos.z = 0;
        pos.x = FX32_CONST(160);
        pos.y = FX32_CONST(688);
        ov96_021EB588(work->slots[i].obj0C, &pos);
        ov96_021EB588(work->slots[i].obj18, &pos);
    }
}

void ov96_022003E8(Ov96MonIconWork *arg0, Ov96MonPicSpec *arg1);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_022003E8(Ov96MonIconWork *arg0, Ov96MonPicSpec *arg1) {
    PokepicTemplate sp10;
    s32 spC;
    Ov96MonPicRes *var_r4;
    s32 temp_r0;
    Ov96MonPicSpec *var_r5;

    var_r5 = arg1;
    spC = 0;
    var_r4 = arg0->pics;
    do {
        GetMonSpriteCharAndPlttNarcIdsEx(&sp10, var_r5->species, var_r5->gender, 2U, var_r5->shiny, var_r5->form, var_r5->personality);
        var_r4->charBuf = sub_0201457C((enum NarcId)sp10.narcID, (s32)sp10.charDataID, arg0->heapID, var_r5->personality, 0, 2, (s32)var_r5->species);
        var_r4->plttBuf = sub_02014450((enum NarcId)sp10.narcID, (s32)sp10.palDataID, arg0->heapID);
        var_r5++;
        temp_r0 = spC + 1;
        var_r4++;
        spC = temp_r0;
    } while (temp_r0 < 3);
}
#else
void ov96_022003E8(Ov96MonIconWork *arg0, Ov96MonPicSpec *arg1) {
    PokepicTemplate sp10;
    s32 spC;
    enum HeapID *var_r4;
    s32 temp_r0;
    Ov96MonPicSpec *var_r5;

    var_r5 = arg1;
    spC = 0;
    var_r4 = &arg0->heapID;
    do {
        GetMonSpriteCharAndPlttNarcIdsEx(&sp10, var_r5->species, var_r5->gender, 2U, var_r5->shiny, var_r5->form, var_r5->personality);
        *(void **)((u8 *)var_r4 + offsetof(Ov96MonIconWork, pics[0].charBuf)) = sub_0201457C((enum NarcId)sp10.narcID, (s32)sp10.charDataID, arg0->heapID, var_r5->personality, 0, 2, (s32)var_r5->species);
        *(void **)((u8 *)var_r4 + offsetof(Ov96MonIconWork, pics[0].plttBuf)) = sub_02014450((enum NarcId)sp10.narcID, (s32)sp10.palDataID, arg0->heapID);
        var_r5++;
        temp_r0 = spC + 1;
        var_r4 += 2;
        spC = temp_r0;
    } while (temp_r0 < 3);
}
#endif

void ov96_02200454(Ov96MonIconWork *work, u32 idx, u32 slot);

Sprite *ov96_021EB5B8(Ov96Obj *);

void ov96_02200454(Ov96MonIconWork *work, u32 idx, u32 slot) {
    if ((1 + 1) <= idx) {
        GF_AssertFail();
    }
    if ((2 + 1) <= slot) {
        GF_AssertFail();
    }
    DC_FlushRange(work->pics[slot].charBuf, 0xc80);
    GXS_LoadOBJ(work->pics[slot].charBuf, work->charLoc[idx], 0xc80);
    DC_FlushRange(work->pics[slot].plttBuf, 0x20);
    GXS_LoadOBJPltt(work->pics[slot].plttBuf, work->plttLoc[idx], 0x20);
    return;
}

void ov96_022004B4(Ov96MonIconWork *work, s32 slot, s32 variant) {
    Sprite *sprite;
    s32 iVar2;

    sprite = ov96_021EB5B8(work->slots[slot].obj0C);
    iVar2 = variant + slot * 3;
    DC_FlushRange(work->charData[iVar2]->pRawData, 0x200);
    GXS_LoadOBJ(work->charData[iVar2]->pRawData, slot * 0x200 + 0x600, 0x200);
    Sprite_SetPaletteOverride(sprite, (u32)work->palette[iVar2]);
    return;
}

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_0220050C(Ov96MonIconWork *arg0, u8 arg1, s32 arg2) {
    VecFx32 *new_var;
    VecFx32 sp3C;
    VecFx32 sp30;
    VecFx32 sp24;
    MtxFx33 sp0;
    int temp_r1;
    s32 temp_r2;
    int temp_r2_2;
    temp_r1 = arg2 % 1024;
    temp_r2 = (u16)(0x10000 - (u16)((s32)temp_r1 * 0x10000 / 1024));
    sp30 = ov96_0221C748;
    sp24 = ov96_0221C754;
    temp_r2_2 = (((s32)temp_r2) >> 4) * 2;
    MTX_RotZ33_(&sp0, FX_SinCosTable_[temp_r2_2], FX_SinCosTable_[temp_r2_2 + 1]);
    MTX_MultVec33(&sp30, &sp0, &sp30);
    VEC_Add(&sp24, &sp30, &sp3C);
    temp_r2 = arg1;
    new_var = &sp3C;
    ov96_021EB588(arg0->slots[temp_r2].obj0C, new_var);
    ov96_021EB588(arg0->slots[temp_r2].obj18, new_var);
    ov96_021EB588(arg0->slots[temp_r2].obj10, new_var);
    ov96_021EB588(arg0->slots[temp_r2].obj14, new_var);
}
#else
void ov96_0220050C(Ov96MonIconWork *arg0, u8 arg1, s32 arg2) {
    VecFx32 *new_var;
    VecFx32 sp3C;
    VecFx32 sp30;
    VecFx32 sp24;
    MtxFx33 sp0;
    int temp_r1;
    s32 temp_r2;
    int temp_r2_2;
    temp_r1 = arg2 % 1024;
    temp_r2 = (u16)(0x10000 - (u16)((s32)temp_r1 * 0x10000 / 1024));
    sp30 = ov96_0221C748;
    sp24 = ov96_0221C754;
    temp_r2_2 = (((s32)temp_r2) >> 4) * 2;
    MTX_RotZ33_(&sp0, FX_SinCosTable_[temp_r2_2], FX_SinCosTable_[temp_r2_2 + 1]);
    MTX_MultVec33(&sp30, &sp0, &sp30);
    VEC_Add(&sp24, &sp30, &sp3C);
    temp_r2 = arg1 << 5;
    temp_r1 = (u32)0x48;
    new_var = &sp3C;
    ((s32(*)(s32, VecFx32 *))ov96_021EB588)(*((s32 *)(((u8 *)arg0) + (temp_r2 + temp_r1))), new_var);
    ((s32(*)(s32, VecFx32 *))ov96_021EB588)(*((s32 *)(((u8 *)arg0) + (temp_r2 + 0x54))), new_var);
    temp_r1 <<= 16;
    ((s32(*)(s32, VecFx32 *))ov96_021EB588)(*((s32 *)((temp_r2 + 0x4C) + ((u8 *)arg0))), new_var);
    ((s32(*)(s32, VecFx32 *))ov96_021EB588)(*((s32 *)(((u8 *)arg0) + (temp_r2 + 0x50))), new_var);
}
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_022005B4(Ov96MonIconWork *work, s32 arg1, u32 arg2) {
    u8 sp20[4];
    s32 sp1C;
    u8 temp_r4;
    u8 temp_r6;
    u8 var_r4;

    sp1C = (s32)(u8)(u32)((double)(f32)arg2 / 102.4);
    temp_r6 = (u8)(arg1 / 10);
    sp20[2] = (s8)(((s32)temp_r6 % 5) * 4);
    sp20[0] = (s8)(((s32)temp_r6 / 5) * 8);
    temp_r4 = (u8)(arg1 % 10);
    sp20[3] = (s8)(((s32)temp_r4 % 5) * 4);
    sp20[1] = (s8)(((s32)temp_r4 / 5) * 8);
    var_r4 = 0;
    do {
        CopyRectToBgTilemapRect(work->bgConfig, 5U, (u8)((var_r4 * 4) + 0xE), 8U, 4U, 8U, (const void *)work->scrnData->rawData, (u8)(s32)sp20[2 + var_r4], (u8)(s32)sp20[var_r4], 0x14U, 0x10U);
        var_r4 += 1;
    } while ((u32)var_r4 < 2U);
    CopyRectToBgTilemapRect(work->bgConfig, 5U, 0x18U, 8U, 4U, 8U, (const void *)work->scrnData->rawData, (u8)(u8)((sp1C % 5) * 4), (u8)(u8)((sp1C / 5) * 8), 0x14U, 0x10U);
    ScheduleBgTilemapBufferTransfer(work->bgConfig, 5U);
}
#else
void ov96_022005B4(Ov96MonIconWork *arg0, s32 arg1, u32 arg2) {
    u8 sp20;
    s32 sp1C;
    u8 temp_r4;
    u8 temp_r6;
    u8 var_r4;

    sp1C = (s32)(u8)(u32)((double)(f32)arg2 / 102.4);
    temp_r6 = (u8)(arg1 / 10);
    *(s8 *)((u8 *)(&sp20) + 2) = (s8)(((s32)temp_r6 % 5) * 4);
    *(s8 *)(&sp20) = (s8)(((s32)temp_r6 / 5) * 8);
    temp_r4 = (u8)(arg1 % 10);
    *(s8 *)((u8 *)(&sp20) + 3) = (s8)(((s32)temp_r4 % 5) * 4);
    *(s8 *)((u8 *)(&sp20) + 1) = (s8)(((s32)temp_r4 / 5) * 8);
    var_r4 = 0;
    do {
        CopyRectToBgTilemapRect(*(BgConfig **)((u8 *)arg0 + 8), 5U, (u8)((var_r4 * 4) + 0xE), 8U, 4U, 8U, (const void *)(*(void **)((u8 *)arg0 + 0x38) + 0xC), (u8)(s32) * (u8 *)(&sp20 + 2 + var_r4), (u8)(s32) * (u8 *)(&sp20 + var_r4), 0x14U, 0x10U);
        var_r4 += 1;
    } while ((u32)var_r4 < 2U);
    CopyRectToBgTilemapRect(*(BgConfig **)((u8 *)arg0 + 8), 5U, 0x18U, 8U, 4U, 8U, (const void *)(*(void **)((u8 *)arg0 + 0x38) + 0xC), (u8)(u8)((sp1C % 5) * 4), (u8)(u8)((sp1C / 5) * 8), 0x14U, 0x10U);
    ScheduleBgTilemapBufferTransfer(*(BgConfig **)((u8 *)arg0 + 8), 5U);
}
#endif

void _s32_div_f(s32, u32);

VecFx32 *ov96_021EB594(Ov96Obj *);

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov96_022006BC(Ov96MonIconWork *work, s32 param_2) {
    switch (work->phase) {
    case 0: {
        VecFx32 pos;
        pos = *ov96_021EB594(work->cursor[work->cursorIdx]);
        pos.y += 0x20000;
        ov96_021EB588(work->cursor[work->cursorIdx], &pos);
        ov96_021EB588(work->marker[work->cursorIdx], &pos);
        if (pos.y >= 0x2e0000) {
            pos.y = 0x1d8000;
            ov96_021EB588(work->cursor[work->cursorIdx], &pos);
            ov96_021EB588(work->marker[work->cursorIdx], &pos);
            work->phase += 1;
        }
        break;
    }
    case 1: {
        u16 *cnt;
        s32 idx;
        ov96_02200454(work, (u8)work->cursorIdx, param_2);
        cnt = work->counter;
        idx = work->cursorIdx;
        if (cnt[idx] == 0) {
            cnt[idx] = 2;
        } else {
            cnt[idx] = cnt[idx] - 1;
        }
        work->cursorIdx = (work->cursorIdx + 1) % 2;
        work->phase += 1;
        break;
    }
    case 2: {
        VecFx32 pos;
        pos = *ov96_021EB594(work->cursor[work->cursorIdx]);
        pos.y += 0x20000;
        ov96_021EB588(work->cursor[work->cursorIdx], &pos);
        ov96_021EB588(work->marker[work->cursorIdx], &pos);
        if (pos.y >= 0x288000) {
            pos.y = 0x288000;
            ov96_021EB588(work->cursor[work->cursorIdx], &pos);
            ov96_021EB588(work->marker[work->cursorIdx], &pos);
            work->cursorIdx = (work->cursorIdx + 1) % 2;
            work->phase += 1;
        }
        break;
    }
    case 3: {
        VecFx32 pos;
        u32 remainder;
        pos = *ov96_021EB594(work->cursor[work->cursorIdx]);
        pos.y += 0x20000;
        ov96_021EB588(work->cursor[work->cursorIdx], &pos);
        ov96_021EB588(work->marker[work->cursorIdx], &pos);
        if (pos.y >= 0x230000) {
            pos.y = 0x230000;
            ov96_021EB588(work->cursor[work->cursorIdx], &pos);
            ov96_021EB588(work->marker[work->cursorIdx], &pos);
            work->cursorIdx = (work->cursorIdx + 1) % 2;
            remainder = (param_2 + 2) % 3;
            ov96_02200BD8(work, (u8)remainder);
            work->phase = 0;
            return 1;
        }
        break;
    }
    }
    return 0;
}
#else
s32 ov96_022006BC(Ov96MonIconWork *param_1, s32 param_2) {
#if defined(SDK_PORT) || defined(NONMATCHING)
    switch (*(u32 *)((u8 *)param_1 + 0x164)) {
#else
    switch (*(u32 *)((s32)param_1 + 0x164)) {
#endif
    case 0: {
        VecFx32 pos;
        pos = *(VecFx32 *)ov96_021EB594(*(void **)((u8 *)param_1 + *(s32 *)((u8 *)param_1 + 0x168) * 4 + 0x150));
        pos.y += 0x20000;
        ((void (*)(u32, void *))ov96_021EB588)(*(u32 *)((u8 *)param_1 + *(s32 *)((u8 *)param_1 + 0x168) * 4 + 0x150), &pos);
        ((void (*)(u32, void *))ov96_021EB588)(*(u32 *)((u8 *)param_1 + *(s32 *)((u8 *)param_1 + 0x168) * 4 + 0x158), &pos);
        if (pos.y >= 0x2e0000) {
            pos.y = 0x1d8000;
#if defined(SDK_PORT) || defined(NONMATCHING)
            ((void (*)(u32, void *))ov96_021EB588)(*(u32 *)((u8 *)param_1 + *(s32 *)((u8 *)param_1 + 0x168) * 4 + 0x150), &pos);
#else
            ((void (*)(u32, void *))ov96_021EB588)(*(u32 *)((s32)param_1 + *(s32 *)((s32)param_1 + 0x168) * 4 + 0x150), &pos);
#endif
#if defined(SDK_PORT) || defined(NONMATCHING)
            ((void (*)(u32, void *))ov96_021EB588)(*(u32 *)((u8 *)param_1 + *(s32 *)((u8 *)param_1 + 0x168) * 4 + 0x158), &pos);
#else
            ((void (*)(u32, void *))ov96_021EB588)(*(u32 *)((s32)param_1 + *(s32 *)((s32)param_1 + 0x168) * 4 + 0x158), &pos);
#endif
#if defined(SDK_PORT) || defined(NONMATCHING)
            *(u32 *)((u8 *)param_1 + 0x164) += 1;
#else
            *(u32 *)((s32)param_1 + 0x164) += 1;
#endif
        }
        break;
    }
    case 1: {
        u16 *cnt;
        s32 idx;
        ((void (*)(void *, u32, s32))ov96_02200454)(param_1, (u8) * (u32 *)((u8 *)param_1 + 0x168), param_2);
#if defined(SDK_PORT) || defined(NONMATCHING)
        cnt = (u16 *)((u8 *)param_1 + 0x160);
#else
        cnt = (u16 *)((s32)param_1 + 0x160);
#endif
        idx = *(s32 *)((u8 *)param_1 + 0x168);
        if (cnt[idx] == 0) {
            cnt[idx] = 2;
        } else {
            cnt[idx] = cnt[idx] - 1;
        }
        *(s32 *)((u8 *)param_1 + 0x168) = (*(s32 *)((u8 *)param_1 + 0x168) + 1) % 2;
#if defined(SDK_PORT) || defined(NONMATCHING)
        *(u32 *)((u8 *)param_1 + 0x164) += 1;
#else
        *(u32 *)((s32)param_1 + 0x164) += 1;
#endif
        break;
    }
    case 2: {
        VecFx32 pos;
        pos = *(VecFx32 *)ov96_021EB594(*(void **)((u8 *)param_1 + *(s32 *)((u8 *)param_1 + 0x168) * 4 + 0x150));
        pos.y += 0x20000;
        ((void (*)(u32, void *))ov96_021EB588)(*(u32 *)((u8 *)param_1 + *(s32 *)((u8 *)param_1 + 0x168) * 4 + 0x150), &pos);
        ((void (*)(u32, void *))ov96_021EB588)(*(u32 *)((u8 *)param_1 + *(s32 *)((u8 *)param_1 + 0x168) * 4 + 0x158), &pos);
        if (pos.y >= 0x288000) {
            pos.y = 0x288000;
            ((void (*)(u32, void *))ov96_021EB588)(*(u32 *)((u8 *)param_1 + *(s32 *)((u8 *)param_1 + 0x168) * 4 + 0x150), &pos);
            ((void (*)(u32, void *))ov96_021EB588)(*(u32 *)((u8 *)param_1 + *(s32 *)((u8 *)param_1 + 0x168) * 4 + 0x158), &pos);
            *(s32 *)((u8 *)param_1 + 0x168) = (*(s32 *)((u8 *)param_1 + 0x168) + 1) % 2;
#if defined(SDK_PORT) || defined(NONMATCHING)
            *(u32 *)((u8 *)param_1 + 0x164) += 1;
#else
            *(u32 *)((s32)param_1 + 0x164) += 1;
#endif
        }
        break;
    }
    case 3: {
        VecFx32 pos;
        u32 remainder;
        pos = *(VecFx32 *)ov96_021EB594(*(void **)((u8 *)param_1 + *(s32 *)((u8 *)param_1 + 0x168) * 4 + 0x150));
        pos.y += 0x20000;
        ((void (*)(u32, void *))ov96_021EB588)(*(u32 *)((u8 *)param_1 + *(s32 *)((u8 *)param_1 + 0x168) * 4 + 0x150), &pos);
        ((void (*)(u32, void *))ov96_021EB588)(*(u32 *)((u8 *)param_1 + *(s32 *)((u8 *)param_1 + 0x168) * 4 + 0x158), &pos);
        if (pos.y >= 0x230000) {
            pos.y = 0x230000;
            ((void (*)(u32, void *))ov96_021EB588)(*(u32 *)((u8 *)param_1 + *(s32 *)((u8 *)param_1 + 0x168) * 4 + 0x150), &pos);
            ((void (*)(u32, void *))ov96_021EB588)(*(u32 *)((u8 *)param_1 + *(s32 *)((u8 *)param_1 + 0x168) * 4 + 0x158), &pos);
            *(s32 *)((u8 *)param_1 + 0x168) = (*(s32 *)((u8 *)param_1 + 0x168) + 1) % 2;
            remainder = (param_2 + 2) % 3;
            ((void (*)(void *, u32))ov96_02200BD8)(param_1, (u8)remainder);
#if defined(SDK_PORT) || defined(NONMATCHING)
            *(u32 *)((u8 *)param_1 + 0x164) = 0;
#else
            *(u32 *)((s32)param_1 + 0x164) = 0;
#endif
            return 1;
        }
        break;
    }
    }
    return 0;
}
#endif

s32 ov96_02200900(Ov96MonIconWork *work, u32 arg1);

s32 ov96_02200900(Ov96MonIconWork *work, u32 arg1) {
    s32 v0;

    if ((arg1 == 0) || (arg1 >= 4U)) {
        GF_AssertFail();
    }
    v0 = arg1 - 1;
    work->scroll[v0].pos = work->scroll[v0].pos + 0x20;
    if (work->scroll[v0].pos >= 0x58U) {
        work->scroll[v0].pos = 0;
        work->scroll[v0].stage = work->scroll[v0].stage + 1;
    }
    if ((u32)work->scroll[v0].stage >= 3U) {
        work->scroll[v0].stage = 0;
        return 1;
    }
    return 0;
}

void ov96_02200950(Ov96MonIconWork *arg0, s32 arg1, s8 arg2, s32 arg3, s32 arg4);

void ov96_02200950(Ov96MonIconWork *arg0, s32 arg1, s8 arg2, s32 arg3, s32 arg4) {
    if (arg2 != arg0->slotState[arg1]) {
        arg0->slotState[arg1] = arg2;
        ov96_022004B4(arg0, arg1, arg2);
    }
    if (arg3 != 0) {
        ov96_021EB52C(arg0->slots[arg1].obj0C, 1, 0);
        ov96_021EB52C(arg0->slots[arg1].obj14, 1, 1);
        ov96_021EB52C(arg0->slots[arg1].obj10, 1, 0);
    } else {
        ov96_021EB52C(arg0->slots[arg1].obj0C, 1, 1);
        ov96_021EB52C(arg0->slots[arg1].obj14, 1, 0);
        switch (arg4) {
        case 1:
            if (arg4 != arg0->slots[arg1].state1C) {
                ov96_021EB52C(arg0->slots[arg1].obj10, 1, 1);
                ov96_021EB564(arg0->slots[arg1].obj10, 8);
            }
            break;
        case 2:
            if (arg4 != arg0->slots[arg1].state1C) {
                ov96_021EB52C(arg0->slots[arg1].obj10, 1, 1);
                ov96_021EB564(arg0->slots[arg1].obj10, 9);
            }
            break;
        default:
            ov96_021EB52C(arg0->slots[arg1].obj10, 1, 0);
            break;
        }
    }
    arg0->slots[arg1].state1C = arg4;
}

void ov96_02200A18(Ov96MonIconWork *work, s32 param_2, s32 param_3);

void ov96_02200A18(Ov96MonIconWork *work, s32 param_2, s32 param_3) {
    if (param_2 == 1) {
        ov96_021EB52C(work->marker[0], 1, 1);
    } else {
        ov96_021EB52C(work->marker[0], 1, 0);
    }
    if (param_3 == 1) {
        ov96_021EB52C(work->marker[1], 1, 1);
        return;
    }
    ov96_021EB52C(work->marker[1], 1, 0);
    return;
}

void ov96_02200A64(Ov96MonIconWork *work, int time);

void ov96_02200A64(Ov96MonIconWork *work, int time) {
    u8 sec = time / 30;
    u8 rem;

    time = (u8)((u8)(time % 30) * 100 / 30);
    Sprite_SetAnimCtrlSeq(work->timeSprites[0], (u8)(sec / 60) + 1);
    rem = sec % 60;
    Sprite_SetAnimCtrlSeq(work->timeSprites[2], (u8)(rem / 10) + 1);
    Sprite_SetAnimCtrlSeq(work->timeSprites[3], (u8)(rem % 10) + 1);
    Sprite_SetAnimCtrlSeq(work->timeSprites[5], (u8)(time / 10) + 1);
}

void ov96_021EB630(Ov96Obj *, s32);

void ov96_02200B04(Ov96MonIconWork *arg0, s32 arg1);

u8 ov96_02200BC8(Ov96MonIconWork *work, s32 arg1);

void ov96_02200B04(Ov96MonIconWork *arg0, s32 arg1) {
    u8 v1[4];
    s32 v0;
    s32 v9;
    s32 v10;
    s32 v11;
    s32 v12;
    u8 *v8;
    u8 v3;
    u8 v2;
    u32 v4;
    u32 v5;

    v9 = 0;
    do {
        Sprite_SetDrawPriority(arg0->timeSprites[v9], 2U);
        v9 += 1;
    } while (v9 < 6);
    v10 = 0;
    do {
        Sprite_SetDrawPriority(ov96_021EB5B8(arg0->cursor[v10]), 2U);
        v10 += 1;
    } while (v10 < 2);
    v3 = 1;
    v4 = 0;
    do {
        if (v4 == arg1) {
            v1[0] = v4;
        }
        v4 += 1;
    } while ((s32)v4 < 4);
    v5 = 0;
    do {
        if (v1[0] != v5) {
            v2 = v3;
            v3 += 1;
            v1[v2] = v5;
        }
        v5 += 1;
    } while ((s32)v5 < 4);
    v12 = 0;
    v11 = 3;
    v8 = v1;
    v0 = 4;
    do {
        ov96_021EB630(arg0->slots[*v8].obj10, v11);
        ov96_021EB630(arg0->slots[*v8].obj14, v11);
        ov96_021EB630(arg0->slots[*v8].obj0C, v0);
        ov96_021EB630(arg0->slots[*v8].obj18, v12 + 0xB);
        v11 += 2;
        v8 += 1;
        v0 += 2;
        v12 += 1;
    } while (v12 < 4);
}

u8 ov96_02200BC8(Ov96MonIconWork *work, s32 arg1) {
    return (u8)work->counter[arg1];
}

void ov96_02200BD8(Ov96MonIconWork *work, s32 param_2) {
    String *string;

    FillWindowPixelBuffer(&work->windows[0], 0);
    BufferString(work->msgFormat, 0, work->names[param_2], 2, 1, 2);
    string = ReadMsgData_ExpandPlaceholders(work->msgFormat, work->msgData, 0x9a, work->heapID);
    AddTextPrinterParameterizedWithColor(&work->windows[0], FONT_SYSTEM, string, 0, 0, 0xff, 0xf0e00, (PrinterCallback_t)0x0);
    String_Delete(string);
    CopyWindowToVram(&work->windows[0]);
    return;
}

void ov96_02200C40(Ov96MonIconWork *work) {
    String *string;

    FillWindowPixelBuffer(&work->windows[1], 0);
    string = NewString_ReadMsgData(work->msgData, 0x131);
    AddTextPrinterParameterizedWithColor(&work->windows[1], FONT_SYSTEM, string, 0, 0, 0xff, 0xf0e00, (PrinterCallback_t)0x0);
    String_Delete(string);
    CopyWindowToVram(&work->windows[1]);
    return;
}

void ov96_02200D7C(void *arg0, Ov96MonIconWork *arg1);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_02200C8C(void *arg0, Ov96MonIconWork *arg1) {
    u32 sp14[2] = { 0x66, 0x67 };
    u32 spC[2] = { 4, 5 };
    VecFx32 sp1C;
    Ov96MonIconWork *sp8;
    void *sp4;
    Sprite *temp_r0;
    s32 var_r6;
    u32 *var_r4;
    u32 *var_r5;
    u32 *var_r7;
    u32 *var_r6_2;
    sp4 = arg0;
    sp8 = arg1;
    var_r5 = &sp14[0];
    var_r4 = &spC[0];
    var_r6 = 0;
    do {
        ov96_021EB408(sp4, 3, 2, *var_r5, *var_r4);
        var_r6 += 1;
        var_r4 += 1;
        var_r5 += 1;
    } while (var_r6 < 2);
    var_r6 = 0;
    var_r7 = &spC[0];
    var_r6_2 = &sp14[0];
    do {
        sp8->cursor[var_r6] = ov96_021EB4F4(sp4, *var_r6_2, *var_r7);
        temp_r0 = ov96_021EB5B8(sp8->cursor[var_r6]);
        sp1C.z = 0;
        sp1C.x = 0x28000;
        sp1C.y = (((0x58 * (1 - var_r6)) + 0x30) << 0xC) + 0x200000;
        Sprite_SetMatrix(temp_r0, &sp1C);
        ov96_021EB52C(sp8->cursor[var_r6], 1, 1);
        sp8->marker[var_r6] = ov96_021EB4F4(sp4, 0x65U, 0x10U);
        ov96_021EB564(sp8->marker[var_r6], 8);
        ov96_021EB588(sp8->marker[var_r6], &sp1C);
        ov96_021EB52C(sp8->marker[var_r6], 1, 0);
        var_r6 += 1;
        var_r7 += 1;
        var_r6_2 += 1;
    } while (var_r6 < 2);
    ov96_02200D7C(sp4, sp8);
}
#else
void ov96_02200C8C(void *arg0, Ov96MonIconWork *arg1) {
    u32 sp14[2] = { 0x66, 0x67 };
    u32 spC[2] = { 4, 5 };
    VecFx32 sp1C;
    void *sp8;
    s32 sp4;
    Sprite *temp_r0;
    s32 var_r6;
    u32 *var_r4;
    u32 *var_r5;
    u32 *var_r7;
    u32 *var_r6_2;
    void *var_r4_2;
    sp4 = (s32)arg0;
    sp8 = arg1;
    var_r5 = &sp14[0];
    var_r4 = &spC[0];
    var_r6 = 0;
    do {
        ((s32(*)(s32, s32, s32, u8, s32))ov96_021EB408)(sp4, 3, 2, (u8)(*var_r5), (s32)((u8)(*var_r4)));
        var_r6 += 1;
        var_r4 += 1;
        var_r5 += 1;
    } while (var_r6 < 2);
    var_r4_2 = sp8;
    var_r6 = 0;
    var_r7 = &spC[0];
    var_r6_2 = &sp14[0];
    do {
        *((s32 *)(((u8 *)var_r4_2) + 0x150)) = ((s32(*)(s32, u8, u8))ov96_021EB4F4)(sp4, (u8)(*var_r6_2), (u8)(*var_r7));
        temp_r0 = ((Sprite * (*)(s32)) ov96_021EB5B8)(*((s32 *)(((u8 *)var_r4_2) + 0x150)));
        sp1C.z = 0;
        sp1C.x = 0x28000;
        sp1C.y = (((0x58 * (1 - var_r6)) + 0x30) << 0xC) + 0x200000;
        Sprite_SetMatrix(temp_r0, &sp1C);
        ov96_021EB52C((Ov96Obj *)*((s32 *)(((u8 *)var_r4_2) + 0x150)), 1, 1);
        *((s32 *)(((u8 *)var_r4_2) + 0x158)) = ((s32(*)(s32, u8, u8))ov96_021EB4F4)(sp4, 0x65U, 0x10U);
        ov96_021EB564((Ov96Obj *)*((s32 *)(((u8 *)var_r4_2) + 0x158)), 8);
        ov96_021EB588((Ov96Obj *)*((s32 *)(((u8 *)var_r4_2) + 0x158)), &sp1C);
        ov96_021EB52C((Ov96Obj *)*((s32 *)(((u8 *)var_r4_2) + 0x158)), 1, 0);
        var_r6 += 1;
        var_r7 += 1;
        var_r6_2 += 1;
        var_r4_2 += 4;
    } while (var_r6 < 2);
    ov96_02200D7C((void *)sp4, sp8);
}
#endif

SpriteResource **ov96_021EB5EC(void *, s32, s32);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_02200D7C(void *arg0, Ov96MonIconWork *arg1) {
    s32 spArr[2] = { 0x66, 0x67 };
    s32 off;
    u8 i;
    s32 res;
    SpriteResource *charRes;
    SpriteResource *palRes;
    NNSG2dImageProxy *charProxy;
    NNSG2dImagePaletteProxy *palProxy;

    i = 0;
    do {
        off = i * 4;
        res = *(s32 *)((u8 *)spArr + off);
        charRes = *ov96_021EB5EC(arg0, res, 0);
        palRes = *ov96_021EB5EC(arg0, res, 1);
        charProxy = (NNSG2dImageProxy *)SpriteTransfer_GetCharProxy(charRes);
        palProxy = SpriteTransfer_GetPaletteProxy(palRes, charProxy);
        arg1->charLoc[i] = NNS_G2dGetImageLocation(charProxy, NNS_G2D_VRAM_TYPE_2DSUB);
        arg1->plttLoc[i] = NNS_G2dGetImagePaletteLocation(palProxy, NNS_G2D_VRAM_TYPE_2DSUB);
        i++;
    } while (i < 2);
}
#else
void ov96_02200D7C(void *arg0, Ov96MonIconWork *arg1) {
    s32 spArr[2] = { 0x66, 0x67 };
    s32 off;
    u8 i;
    s32 res;
    SpriteResource *charRes;
    SpriteResource *palRes;
    NNSG2dImageProxy *charProxy;
    NNSG2dImagePaletteProxy *palProxy;
    u8 *dst;

    i = 0;
    do {
        off = i * 4;
        res = *(s32 *)((u8 *)spArr + off);
        charRes = *ov96_021EB5EC(arg0, res, 0);
        palRes = *ov96_021EB5EC(arg0, res, 1);
        charProxy = (NNSG2dImageProxy *)SpriteTransfer_GetCharProxy(charRes);
        palProxy = SpriteTransfer_GetPaletteProxy(palRes, charProxy);
        dst = (u8 *)arg1 + off;
        *(u32 *)(dst + 0x140) = NNS_G2dGetImageLocation(charProxy, NNS_G2D_VRAM_TYPE_2DSUB);
        *(u32 *)(dst + 0x148) = NNS_G2dGetImagePaletteLocation(palProxy, NNS_G2D_VRAM_TYPE_2DSUB);
        i++;
    } while (i < 2);
}
#endif
