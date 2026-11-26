#include "global.h"

#include "system.h"
#include "overlay_43_palpad.h"
#include "overlay_43_palpad_fns.h"

void ov43_0222A520(PalPadGfx *);

s32 ov43_0222B1D8(PalPadMenu *, PalPadShared *, PalPadGfx *);

s32 ov43_0222CD40(PalPadNaming *, PalPadShared *, PalPadGfx *);

s32 ov43_0222D008(PalPadFriendCode *, PalPadShared *, PalPadGfx *);

s32 ov43_0222D610(PalPadDetails *, PalPadShared *, PalPadGfx *);

void ov43_0222A1DC(PalPadWork *work);

void ov43_0222A1DC(PalPadWork *work) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    ov43_0222B1D8(&work->menu, &work->shared, &work->gfx);
    ov43_0222BAB8(&work->roster, &work->shared, &work->gfx);
    ov43_0222CD40(&work->naming, &work->shared, &work->gfx);
    ov43_0222D008(&work->friendCode, &work->shared, &work->gfx);
    ov43_0222D610(&work->details, &work->shared, &work->gfx);
    ov43_0222A520(&work->gfx);
}
