#include "global.h"

#include "constants/heap.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "error_handling.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "save.h"
#include "screen_fade.h"
#include "sound.h"
#include "system.h"
#include "unk_0203A3B0.h"
#include "overlay_01_choose_starter_app.h"
#include "overlay_96_unk_struct_ov96_021_f7934.h"
#include "overlay_96_text_box.h"
#include "overlay_96_ov96_sprite_mgr.h"

void ov96_021EE6A0(void *arg0);

void ov96_021F7DA8(UnkStruct_ov96_021F7934 *arg0);

void sub_0203A86C(void);

void sub_0203A9C8(void);

u8 ov96_021E9524(ChooseStarterApp *app);

s32 ov96_021E9528(void *);

s32 ov96_021EB5C8(void *, s32, s32, s32, s32);

void *ov96_021EE5B4(u32, s32);

s32 ov96_021EE60C(void *, BgConfig *);

s32 ov96_021EE644(void *);

Ov96TextBox *ov96_021EE740(enum HeapID heapId);

void ov96_021EE75C(Ov96TextBox *box, BgConfig *bgConfig, s32 bgLayer, s32 mode, SaveData *saveData);

s32 ov96_021EEA88(void *, s32, s32, enum HeapID);

u16 ov96_021EEBC8(s32);

s32 ov96_021F7878(Window *, BgConfig *, enum HeapID);

s32 ov96_021F78C4(Window *, enum HeapID);

s32 ov96_021F7D10(void);

s32 ov96_021F7D30(BgConfig *, s32);

s32 ov96_021F8094(UnkStruct_ov96_021F8094 *, enum HeapID, BgConfig *, PokeathlonCourseData *);

s32 ov96_021F80A8(UnkStruct_ov96_021F8094 *, PokeathlonCourseData *, u8, s32);

s32 ov96_021F8448(UnkStruct_ov96_021F7934 *);

s32 ov96_021F8528(UnkStruct_ov96_021F7934 *);

s32 ov96_021F85A0(UnkStruct_ov96_021F7934 *);

void *ov96_021F8EB0(enum HeapID, BgConfig *, void *);

s32 ov96_021F8F44(u8 *, s32);

s32 ov96_021F8F94(enum HeapID *, PokeathlonCourseData *, s32);

extern Ov96SpriteMgrParam ov96_0221C2A8;

BOOL ov96_021F7934(PokeathlonCourseData *arg0, int unused);

