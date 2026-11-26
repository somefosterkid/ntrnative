#include "global.h"

#include "constants/heap.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "error_handling.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "screen_fade.h"
#include "sprite.h"
#include "sprite_system.h"
#include "system.h"
#include "unk_02009D48.h"
#include "unk_0203A3B0.h"
#include "overlay_96_app_0220a87c.h"
#include "overlay_96_ov96_0220_data.h"
#include "overlay_96_ov96rival.h"
#include "overlay_96_sprite_res_man.h"
#include "overlay_96_unk_struct_ov96_021e9a78.h"

PokeathlonMonPerf *ov96_021E60C0(PokeathlonCourseData *arg0, s32 arg1, s32 arg2);

s32 ov96_021E6108(void *arg0);

void ov96_021E6168(PokeathlonCourseData *data, s32 index, s32 arg2, Ov96SpriteResDesc *out);

Sprite **ov96_021E61D8(PokeathlonCourseData *param_1, s32 param_2, UnkStruct_ov96_021E9A78 *param_3, SpriteList *param_4);

void ov96_021E6670(void *arg0, s32 arg1);

void ov96_021EB144(Ov96SpriteSet *set, s32 arg1);

void ov96_02209820(void);

void ov96_02209840(Ov96_0220Data *arg0);

void ov96_02209910(Ov96_0220Data *arg0);

void ov96_02209A14(Ov96_0220Data *param_1);

void ov96_02209B04(SpriteSystem *param_1, SpriteManager *param_2);

void ov96_02209C14(BgConfig *param_1, enum HeapID param_2);

void ov96_02209D14(Ov96_0220Data *arg0);

void ov96_02209DE4(PokeathlonCourseData *param_1);

void ov96_02209F14(Ov96_0220Data *arg0);

void ov96_02209F8C(PokeathlonCourseData *param_1);

void ov96_0220A424(PokeathlonCourseData *param_1);

void ov96_0220A4DC(PokeathlonCourseData *param_1);

Ov96App_0220A87C *ov96_0220A744(enum HeapID heapId, SpriteSystem *spriteSystem, SpriteManager *spriteManager, PokeathlonCourseData *course);

void *ov96_0220B374(enum HeapID arg0, SpriteSystem *arg1, SpriteManager *arg2, PokeathlonCourseData *arg3);

void *ov96_0220B7F4(enum HeapID arg0, void *arg1, void *arg2, PokeathlonCourseData *arg3, Ov96SpriteSet *arg4);

void *ov96_0220C93C(PokeathlonCourseData *data, enum HeapID heapID);

void ov96_0220C9CC(Ov96RivalSet *arg0);

BOOL ov96_02209448(PokeathlonCourseData *param_1, int unused);

