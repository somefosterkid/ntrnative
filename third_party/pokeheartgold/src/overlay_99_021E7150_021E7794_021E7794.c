#include "global.h"

#include "gf_gfx_planes.h"

s32 ov99_021E7198(void *, u32);

u16 ov99_021E71B0(void *);

s32 ov99_021E7258(void *, u32);

s32 ov99_021E76D8(void *, u32);

s32 ov99_021E7758(void *, u32);

void ov99_021E7794(void *arg0);

void ov99_021E7794(void *arg0) {
    u32 v0;

    v0 = ov99_021E71B0(arg0);
    if (v0 > 0x1EDU) {
        v0 = 0;
    }
    ov99_021E76D8(arg0, v0);
    ov99_021E7758(arg0, v0);
    ov99_021E7258(arg0, v0);
    GfGfx_EngineBTogglePlanes(4U, ov99_021E7198(arg0, v0));
}