BOOL ov96_021F7934(PokeathlonCourseData *arg0, int unused) {
    Ov96SpriteMgrParam sp1C;
    s32 spC[4];
    u8 *temp_r0_3;
    u32 temp_r0;
    u32 temp_r6_2;
    u8 *temp_r4_2;
    u16 temp_r6;
    UnkStruct_ov96_021F7934 *temp_r0_2;
    UnkStruct_ov96_021F7934 *temp_r4;
    void *temp_r7;

    temp_r4 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    temp_r0 = (u32)PokeathlonCourse_GetField1ED(arg0);
    switch (temp_r0) {
    case 0:
        Heap_Create(HEAP_ID_POKEATHLON, HEAP_ID_137, 0x40000U);
        Main_SetVBlankIntrCB(NULL, NULL);
        Main_SetHBlankIntrCB(NULL, NULL);
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        *(s32 *)REG_DISPCNT_ADDR &= 0xFFFFE0FF;
        *(s32 *)REG_DB_DISPCNT_ADDR &= 0xFFFFE0FF;
        ov96_021F7D10();
        temp_r0_2 = PokeathlonCourse_AllocPtr4FromHeap(arg0, sizeof(UnkStruct_ov96_021F7934));
        MI_CpuFill8(temp_r0_2, 0U, sizeof(UnkStruct_ov96_021F7934));
        temp_r0_2->heapID = HEAP_ID_137;
        FontID_Alloc(FONT_UNK_4, HEAP_ID_137);
        temp_r0_2->bgConfig = BgConfig_Alloc(HEAP_ID_137);
        temp_r0_2->unk_18 = ov96_021EE740(temp_r0_2->heapID);
        ov96_021F8094(&temp_r0_2->unk_1C, HEAP_ID_137, temp_r0_2->bgConfig, arg0);
        temp_r0_2->unk_80 = ov96_021EE5B4(PokeathlonCourse_GetField3D8_ForCurrentParticipant(arg0), temp_r0_2->heapID);
        ov96_021F8448(temp_r0_2);
        gSystem.screensFlipped = 1;
        GfGfx_SwapDisplay();
        PokeathlonCourse_IncrementField1ED(arg0);
        break;
    case 1:
        sp1C = ov96_0221C2A8;
        temp_r4->unk_10 = ov96_021EB180(temp_r4->heapID, &sp1C);
        ov96_021EB5C8(temp_r4->unk_10, 0, 0, 0, 0x12C000);
        temp_r7 = PokeathlonCourse_GetGraphicsSystem(arg0);
        temp_r6 = ov96_021E9524(temp_r7);
        ov96_021F80A8(&temp_r4->unk_1C, arg0, temp_r6, ov96_021E9528(temp_r7));
        temp_r0_3 = ov96_021F8EB0(temp_r4->heapID, temp_r4->bgConfig, temp_r4->unk_10);
        temp_r4->unk_14 = temp_r0_3;
        ov96_021F8F44(temp_r0_3, 2);
        ov96_021EB3A4(temp_r4->unk_10);
        ov96_021F8F94(temp_r4->unk_14, arg0, 0x12C);
        ov96_021F8528(temp_r4);
        ov96_021F85A0(temp_r4);
        ov96_021EEA88(temp_r4->sprites[0], ov96_021EEBC8(2), 0xB, temp_r4->heapID);
        PokeathlonCourse_IncrementField1ED(arg0);
        break;
    case 2:
        PokeathlonCourse_SetVBlankIntrCB(temp_r4->bgConfig);
        ov96_021F7D30(temp_r4->bgConfig, temp_r4->heapID);
        ov96_021EE60C(temp_r4->unk_80, temp_r4->bgConfig);
        ov96_021F7DA8(temp_r4);
        ov96_021EE644(temp_r4->unk_80);
        GfGfx_EngineBTogglePlanes(4U, 0U);
        GfGfx_EngineBTogglePlanes(8U, 0U);
        temp_r6_2 = PokeathlonCourse_GetMode(arg0);
        ov96_021EE75C(temp_r4->unk_18, temp_r4->bgConfig, 5, temp_r6_2, PokeathlonCourse_GetSaveData(arg0));
        ov96_021EE6A0(temp_r4->unk_80);
        spC[0] = 0x1000;
        spC[1] = 0;
        spC[2] = 0;
        spC[3] = 0x1000;
        OS_WaitVBlankIntr();
        Bg_SetTextDimAndAffineParams(temp_r4->bgConfig, 7U, BG_POS_OP_ADD_ROT, 0, (MtxFx22 *)spC, 0x80, 0x60);
        Bg_SetTextDimAndAffineParams(temp_r4->bgConfig, 7U, BG_POS_OP_SUB_XSCALE, 0x10, (MtxFx22 *)spC, 0x80, 0x60);
        Bg_SetTextDimAndAffineParams(temp_r4->bgConfig, 6U, BG_POS_OP_ADD_ROT, 0, (MtxFx22 *)spC, 0x80, 0x60);
        Bg_SetTextDimAndAffineParams(temp_r4->bgConfig, 6U, BG_POS_OP_SUB_XSCALE, 0x10, (MtxFx22 *)spC, 0x80, 0x60);
        ov96_021F7878(&temp_r4->window, temp_r4->bgConfig, temp_r4->heapID);
        ov96_021F78C4(&temp_r4->window, temp_r4->heapID);
        PokeathlonCourse_IncrementField1ED(arg0);
        break;
    case 3:
        if (ov96_021E5F24(arg0) == 0) {
            temp_r4_2 = ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(arg0) + 0x28);
            *temp_r4_2 = (u8)(MTRandom() % 5U);
        }
        if (PokeathlonCourse_GetMode(arg0) == 1) {
            sub_0203A994(1);
            sub_0203A9C8();
            sub_0203A86C();
        }
        GfGfx_EngineATogglePlanes(0x10U, 1U);
        GfGfx_EngineBTogglePlanes(0x10U, 1U);
        GfGfx_EngineBTogglePlanes(4U, 1U);
        GfGfx_EngineBTogglePlanes(8U, 1U);
        PokeathlonCourse_SetField1F4(arg0, 1);
        PokeathlonCourse_IncrementField1ED(arg0);
        break;
    case 4:
        PlayBGM(0x473U);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, temp_r4->heapID);
        PokeathlonCourse_IncrementField1ED(arg0);
        break;
    case 5:
        if (IsPaletteFadeFinished() != 0) {
            return 1;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}
