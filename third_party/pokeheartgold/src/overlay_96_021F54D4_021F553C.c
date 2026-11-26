#include "global.h"

#include "constants/heap.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "camera.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "system.h"
#include "unk_0200B150.h"
#include "unk_0203A3B0.h"
#include "vram_transfer_manager.h"
#include "overlay_96_unk_struct_ov96_021_f5018.h"
#include "overlay_96_unk_struct_ov96_021_f74a4.h"
#include "overlay_96_ov96_sprite_mgr.h"

void ov96_021E9C0C(void *arg0);

void ov96_021EA894(void *arg0);

void ov96_021F74C8(UnkStruct_ov96_021F74A4 *);

void ov96_021F7738(void *);

BOOL ov96_021F553C(PokeathlonCourseData *param_1, int unused);

BOOL ov96_021F553C(PokeathlonCourseData *param_1, int unused) {
    UnkStruct_ov96_021F5018 *work;
    s32 i;

    work = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    sub_0203A914();
    Camera_Delete(work->unk_190);
    ov96_021F74C8(work->unk_8C);
    Heap_Free(work->unk_58);
    FreeBgTilemapBuffer(work->bgConfig, 2);
    FreeBgTilemapBuffer(work->bgConfig, 1);
    FreeBgTilemapBuffer(work->bgConfig, 3);
    FreeBgTilemapBuffer(work->bgConfig, 4);
    FreeBgTilemapBuffer(work->bgConfig, 5);
    FreeBgTilemapBuffer(work->bgConfig, 6);
    for (i = 0; i < 5; i++) {
        RemoveWindow(&work->windows[i]);
    }
    Heap_Free(work->bgConfig);
    PokeathlonCourse_ResetField3A4(param_1);
    ov96_021EB21C(work->unk_60);
    ov96_021EA894(work->unk_18C);
    ov96_021E9C0C(work->unk_188);
    ov96_021F7738(work->unk_138);
    FontID_Release(FONT_UNK_4);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    Main_SetVBlankIntrCB((GFIntrCB)0x0, (void *)0x0);
    Main_SetHBlankIntrCB((GFIntrCB)0x0, (void *)0x0);
    ((u8 *)&gSystem)[0x69] = 0;
    GfGfx_SwapDisplay();
    sub_0202067C(work->unk_1000);
    GF_DestroyVramTransferManager();
    PokeathlonCourse_FreePtr4HeapAlloc(param_1);
    reg_G2_BLDCNT = 0;
    Heap_Destroy(HEAP_ID_143);
    return 1;
}
