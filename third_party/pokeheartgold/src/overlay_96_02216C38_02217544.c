#include "global.h"

#include "palette.h"
#include "sprite_system.h"

#include "overlay_96_ov96_rd_work.h"






ManagedSprite *ov96_022164EC(SpriteSystem *, SpriteManager *, s16, u8, u16, u16);

#if defined(SDK_PORT) || defined(NONMATCHING)
struct Ov96SpriteResDesc *ov96_021E8BB0(void *entry);
void ov96_021E8BB4(struct Ov96SpriteResDesc *desc, enum HeapID heapID, void *dest);
struct Ov96SpriteSetElem *ov96_021EAA04(struct Ov96SpriteSet *set, s32 index);
void *ov96_021EAA20(struct Ov96SpriteSetElem *elem);

void ov96_02217544(Ov96RdSprites *sprites, SpriteSystem *spriteSystem, SpriteManager *spriteManager, void *unk0, struct Ov96SpriteSet *spriteSet, enum HeapID heapId);

void ov96_02217544(Ov96RdSprites *sprites, SpriteSystem *spriteSystem, SpriteManager *spriteManager, void *unk0, struct Ov96SpriteSet *spriteSet, enum HeapID heapId) {
    s32 i;
    s32 base;
    enum HeapID heap;
    s32 j;
    u16 *normal;
    u16 *gray;
    u8 idx;
    Ov96RdSlot *slot;
    ManagedSprite *sprite;

    sprites->unk_0 = unk0;
    sprites->spriteSystem = spriteSystem;
    sprites->spriteManager = spriteManager;
    sprites->sprite = ov96_022164EC(spriteSystem, spriteManager, 0x80, 0x10, 3, 1);
    i = 0;
    slot = sprites->slots;
    base = 0;
    heap = heapId;
    do {
        idx = base;
        slot->elem = ov96_021EAA04(spriteSet, idx);
        j = 0;
        normal = slot->pal[0].normal;
        gray = slot->pal[0].gray;
        do {
            ov96_021E8BB4(ov96_021E8BB0(ov96_021EAA20(ov96_021EAA04(spriteSet, idx + j))), heap, normal);
            MI_CpuCopy8(normal, gray, sizeof(slot->pal[0].gray));
            TintPalette_GrayScale(gray, 16);
            j++;
            normal += 32;
            gray += 32;
        } while (j < 3);
        sprite = ov96_022164EC(spriteSystem, spriteManager, 0, 0, 7, 3);
        slot->sprites[4] = sprite;
        ManagedSprite_SetDrawFlag(sprite, 0);
        sprite = ov96_022164EC(spriteSystem, spriteManager, 0, 0, 1, 5);
        slot->sprites[1] = sprite;
        ManagedSprite_SetDrawFlag(sprite, 0);
        sprite = ov96_022164EC(spriteSystem, spriteManager, 0, 0, 0xB, 4);
        slot->sprites[0] = sprite;
        ManagedSprite_SetDrawFlag(sprite, 0);
        sprite = ov96_022164EC(spriteSystem, spriteManager, 0, 0, 0x15, 6);
        slot->sprites[2] = sprite;
        ManagedSprite_SetDrawFlag(sprite, 0);
        sprite = ov96_022164EC(spriteSystem, spriteManager, 0, 0, 8, 2);
        slot->sprites[3] = sprite;
        ManagedSprite_SetDrawFlag(sprite, 0);
        slot++;
        base += 3;
    } while (++i < 4);
}
#else
// DS: the asm spills arg0..arg2 to the stack (sp8/spC/sp10) and walks the slots through a void pointer;
// no typed form found yet that keeps that frame (the PC form above is the meaning)
void *ov96_021E8BB0();

s32 ov96_021E8BB4(void *, s32, void *);

s32 ov96_021EAA04(s32, u8);

s32 ov96_021EAA20();

void ov96_02217544(void *arg0, void *arg1, void *arg2, s32 arg3, s32 arg4, u32 arg5);

void ov96_02217544(void *arg0, void *arg1, void *arg2, s32 arg3, s32 arg4, u32 arg5) {
    s32 sp1C;
    s32 sp18;
    s32 sp14;
    void *sp10;
    void *spC;
    void *sp8;
    ManagedSprite *temp_r0;
    ManagedSprite *temp_r0_2;
    ManagedSprite *temp_r0_3;
    ManagedSprite *temp_r0_4;
    ManagedSprite *temp_r0_5;
    s32 temp_r0_6;
    s32 var_r7;
    u16 *var_r4;
    u16 *var_r5;
    u8 var_r6;

    sp8 = arg0;
    spC = arg1;
    sp10 = arg2;
    *(s32 *)arg0 = arg3;
    *(void **)((u8 *)arg0 + 4) = arg1;
    *(void **)((u8 *)arg0 + 8) = arg2;
    *(ManagedSprite **)((u8 *)sp8 + 0xC) = ov96_022164EC(spC, arg2, 0x80, 0x10, 3, 1);
    sp1C = 0;
    sp8 += 0x10;
    sp18 = 0;
    sp14 = arg5;
loop_1:
    var_r6 = (u8)sp18;
    *(s32 *)sp8 = ov96_021EAA04(arg4, var_r6);
    var_r7 = 0;
    var_r4 = sp8 + 0x1C;
    var_r5 = sp8 + 0x3C;
    do {
        ov96_021EAA04(arg4, var_r6 + var_r7);
        ov96_021EAA20();
        ov96_021E8BB4(ov96_021E8BB0(), sp14, var_r4);
        MI_CpuCopy8(var_r4, var_r5, 0x20U);
        TintPalette_GrayScale(var_r5, 0x10);
        var_r7 += 1;
        var_r4 += 32;
        var_r5 += 32;
    } while (var_r7 < 3);
    temp_r0 = ov96_022164EC(spC, sp10, 0, 0, 7, 3);
    *(ManagedSprite **)((u8 *)sp8 + 0x14) = temp_r0;
    ManagedSprite_SetDrawFlag(temp_r0, 0);
    temp_r0_2 = ov96_022164EC(spC, sp10, 0, 0, 1, 5);
    *(ManagedSprite **)((u8 *)sp8 + 8) = temp_r0_2;
    ManagedSprite_SetDrawFlag(temp_r0_2, 0);
    temp_r0_3 = ov96_022164EC(spC, sp10, 0, 0, 0xB, 4);
    *(ManagedSprite **)((u8 *)sp8 + 4) = temp_r0_3;
    ManagedSprite_SetDrawFlag(temp_r0_3, 0);
    temp_r0_4 = ov96_022164EC(spC, sp10, 0, 0, 0x15, 6);
    *(ManagedSprite **)((u8 *)sp8 + 0xC) = temp_r0_4;
    ManagedSprite_SetDrawFlag(temp_r0_4, 0);
    temp_r0_5 = ov96_022164EC(spC, sp10, 0, 0, 8, 2);
    *(ManagedSprite **)((u8 *)sp8 + 0x10) = temp_r0_5;
    ManagedSprite_SetDrawFlag(temp_r0_5, 0);
    sp8 += 0xE8;
    sp18 += 3;
    temp_r0_6 = sp1C + 1;
    sp1C = temp_r0_6;
    if (temp_r0_6 < 4) {
        goto loop_1;
    }
}
#endif
