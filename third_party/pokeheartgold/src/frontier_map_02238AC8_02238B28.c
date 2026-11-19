#include "global.h"

#include "frontier/frontier.h"

#include "bg_window.h"
#include "frontier_map.h"
#include "overlay_42.h"
#include "sprite_system.h"
#include "unk_02009D48.h"
#include "frontier_graphics.h"

static void ov80_02238B7C(void *arg0);

s32 ov80_0222ACA0(u8, s32);

void FrontierMap_Scroll(void *arg0);

void FrontierMap_Scroll(void *arg0) {
    FrontierLaunchArgs *frontierLaunchArgs;
    UnkStruct_ov42_022293B8 *v1;
    s32 v0;

    frontierLaunchArgs = Frontier_GetLaunchArgs(((void *)((FrontierGraphics *)arg0)->frontier));
    v0 = ov80_0222ACA0(frontierLaunchArgs->unk20, 0xC);
    if ((v0 == 0) || (v0 != 1)) {
        v1 = ((UnkStruct_ov42_022293B8 *)((FrontierGraphics *)arg0)->unk_24);
        if (v1 != NULL) {
            ov42_02229420(v1, arg0 + 0x1C);
        }
        if ((((UnkStruct_ov42_022293B8 *)((FrontierGraphics *)arg0)->unk_28) != NULL) && (ov80_0222ACA0(frontierLaunchArgs->unk20, 0xD) == 1)) {
            ov42_02229420(((UnkStruct_ov42_022293B8 *)((FrontierGraphics *)arg0)->unk_28), arg0 + 0x1C);
        }
    } else {
        ov80_02238B7C(arg0);
    }
}

static void ov80_02238B7C(void *arg0) {
    s32 x;
    G2dRenderer *g2dRenderer;
    f32 y;
    f32 v0;
    FrontierLaunchArgs *frontierLaunchArgs;
    G2dRenderer *renderer;
    s16 value;
    s16 v1;

    frontierLaunchArgs = Frontier_GetLaunchArgs(((void *)((FrontierGraphics *)arg0)->frontier));
    value = ((FrontierGraphics *)arg0)->unk_A4.unk_06 + ov42_022293A8(arg0 + 0x1C);
    v1 = ((FrontierGraphics *)arg0)->unk_A4.unk_04 + ov42_022293B0(arg0 + 0x1C);
    if ((s32)value > 0) {
        y = 0.5f + (f32)(value << 0xC);
    } else {
        y = (f32)(value << 0xC) - 0.5f;
    }
    if ((s32)v1 > 0) {
        v0 = 0.5f + (f32)(v1 << 0xC);
    } else {
        v0 = (f32)(v1 << 0xC) - 0.5f;
    }
    renderer = SpriteSystem_GetRenderer(((FrontierGraphics *)arg0)->spriteSystem);
    g2dRenderer = renderer;
    x = (s32)v0;
    G2dRenderer_SetMainSurfaceCoords(renderer, x, (s32)y);
    ScheduleSetBgPosText(*(BgConfig **)arg0, 3U, BG_POS_OP_SET_ROT, (s32)v1);
    ScheduleSetBgPosText(*(BgConfig **)arg0, 3U, BG_POS_OP_SET_XSCALE, (s32)value);
    if ((ov80_0222ACA0(frontierLaunchArgs->unk20, 9) != 0xFFFF) && (ov80_0222ACA0(frontierLaunchArgs->unk20, 0xD) == 1)) {
        ScheduleSetBgPosText(*(BgConfig **)arg0, 2U, BG_POS_OP_SET_ROT, (s32)v1);
        ScheduleSetBgPosText(*(BgConfig **)arg0, 2U, BG_POS_OP_SET_XSCALE, (s32)value);
    }
}
