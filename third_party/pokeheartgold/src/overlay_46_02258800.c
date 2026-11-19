#include "global.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_manager.h"
#include "sound_02004A44.h"
#include "system.h"
#include "overlay_46_unk_struct_ov67_0225d154.h"

void ov46_02258F70(void *);

s32 ov46_02258F78(void *, s32);

s32 ov46_022592EC(void *, void *, s32, s32, s32, s32, s32, s32, s32, void *, s32);

s32 ov46_022594E0(void *, s32);

s32 ov46_02258800(OverlayManager *arg0, int *state);

s32 ov46_02258800(OverlayManager *arg0, int *state) {
    void *temp_r0;
    void *temp_r0_2;

    Heap_Create(HEAP_ID_3, HEAP_ID_119, 0x20000U);
    temp_r0 = OverlayManager_CreateAndGetData(arg0, 0x404U, HEAP_ID_119);
    memset(temp_r0, 0, 0x404U);
    temp_r0_2 = OverlayManager_GetArgs(arg0);
    *(s32 *)temp_r0 = (s32) *(s32 *)temp_r0_2;
    Sound_SetSceneAndPlayBGM(0xBU, 0x47DU, 0);
    *(s32 *)((u8 *)temp_r0_2 + 0x10) = 0;
    ov46_02258F78(temp_r0, 0x77);
    ov46_022592EC(temp_r0 + 0x40, ((void *)((UnkStruct_ov67_0225D154 *)temp_r0)->unk_0C), 1, 0x30A, 2, 0x13, 0x1B, 4, 0x28, *(void **)temp_r0, 0x77);
    ov46_022592EC(temp_r0 + 0x70, ((void *)((UnkStruct_ov67_0225D154 *)temp_r0)->unk_0C), 1, 0x320, 2, 0x13, 0x1B, 4, 0x28, *(void **)temp_r0, 0x77);
    ov46_022592EC(temp_r0 + 0xA0, ((void *)((UnkStruct_ov67_0225D154 *)temp_r0)->unk_0C), 0, 0x320, 4, 4, 0x17, 0x10, 0x94, *(void **)temp_r0, 0x77);
    ov46_022592EC(temp_r0 + 0x10, ((void *)((UnkStruct_ov67_0225D154 *)temp_r0)->unk_0C), 1, 0x30A, 5, 1, 0x16, 2, 0x204, *(void **)temp_r0, 0x77);
    ov46_022594E0(temp_r0 + 0x10, 0x15);
    Main_SetVBlankIntrCB(ov46_02258F70, temp_r0);
    HBlankInterruptDisable();
    gSystem.screensFlipped = 1;
    GfGfx_SwapDisplay();
    return 1;
}
