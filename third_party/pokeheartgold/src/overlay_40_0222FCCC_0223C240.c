#include "global.h"

#include "gf_gfx_planes.h"
#include "overlay_40.h"
#include "touch_hitbox_controller.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

s32 ov40_0223C240(void *arg0) {
    TouchHitboxController_IsTriggered(*(TouchHitboxController **)((u8 *)(*(void **)((u8 *)arg0 + 0x860)) + 0x188));
    return 0;
}

s32 ov40_0222BF80(void *, s32);

s32 ov40_0222DA00(void *, void *, s32, s32);

s32 ov40_0222E79C(void *, void *);

s32 ov40_0222E7B8(void *, void *);

s32 ov40_0222E7DC(void *, s32);

s32 ov40_02230410(void *);

s32 ov40_02230638(void *, void *);

s32 ov40_0223064C(void *, void *);

s32 ov40_022306A0(void *, s32);

s32 ov40_022307DC(void *, s32, s32);

s32 ov40_02230964(void *, s32);

s32 ov40_0223CCBC(void *, s32);

s32 ov40_0223C258(UnkStruct_0208C06C *arg0) {
    s32 v1;
    u32 v0;
    void *v2;

    v2 = arg0->unk_860;
    v0 = (u32)arg0->unk_08;
    switch (v0) {
    case 0:
        ov40_02230964(arg0, 1);
        if (*(s32 *)((u8 *)v2 + 0x10) == 0) {
            ov40_0223064C(v2 + 0x220, arg0);
        } else {
            ov40_0222E7B8(v2 + 0x194, arg0);
        }
        GfGfx_EngineATogglePlanes(4U, 0U);
        ov40_02230964(arg0, 0);
        arg0->unk_08 = (u32)((u32)arg0->unk_08 + 1);
        break;
    case 1:
        if (ov40_0222DA00(v2, v2 + 4, 1, 1) != 0) {
            arg0->unk_08 = (u32)((u32)arg0->unk_08 + 1);
        }
        break;
    case 2:
        ov40_02230964(arg0, 1);
        if (*(s32 *)((u8 *)v2 + 0x10) == 0) {
            ov40_0222E79C(v2 + 0x194, arg0);
            ov40_0222E7DC(v2 + 0x194, 0);
            ov40_022307DC(arg0, 0x50, 3);
        } else {
            ov40_02230638(v2 + 0x220, arg0);
            ov40_022306A0(v2 + 0x220, 0);
            ov40_022307DC(arg0, ov40_02230410(v2 + 0x220), 3);
        }
        ov40_02230964(arg0, 0);
        arg0->unk_08 = (u32)((u32)arg0->unk_08 + 1);
        break;
    case 3:
        if (ov40_0222DA00(v2, v2 + 4, 0, 1) != 0) {
            if (*(s32 *)((u8 *)v2 + 0x10) == 0) {
                ov40_0222E7DC(v2 + 0x194, 1);
            } else {
                ov40_022306A0(v2 + 0x220, 1);
            }
            GfGfx_EngineBTogglePlanes(4U, 1U);
            GfGfx_EngineATogglePlanes(4U, 1U);
            arg0->unk_08 = (u32)((u32)arg0->unk_08 + 1);
        }
        break;
    default:
        v1 = *(s32 *)((u8 *)v2 + 0x10) ^ 1;
        *(s32 *)((u8 *)v2 + 0x10) = v1;
        ov40_0223CCBC(arg0, v1 + 0x79);
        ov40_0222BF80(arg0, 7);
        break;
    }
    return 0;
}
