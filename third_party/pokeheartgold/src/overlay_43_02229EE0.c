#include "global.h"

#include "heap.h"
#include "overlay_43.h"
#include "overlay_43_palpad.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "system.h"

void ov43_0222A148(void *param0);

s32 ov43_0222A290(void *, void *, s32);

s32 ov43_0222A48C(void *, void *, s32);

void ov43_0222AE5C(PalPadMenu *, PalPadShared *, PalPadGfx *, enum HeapID);

s32 ov43_0222B5A8(void *, void *, void *, s32);

s32 ov43_0222CBAC(void *, void *, void *, s32);

s32 ov43_0222CD90(void *, void *, void *, s32);

s32 ov43_0222D134(void *, void *, void *, s32);

FS_EXTERN_OVERLAY(OVY_42);

BOOL PalPad_Init(OverlayManager *man, int *state) {
    PalPadWork *work;

    HandleLoadOverlay(FS_OVERLAY_ID(OVY_42), OVY_LOAD_ASYNC);
    Heap_Create(HEAP_ID_3, HEAP_ID_51, 0x70000U);
    work = OverlayManager_CreateAndGetData(man, sizeof(PalPadWork), HEAP_ID_51);
    memset(work, 0, sizeof(PalPadWork));
    work->args = OverlayManager_GetArgs(man);
    ov43_0222A290(&work->shared, work->args, 0x33);
    ov43_0222A48C(&work->gfx, &work->shared, 0x33);
    ov43_0222AE5C(&work->menu, &work->shared, &work->gfx, HEAP_ID_51);
    ov43_0222B5A8(&work->roster, &work->shared, &work->gfx, 0x33);
    ov43_0222CBAC(&work->naming, &work->shared, &work->gfx, 0x33);
    ov43_0222CD90(&work->friendCode, &work->shared, &work->gfx, 0x33);
    ov43_0222D134(&work->details, &work->shared, &work->gfx, 0x33);
    Main_SetVBlankIntrCB(ov43_0222A148, work);
    HBlankInterruptDisable();
    return TRUE;
}
