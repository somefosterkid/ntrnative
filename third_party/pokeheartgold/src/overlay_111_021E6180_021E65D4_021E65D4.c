#include "global.h"

#include "bg_window.h"
#include "error_handling.h"
#include "overlay_111.h"
#include "sprite_system.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov111_021E65D4(SwapMonPanel *panel) {
    s16 pos[2]; // [0]: y, [1]: x
    BgConfig *bgConfig;
    u8 bgId;
    Window *window;
    s16 temp_r0;
    s16 temp_r1;
    s16 temp_r2;
    s32 var_r0;
    u32 step;
    s32 i;
    ManagedSprite **sprite;

    if (panel == NULL) {
        GF_AssertFail();
    }
    window = &panel->windows[0];
    bgConfig = GetWindowBgConfig(window);
    bgId = GetWindowBgId(window);
    temp_r1 = panel->y;
    temp_r2 = panel->targetY;
    var_r0 = temp_r1 - temp_r2;
    if (var_r0 < 0) {
        var_r0 = 0 - var_r0;
    }
    temp_r0 = (s16)var_r0;
    step = temp_r0 >> 2;
    if (temp_r0 <= 1 || step == 0) {
        panel->sliding = 0;
    } else {
        if (temp_r1 > temp_r2) {
            step = (s16)(-1 * step);
        }
        panel->y = (s16)(panel->y + step);
        if (panel->y < 0xE8) {
            ScheduleSetBgPosText(bgConfig, bgId, BG_POS_OP_SUB_Y, step);
            i = 0;
            sprite = panel->sprites;
            do {
                ManagedSprite_GetPositionXYWithSubscreenOffset(*sprite, &pos[1], &pos[0], FX32_CONST(524));
                pos[0] += step;
                ManagedSprite_SetPositionXYWithSubscreenOffset(*sprite, pos[1], pos[0], FX32_CONST(524));
                sprite++;
                i++;
            } while (i < 2);
        }
    }
    BgCommitTilemapBufferToVram(bgConfig, bgId);
}
#else
void ov111_021E65D4(SwapMonPanel *arg0) {
    s16 sp8;
    s16 *new_var2;
    BgConfig *sp4;
    u8 sp0;
    Window *temp_r4;
    s16 temp_r0;
    s16 temp_r1;
    s16 temp_r2;
    s16 *new_var;
    s32 var_r0;
    u32 var_r6;
    s32 var_r7;
    void *var_r5;
    var_r5 = arg0;
    if (var_r5 == 0L) {
        GF_AssertFail();
    }
    temp_r4 = var_r5 + 0xC;
    sp4 = GetWindowBgConfig(temp_r4);
    sp0 = GetWindowBgId(temp_r4);
    temp_r1 = *((u16 *)(((u8 *)var_r5) + 0x74));
    temp_r2 = *((s16 *)(((u8 *)var_r5) + 0x70));
    var_r0 = temp_r1 - temp_r2;
    if (var_r0 < 0) {
        var_r0 = 0 - var_r0;
    }
    temp_r0 = (s16)var_r0;
    var_r6 = temp_r0 >> 2;
    if ((((s32)temp_r0) <= 1) || (var_r6 == 0)) {
        *((s32 *)(((u8 *)var_r5) + 0x6C)) = 0;
    } else {
        if (((s32)temp_r1) > ((s32)temp_r2)) {
            var_r6 = (s32)((s16)((-1) * var_r6));
        }
        *((s16 *)(((u8 *)var_r5) + 0x74)) = (s16)((*((s16 *)(((u8 *)var_r5) + 0x74))) + var_r6);
        new_var2 = &sp8;
        if (((s32)(*((s16 *)(((u8 *)var_r5) + 0x74)))) < 0xE8) {
            ScheduleSetBgPosText(sp4, (u8)sp0, BG_POS_OP_SUB_Y, var_r6);
            var_r7 = 0;
            new_var = &sp8;
            do {
                ManagedSprite_GetPositionXYWithSubscreenOffset(*((ManagedSprite **)(((u8 *)var_r5) + 4)), (&sp8) + 1, new_var2, (fx32)((524 > 0) ? ((524 * (1 << 12)) + 0.5f) : ((524 * (1 << 12)) - 0.5f)));
                *((u16 *)new_var) = (*((s16 *)new_var)) + ((s32)var_r6);
                ManagedSprite_SetPositionXYWithSubscreenOffset(*((ManagedSprite **)(((u8 *)var_r5) + 4)), *((s16 *)(((u8 *)new_var) + 2)), (s32)(*((s16 *)new_var)), (fx32)((524 > 0) ? ((524 * (1 << 12)) + 0.5f) : ((524 * (1 << 12)) - 0.5f)));
                var_r5 += 4;
                var_r7 += 1;
            } while (var_r7 < 2);
        }
    }
    BgCommitTilemapBufferToVram(sp4, (u8)sp0);
}
#endif
