#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "overlay_64_hall_of_fame_showcase.h"
#include "sprite_system.h"
#include "vram_transfer_manager.h"

void ov64_021E5A88(void *data);

void ov64_021E5AAC(void *arg0, u32 arg1, u32 arg2);
void ov64_021E5AC8(void *arg0, u32 arg1, u32 arg2);
void ov64_021E5AE4(void *arg0, u32 arg1, u32 arg2);

extern GraphicsBanks ov64_021E6FAC;
void ov64_021E5B00(void);

void ov64_021E5A88(void *data) {
    DoScheduledBgGpuUpdates(((HallOfFameShowcase *)data)->bgConfig);
    SpriteSystem_TransferOam();

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

void ov64_021E5AAC(void *arg0, u32 offset, u32 arg2) {
    DC_FlushRange(arg0, arg2);
    GX_LoadOBJ(arg0, offset, arg2);
}

void ov64_021E5AC8(void *arg0, u32 offset, u32 arg2) {
    DC_FlushRange(arg0, arg2);
    GXS_LoadOBJ(arg0, offset, arg2);
}

void ov64_021E5AE4(void *arg0, u32 offset, u32 arg2) {
    DC_FlushRange(arg0, arg2);
    GX_LoadOBJPltt(arg0, offset, arg2);
}

void ov64_021E5B00(void) {
    GfGfx_SetBanks(&ov64_021E6FAC);
}
