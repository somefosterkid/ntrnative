#include "global.h"

#include "constants/heap.h"

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

#include "overlay_96_app_02210C98.h"
#include "overlay_96_ov96_course_hud.h"
#include "overlay_96_ov96_sprite_mgr.h"

void ov96_021E9C0C(void *arg0);

void ov96_021EA894(void *arg0);


void ov96_022141B0(PokeathlonCourseData *);

void ov96_02214234(Ov96App_02210C98 *);

void ov96_02214B7C(Ov96App_02210C98_Cpu *cpu);

BOOL ov96_022116C8(PokeathlonCourseData *param_1, int unused);

BOOL ov96_022116C8(PokeathlonCourseData *param_1, int unused) {
    Ov96App_02210C98 *app;
    Window *window;
    s32 iVar2;

    app = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    sub_0203A914();
    ov96_021E6550(param_1);
    FreeBgTilemapBuffer(app->unk_4, 0);
    FreeBgTilemapBuffer(app->unk_4, 1);
    FreeBgTilemapBuffer(app->unk_4, 2);
    FreeBgTilemapBuffer(app->unk_4, 3);
    FreeBgTilemapBuffer(app->unk_4, 4);
    FreeBgTilemapBuffer(app->unk_4, 5);
    FreeBgTilemapBuffer(app->unk_4, 6);
    ov96_02214234(app);
    ov96_022141B0(param_1);
    RemoveWindow(&app->unk_8[0]);
    iVar2 = 0;
    window = &app->unk_8[1];
    do {
        RemoveWindow(window);
        iVar2 = iVar2 + 1;
        window = window + 1;
    } while (iVar2 < 4);
    Heap_Free(app->unk_4);
    ov96_021EB21C(app->unk_0);
    ov96_021EA894(app->unk_748);
    ov96_021E9C0C(app->unk_744);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    ov96_02214B7C(app->unk_74C);
    ov96_02214690(app->unk_750);
    FontID_Release(FONT_UNK_4);
    Main_SetVBlankIntrCB((GFIntrCB)0x0, (void *)0x0);
    Main_SetHBlankIntrCB((GFIntrCB)0x0, (void *)0x0);
    Heap_Free(app->unk_81C);
    PokeathlonCourse_FreePtr4HeapAlloc(param_1);
    ((u8 *)&gSystem)[0x69] = 0;
    GfGfx_SwapDisplay();
    *(u16 *)REG_BLDCNT_ADDR = 0;
    Heap_Destroy(HEAP_ID_147);
    return 1;
}
