#include "global.h"

#include "gf_gfx_planes.h"
#include "screen_fade.h"
#include "system.h"
#include "unk_02037C94.h"
#include "overlay_70_gtsapplication_state.h"

typedef struct UnkStruct_ov70_02244038_v0 {
    u8 unk_0[0x20];
    SaveData *unk_20;
    u8 unk_24[0x18];
    s32 unk_3C;
} UnkStruct_ov70_02244038_v0;

s32 ov70_02238D84(void *, s32, s32);

s32 ov70_02238F64(GTSApplicationState *);

s32 ov70_022441A4(BgConfig *);

s32 ov70_022442B4(void *);

s32 ov70_0224458C(void *);

s32 ov70_02244670(GTSApplicationState *arg0);

s32 ov70_02244FA4(void *, MsgData *, s32, s32, s32);

SysTask *ov70_02245124(void *);

s32 ov70_02244038(void *arg0);

s32 ov70_02244038(void *arg0) {
    UnkStruct_ov70_02244038_v0 *v0;

    ov70_02244670(arg0);
    *(u16 *)REG_POWCNT_ADDR &= 0xFFFF7FFF;
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, HEAP_ID_61);
    ov70_022441A4(*((void **)&((GTSApplicationState *)arg0)->bgConfig));
    ov70_022442B4(arg0);
    ov70_0224458C(arg0);
    GfGfx_EngineATogglePlanes(1U, 1U);
    GfGfx_EngineATogglePlanes(2U, 1U);
    GfGfx_EngineATogglePlanes(4U, 0U);
    GfGfx_EngineATogglePlanes(8U, 0U);
    GfGfx_EngineBTogglePlanes(1U, 1U);
    GfGfx_EngineBTogglePlanes(2U, 1U);
    GfGfx_EngineBTogglePlanes(4U, 0U);
    GfGfx_EngineBTogglePlanes(8U, 0U);
    if (DWC_CheckInet() == 0) {
        v0 = *(void **)arg0;
        if (v0->unk_3C != 0) {
            sub_02039418(v0->unk_20);
            ov70_02244FA4(arg0, ((GTSApplicationState *)arg0)->connProgressMessageLoader, 1, 1, 0xF0F);
            ov70_02238D84(arg0, 0xC, 2);
            ov70_02238F64(arg0);
        } else {
            Sys_ClearSleepDisableFlag(4);
            *((s32 *)&((GTSApplicationState *)arg0)->currentScreenInstruction) = 0;
        }
    } else {
        sub_02039418(*(SaveData **)((u8 *)(*(void **)arg0) + 0x20));
        ov70_02245124(arg0);
        *((s32 *)&((GTSApplicationState *)arg0)->currentScreenInstruction) = 0x11;
    }
    return 2;
}
