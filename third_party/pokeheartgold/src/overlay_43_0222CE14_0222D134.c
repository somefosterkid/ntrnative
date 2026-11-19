#include "global.h"

#include "save_vars_flags.h"
#include "sys_flags.h"
#include "overlay_43_palpad.h"

void ov43_0222D584(PalPadDetails *, PalPadShared *, PalPadGfx *, s32);

void ov43_0222D134(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, s32 arg3);

void ov43_0222D134(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, s32 arg3) {
    details->hasFrontierPages = Save_VarsFlags_FlypointFlagAction(Save_VarsFlags_Get(shared->saveData), 2U, 0x1BU);
    ov43_0222D584(details, shared, gfx, arg3);
}
