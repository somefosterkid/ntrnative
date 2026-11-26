#include "global.h"

#include "heap.h"
#include "pokemon.h"
#include "sprite_system.h"
#include "unk_02013FDC.h"

s32 ov96_021EED64(Sprite *, void *, s32);

s32 ov96_021EED70(ManagedSprite *, int, s32, s32, void *, enum HeapID);

extern UnkStruct_02014E30 ov96_0221B1AC;

void ov96_021EEC0C(ManagedSprite *arg0, void *arg1, u32 arg2, void *arg3, enum HeapID arg4, s32 arg5);

void ov96_021EEC0C(ManagedSprite *arg0, void *arg1, u32 arg2, void *arg3, enum HeapID arg4, s32 arg5) {
    PokepicTemplate sp28;
    UnkStruct_02014E30 sp18;
    s32 sp14;
    u16 temp_r1;
    void *temp_r0;
    temp_r1 = *((u16 *)arg1);
    sp14 = arg2;
    if (temp_r1 == 0) {
        ManagedSprite_SetDrawFlag(arg0, 0);
        return;
    }
    GetMonSpriteCharAndPlttNarcIdsEx(&sp28, temp_r1, *((u8 *)(((u8 *)arg1) + 7)), 2U, (u8)((s32)(*((u8 *)(((u8 *)arg1) + 6)))), (u8)((u8)(*((u16 *)(((u8 *)arg1) + 2)))), *((u32 *)(((u8 *)arg1) + 0xC)));
    sp18 = ov96_0221B1AC;
    temp_r0 = Heap_AllocAtEnd(arg4, 0xC80U);
    sub_02014510((enum NarcId)sp28.narcID, sp28.charDataID, arg4, &sp18, temp_r0, *((u32 *)(((u8 *)arg1) + 0xC)), 0, 2, (s32)(*((u16 *)arg1)));
    ov96_021EED64(arg0->sprite, temp_r0, 0xC80);
    if (arg5 != 0) {
        ov96_021EED70(arg0, sp28.narcID, sp28.palDataID, sp14, arg3, arg4);
    }
    ManagedSprite_SetDrawFlag(arg0, 1);
    Heap_Free(temp_r0);
}
