#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "gf_gfx_planes.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"

s32 ov14_021E8328(void *);

s32 ov14_021E84A4(void *);

s32 ov14_021E88F8(void *);

s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov14_021F40DC(void *);
#else
s32 ov14_021F40DC(void *);
#endif

s32 ov14_021EA674(UnkStruct_ov14_boxapp *app);

void ov14_021F2930(UnkStruct_ov14_boxapp *arg0);

void ov14_021F2930(UnkStruct_ov14_boxapp *arg0) {
    UnkStruct_ov14_boxsys *v0;

    ov14_021E84A4(((u8 *)arg0->sys->unk_2F0));
    ov14_021E8328(((u8 *)arg0->sys->unk_2F0));
    ov14_021F40DC(arg0);
    v0 = arg0->sys;
    if (v0->unk_88C8 != 0) {
        ov14_021E88F8(v0->unk_2F0);
    }
    ov14_021F0234(arg0, ov14_021EA674, 0x8C);
}

s32 ov14_021F2AC8(UnkStruct_ov14_boxsys *);

s32 ov14_021F2B88(void *);

s32 ov14_021F2BB8(void *);

s32 ov14_021F4D10(UnkStruct_ov14_boxsys *);

void ov14_021F297C(struct UnkStruct_ov14_boxapp *arg0);

void ov14_021F297C(UnkStruct_ov14_boxapp *arg0) {
    GfGfx_EngineATogglePlanes(0x10U, 1U);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
    ov14_021F2AC8(arg0->sys);
    ov14_021F2B88(arg0->sys);
    ov14_021F2BB8(arg0);
    ov14_021F4D10(arg0->sys);
}

s32 ov14_021F2B68(void *);

s32 ov14_021F2C04(void *);

void ov14_021F4E68(UnkStruct_ov14_boxsys *param_1);

void ov14_021F29AC(void *arg0);

void ov14_021F29AC(void *arg0) {
    ov14_021F4E68(arg0);
    ov14_021F2C04(arg0);
    ov14_021F2B68(arg0);
}

void ov14_021F29C4(UnkStruct_ov14_boxsys *arg0);

void ov14_021F29C4(UnkStruct_ov14_boxsys *arg0) {
    ManagedSprite *managedSprite;
    u32 v0;

    v0 = 0;
    do {
        managedSprite = arg0->sprites[v0];
        if (managedSprite != NULL) {
            ManagedSprite_TickFrame(managedSprite);
        }
        v0 += 1;
    } while (v0 < 0xFU);
}

void ov14_021F29E4(UnkStruct_ov14_boxsys *param_1, s32 param_2, s32 param_3);

void ov14_021F29E4(UnkStruct_ov14_boxsys *param_1, s32 param_2, s32 param_3) {
    ManagedSprite_SetAnimationFrame(param_1->sprites[param_2], 0);
    ManagedSprite_SetAnim(param_1->sprites[param_2], param_3);
    return;
}

void ov14_021F2A04(void *param_1, s32 param_2);

void ov14_021F2A04(void *param_1, s32 param_2) {
    ManagedSprite_IsAnimated(((UnkStruct_ov14_boxsys *)param_1)->sprites[param_2]);
    return;
}

void ov14_021F2A18(void *arg0, s32 arg1, s32 arg2);

void ov14_021F2A18(void *arg0, s32 arg1, s32 arg2) {
    if (arg2 == 1) {
        ManagedSprite_SetDrawFlag(((UnkStruct_ov14_boxsys *)arg0)->sprites[arg1], 1);
        return;
    }
    ManagedSprite_SetDrawFlag(((UnkStruct_ov14_boxsys *)arg0)->sprites[arg1], 0);
}

s32 ov14_021F2A44(UnkStruct_ov14_boxsys *arg0, s32 arg1);

s32 ov14_021F2A44(UnkStruct_ov14_boxsys *arg0, s32 arg1) {
    if (ManagedSprite_GetDrawFlag(arg0->sprites[arg1]) == 1) {
        return 1;
    }
    return 0;
}

void ov14_021F2A60(UnkStruct_ov14_boxsys *param_1, s32 param_2, s32 param_3);

void ov14_021F2A60(UnkStruct_ov14_boxsys * param_1, s32 param_2, s32 param_3) {
    ManagedSprite_SetPriority(param_1->sprites[param_2], param_3);
    return;
}

void ov14_021F2A74(void *arg0, s32 arg1, u32 priority);

void ov14_021F2A74(void *arg0, s32 arg1, u32 priority) {
    ((UnkStruct_ov14_boxsys *)arg0)->unk_88D2 = 1;
    ManagedSprite_SetDrawPriority(((UnkStruct_ov14_boxsys *)arg0)->sprites[arg1], priority);
    ((UnkStruct_ov14_boxsys *)arg0)->unk_88D2 = 0;
}

void ov14_021F2A9C(UnkStruct_ov14_boxsys *arg0, s32 arg1, s32 arg2);

void ov14_021F2A9C(UnkStruct_ov14_boxsys * arg0, s32 arg1, s32 arg2) {
    if (arg2 == 1) {
        ManagedSprite_SetOamMode(arg0->sprites[arg1], GX_OAM_MODE_XLU);
        return;
    }
    ManagedSprite_SetOamMode(arg0->sprites[arg1], GX_OAM_MODE_NORMAL);
}
