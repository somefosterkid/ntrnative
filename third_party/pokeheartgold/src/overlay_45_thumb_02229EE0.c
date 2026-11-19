#include "global.h"

#include "heap.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "sys_task_api.h"
#include "system.h"
#include "unk_02037C94.h"
#include "unk_020915B0.h"

void ov45_02229FE0(SysTask *task, void *data);

void *ov45_02229FF4(void *, s32);

void *ov45_0222CD1C(s32, void *, void *, void *, s32);

s32 ov45_0222CD84(void *);

FS_EXTERN_OVERLAY(OVY_42);

s32 ov45_02229EE0(OverlayManager *arg0);

s32 ov45_0222A15C(void *);
s32 ov45_0222CD90(void *);
s32 ov45_02229F70(OverlayManager *arg0);
s32 ov45_0222A0F0(void *);
s32 ov45_0222CD68(void *);
s32 ov45_02229F94(OverlayManager *arg0);

s32 ov45_02229EE0(OverlayManager *man) {
    void *v2;
    u32 v3;
    s8 *v0;
    s8 *v4;
    void *v1;

    HandleLoadOverlay(FS_OVERLAY_ID(OVY_42), OVY_LOAD_ASYNC);
    LoadDwcOverlay();
    LoadOVY38();
    sub_02039FD8(HEAP_ID_3);
    Heap_Create(HEAP_ID_3, HEAP_ID_111, 0x5000U);
    v0 = OverlayManager_CreateAndGetData(man, 0x10U, HEAP_ID_111);
    v4 = v0;
    v3 = 0x10;
    do {
        *v4 = 0;
        v4 += 1;
        v3 -= 1;
    } while (v3 != 0);
    v1 = OverlayManager_GetArgs(man);
    v2 = *(void **)((u8 *)v1 + 4);
    *(void **)v0 = v2;
    *(void **)((u8 *)v0 + 4) = ov45_02229FF4(v2, 0x6F);
    *(void **)((u8 *)v0 + 8) = ov45_0222CD1C(*(s32 *)((u8 *)v1 + 8), *(void **)((u8 *)v1 + 4), *(void **)v1, *(void **)((u8 *)v0 + 4), 0x6F);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    *(SysTask **)((u8 *)v0 + 0xC) = SysTask_CreateOnVWaitQueue(ov45_02229FE0, v0, 0U);
    ov45_0222CD84(*(void **)((u8 *)v0 + 8));
    return 1;
}

s32 ov45_02229F70(OverlayManager *man) {
    s32 v1;
    void *v0;

    v0 = OverlayManager_GetData(man);
    v1 = ov45_0222CD90(*(void **)((u8 *)v0 + 8));
    ov45_0222A15C(*(void **)((u8 *)v0 + 4));
    if (v1 == 1) {
        return 1;
    }
    return 0;
}

s32 ov45_02229F94(OverlayManager *man) {
    void *v0;

    v0 = OverlayManager_GetData(man);
    SysTask_Destroy(*(SysTask **)((u8 *)v0 + 0xC));
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    ov45_0222CD68(*(void **)((u8 *)v0 + 8));
    ov45_0222A0F0(*(void **)((u8 *)v0 + 4));
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_111);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_42));
    UnloadOVY38();
    UnloadDwcOverlay();
    return 1;
}
