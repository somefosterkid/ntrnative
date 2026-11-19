#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "assert.h"
#include "bg_window.h"
#include "error_handling.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "poke_overlay.h"
#include "sprite.h"
#include "sprite_system.h"
#include "system.h"
#include "unk_0203A3B0.h"
#include "overlay_96_sprite_res_man.h"
#include "overlay_96_app_021ed1d0.h"

struct BgConfig;

struct OverlayManager;

struct OverlayManagerTemplate;

struct PlayerProfile;

struct PokeathlonCourseArgs;

struct PokeathlonCourseData;

struct PokeathlonCourseState;

struct PokeathlonFieldData;

struct PokeathlonParticipantData;

struct PokeathlonStateInfo;

struct PokeathlonUnkSubStruct_974;

struct SaveData;

struct Sprite;

struct SpriteList;

struct SpriteManager;

struct SysTask;

struct SysTaskQueue;

struct System;

void ov96_021EB6A8(void);

void *ov96_021EE740(u32 heapId);

void ov96_021EC490(Ov96App_021ED1D0 *work);

UnkStruct_ov96_021E9A78 *ov96_021E9A78(u32 heapId, u32 flags, u32 param2);

void ov96_021E6168(PokeathlonCourseData *course, int quot, int rem, void *out);

PokeathlonMonPerf *ov96_021E60C0(PokeathlonCourseData *course, int quot, int rem);

s32 ov96_021E6108(void *arg0);

void ov96_021EC550(Ov96App_021ED1D0 *work);

void ov96_021EC5C0(Ov96App_021ED1D0 *work);

void ov96_021EC68C(Ov96App_021ED1D0 *work);

void ov96_021EC70C(Ov96App_021ED1D0 *work);

void ov96_021EC82C(PokeathlonCourseData *course);

Ov96PlayerSlot *ov96_021ECBB8(u32 heapId, ManagedSprite **sprites);

void *ov96_021ED054(u32 heapId, SpriteSystem *spriteSystem, SpriteManager *spriteManager);

void ov96_021ECAC4(PokeathlonCourseData *course);

void ov96_021EAF94(Ov96SpriteSetElem *elem, u32 param1, u32 param2);

void ov96_021EC2E8(BgConfig *bgConfig);

void ov96_021EC3D8(BgConfig *bgConfig, u32 heapId);

void ov96_021EE75C(void *textBox, BgConfig *bgConfig, u32 param2, u32 param3, SaveData *param4);

FS_EXTERN_OVERLAY(OVY_98);

BOOL ov96_021ED1D0(PokeathlonCourseData *param0);

BOOL ov96_021ED1D0(PokeathlonCourseData *param0) {
    Ov96App_021ED1D0 *work = PokeathlonCourse_GetHeapAllocPtr4(param0);
    Ov96SpriteResDesc descs[12];
    int i;
    int j;
    int quot;
    int rem;
    Ov96SpriteSetElem *obj;
    u32 *cell;

    switch (PokeathlonCourse_GetField1ED(param0)) {
    case 0:
        Heap_Create((enum HeapID)0x5c, (enum HeapID)0x87, 0x60000);
        HandleLoadOverlay(FS_OVERLAY_ID(OVY_98), (PMOverlayLoadType)2);
        Main_SetVBlankIntrCB(0L, 0L);
        Main_SetHBlankIntrCB(0L, 0L);
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        reg_GX_DISPCNT = reg_GX_DISPCNT & 0xFFFFE0FF;
        reg_GXS_DB_DISPCNT = reg_GXS_DB_DISPCNT & 0xFFFFE0FF;
        ov96_021EB6A8();
        work = PokeathlonCourse_AllocPtr4FromHeap(param0, sizeof(Ov96App_021ED1D0));
        MI_CpuFill8(work, 0, sizeof(Ov96App_021ED1D0));
        work->heapId = (enum HeapID)0x87;
        work->courseId = ov96_021E5E7C(param0);
        work->bgConfig = BgConfig_Alloc(work->heapId);
        work->textBox = ov96_021EE740(work->heapId);
        ov96_021EC490(work);
        work->resSrc = ov96_021E9A78(work->heapId, 6, 1);
        gSystem.screensFlipped = 1;
        GfGfx_SwapDisplay();
        PokeathlonCourse_IncrementField1ED(param0);
        break;
    case 1: {
        Ov96SpriteResLoadParams params = { 0 };

        work->spriteSet = ov96_021EA854(work->heapId, 0xc, 3, work->resSrc, SpriteManager_GetSpriteList(work->spriteManager));
        for (j = 0; j < 12; j++) {
            rem = j % 3;
            quot = j / 3;
            ov96_021E6168(param0, quot, rem, &descs[j]);
            params.unk_14[j] = ov96_021E6108(ov96_021E60C0(param0, quot, rem));
        }
        params.unk_4 = 3;
        params.runToEnd = 0;
        params.unk_8 = 0;
        params.vramType = 1;
        params.screen = NNS_G2D_VRAM_TYPE_2DMAIN;
        ov96_021EA8A8(work->spriteSet, 0xc, descs, &params, 1, 0);
        PokeathlonCourse_IncrementField1ED(param0);
        break;
    }
    case 2:
        if (ov96_021EAA00(work->spriteSet) != 0) {
            ov96_021EC82C(param0);
            ov96_021EC550(work);
            ov96_021EC5C0(work);
            ov96_021EC68C(work);
            ov96_021EC70C(work);
            work->playerSlots = ov96_021ECBB8(work->heapId, work->sprites);
            work->unk_90 = ov96_021ED054(work->heapId, work->spriteSystem, work->spriteManager);
            ov96_021ECAC4(param0);
            PokeathlonCourse_IncrementField1ED(param0);
        }
        break;
    case 3:
        PokeathlonCourse_SetVBlankIntrCB(work->bgConfig);
        PokeathlonCourse_SetField1F4(param0, 1);
        for (i = 0; i < 12; i++) {
            obj = ov96_021EAA04(work->spriteSet, (u8)i);
            ov96_021EAB38(obj, 1);
            cell = (u32 *)Sprite_GetCellAnim(ov96_021E8BAC(ov96_021EAA20(obj)));
            cell[4] = (i * 3) << 12;
            ov96_021EAC0C(obj, 1);
            quot = i / 3;
            rem = i % 3;
            ov96_021EAF94(obj, (quot << 6) + 0x20, 0x30 * (rem + 1));
        }
        ov96_021EC2E8(work->bgConfig);
        ov96_021EC3D8(work->bgConfig, work->heapId);
        ov96_021EE75C(work->textBox, work->bgConfig, 4, PokeathlonCourse_GetMode(param0), PokeathlonCourse_GetSaveData(param0));
        sub_0203A994(2);
        GfGfx_EngineBTogglePlanes(0x10, 1);
        GfGfx_EngineATogglePlanes(0x10, 1);
        work->state = 0;
        return 1;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}
