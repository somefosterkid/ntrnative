#include "global.h"

#include "bg_window.h"
#include "field_system.h"
#include "gf_gfx_planes.h"
#include "overlay_01_021FC05C.h"
#include "unk_02005D10.h"

void ov01_021FC14C(BgConfig *);

void ov01_021FC1A4(LinearInterpolationTaskS32 *, u32, u32, u32);

void ov01_021FC05C(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data) {
    HoneyScentFadeData *fade = data;

    ov01_021FC14C(fieldSystem->bgConfig);
    ov01_021FC1A4(&fade->lerp, 0, 10, 0x13);
    fade->done = 0;
    G2_SetBlendAlpha(4, 0x29, 0, 0x10);
    SetBgPriority(2, 0);
    GfGfx_EngineATogglePlanes(4, 1);
    PlaySE(0x647);
    return;
}
