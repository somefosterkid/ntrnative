#include "global.h"

#include "constants/heap.h"

#include "pokeathlon/pokeathlon.h"

#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "overlay_96_app_021ed1d0.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov96_021E6168(PokeathlonCourseData *, s32, s32, s32 *);


s32 ov96_021EEBF8(ManagedSprite *, s32 *, s32, s16, enum HeapID, void *);

void ov96_021EC790(PokeathlonCourseData *arg0, s32 arg1);

void ov96_021EC790(PokeathlonCourseData *arg0, s32 arg1) {
    s32 sp30[4];
    PokepicTemplate sp20;
    u16 *new_var;
    NNSG2dPaletteData *sp1C;
    unsigned char new_var2;
    void *sp18;
    s32 sp14;
    s32 sp10;
    PokeathlonCourseData *spC;
    s32 temp_r0;
    ManagedSprite **var_r6;
    void *temp_r0_2;
    Ov96App_021ED1D0 *temp_r4;
    u32 var_r5;
    spC = arg0;
    sp10 = arg1;
    temp_r4 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    PokeathlonCourse_GetParticipantUnk04(arg0, arg1);
    PokeathlonCourse_GetParticipantData(arg0, arg1);
    sp14 = 0;
    var_r6 = &temp_r4->sprites[21];
    var_r5 = 1;
    do {
        ov96_021E6168(spC, sp10, sp14, sp30);
        ov96_021EEBF8(*var_r6, sp30, 1, 0, temp_r4->heapId, 0);
        GetMonSpriteCharAndPlttNarcIdsEx(&sp20, *((u16 *)(((u8 *)(&sp1C)) + 0x14)), *((u8 *)(((u8 *)(&sp1C)) + 0x1B)), 2U, (u8)((s32)(*((u8 *)(((u8 *)(&sp1C)) + 0x1A)))), (u8)((u8)(*((u16 *)(((u8 *)(&sp1C)) + 0x16)))), sp30[3]);
        new_var = &(*((u16 *)(((u8 *)(&sp1C)) + 8)));
        temp_r0_2 = GfGfxLoader_GetPlttData((enum NarcId)(*((u16 *)(((u8 *)(&sp1C)) + 4))), (s32)(*new_var), &sp1C, temp_r4->heapId);
        new_var2 = var_r5;
        sp18 = temp_r0_2;
        ov96_021EAF60(temp_r4->spriteSet, new_var2, 1, sp1C->pRawData);
        Heap_Free(temp_r0_2);
        var_r6 += 1;
        var_r5 = var_r5 + 3;
        temp_r0 = sp14 + 1;
        sp14 = temp_r0;
    } while (temp_r0 < 3);
}
#else
s32 ov96_021E6168(PokeathlonCourseData *, s32, s32, s32 *);


s32 ov96_021EEBF8(void *, s32 *, s32, s32, enum HeapID, void *);

void ov96_021EC790(PokeathlonCourseData *arg0, s32 arg1);

void ov96_021EC790(PokeathlonCourseData *arg0, s32 arg1) {
    s32 sp30[4];
    PokepicTemplate sp20;
    u16 *new_var;
    NNSG2dPaletteData *sp1C;
    unsigned char new_var2;
    void *sp18;
    s32 sp14;
    s32 sp10;
    PokeathlonCourseData *spC;
    s32 temp_r0;
    void *var_r6;
    void *temp_r0_2;
    void *temp_r4;
    u32 var_r5;
    spC = arg0;
    sp10 = arg1;
    temp_r4 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    PokeathlonCourse_GetParticipantUnk04(arg0, arg1);
    PokeathlonCourse_GetParticipantData(arg0, arg1);
    sp14 = 0;
    var_r6 = temp_r4;
    var_r5 = 1;
    do {
        ov96_021E6168(spC, sp10, sp14, sp30);
        ov96_021EEBF8(*((void **)(((u8 *)var_r6) + 0x74)), sp30, 1, 0, *((enum HeapID *)temp_r4), 0);
        GetMonSpriteCharAndPlttNarcIdsEx(&sp20, *((u16 *)(((u8 *)(&sp1C)) + 0x14)), *((u8 *)(((u8 *)(&sp1C)) + 0x1B)), 2U, (u8)((s32)(*((u8 *)(((u8 *)(&sp1C)) + 0x1A)))), (u8)((u8)(*((u16 *)(((u8 *)(&sp1C)) + 0x16)))), sp30[3]);
        new_var = &(*((u16 *)(((u8 *)(&sp1C)) + 8)));
        temp_r0_2 = GfGfxLoader_GetPlttData((enum NarcId)(*((u16 *)(((u8 *)(&sp1C)) + 4))), (s32)(*new_var), &sp1C, *((enum HeapID *)temp_r4));
        new_var2 = var_r5;
        sp18 = temp_r0_2;
        ov96_021EAF60(*((void **)(((u8 *)temp_r4) + 0x14)), new_var2, 1, sp1C->pRawData);
        Heap_Free(temp_r0_2);
        var_r6 += 4;
        var_r5 = var_r5 + 3;
        temp_r0 = sp14 + 1;
        sp14 = temp_r0;
    } while (temp_r0 < 3);
}
#endif
