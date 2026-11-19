#include "bg_window.h"
#include "error_handling.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "pokeathlon/pokeathlon.h"
#include "pokemon.h"
#include "screen_fade.h"
#include "sprite.h"
#include "system.h"
#include "unk_0200B150.h"
#include "unk_0203A3B0.h"
s32 ov96_021E60C0(PokeathlonCourseData *, s32, s32);
s32 ov96_021E6108(s32);
s32 ov96_021E6168(PokeathlonCourseData *, s32, s32, s32 *);
Sprite **ov96_021E6290(PokeathlonCourseData *, s32, s32, s32);
s32 ov96_021E6670(PokeathlonCourseData *, s32);
void *ov96_021E8BB0(s32 arg0);
s32 ov96_021E92B0(s32 *, s32, s32, s32, s32);
s32 ov96_021E9A78(enum HeapID, s32, s32);
s32 ov96_021EA854(enum HeapID, s32, s32, s32, s32);
s32 ov96_021EA8A8(s32, s32, s32 *, s32 *, s32, s32);
s32 ov96_021EAA00();
s32 ov96_021EAA04(s32, u8);
s32 ov96_021EAA20(s32);
s32 ov96_021EAB38(s32, s32 arg1);
s32 ov96_021EABA8(s32, s32);
s32 ov96_021EAC0C(s32, s32);
s32 ov96_021EAF94(s32, s32, s32 arg2);
s32 ov96_021EB180(enum HeapID, VecFx32 *);
s32 ov96_021EB29C(s32, s32, s32);
s32 ov96_021EB3A4(s32);
s32 ov96_021EB588(s32, s32 *);
s32 ov96_021EB5C8(s32, s32, s32, s32, s32);
s32 ov96_021EB5E8(s32);
s32 ov96_02201C90();
s32 ov96_02201CB0(BgConfig *);
s32 ov96_02201E10(void *, u8);
s32 ov96_02201E70(s32);
s32 ov96_02201EF0(void *, s32, u8);
s32 ov96_02202738(void *, u8 *);
s32 ov96_0220329C(PokeathlonCourseData *, s32 *, s32, u8, void *);
s32 ov96_02203310(void *);
s32 ov96_0220382C(void *, PokeathlonCourseData *);
s32 ov96_02203A00(enum HeapID, PokeathlonCourseData *, PokeathlonCourseData *);
s32 ov96_02203B44(s32, s32);
s32 ov96_02203B8C(s32, s32, s32 *);
s32 ov96_02203D74(s32, s32, s32);
s32 ov96_02204364(enum HeapID, s32, u32);
s32 ov96_022043AC(s32, u8, void *, void *);
extern VecFx32 ov96_0221C7B8;
extern s32 ov96_0221C7D0;
extern s32 ov96_0221C8C8;
extern s32 ov96_0221C8EC;
extern u8 ov96_0221CA1C;

