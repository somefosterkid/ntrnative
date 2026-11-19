#include "global.h"
struct SafariAreaCustomizer;

#include "gf_gfx_planes.h"
#include "system.h"
#include "unk_020210A0.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E6D24(SafariAreaCustomizer *param_1);

void ov108_021E6F58(SafariAreaCustomizer *arg0);

void ov108_021E7014(SafariAreaCustomizer *arg0);

void ov108_021E71EC(SafariAreaCustomizer *param_1);

void ov108_021E72A4(SafariAreaCustomizer *arg0);

void ov108_021E730C(SafariAreaCustomizer *arg0);

void ov108_021E7510(SafariAreaCustomizer *arg0);

u32 ov108_021E5D0C(SafariAreaCustomizer *param_1);

u32 ov108_021E6240(SafariAreaCustomizer *param_1);
s32 ov108_021E62B4(SafariAreaCustomizer *arg0);
s32 ov108_021E63B8(SafariAreaCustomizer *arg0);
s32 ov108_021E5D90(SafariAreaCustomizer *arg0);

u32 ov108_021E5D0C(SafariAreaCustomizer *param_1) {
    switch (param_1->unk_8) {
    case 0:
        Main_SetVBlankIntrCB((GFIntrCB)0x0, (void *)0x0);
        HBlankInterruptDisable();
        ov108_021E7510(param_1);
        ov108_021E730C(param_1);
        ov108_021E72A4(param_1);
        ov108_021E71EC(param_1);
        ov108_021E7014(param_1);
        ov108_021E6F58(param_1);
        ov108_021E6D24(param_1);
        break;
    case 2:
        DisableTouchPad();
        reg_GX_DISPCNT = reg_GX_DISPCNT & 0xffffe0ff;
        reg_GXS_DB_DISPCNT = reg_GXS_DB_DISPCNT & 0xffffe0ff;
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        param_1->unk_8 = 0;
        return 1;
    }
    param_1->unk_8 = param_1->unk_8 + 1;
    return 0;
}

s32 ov108_021E5D90(SafariAreaCustomizer *arg0) {
    if (ov108_021E6240(arg0) != 0) {
        return 0;
    }
    if (arg0->inTouchMode == 0) {
        return ov108_021E62B4(arg0);
    }
    return ov108_021E63B8(arg0);
}
