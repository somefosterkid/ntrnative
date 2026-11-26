#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "screen_fade.h"
#include "sprite.h"
#include "system.h"
#include "unk_0200B150.h"
#include "unk_0203A3B0.h"

#include "overlay_96_ov96_icon_work.h"
#include "overlay_96_unk_struct_ov96_021_f94a8.h"

PokeathlonMonPerf *ov96_021E60C0(PokeathlonCourseData *, s32, s32);

s32 ov96_021E6108(void *arg0);

s32 ov96_021E6168(PokeathlonCourseData *, s32, s32, s32 *);

Sprite **ov96_021E6290(PokeathlonCourseData *, s32, void *, void *);

s32 ov96_021E634C(PokeathlonCourseData *, s32, void *, void *, s32, u8, s32 *);

s32 ov96_021E6670(PokeathlonCourseData *, s32);

s32 ov96_021E92B0(s32 *, s32, s32, s32, s32);

void *ov96_021E9A78(enum HeapID, s32, s32);

Sprite *ov96_021EA374(void *, s32, s32, enum HeapID);

void *ov96_021EA854(enum HeapID, s32, s32, void *, s32);

SysTask *ov96_021EA8A8(void *, s32, s32 *, s32 *, s32, s32);

s32 ov96_021EAA00(void *);

void *ov96_021EAA04(void *, s32);

s32 ov96_021EAB38(void *, s32);

void *ov96_021EB180(enum HeapID, s32 *);

s32 ov96_021EB29C(void *, s32, s32);

s32 ov96_021EB2BC(void *, s32, s32, s32, s32);

s32 ov96_021EB2F4(enum HeapID *, s32, s32, s32, s32, u8);

s32 ov96_021EB334(void *, s32, s32, s32);

s32 ov96_021EB36C(void *, s32, s32, s32);

s32 ov96_021EB3A4(void *);

void *ov96_021EB3E4(enum HeapID *, s32, s32, u8, u8);

Sprite *ov96_021EB408(enum HeapID *, s32, s32, u8, u8);

s32 ov96_021EB52C(void *, s32, s32);

s32 ov96_021EB564(void *, s32);

s32 ov96_021EB588(void *, s32 *);

s32 ov96_021EB5C8(u8 *, s32, s32, s32, s32);

s32 ov96_021EB5E8(void *);

s32 ov96_021EB630(void *, s32);

s32 ov96_021F9E3C(void);

s32 ov96_021F9E5C(BgConfig *);

void ov96_021F9FE8(BgConfig *, enum HeapID);

void ov96_021FA020(BgConfig *, enum HeapID);

s32 ov96_021FA0E8(PokeathlonCourseData *, s32 *, s32 *, void *, void *, void *, s32, UnkStruct_ov96_021FAB24 *);

void ov96_021FB7C8(UnkStruct_ov96_021F94A8 *);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_021FBBB4(PokeathlonCourseData *, s32 *);
#else
s32 ov96_021FBBB4(PokeathlonCourseData *, s32 *);
#endif

u32 *ov96_021FC028(enum HeapID);

s32 ov96_021FC07C(u32 *, void *);

u8 *ov96_021FC0E4(void *);

u8 *ov96_021FC0E8(void *);

u32 ov96_021FC0F4(void *);

s32 ov96_021FC144(void *);

void *ov96_021FC618(enum HeapID);

s32 ov96_021FC630(void *, void *, u8);

extern u8 ov96_0221C3EC;

extern u8 ov96_0221C3F4;

extern s32 ov96_0221C404;

extern s32 ov96_0221C410;

BOOL ov96_021F94A8(PokeathlonCourseData *arg0, int unused);

