#include "global.h"

#include "bg_window.h"
#include "sprite.h"
#include "system.h"
#include "overlay_90_unk_struct_ov114_0225cdb4.h"
#include "overlay_90_unk_struct_ov114_0225fab8.h"

Sprite *ov90_02258EB4(UnkStruct_ov114_0225CDB4 *param0, SpriteList *param1, s32 param2, s32 param3, u16 param4, u32 param5);

void ov90_0225BD84(UnkStruct_ov114_0225FAB8 *param0);

void ov90_0225BE08(void *param0);

extern const VecFx32 ov90_0225C2B4[2];

void ov90_0225BAD0(UnkStruct_ov114_0225FAB8 *param0, UnkStruct_ov114_0225CDB4 *param1, SpriteList *param2, u32 param3);

s32 ov90_0225BC28(void *, s32);
s32 ov90_0225BBD0(UnkStruct_ov114_0225FAB8 *arg0);
s32 ov90_0225BBF0(void *arg0);

void ov90_0225BAD0(UnkStruct_ov114_0225FAB8 *param0, UnkStruct_ov114_0225CDB4 *param1, SpriteList *param2, u32 param3) {
    int v0;
    int v1;

    memset(param0->unk_0C[0], 0, 192);
    memset(param0->unk_0C[1], 0, 192);

    param0->unk_08 = 0;
    param0->unk_0A = 0;
    param0->unk_18C = NULL;

    for (v0 = 0; v0 < 2; v0++) {
        param0->unk_00[v0] = ov90_02258EB4(param1, param2, (s16)(ov90_0225C2B4[v0].x >> FX32_SHIFT), (s16)(ov90_0225C2B4[v0].y >> FX32_SHIFT), 0, param3);
        Sprite_SetAnimCtrlSeq(param0->unk_00[v0], 5);
        Sprite_SetPriority(param0->unk_00[v0], 0);
    }

    v1 = GX_GetVisibleWnd();
    v1 &= ~GX_WNDMASK_W0;

    GX_SetVisibleWnd(v1);
    G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, 1);
    G2_SetWnd0Position(0, 0, 0, 0);

    param0->unk_190 = G2_GetWndOutsidePlane();
    param0->unk_194 = GX_GetVisibleWnd() & GX_WNDMASK_W1;
    param0->battlerType.planeMask = GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3;
    param0->battlerType.effect = 1;

    Main_SetHBlankIntrCB(ov90_0225BE08, param0);
    Sprite_SetFlipMode(param0->unk_00[1], 2);

    ov90_0225BD84(param0);
}

s32 ov90_0225BBD0(UnkStruct_ov114_0225FAB8 *arg0) {
    s32 v0;

    v0 = 0;
    arg0->unk_08 = (s16)(arg0->unk_08 + 1);
    if ((s32) arg0->unk_08 >= 8) {
        arg0->unk_08 = 8;
        v0 = 1;
    }
    ov90_0225BC28(arg0, 1);
    return v0;
}

s32 ov90_0225BBF0(void *arg0) {
    s32 v0;

    v0 = 0;
    ((UnkStruct_ov114_0225FAB8 *)arg0)->unk_08 = (s16)(((UnkStruct_ov114_0225FAB8 *)arg0)->unk_08 - 1);
    if ((s32) ((UnkStruct_ov114_0225FAB8 *)arg0)->unk_08 <= 0) {
        v0 = 1;
        ((UnkStruct_ov114_0225FAB8 *)arg0)->unk_08 = 1;
        Sprite_SetDrawFlag(*(Sprite **)arg0, 0);
        Sprite_SetDrawFlag(((UnkStruct_ov114_0225FAB8 *)arg0)->unk_00[1], 0);
    }
    ov90_0225BC28(arg0, -1);
    return v0;
}
