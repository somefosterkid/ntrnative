#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "system.h"
#include "unk_0200B150.h"
#include "unk_0203A3B0.h"
#include "overlay_96_ov96_mon_icon_work.h"
#include "overlay_96_ov96_rower_work.h"
#include "overlay_96_ov96_sprite_mgr.h"

s32 ov96_021E9C0C(void *);

s32 ov96_021EA894(void *);

SysTask *ov96_02200EEC(void *);

BOOL ov96_021FCE10(PokeathlonCourseData *arg0, int unused);

BOOL ov96_021FCE10(PokeathlonCourseData *data, int unused) {
    Ov96RowerWork *v0;

    v0 = PokeathlonCourse_GetHeapAllocPtr4(data);
    sub_0203A914();
    PokeathlonCourse_ResetField3A4(data);
    FreeBgTilemapBuffer(v0->bgConfig, 0U);
    FreeBgTilemapBuffer(v0->bgConfig, 1U);
    FreeBgTilemapBuffer(v0->bgConfig, 2U);
    FreeBgTilemapBuffer(v0->bgConfig, 3U);
    FreeBgTilemapBuffer(v0->bgConfig, 4U);
    FreeBgTilemapBuffer(v0->bgConfig, 5U);
    FreeBgTilemapBuffer(v0->bgConfig, 6U);
    RemoveWindow(&v0->window);
    Heap_Free(v0->bgConfig);
    ov96_021EB21C(v0->unk18);
    ov96_021EA894(v0->unk3C8);
    ov96_021E9C0C(v0->unk3C4);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    ov96_02200EEC(v0->unk3DC);
    ov96_021FFFE8(v0->unk3E0);
    FontID_Release(FONT_UNK_4);
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    PokeathlonCourse_FreePtr4HeapAlloc(data);
    gSystem.screensFlipped = 0;
    GfGfx_SwapDisplay();
    reg_G2_BLDCNT = 0;
    Heap_Destroy(HEAP_ID_144);
    return 1;
}
