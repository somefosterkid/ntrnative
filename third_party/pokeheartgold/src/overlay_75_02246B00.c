#include "global.h"
struct UnkStruct_ov98_02246E88;

#include "constants/heap.h"

#include "heap.h"
#include "main.h"
#include "overlay_75.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "title_screen.h"
#include "poke_overlay.h"
#include "ptr_align.h"
#include "system.h"
#include "unk_02034B0C.h"
#include "unk_02037C94.h"
#include "unk_020915B0.h"
#include "overlay_75_unk_struct_ov98_02246_e88.h"

s32 ov75_02246B98(UnkStruct_ov98_02246E88 *param0);

FS_EXTERN_OVERLAY(OVY_70);
void ov75_02246B48(struct UnkStruct_ov98_02246E88 *arg0);

s32 ov75_02246B00(OverlayManager *man) {
    void *v0;

    v0 = OverlayManager_GetData(man);
    ov75_02246B98(v0);
    String_Delete(((UnkStruct_ov98_02246E88 *)v0)->unk_118);
    String_Delete(((UnkStruct_ov98_02246E88 *)v0)->unk_114);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_115);
    Heap_Destroy(HEAP_ID_89);
    RegisterMainOverlay(FS_OVERLAY_ID(intro_title), &gApplication_TitleScreen);
    return 1;
}

void ov75_02246B48(UnkStruct_ov98_02246E88 *arg0) {
    void *v0;

    if (*((s32 *)&arg0->unk_80) == 0) {
        HandleLoadOverlay(FS_OVERLAY_ID(OVY_70), OVY_LOAD_ASYNC);
        LoadDwcOverlay();
        LoadOVY38();
        sub_02039FD8(HEAP_ID_115);
        v0 = Heap_Alloc(HEAP_ID_115, 0x20020U);
        arg0->unk_0C = v0;
        arg0->unk_10 = NNS_FndCreateExpHeapEx(PTR_ALIGN_UP(v0, 0x20), 0x20000U, 0U);
        sub_02034D8C();
        Sys_ClearSleepDisableFlag(4);
    }
}
