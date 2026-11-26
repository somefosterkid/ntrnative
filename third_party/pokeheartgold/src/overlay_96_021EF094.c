#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "screen_fade.h"
#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "poke_overlay.h"
#include "sprite_system.h"
#include "system.h"
#include "unk_0203A3B0.h"
#include "overlay_96_app_021eefac.h"
#include "overlay_96_unk_struct_ov96_021_ef610.h"
#include "overlay_98_ov98text_sys.h"

void ov96_021EE944(Ov96TextBox *box);

void ov96_021EE9BC(Ov96TextBox *box);

BOOL ov96_021EF094(PokeathlonCourseData *arg0, u8 *arg1);

void *ov96_021EE5B4(s32, s32);
s32 ov96_021EE60C(void *, void *);
s32 ov96_021EE644(void *);
s32 ov96_021EF2C0(PokeathlonCourseData *);
s32 ov96_021EF4D0(BgConfig *, s32);
s32 ov96_021EF54C(void *);
s32 ov96_021EF818(void *);
s32 ov96_021EFA28(void *);
BOOL ov96_021EF0FC(PokeathlonCourseData *arg0, int unused);
BOOL ov96_021EF18C(PokeathlonCourseData *arg0, int unused);
s32 ov96_021EE5E0(void *);
FS_EXTERN_OVERLAY(OVY_98);
extern u8 ov96_0221BA18;
BOOL ov96_021EF19C(PokeathlonCourseData *arg0, int unused);

BOOL ov96_021EF094(PokeathlonCourseData *data, u8 *arg1) {
    s32 v1;
    u8 v0;
    Ov96App_021EEFAC *v2;

    v2 = PokeathlonCourse_GetHeapAllocPtr4(data);
    v0 = *arg1;
    switch (v0) {
    case 0:
        v1 = v2->frames + 1;
        v2->frames = v1;
        if (v1 > 0xF) {
            ov96_021EE9BC(v2->textBox);
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 4, 1, v2->heapId);
            *arg1 += 1;
        }
        break;
    case 1:
        if (IsPaletteFadeFinished() != 0) {
            ov96_021EE944(v2->textBox);
            return 1;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}

BOOL ov96_021EF0FC(PokeathlonCourseData *data, int unused) {
    void *v1;
    u8 v0;
    void *v2;

    v2 = PokeathlonCourse_GetHeapAllocPtr4(data);
    v0 = PokeathlonCourse_GetField1ED(data);
    switch (v0) {
    case 0:
        ov96_021EF2C0(data);
        PokeathlonCourse_IncrementField1ED(data);
        break;
    case 1:
        ov96_021EF818(v2);
        ov96_021EF4D0(*((void **)&((UnkStruct_ov96_021EF610 *)v2)->unk_4), *((s32 *)&((UnkStruct_ov96_021EF610 *)v2)->unk_C));
        v1 = ov96_021EE5B4(((UnkStruct_ov96_021EF610 *)v2)->unk_20, *((s32 *)&((UnkStruct_ov96_021EF610 *)v2)->unk_C));
        *((void **)&((UnkStruct_ov96_021EF610 *)v2)->unk_34) = v1;
        ov96_021EE60C(v1, *((void **)&((UnkStruct_ov96_021EF610 *)v2)->unk_4));
        ov96_021EF54C(v2);
        ov96_021EE644(*((void **)&((UnkStruct_ov96_021EF610 *)v2)->unk_34));
        ov96_021EFA28(v2);
        sub_0203A994(2);
        GfGfx_EngineATogglePlanes(0x10U, 1U);
        GfGfx_EngineBTogglePlanes(0x10U, 1U);
        PokeathlonCourse_SetField1F4(data, 1);
        PokeathlonCourse_SetField1ED(data, 0);
        return 1;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}

BOOL ov96_021EF18C(PokeathlonCourseData *data, int unused) {
    SpriteSystem_DrawSprites(*(SpriteManager **)((u8 *)(PokeathlonCourse_GetHeapAllocPtr4(data)) + 0x14));
    return 1;
}

BOOL ov96_021EF19C(PokeathlonCourseData *data, int unused) {
    PokeathlonCourseData *pokeathlonCourseData;
    ManagedSprite *managedSprite;
    PokeathlonCourseData *new_var2;
    s32 v1;
    void *v2;
    u8 *new_var;
    s32 v4;
    u8 *v3;
    void *v0;
    pokeathlonCourseData = data;
    v0 = PokeathlonCourse_GetHeapAllocPtr4(data);
    NARC_Delete(*((NARC **)(((u8 *)v0) + 8)));
    sub_0203A914();
    ov96_021EE5E0(*((void **)(((u8 *)v0) + 0x34)));
    ov98_0221EB84(((UnkStruct_ov96_021EF610 *)v0)->textSys, 0xC);
    v3 = &ov96_0221BA18;
    new_var = (u8 *)v0;
    v1 = 0;
    do {
        FreeBgTilemapBuffer(*((BgConfig **)(((u8 *)v0) + 4)), *v3);
        v1 += 1;
        v3 += 1;
    } while (v1 < 7);
    Heap_Free(*((BgConfig **)(new_var + 4)));
    v4 = 0;
    v2 = v0;
    do {
        managedSprite = *((ManagedSprite **)(((u8 *)v2) + 0x38));
        if (managedSprite != 0L) {
            Sprite_DeleteAndFreeResources(managedSprite);
            *((ManagedSprite **)(((u8 *)v2) + 0x38)) = 0L;
        }
        v4 += 1;
        v2 += 4;
    } while (v4 < 4);
    new_var2 = pokeathlonCourseData;
    SpriteSystem_FreeResourcesAndManager(*((SpriteSystem **)(((u8 *)v0) + 0x10)), *((SpriteManager **)(((u8 *)v0) + 0x14)));
    SpriteSystem_Free(*((SpriteSystem **)(((u8 *)v0) + 0x10)));
    Main_SetVBlankIntrCB(0L, 0L);
    Main_SetHBlankIntrCB(0L, 0L);
    PokeathlonCourse_FreePtr4HeapAlloc(new_var2);
    gSystem.screensFlipped = 0;
    GfGfx_SwapDisplay();
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_98));
    Heap_Destroy(HEAP_ID_136);
    return 1;
}