typedef struct LoopElem_ov96_02201558_0 { s32 m_418; u8 pad_4[0x1C]; } LoopElem_ov96_02201558_0; /* invented: loop element, stride 0x20 (hk9) */
s32 ov96_02201558(PokeathlonCourseData *arg0) {
    s32 sp13C[12][4];
    s32 spF8[17];
    s32 spA8[20];
    s32 sp98[4];
    VecFx32 sp8C;
    s32 sp74[3];
    VecFx32 sp68;
    void *sp64;
    s32 sp60;
    void *sp5C;
    s32 sp58;
    s32 sp54;
    s32 sp50;
    s32 sp4C;
    s32 sp48;
    s32 sp44;
    s32 sp40;
    void *sp3C;
    s32 sp38;
    s32 sp34;
    s32 *sp30;
    s32 *sp2C;
    u8 *sp28;
    s32 sp24;
    s32 sp20;
    s32 sp1C;
    s32 sp18;
    s32 sp14;
    s32 *var_r5;
    s32 *var_r4;
    s32 *temp_r3;
    s32 y;
    s32 j;
    s32 n;
    u8 *var_r5_3;
    s32 temp_r4;
    s32 h;
    void *temp_r7;
    void *temp_r0_2;

    temp_r7 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    switch (PokeathlonCourse_GetField1ED(arg0)) {
    case 0:
        Heap_Create(HEAP_ID_POKEATHLON, HEAP_ID_146, 0x40000U);
        Main_SetVBlankIntrCB(NULL, NULL);
        Main_SetHBlankIntrCB(NULL, NULL);
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        *(s32 *)0x04000000 &= 0xFFFFE0FF;
        *(s32 *)0x04001000 &= 0xFFFFE0FF;
        ov96_02201C90();
        temp_r0_2 = PokeathlonCourse_AllocPtr4FromHeap(arg0, 0x5F4U);
        MI_CpuFill8(temp_r0_2, 0U, 0x5F4U);
        *(void **)((u8 *)temp_r0_2 + 0x598) = Heap_Alloc(HEAP_ID_146, 0x28U);
        MI_CpuFill8(*(void **)((u8 *)temp_r0_2 + 0x598), 0U, 0x28U);
        *(BgConfig **)temp_r0_2 = BgConfig_Alloc(HEAP_ID_146);
        ov96_021E6670(arg0, 8);
        sp98[0] = 0x73;
        sp98[1] = 0x40000;
        sp98[2] = 0x40000U >> 4;
        sp98[3] = PokeathlonCourse_GetHeapID(arg0);
        ov96_021E92B0(sp98, 0x16, 0x92, 0x300010, 0x300010);
        NNS_G2dInitOamManagerModule();
        OamManager_Create(0, 0x7E, 0, 0x20, 0, 0x7E, 0, 0x20, HEAP_ID_146);
        *(s32 *)((u8 *)temp_r0_2 + 0x44) = 0x92;
        FontID_Alloc(FONT_UNK_4, HEAP_ID_146);
        ov96_02201CB0(*(BgConfig **)temp_r0_2);
        ov96_02203310(temp_r0_2);
        ov96_0220382C(temp_r0_2, arg0);
        gSystem.screensFlipped = 0;
        GfGfx_SwapDisplay();
        PokeathlonCourse_IncrementField1ED(arg0);
        break;
    case 1:
        *(s32 *)((u8 *)temp_r7 + 0x5E0) = ov96_02203A00(*(enum HeapID *)((u8 *)temp_r7 + 0x44), *(PokeathlonCourseData **)temp_r7, arg0);
        temp_r4 = PokeathlonCourse_GetParticipantCount(arg0);
        *(s32 *)((u8 *)temp_r7 + 0x5DC) = ov96_02204364(*(enum HeapID *)((u8 *)temp_r7 + 0x44), 4 - temp_r4, PokeathlonCourse_GetMode(arg0));
        PokeathlonCourse_IncrementField1ED(arg0);
        break;
    case 2:
        sp8C = ov96_0221C7B8;
        *(s32 *)((u8 *)temp_r7 + 0x48) = ov96_021EB180(*(enum HeapID *)((u8 *)temp_r7 + 0x44), &sp8C);
        ov96_021EB5C8(*(s32 *)((u8 *)temp_r7 + 0x48), 0, 0, 0, 0x120000);
        ov96_021EB29C(*(s32 *)((u8 *)temp_r7 + 0x48), 0, 0x6A);
        ov96_021EB29C(*(s32 *)((u8 *)temp_r7 + 0x48), 1, 0x65);
        ov96_021EB29C(*(s32 *)((u8 *)temp_r7 + 0x48), 2, 0x69);
        ov96_02201E70(*(s32 *)((u8 *)temp_r7 + 0x48));
        ov96_02203B44(*(s32 *)((u8 *)temp_r7 + 0x5E0), *(s32 *)((u8 *)temp_r7 + 0x48));
        ov96_021EB3A4(*(s32 *)((u8 *)temp_r7 + 0x48));
        *(s32 *)((u8 *)temp_r7 + 0x5D4) = ov96_021E9A78(*(enum HeapID *)((u8 *)temp_r7 + 0x44), 0x4E7, 1);
        *(s32 *)((u8 *)temp_r7 + 0x5D8) = ov96_021EA854(*(enum HeapID *)((u8 *)temp_r7 + 0x44), 9, 0x20, *(s32 *)((u8 *)temp_r7 + 0x5D4), ov96_021EB5E8(*(s32 *)((u8 *)temp_r7 + 0x48)));
        Sprite_SetDrawPriority(*ov96_021E6290(arg0, 0, *(s32 *)((u8 *)temp_r7 + 0x5D4), *(s32 *)((u8 *)temp_r7 + 0x48)), 1U);
        ov96_02201EF0(temp_r7, *(s32 *)((u8 *)temp_r7 + 0x48), (u8)ov96_021E5F24(arg0));
        PokeathlonCourse_IncrementField1ED(arg0);
        break;
    case 3:
        sp1C = (u8)ov96_021E5F24(arg0);
        sp54 = 0;
        sp24 = (u8)(((sp1C + 1) * 3) % 12);
        var_r5 = sp13C[0];
        var_r4 = spF8;
        do {
            sp20 = (u8)(sp24 % 12);
            sp14 = sp20 % 3;
            sp58 = sp20 / 3;
            ov96_021E6168(arg0, sp58, sp14, var_r5);

            var_r4[5] = ov96_021E6108(ov96_021E60C0(arg0, sp58, sp14));
            var_r5 += 4;
            sp24++;
            var_r4++;
            sp54++;
        } while (sp54 < 12);
        ov96_02203B8C(*(s32 *)((u8 *)temp_r7 + 0x5E0), *(s32 *)((u8 *)temp_r7 + 0x48), sp13C[9]);
        sp18 = 0;
        do {
            VecFx32 sp80 = { 0, 0, 0 };
            temp_r3 = sp13C[sp18 + 9];
            h = GetMonPicHeightBySpeciesGenderForm(*(u16 *)temp_r3, ((u8 *)temp_r3)[7], 0, (u8)((u16 *)temp_r3)[1], temp_r3[3]);
            sp80.x = (sp18 * 50 + 16) << 12;
            sp80.y = (h + 0x178) << 12;
            sp80.y = sp80.y - 0x10000;
            ov96_021EB588(((LoopElem_ov96_02201558_0 *)((u8 *)((u8 *)temp_r7 + ((sp1C * 3) << 5)) + 0x418))[sp18].m_418, (s32 *)&sp80);
            sp18++;
        } while (sp18 < 3);
        spF8[1] = 3;
        spF8[0] = 0;
        spF8[2] = 0;
        spF8[3] = 1;
        spF8[4] = 1;
        ov96_021EA8A8(*(s32 *)((u8 *)temp_r7 + 0x5D8), 9, sp13C[0], spF8, 0, 0);
        PokeathlonCourse_IncrementField1ED(arg0);
        break;
    case 4:
        if (ov96_021EAA00(*(s32 *)((u8 *)temp_r7 + 0x5D8)) == 0) {
            break;
        }
        *(VecFx32 *)sp74 = *(VecFx32 *)&ov96_0221C7D0;
        PokeathlonCourse_SetVBlankIntrCB(*(PokeathlonCourseData **)temp_r7);
        PokeathlonCourse_SetField1F4(arg0, 1);
        h = ov96_021E5F24(arg0);
        sp50 = 0;
        var_r5_3 = temp_r7;
        sp34 = (u8)(((h + 1) * 3) % 12);
        sp30 = &ov96_0221C8C8;
        sp2C = &ov96_0221C8EC;
        sp28 = &ov96_0221CA1C;
        do {
            *(s32 *)(var_r5_3 + 0x94) = ov96_021EAA04(*(s32 *)((u8 *)temp_r7 + 0x5D8), sp50);
            sp4C = *(s32 *)(var_r5_3 + 0x94);
            ov96_021EAB38(sp4C, 1);
            sp48 = (u8)(sp34 % 12);
            sp44 = *sp30;
            y = *sp2C;
            ov96_021EAC0C(sp4C, sp74[sp50 / 3]);
            ov96_021EAF94(sp4C, sp44, y);
            sp68.x = 0;
            sp68.y = 0;
            sp68.z = 0;
            if (*(u16 *)((u8 *)ov96_021E8BB0(ov96_021EAA20(sp4C)) + 4) != 0) {
                sp68.x = sp44 << 12;
                sp68.y = (y - 0x28) << 12;
            } else {
                sp68.x = sp44 << 12;
                sp68.y = (y - 0x19) << 12;
            }
            ov96_021EB588(*(s32 *)((u8 *)temp_r7 + (sp48 << 5) + 0x418), (s32 *)&sp68);
            ov96_021EB588(*(s32 *)(var_r5_3 + 0x70), (s32 *)&sp68);
            ov96_021EABA8(sp4C, *sp28 + 0x20);
            var_r5_3 += 4;
            sp34++;
            sp30++;
            sp2C++;
            sp28++;
            sp50++;
        } while (sp50 < 9);
        ReadWholeNarcMemberByIdPair(spA8, NARC_a_1_7_0, 3);
        *(s16 *)((u8 *)temp_r7 + 0x5E8) = 0x708;
        sp40 = 0;
        sp38 = 0;
        sp64 = temp_r7;
        sp64 = (u8 *)temp_r7 + 0xB8;
        sp5C = (u8 *)temp_r7 + 0x418;
        do {
            j = 0;
            *((u8 *)temp_r7 + sp40 + 0x5CC) = 12;
            sp60 = (u8)sp40;
            do {
                n = j + sp38;
                sp3C = (u8 *)sp5C + (n << 5);
                ov96_0220329C(arg0, spA8, sp60, j, sp3C);
                if (ov96_021E5F24(arg0) == 0) {
                    ov96_022043AC(*(s32 *)((u8 *)temp_r7 + 0x5DC), n, sp3C, (u8 *)sp64 + 0x48 * n);
                }
                j++;
            } while (j < 3);
            sp38 += 3;
            sp40++;
        } while (sp40 < 4);
        if (ov96_021E5F24(arg0) == 0) {
            ov96_02202738(temp_r7, ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(arg0) + 0x28));
        }
        ov96_02203D74(*(s32 *)((u8 *)temp_r7 + 0x5E0), *(s32 *)((u8 *)temp_r7 + 0x48), *(s32 *)((u8 *)temp_r7 + 0x5D4));
        ov96_02201E10(temp_r7, (u8)ov96_021E5F24(arg0));
        GfGfx_EngineATogglePlanes(0x10U, 1U);
        GfGfx_EngineBTogglePlanes(0x10U, 1U);
        sub_0203A994(1);
        BeginNormalPaletteFade(FADE_SUB_THEN_MAIN, FADE_TYPE_DOWNWARD_IN, FADE_TYPE_DOWNWARD_IN, 0U, 6, 1, *(enum HeapID *)((u8 *)temp_r7 + 0x44));
        PokeathlonCourse_IncrementField1ED(arg0);
        break;
    case 5:
        if (IsPaletteFadeFinished() != 0) {
            if (GF_heap_c_dummy_return_true(HEAP_ID_POKEATHLON) == 0) {
                GF_AssertFail();
            }
            return 1;
        }
        break;
    }
    return 0;
}
