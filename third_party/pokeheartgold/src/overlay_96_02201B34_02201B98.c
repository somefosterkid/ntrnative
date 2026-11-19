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
#include "overlay_96_ov96_02203468work.h"
#include "overlay_96_ov96_sprite_mgr.h"

void ov96_021E9C0C(void *arg0);

void ov96_021EA894(void *arg0);

void ov96_02203A30(void *);

void ov96_022043B8(void *);

u32 ov96_02201B98(PokeathlonCourseData *param_1);

u32 ov96_02201B98(PokeathlonCourseData *param_1) {
    Ov96_02203468Work *puVar1;
    Window *window;
    s32 iVar2;

    puVar1 = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    sub_0203A914();
    PokeathlonCourse_ResetField3A4(param_1);
    FreeBgTilemapBuffer(puVar1->bgConfig, 0);
    FreeBgTilemapBuffer(puVar1->bgConfig, 1);
    FreeBgTilemapBuffer(puVar1->bgConfig, 3);
    FreeBgTilemapBuffer(puVar1->bgConfig, 4);
    FreeBgTilemapBuffer(puVar1->bgConfig, 5);
    FreeBgTilemapBuffer(puVar1->bgConfig, 6);
    FreeBgTilemapBuffer(puVar1->bgConfig, 7);
    RemoveWindow(&puVar1->windows[0]);
    iVar2 = 0;
    window = &puVar1->windows[1];
    do {
        RemoveWindow(window);
        iVar2 = iVar2 + 1;
        window = window + 1;
    } while (iVar2 < 3);
    Heap_Free(puVar1->bgConfig);
    ov96_021EB21C(puVar1->unk_48);
    ov96_021EA894(puVar1->unk_5D8);
    ov96_021E9C0C(puVar1->unk_5D4);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    ov96_022043B8(puVar1->unk_5DC);
    ov96_02203A30(puVar1->unk_5E0);
    FontID_Release(FONT_UNK_4);
    Main_SetVBlankIntrCB((GFIntrCB)0x0, (void *)0x0);
    Main_SetHBlankIntrCB((GFIntrCB)0x0, (void *)0x0);
    Heap_Free(puVar1->unk_598);
    PokeathlonCourse_FreePtr4HeapAlloc(param_1);
    ((u8 *)&gSystem)[0x69] = 0;
    GfGfx_SwapDisplay();
    reg_G2_BLDCNT = 0;
    Heap_Destroy(HEAP_ID_146);
    return 1;
}
