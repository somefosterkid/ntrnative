#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_96_unk_struct_ov96_021_f7934.h"
#include "system.h"
#include "unk_0203A3B0.h"
#include "overlay_96_ov96_sprite_mgr.h"

s32 ov96_021EE5E0(void *);

s32 ov96_021EE808(void *);

s32 ov96_021F84E4(UnkStruct_ov96_021F7934 *);

s32 ov96_021F8728(void *);

s32 ov96_021F8F0C(void *);

extern u8 ov96_0221C290;

BOOL ov96_021F7C70(PokeathlonCourseData *arg0, int unused);

BOOL ov96_021F7C70(PokeathlonCourseData *arg0, int unused) {
    s32 var_r6;
    UnkStruct_ov96_021F7934 *temp_r4;
    u8 *var_r5;
    temp_r4 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    Main_SetVBlankIntrCB(0L, 0L);
    Main_SetHBlankIntrCB(0L, 0L);
    FontID_Release(FONT_UNK_4);
    G2_SetBlendBrightness(0x3F, 0);
    ov96_021EE5E0(temp_r4->unk_80);
    RemoveWindow(&temp_r4->window);
    ov96_021EE808(temp_r4->unk_18);
    var_r5 = &ov96_0221C290;
    var_r6 = 0;
    do {
        FreeBgTilemapBuffer(temp_r4->bgConfig, *var_r5);
        var_r5 += 1;
        var_r6 += 1;
    } while (var_r6 < 6);
    ov96_021F8F0C(temp_r4->unk_14);
    ov96_021F8728(temp_r4->unk_1C.scene);
    ov96_021EB21C(temp_r4->unk_10);
    ov96_021F84E4(temp_r4);
    Heap_Free(temp_r4->bgConfig);
    PokeathlonCourse_FreePtr4HeapAlloc(arg0);
    gSystem.screensFlipped = 0;
    GfGfx_SwapDisplay();
    Heap_Destroy(HEAP_ID_137);
    sub_0203A914();
    return 1;
}
