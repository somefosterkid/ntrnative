#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "overlay_96_sprite_res_man.h"
#include "overlay_96_unk_struct_ov96_0220e3f0.h"
#include "overlay_96_app_0221040c.h"
#include "error_handling.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "sprite.h"
#include "sprite_system.h"
#include "system.h"
#include "unk_0203A3B0.h"

Sprite **ov96_021E61D8(PokeathlonCourseData *, s32, struct UnkStruct_ov96_021E9A78 *, SpriteList *);

s32 ov96_021E64B8(PokeathlonCourseData *);

s32 ov96_021E6670(PokeathlonCourseData *, s32);

s32 ov96_0220D420(UnkStruct_ov96_0220E3F0_6A8 *, BgConfig *, SpriteSystem *);

s32 ov96_0220E868(void);

s32 ov96_0220EA08(UnkStruct_ov96_0220E3F0 *);

s32 ov96_0220EB3C(UnkStruct_ov96_0220E3F0 *);

s32 ov96_0220ED9C(UnkStruct_ov96_0220E3F0 *);

s32 ov96_0220EE8C(UnkStruct_ov96_0220E3F0 *);

s32 ov96_0220F3FC(UnkStruct_ov96_0220E3F0 *);

s32 ov96_0220F4A0(UnkStruct_ov96_0220E3F0 *);

s32 ov96_0220FA18(u8 (*)[12], PokeathlonCourseData *);

s32 ov96_0220FF64(UnkStruct_ov96_0220E3F0_Score *, UnkStruct_ov96_0220E3F0_Entry *);

s32 ov96_0221007C(UnkStruct_ov96_0220E3F0_Hud *, SpriteSystem *, SpriteManager *, Ov96App_0221040C *);

s32 ov96_0221022C(UnkStruct_ov96_0220E3F0 *);


void *ov96_022107F0(PokeathlonCourseData *, enum HeapID);

BOOL ov96_0220E3F0(PokeathlonCourseData *arg0, int unused);

BOOL ov96_0220E3F0(PokeathlonCourseData *arg0, int unused) {
    u32 temp_r0;
    PokeathlonRecord1 *temp_r0_3;
    UnkStruct_ov96_0220E3F0 *temp_r0_2;
    UnkStruct_ov96_0220E3F0 *temp_r4;
    u8 new_var;
    temp_r4 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    temp_r0 = (u32)PokeathlonCourse_GetField1ED(arg0);
    switch (temp_r0) {
    case 0:
        Heap_Create(HEAP_ID_POKEATHLON, HEAP_ID_142, 0x60000U);
        Main_SetVBlankIntrCB(0L, 0L);
        Main_SetHBlankIntrCB(0L, 0L);
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        reg_GX_DISPCNT &= 0xFFFFE0FF;
        reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
        ov96_0220E868();
        gSystem.screensFlipped = 1;
        GfGfx_SwapDisplay();
        temp_r0_2 = PokeathlonCourse_AllocPtr4FromHeap(arg0, sizeof(UnkStruct_ov96_0220E3F0));
        MI_CpuFill8(temp_r0_2, 0U, sizeof(UnkStruct_ov96_0220E3F0));
        temp_r0_2->heapId = HEAP_ID_142;
        temp_r0_2->course = arg0;
        temp_r0_2->timer = 0x708;
        PokeathlonCourse_IncrementField1ED(arg0);
        break;

    case 1:
        temp_r4->bgConfig = BgConfig_Alloc(temp_r4->heapId);
        ov96_021E6670(arg0, 4);
        ov96_0220ED9C(temp_r4);
        ov96_0220EE8C(temp_r4);
        ov96_0221022C(temp_r4);
        sub_0203A994(2);
        ov96_0220EA08(temp_r4);
        ov96_0220F3FC(temp_r4);
        PokeathlonCourse_IncrementField1ED(arg0);
        break;

    case 2:
        if (ov96_021EAA00(temp_r4->unk_20) != 0) {
            ov96_0220EB3C(temp_r4);
            Sprite_SetDrawPriority(*ov96_021E61D8(arg0, 0, temp_r4->unk_1C, SpriteManager_GetSpriteList(temp_r4->spriteManager)), 0U);
            PokeathlonCourse_IncrementField1ED(arg0);
        }
        break;

    case 3:
        new_var = ov96_021E5F24(arg0);
        temp_r4->unk_C4 = ov96_02210240(temp_r4->spriteSystem, temp_r4->spriteManager, temp_r4->bgConfig, temp_r4->msgData, temp_r4->msgFormat, new_var, temp_r4->heapId, arg0);
        ov96_0221007C(&temp_r4->hud, temp_r4->spriteSystem, temp_r4->spriteManager, temp_r4->unk_C4);
        temp_r4->unk_C8 = ov96_022107F0(arg0, temp_r4->heapId);
        ov96_021E64B8(arg0);
        ov96_0220D420(&temp_r4->unk_6A8, temp_r4->bgConfig, temp_r4->spriteSystem);
        PokeathlonCourse_SetVBlankIntrCB(temp_r4->bgConfig);
        PokeathlonCourse_SetField1F4(arg0, 1);
        ov96_0220F4A0(temp_r4);
        if (ov96_021E5F24(arg0) == 0) {
            temp_r0_3 = ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(arg0) + 0x28);
            temp_r0_3->words[7] = (s32)((0xC0FFFFFF & temp_r0_3->words[7]) | (((u32)((((s32)(temp_r4->timer + 0x1E)) / 30) << 0x1A)) >> 2));
            ov96_0220FA18(temp_r4->hitFlags, arg0);
        }
        ov96_0220FF64(&temp_r4->score, temp_r4->entries);
        GfGfx_EngineATogglePlanes(0x10U, 1U);
        GfGfx_EngineBTogglePlanes(0x10U, 1U);
        return 1;

    default:
        GF_AssertFail();
        break;
    }

    return 0;
}
