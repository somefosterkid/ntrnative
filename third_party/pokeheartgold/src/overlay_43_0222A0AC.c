#include "global.h"

#include "heap.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "system.h"
#include "overlay_43_palpad.h"

s32 ov43_0222A2F0(PalPadShared *, struct PalPadArgs *);

void ov43_0222A520(PalPadGfx *);

SysTask *ov43_0222B098(PalPadMenu *, PalPadShared *, PalPadGfx *);

SysTask *ov43_0222B93C(PalPadRoster *, PalPadShared *, PalPadGfx *);

SysTask *ov43_0222CD34(PalPadNaming *, PalPadShared *, PalPadGfx *);

SysTask *ov43_0222CE0C(PalPadFriendCode *, PalPadShared *, PalPadGfx *);

SysTask *ov43_0222D228(PalPadDetails *, PalPadShared *, PalPadGfx *);

FS_EXTERN_OVERLAY(OVY_42);

s32 PalPad_Exit(OverlayManager *man, s32 *state);

void ov43_0222A50C(PalPadGfx *gfx);
void ov43_0222A148(void *param0);

s32 PalPad_Exit(OverlayManager *man, s32 *state) {
    PalPadWork *work;

    work = OverlayManager_GetData(man);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    ov43_0222B098(&work->menu, &work->shared, &work->gfx);
    ov43_0222B93C(&work->roster, &work->shared, &work->gfx);
    ov43_0222CD34(&work->naming, &work->shared, &work->gfx);
    ov43_0222CE0C(&work->friendCode, &work->shared, &work->gfx);
    ov43_0222D228(&work->details, &work->shared, &work->gfx);
    ov43_0222A520(&work->gfx);
    ov43_0222A2F0(&work->shared, work->args);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_51);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_42));
    return 1;
}

void ov43_0222A148(void *param0_) {
    PalPadWork *work = param0_;
    ov43_0222A50C(&work->gfx);
}
