#include "global.h"

#include "constants/heap.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "error_handling.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "sprite.h"
#include "sprite_system.h"
#include "system.h"
#include "unk_02009D48.h"
#include "unk_0203A3B0.h"
#include "overlay_96_ov96_rd_work.h"

void ov96_02215FA8(void);
void ov96_022163AC(Ov96RdWork *work);
void ov96_0221654C(Ov96RdWork *work);
void ov96_02217AE4(Ov96RdWork *work, PokeathlonCourseData *data);
void *ov96_022194C4(SpriteSystem *spriteSystem, SpriteManager *spriteManager, struct UnkStruct_ov96_021E9A78 *resSrc, BgConfig *bgConfig, struct MsgData *msgData, struct MessageFormat *msgFormat, u8 playerNo, enum HeapID heapId, PokeathlonCourseData *data);
Sprite **ov96_021E61D8(PokeathlonCourseData *, s32, struct UnkStruct_ov96_021E9A78 *, SpriteList *);
s32 ov96_021E6670(PokeathlonCourseData *, s32);
BOOL ov96_021EAA00(struct Ov96SpriteSet *set);
void ov96_0221587C(Ov96RdScroll *scroll, BgConfig *bgConfig, SpriteSystem *spriteSystem);
void ov96_022162F4(Ov96RdWork *work);
void ov96_0221663C(Ov96RdWork *work);
void ov96_02217544(Ov96RdSprites *sprites, SpriteSystem *spriteSystem, SpriteManager *spriteManager, void *unk_180, struct Ov96SpriteSet *spriteSet, enum HeapID heapId);
void ov96_02217B84(Ov96RdWork *work, PokeathlonCourseData *data);
void ov96_02218330(Ov96RdCourse *course, PokeathlonCourseData *data);
void ov96_022193E4(Ov96RdWork *work);
void ov96_022195C8(void *arg0);
void *ov96_02219F7C(PokeathlonCourseData *, enum HeapID);

BOOL ov96_02215AC4(PokeathlonCourseData *arg0, int unused);

BOOL ov96_02215AC4(PokeathlonCourseData *arg0, int unused) {
    u32 temp_r0;
    PokeathlonRecord1 *temp_r0_3;
    u8 temp_r6;
    Ov96RdWork *temp_r0_2;
    Ov96RdWork *temp_r4;

    temp_r4 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    temp_r0 = (u32)PokeathlonCourse_GetField1ED(arg0);
    switch (temp_r0) {
    case 0:
        Heap_Create(HEAP_ID_POKEATHLON, HEAP_ID_145, 0x60000U);
        Main_SetVBlankIntrCB(NULL, NULL);
        Main_SetHBlankIntrCB(NULL, NULL);
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        *(s32 *)REG_DISPCNT_ADDR &= 0xFFFFE0FF;
        *(s32 *)REG_DB_DISPCNT_ADDR &= 0xFFFFE0FF;
        ov96_02215FA8();
        gSystem.screensFlipped = 1;
        GfGfx_SwapDisplay();
        temp_r0_2 = PokeathlonCourse_AllocPtr4FromHeap(arg0, sizeof(Ov96RdWork));
        MI_CpuFill8(temp_r0_2, 0U, sizeof(Ov96RdWork));
        temp_r0_2->heapId = HEAP_ID_145;
        temp_r0_2->unk_438 = 1800;
        PokeathlonCourse_IncrementField1ED(arg0);
        break;
    case 1:
        temp_r4->bgConfig = BgConfig_Alloc(temp_r4->heapId);
        ov96_021E6670(arg0, 8);
        ov96_0221654C(temp_r4);
        ov96_0221663C(temp_r4);
        ov96_022193E4(temp_r4);
        ov96_022162F4(temp_r4);
        ov96_02217AE4(temp_r4, arg0);
        PokeathlonCourse_IncrementField1ED(arg0);
        break;
    case 2:
        if (ov96_021EAA00(temp_r4->spriteSet) == 0) {
        } else {
            ov96_022163AC(temp_r4);
            Sprite_SetDrawPriority(*ov96_021E61D8(arg0, 0, temp_r4->resSrc, SpriteManager_GetSpriteList(temp_r4->spriteManager)), 0U);
            PokeathlonCourse_IncrementField1ED(arg0);
        }
        break;
    case 3:
        temp_r6 = (u8)ov96_021E5F24(arg0);
        temp_r4->unk_184 = ov96_02219F7C(arg0, temp_r4->heapId);
        temp_r4->unk_180 = ov96_022194C4(temp_r4->spriteSystem, temp_r4->spriteManager, temp_r4->resSrc, temp_r4->bgConfig, temp_r4->msgData, temp_r4->msgFormat, temp_r6, temp_r4->heapId, arg0);
        PokeathlonCourse_IncrementField1ED(arg0);
        break;
    case 4:
        ov96_02217B84(temp_r4, arg0);
        ov96_022195C8(temp_r4->unk_180);
        ov96_02217544(&temp_r4->sprites, temp_r4->spriteSystem, temp_r4->spriteManager, temp_r4->unk_180, temp_r4->spriteSet, temp_r4->heapId);
        PokeathlonCourse_SetVBlankIntrCB(temp_r4->bgConfig);
        ov96_0221587C(&temp_r4->scroll, temp_r4->bgConfig, temp_r4->spriteSystem);
        PokeathlonCourse_SetField1F4(arg0, 1);
        if (ov96_021E5F24(arg0) == 0) {
            temp_r0_3 = ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(arg0) + 0x28);
            temp_r0_3->words[8] = (s32)((0xFFF80007 & temp_r0_3->words[8]) | ((u32)(temp_r4->unk_438 << 0x10) >> 0xD));
            ov96_02218330(&temp_r4->course, arg0);
        }
        sub_0203A994(2);
        GfGfx_EngineATogglePlanes(0x10U, 1U);
        GfGfx_EngineBTogglePlanes(0x10U, 1U);
        return 1;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}
