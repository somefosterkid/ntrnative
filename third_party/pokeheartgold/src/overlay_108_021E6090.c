#include "global.h"

#include "gf_gfx_planes.h"
#include "safari_zone.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E6850(SafariAreaCustomizer *arg0);

void ov108_021E7BB4(SafariAreaCustomizer *arg0, u8 arg1, u8 arg2);

s32 ov108_021E82E0(SafariAreaCustomizer *arg0, s32 arg1);

s32 ov108_021E84F8(void *, s32, s32);

s32 ov108_021E6090(SafariAreaCustomizer *arg0);

void ov108_021E78F4(SafariAreaCustomizer *arg0, s32 arg1, u32 arg2);
void ov108_021E61E8(SafariAreaCustomizer *arg0);

s32 ov108_021E6090(SafariAreaCustomizer *arg0) {
    s32 v0;
    s32 v5;
    u8 v1;
    u8 v2;
    u8 v3;
    u8 v4;

    v5 = arg0->unk_184E2_2 * 6;
    v0 = arg0->unk_8;
    switch (v0) {
    case 0:
        ManagedSprite_SetPriority(arg0->sprites[(arg0->areaIdx + v5)], 0);
        ManagedSprite_SetPriority(arg0->sprites[(arg0->cursor + v5)], 0);
        GfGfx_EngineBTogglePlanes(0x10U, 0U);
        arg0->unk_8 = (s32)(arg0->unk_8 + 1);
        break;
    case 1:
        if (ov108_021E82E0(arg0, 0) != 0) {
            arg0->areaChanged = 1;
            SafariZone_SwapAreasInSet(&arg0->areaSet, (u32) arg0->areaIdx, (u32) arg0->cursor);
            v3 = arg0->areaIdx;
            ov108_021E84F8(arg0->unk_348, (u8)(v3 + v5), arg0->areaSet.areas[v3].area_no);
            v1 = arg0->areaIdx;
            ov108_021E7BB4(arg0, v1, arg0->areaSet.areas[v1].area_no);
            v4 = arg0->cursor;
            ov108_021E84F8(arg0->unk_348, (u8)(v4 + v5), arg0->areaSet.areas[v4].area_no);
            v2 = arg0->cursor;
            ov108_021E7BB4(arg0, v2, arg0->areaSet.areas[v2].area_no);
            arg0->unk_8 = (s32)(arg0->unk_8 + 1);
        }
        break;
    case 2:
        if (ov108_021E82E0(arg0, 1) != 0) {
            ManagedSprite_SetPriority(arg0->sprites[(arg0->areaIdx + v5)], 2);
            ManagedSprite_SetPriority(arg0->sprites[(arg0->cursor + v5)], 2);
            GfGfx_EngineBTogglePlanes(0x10U, 1U);
            arg0->unk_8 = (s32)(arg0->unk_8 + 1);
        }
        break;
    default:
        arg0->areaIdx = (u8) arg0->cursor;
        ov108_021E6850(arg0);
        arg0->unk_8 = 0;
        arg0->unk_C = 2;
        return 4;
    }
    return 3;
}

void ov108_021E61E8(SafariAreaCustomizer *arg0) {
    u32 v0;

    v0 = arg0->unk_184E2_3;
    switch (v0) {
    case 0:
        ov108_021E78F4(arg0, 0, arg0->areaIdx);
        return;
    case 1:
        ov108_021E78F4(arg0, 1, arg0->cursor);
        ov108_021E78F4(arg0, 2, arg0->areaIdx);
        return;
    case 2:
        ov108_021E78F4(arg0, 0, arg0->cursor);
        return;
    }
}