BOOL ov96_02209448(PokeathlonCourseData *param_1, int unused) {
    u8 uVar1;
    Ov96_0220Data *data;
    s32 iVar13;
    s32 iVar6;
    SpriteList *spriteList;
    Ov96_0220Data *work;
    Sprite **sprites;
    BOOL BVar10;
    s32 iStack_f0;
    Ov96SpriteResDesc *desc;
    u32 *slot;
    Ov96SpriteResDesc descs[9];
    data = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    uVar1 = PokeathlonCourse_GetField1ED(param_1);
    switch (uVar1) {
    case 0:
        Heap_Create(HEAP_ID_POKEATHLON, HEAP_ID_141, 0x60000);
        Main_SetVBlankIntrCB(NULL, NULL);
        Main_SetHBlankIntrCB(NULL, NULL);
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        reg_GX_DISPCNT = reg_GX_DISPCNT & 0xffffe0ff;
        reg_GXS_DB_DISPCNT = reg_GXS_DB_DISPCNT & 0xffffe0ff;
        ov96_02209820();
        ((u8 *)&gSystem)[0x69] = 1;
        GfGfx_SwapDisplay();
        work = PokeathlonCourse_AllocPtr4FromHeap(param_1, sizeof(Ov96_0220Data));
        MI_CpuFill8(work, 0, sizeof(Ov96_0220Data));
        work->heapId = HEAP_ID_141;
        work->unk_260 = 900;
        PokeathlonCourse_IncrementField1ED(param_1);
        break;
    case 1:
        data->bgConfig = BgConfig_Alloc(data->heapId);
        ov96_021E6670(param_1, 4);
        ov96_02209C14(data->bgConfig, data->heapId);
        ov96_02209DE4(param_1);
        ov96_02209D14(data);
        ov96_02209840(data);
        spriteList = SpriteManager_GetSpriteList(data->spriteManager);
        data->spriteSet = ov96_021EA854(data->heapId, 9, 32, data->resSrc, spriteList);
        PokeathlonCourse_IncrementField1ED(param_1);
        break;
    case 2: {
        u32 params[17] = { 0 };

        iVar6 = 0;
        slot = params;
        iStack_f0 = 0;
        desc = descs;
        do {
            BVar10 = ov96_021E5F24(param_1);
            if (iStack_f0 != BVar10) {
                iVar13 = 0;
                do {
                    if (iVar6 >= 9) {
                        GF_AssertFail();
                    }
                    ov96_021E6168(param_1, iStack_f0, iVar13, desc);
                    slot[5] = ov96_021E6108(ov96_021E60C0(param_1, iStack_f0, iVar13));
                    iVar13 = iVar13 + 1;
                    desc = desc + 1;
                    slot = slot + 1;
                    iVar6 = iVar6 + 1;
                } while (iVar13 < 3);
            }
            iStack_f0 = iStack_f0 + 1;
        } while (iStack_f0 < 4);
        params[1] = 1; // unk_4
        params[3] = 1; // vramType
        params[4] = 1; // screen
        ov96_021EA8A8(data->spriteSet, 9, descs, (Ov96SpriteResLoadParams *)params, 0, 0);
        PokeathlonCourse_IncrementField1ED(param_1);
        break;
    }
    case 3:
        if (ov96_021EAA00(data->spriteSet) != 0) {
            ov96_02209A14(data);
            ov96_02209B04(data->spriteSystem, data->spriteManager);
            spriteList = SpriteManager_GetSpriteList(data->spriteManager);
            sprites = ov96_021E61D8(param_1, 0, data->resSrc, spriteList);
            Sprite_SetDrawPriority(*sprites, 0);
            PokeathlonCourse_IncrementField1ED(param_1);
        }
        break;
    case 4:
        ov96_02209910(data);
        data->unk_40 = ov96_0220A744(data->heapId, data->spriteSystem, data->spriteManager, param_1);
        PokeathlonCourse_IncrementField1ED(param_1);
        break;
    case 5:
        data->unk_4C = ov96_0220B374(data->heapId, data->spriteSystem, data->spriteManager, param_1);
        data->unk_44 = ov96_0220B7F4(data->heapId, data->spriteSystem, data->spriteManager, param_1, data->spriteSet);
        ov96_021EB144(data->spriteSet, 1);
        if (ov96_021E5F24(param_1) == 0) {
            data->unk_48 = ov96_0220C93C(param_1, data->heapId);
        }
        ov96_02209F14(data);
        ov96_02209F8C(param_1);
        PokeathlonCourse_SetVBlankIntrCB(data->bgConfig);
        sub_0203A994(1);
        PokeathlonCourse_SetField1F4(param_1, 1);
        GfGfx_EngineATogglePlanes(0x10, 1);
        GfGfx_EngineBTogglePlanes(0x10, 1);
        if (ov96_021E5F24(param_1) == 0) {
            ov96_0220C9CC(data->unk_48);
        }
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7fff, 6, 1, data->heapId);
        PokeathlonCourse_IncrementField1ED(param_1);
        break;
    case 6:
        if (ov96_021E5F24(param_1) == 0) {
            ov96_0220A424(param_1);
        }
        ov96_0220A4DC(param_1);
        PokeathlonCourse_IncrementField1ED(param_1);
        break;
    case 7:
        if (IsPaletteFadeFinished() != 0) {
            return 1;
        }
        break;
    default:
        GF_AssertFail();
    }
    return 0;
}