BOOL ov96_021F94A8(PokeathlonCourseData *arg0, int unused) {
    s32 sp40[80];
    s32 sp28[6];
    s32 sp24;
    UnkStruct_ov96_021FAB24 *sp20;
    s32 *sp1C;
    u8 *sp18;
    s32 sp14;
    Ov96IconSpec *var_r5_4;
    s32 *var_r5_7;
    s32 *var_r7_3;
    s32 *var_r7_4;
    s32 temp_r0_3;
    s32 temp_r0_4;
    s32 var_r1;
    s32 var_r5;
    s32 var_r5_2;
    s32 var_r5_5;
    s32 var_r7_2;
    u16 temp_r2;
    u32 temp_r0;
    u8 *temp_r5;
    u8 *temp_r7;
    u8 *temp_r0_6;
    u8 *temp_r0_7;
    u8 *temp_r0_8;
    u8 *var_r0;
    u8 *var_r7;
    u8 var_r3;
    u8 var_r3_2;
    u32 var_r5_6;
    UnkStruct_ov96_021F94A8 *temp_r0_2;
    UnkStruct_ov96_021F94A8 *temp_r4;
#if !defined(SDK_PORT) && !defined(NONMATCHING)
    u8 *var_r5_3;
#endif

    temp_r4 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    temp_r0 = (u32)PokeathlonCourse_GetField1ED(arg0);
    switch (temp_r0) {
    case 0:
        Heap_Create(HEAP_ID_POKEATHLON, HEAP_ID_138, 0x48000U);
        Main_SetVBlankIntrCB(NULL, NULL);
        Main_SetHBlankIntrCB(NULL, NULL);
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        *(s32 *)REG_DISPCNT_ADDR &= 0xFFFFE0FF;
        *(s32 *)REG_DB_DISPCNT_ADDR &= 0xFFFFE0FF;
        ov96_021F9E3C();
        temp_r0_2 = PokeathlonCourse_AllocPtr4FromHeap(arg0, sizeof(UnkStruct_ov96_021F94A8));
        MI_CpuFill8(temp_r0_2, 0U, sizeof(UnkStruct_ov96_021F94A8));
        temp_r0_2->bgConfig = BgConfig_Alloc(HEAP_ID_138);
        temp_r0_2->unk_D8 = ov96_021FC028(temp_r0_2->heapID);
        temp_r0_2->unk_DC = ov96_021FC618(temp_r0_2->heapID);
        temp_r5 = ov96_021FC0E4(temp_r0_2->unk_D8);
        temp_r7 = ov96_021FC0E8(temp_r0_2->unk_D8);
        PokeathlonCourse_SetField3A4(arg0, temp_r5, temp_r7, ov96_021FC0F4(temp_r0_2->unk_D8));
        ov96_021E6670(arg0, 8);
        sp40[39] = 0x6B;
        sp40[40] = 0x40000;
        sp40[41] = 0x40000U >> 4;
        sp40[42] = 0x8A;
        ov96_021E92B0(&sp40[39], 0x12, 0x8A, 0x300010, 0x300010);
        NNS_G2dInitOamManagerModule();
        OamManager_Create(0, 0x7E, 0, 0x20, 0, 0x7E, 0, 0x20, HEAP_ID_138);
        temp_r0_2->heapID = HEAP_ID_138;
        FontID_Alloc(FONT_UNK_4, HEAP_ID_138);
        ov96_021F9E5C(temp_r0_2->bgConfig);
        ov96_021FB7C8(temp_r0_2);
        gSystem.screensFlipped = 1;
        GfGfx_SwapDisplay();
        PokeathlonCourse_IncrementField1ED(arg0);
        break;
    case 1:
        *(VecFx32 *)&sp40[36] = *(VecFx32 *)((u8 *)&ov96_0221C404);
        temp_r4->unk_224 = ov96_021EB180(temp_r4->heapID, &sp40[36]);
        ov96_021EB5C8(temp_r4->unk_224, 0, 0x110000, 0, 0);
        ov96_021EB29C(temp_r4->unk_224, 0, 0x65);
        ov96_021EB29C(temp_r4->unk_224, 1, 0x66);
        ov96_021EB29C(temp_r4->unk_224, 2, 0x67);
        ov96_021EB29C(temp_r4->unk_224, 3, 0x68);
        ov96_021EB29C(temp_r4->unk_224, 4, 0x69);
        ov96_021EB29C(temp_r4->unk_224, 5, 0x6A);
        ov96_021EB2BC(temp_r4->unk_224, 0x9C, 9, 0x65, 3);
        ov96_021EB2F4(temp_r4->unk_224, 0x9C, 6, 0x65, 3, 1);
        ov96_021EB334(temp_r4->unk_224, 0x9C, 8, 0x65);
        ov96_021EB36C(temp_r4->unk_224, 0x9C, 7, 0x65);
        ov96_021EB2BC(temp_r4->unk_224, 0x9C, 0x10, 0x66, 1);
        ov96_021EB2F4(temp_r4->unk_224, 0x9C, 0xD, 0x66, 1, 1);
        ov96_021EB334(temp_r4->unk_224, 0x9C, 0xF, 0x66);
        ov96_021EB36C(temp_r4->unk_224, 0x9C, 0xE, 0x66);
        ov96_021EB2BC(temp_r4->unk_224, 0x9C, 0xC, 0x67, 1);
        ov96_021EB2F4(temp_r4->unk_224, 0x9C, 6, 0x67, 1, 1);
        ov96_021EB334(temp_r4->unk_224, 0x9C, 0xB, 0x67);
        ov96_021EB36C(temp_r4->unk_224, 0x9C, 0xA, 0x67);
        ov96_021EB2BC(temp_r4->unk_224, 0x9C, 0x14, 0x69, 1);
        ov96_021EB2F4(temp_r4->unk_224, 0x9C, 0x11, 0x69, 1, 1);
        ov96_021EB334(temp_r4->unk_224, 0x9C, 0x13, 0x69);
        ov96_021EB36C(temp_r4->unk_224, 0x9C, 0x12, 0x69);
        ov96_021EB2BC(temp_r4->unk_224, 0x9C, 0x1C, 0x68, 1);
        ov96_021EB2F4(temp_r4->unk_224, 0x9C, 0x19, 0x68, 1, 1);
        ov96_021EB334(temp_r4->unk_224, 0x9C, 0x1B, 0x68);
        ov96_021EB36C(temp_r4->unk_224, 0x9C, 0x1A, 0x68);
        ov96_021EB2BC(temp_r4->unk_224, 0x9C, 0x20, 0x6A, 1);
        ov96_021EB2F4(temp_r4->unk_224, 0x9C, 0x1D, 0x6A, 1, 1);
        ov96_021EB334(temp_r4->unk_224, 0x9C, 0x1F, 0x6A);
        ov96_021EB36C(temp_r4->unk_224, 0x9C, 0x1E, 0x6A);
        ov96_021EB3A4(temp_r4->unk_224);
        temp_r4->unk_3A0 = ov96_021E9A78(temp_r4->heapID, 0x2E7, 1);
        temp_r4->unk_3A4 = ov96_021EA854(temp_r4->heapID, 3, 4, temp_r4->unk_3A0, ov96_021EB5E8(temp_r4->unk_224));
        PokeathlonCourse_IncrementField1ED(arg0);
        break;
    case 2:
        var_r5 = 0;
        do {
            Sprite_SetDrawPriority(ov96_021EB408(temp_r4->unk_224, 1, 3, 0x65, 2), 0x1BU);
            Sprite_SetDrawPriority(ov96_021EB408(temp_r4->unk_224, 1, 1, 0x67, 6), 0x1BU);
            var_r5 += 1;
        } while (var_r5 < 0xC);
        var_r5_2 = 0;
        do {
            ov96_021EB408(temp_r4->unk_224, 1, 1, 0x66, 5);
            ov96_021EB408(temp_r4->unk_224, 1, 1, 0x68, 7);
            ov96_021EB408(temp_r4->unk_224, 1, 1, 0x69, 8);
            var_r5_2 += 1;
        } while (var_r5_2 < 3);
        temp_r4->unk_3A8 = ov96_021EB3E4(temp_r4->unk_224, 1, 1, 0x6A, 9);
        ov96_021EB564(temp_r4->unk_3A8, 0);
        sp40[35] = 0;
        sp40[33] = 0xA0000;
        sp40[34] = 0x1C8000;
        ov96_021EB588(temp_r4->unk_3A8, &sp40[33]);
        ov96_021EB52C(temp_r4->unk_3A8, 1, 1);
        ov96_021EB630(temp_r4->unk_3A8, 1);
#if defined(SDK_PORT) || defined(NONMATCHING)
        sp14 = 0;
        var_r7 = &ov96_0221C3EC;
        sp18 = &ov96_0221C3F4;
        do {
            temp_r4->sprites[sp14] = ov96_021EA374(temp_r4->unk_3A0, ov96_021EB5E8(temp_r4->unk_224), 0, temp_r4->heapID);
            Sprite_SetDrawFlag(temp_r4->sprites[sp14], 1);
            sp40[30] = *sp18 << 0xC;
            sp40[31] = 0x1C8000;
            sp40[32] = 0;
            Sprite_SetMatrix(temp_r4->sprites[sp14], (VecFx32 *)&sp40[30]);
            Sprite_SetAnimCtrlSeq(temp_r4->sprites[sp14], (s32)*var_r7);
            sp18 += 1;
            var_r7 += 1;
            temp_r0_3 = sp14 + 1;
            sp14 = temp_r0_3;
        } while (temp_r0_3 < 6);
#else
        // DS form: the base steps 4 bytes per sprite and every access re-adds 0x3AC (== sprites)
        sp14 = 0;
        var_r7 = &ov96_0221C3EC;
        var_r5_3 = (u8 *)temp_r4;
        sp18 = &ov96_0221C3F4;
        do {
            *(Sprite **)(var_r5_3 + 0x3AC) = ov96_021EA374(temp_r4->unk_3A0, ov96_021EB5E8(temp_r4->unk_224), 0, temp_r4->heapID);
            Sprite_SetDrawFlag(*(Sprite **)(var_r5_3 + 0x3AC), 1);
            sp40[30] = *sp18 << 0xC;
            sp40[31] = 0x1C8000;
            sp40[32] = 0;
            Sprite_SetMatrix(*(Sprite **)(var_r5_3 + 0x3AC), (VecFx32 *)&sp40[30]);
            Sprite_SetAnimCtrlSeq(*(Sprite **)(var_r5_3 + 0x3AC), (s32)*var_r7);
            var_r5_3 += 4;
            sp18 += 1;
            var_r7 += 1;
            temp_r0_3 = sp14 + 1;
            sp14 = temp_r0_3;
        } while (temp_r0_3 < 6);
#endif
        ov96_021FC630(temp_r4->unk_DC, temp_r4->unk_224, 1);
        temp_r4->unk_228 = ov96_021FC188(temp_r4->heapID);
        var_r7_2 = 0;
        var_r5_4 = (Ov96IconSpec *)&sp40[18];
        do {
            var_r0 = PokeathlonCourse_GetParticipantUnk04(arg0, var_r7_2);
            var_r1 = 0;
loop_14:
            var_r1 += 1;
            var_r5_4->species = (u16) * (u16 *)var_r0;
            temp_r2 = *(u16 *)((u8 *)var_r0 + 2);
            var_r0 += 0x28;
            var_r5_4->form = temp_r2;
            var_r5_4++;
            if (var_r1 < 3) {
                goto loop_14;
            }
            var_r7_2 += 1;
        } while (var_r7_2 < 4);
        ov96_021FC214(temp_r4->unk_228, (Ov96IconSpec *)&sp40[18]);
        ov96_021FC2B4(temp_r4->unk_228, 0);
        var_r5_5 = 0;
        sp1C = &sp40[6];
        var_r7_3 = &sp40[63];
        do {
            ov96_021E6168(arg0, ov96_021E5F24(arg0), var_r5_5, sp1C);
            *(s32 *)((u8 *)var_r7_3 + 0x14) = ov96_021E6108(ov96_021E60C0(arg0, ov96_021E5F24(arg0), var_r5_5));
            var_r5_5 += 1;
            var_r7_3 = (void *)((u8 *)var_r7_3 + (4));
            sp1C = (void *)((u8 *)sp1C + (0x10));
        } while (var_r5_5 < 3);
        sp40[63] = 0;
        sp40[64] = 1;
        sp40[65] = 0;
        sp40[66] = 1;
        sp40[67] = 1;
        ov96_021EA8A8(temp_r4->unk_3A4, 3, &sp40[6], &sp40[63], 0, 0);
        PokeathlonCourse_IncrementField1ED(arg0);
        break;
    case 3:
        if (ov96_021EAA00(temp_r4->unk_3A4) == 0) {

        } else {
            PokeathlonCourse_SetVBlankIntrCB(temp_r4->bgConfig);
            ov96_021F9FE8(temp_r4->bgConfig, temp_r4->heapID);
            PokeathlonCourse_IncrementField1ED(arg0);
        }
        break;
    case 4:
        ov96_021FA020(temp_r4->bgConfig, temp_r4->heapID);
        reg_G2_BG0OFS = 0x01400000;
        reg_G2_BG1OFS = 0x01400000;
        reg_G2_BG2OFS = 0x01400000;
        reg_G2S_DB_BG0OFS = 0x300000;
        reg_G2S_DB_BG1OFS = 0x300000;
        reg_G2S_DB_BG2OFS = 0x300000;
        PokeathlonCourse_IncrementField1ED(arg0);
        break;
    case 5:
        GfGfx_EngineATogglePlanes(0x10U, 1U);
        GfGfx_EngineBTogglePlanes(0x10U, 1U);
        PokeathlonCourse_SetField1F4(arg0, 1);
        var_r5_6 = 0;
        do {
            ov96_021EAB38(ov96_021EAA04(temp_r4->unk_3A4, (u8)var_r5_6), 1);
            var_r5_6 += 1;
        } while ((s32)var_r5_6 < 3);
        PokeathlonCourse_IncrementField1ED(arg0);
        break;
    case 6:
        if (ov96_021E5F24(arg0) == 0) {
            ov96_021FC144(temp_r4->unk_D8);
        }
        PokeathlonCourse_IncrementField1ED(arg0);
        break;
    case 7:
        ov96_021FC07C(temp_r4->unk_D8, temp_r4->unk_224);
        Sprite_SetDrawPriority(*ov96_021E6290(arg0, 0x110, temp_r4->unk_3A0, temp_r4->unk_224), 1U);
        *(VecFx32 *)&sp40[3] = *(VecFx32 *)((u8 *)&ov96_0221C410);
        ReadWholeNarcMemberByIdPair(sp28, NARC_a_1_7_0, 0xA);
        ReadWholeNarcMemberByIdPair(&sp40[43], NARC_a_1_7_0, 0);
        sp24 = 0;
        sp20 = temp_r4->rows;
        var_r7_4 = &sp40[3];
        var_r5_7 = &sp40[0];
        do {
            ov96_021FA0E8(arg0, &sp40[43], sp28, temp_r4->unk_D8, temp_r4->unk_224, temp_r4->unk_3A4, (s32)(u8)sp24, sp20);
            *(s16 *)var_r5_7 = (s16)(*var_r7_4);
            var_r7_4 = (void *)((u8 *)var_r7_4 + (4));
            *(s16 *)((u8 *)var_r5_7 + 2) = 0x188;
            var_r5_7 = (void *)((u8 *)var_r5_7 + (4));
            sp20 += 1;
            temp_r0_4 = sp24 + 1;
            sp24 = temp_r0_4;
        } while (temp_r0_4 < 3);
        ov96_021E634C(arg0, 0, temp_r4->unk_3A0, temp_r4->unk_224, 1, 3, &sp40[0]);
        if (ov96_021E5F24(arg0) == 0) {
            ov96_021FBBB4(arg0, &sp40[43]);
        }
        temp_r0_6 = PokeathlonCourse_GetDataCopyArea(arg0);
        temp_r0_7 = ov96_021E8A20(temp_r0_6 + 0xF0);
        var_r3 = 0;
        do {
            *(s8 *)((u8 *)(&temp_r0_7[var_r3]) + 0x1C) = 0x11;
            var_r3 += 1;
        } while ((u32)var_r3 < 6U);
        if (ov96_021E5F24(arg0) == 0) {
            temp_r0_8 = ov96_021E8A20(temp_r0_6 + 0x28);
            var_r3_2 = 0;
            do {
                *(s8 *)((u8 *)(&temp_r0_8[var_r3_2]) + 0x1C) = 0x11;
                var_r3_2 += 1;
            } while ((u32)var_r3_2 < 6U);
        }
        sub_0203A994(1);
        BeginNormalPaletteFade(FADE_SUB_THEN_MAIN, FADE_TYPE_DOWNWARD_IN, FADE_TYPE_DOWNWARD_IN, 0U, 6, 1, temp_r4->heapID);
        PokeathlonCourse_IncrementField1ED(arg0);
        break;
    case 8:
        if (IsPaletteFadeFinished() != 0) {
            return 1;
        }
        break;
    }
    return 0;
}
