#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "screen_fade.h"
#include "sprite.h"
#include "sprite_system.h"
#include "system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "bg_window.h"
#include "overlay_99_obj99.h"
#include "overlay_98.h"
#include "overlay_98_ov98text_sys.h"
#include "overlay_99.h"

s32 ov99_021E64E0(s32 *, s32);

extern s32 ov99_021E9574;

extern s32 ov99_021E9576;

extern TouchscreenHitbox ov99_021E95BC;

s32 ov99_021E6638(s32 *arg0);

s32 ov99_021E6638(s32 *arg0) {
    s32 sp0;
    ManagedSprite *temp_r0_2;
    s32 *var_r4;
    s32 *var_r5;
    s32 temp_r2;
    s32 var_r0;
    s32 var_r1;
    s32 var_r7;
    s8 temp_r0;
    s8 var_r1_2;
    s8 var_r6;

    var_r5 = arg0;
    var_r4 = var_r5;
    var_r4 = (void *)((u8 *)var_r4 + (0x80));
    var_r6 = *(s8 *)((u8 *)var_r5 + 0x80);
    sp0 = 0;
    if (2 & gSystem.newKeys) {
        return 1;
    }
    if (gSystem.touchNew != 0) {
        var_r7 = TouchscreenHitbox_FindHitboxAtTouchNew(&ov99_021E95BC);
        if (var_r7 != -1) {
            if (var_r7 == 5) {
                sp0 = 1;
            } else {
                var_r1 = 0;
                if ((s32) * (s8 *)var_r4 < var_r7) {
                    var_r1 = 1;
                }
                *(s32 *)((u8 *)var_r4 + 4) = var_r1;
                ManagedSprite_SetAnim(*(ManagedSprite **)((u8 *)var_r5 + 0x18), 9);
                ManagedSprite_SetAnimateFlag(*(ManagedSprite **)((u8 *)var_r5 + 0x18), 1);
                PlaySE(0x5DCU);
            }
            *(s8 *)var_r4 = (s8)var_r7;
        }
        goto block_20;
    }
    if ((0x10 & gSystem.newAndRepeatedKeys) || (0x80 & gSystem.newAndRepeatedKeys)) {
        if ((s32)var_r6 < 5) {
            *(s8 *)var_r4 = (s8)(*(s8 *)var_r4 + 1);
            *(s32 *)((u8 *)var_r4 + 4) = 1;
            PlaySE(0x5DCU);
        }
        goto block_20;
    }
    if ((0x20 & gSystem.newAndRepeatedKeys) || (0x40 & gSystem.newAndRepeatedKeys)) {
        if ((s32)var_r6 > 0) {
            *(s8 *)var_r4 = (s8)(*(s8 *)var_r4 - 1);
            *(s32 *)((u8 *)var_r4 + 4) = 0;
            PlaySE(0x5DCU);
        }
        goto block_20;
    }
    var_r0 = 1;
    if (!(gSystem.newKeys & 1) || (var_r6 != 5)) {
block_20:
        var_r1_2 = *(s8 *)var_r4;
        if ((s32)var_r1_2 > 5) {
            var_r1_2 = 5;
        } else if ((s32)var_r1_2 < 0) {
            var_r1_2 = 0;
        }
        *(s8 *)var_r4 = var_r1_2;
        *(s32 *)((u8 *)var_r5 + 0x88) = (s32) * (s8 *)var_r4;
        temp_r0 = *(s8 *)var_r4;
        if (var_r6 != temp_r0) {
            temp_r2 = temp_r0 * 4;
            ManagedSprite_SetPositionXY(*(ManagedSprite **)((u8 *)var_r5 + 0x18), *(s16 *)(((u8 *)&ov99_021E9574) + temp_r2), *(s16 *)(((u8 *)&ov99_021E9576) + temp_r2));
            if (*(s8 *)var_r4 == 5) {
                ManagedSprite_SetAnim(*(ManagedSprite **)((u8 *)var_r5 + 0x18), 0xC);
            } else {
                temp_r0_2 = *(ManagedSprite **)((u8 *)var_r5 + 0x18);
                if (ManagedSprite_GetActiveAnim(temp_r0_2) != 9) {
                    ManagedSprite_SetAnim(*(ManagedSprite **)((u8 *)var_r5 + 0x18), 8);
                }
                if ((var_r6 != 5) || (*(s8 *)var_r4 != 4)) {
                    ov99_021E64E0(var_r5, 0);
                    var_r5 = (void *)((u8 *)var_r5 + (0x90));
                    *var_r5 = 1;
                }
            }
        }
        var_r0 = sp0;
    }
    return var_r0;
}

FS_EXTERN_OVERLAY(OVY_98);

extern const SpriteResourceCountsListUnion ov99_021E9DEC;

extern WindowTemplate ov99_021E9ED0[];

BOOL ov99_021E677C(OverlayManager *man, int *state) {
    SpriteResourceCountsListUnion counts;
    UnkStruct_0203EFA0 *args;
    Obj99 *obj;

    HandleLoadOverlay(FS_OVERLAY_ID(OVY_98), OVY_LOAD_ASYNC);
    args = OverlayManager_GetArgs(man);
    if (args == NULL) {
        GF_AssertFail();
    }
    Heap_Create(HEAP_ID_3, HEAP_ID_132, 0x30000U);
    obj = OverlayManager_CreateAndGetData(man, sizeof(Obj99), HEAP_ID_132);
    MI_CpuFill8(obj, 0U, sizeof(Obj99));
    obj->heap = HEAP_ID_132;
    ov99_021E6FD0(obj, args);
    ov98_0221F090();
    ov99_021E695C(obj);
    ov99_021E69D8(obj);
    obj->textSys = ov98_0221EABC(obj->heap, obj->bgConfig, 0xF, ov99_021E9ED0, 0x13A);
    counts = ov99_021E9DEC;
    obj->spriteContext = ov98_0221E5E0(obj->heap, &counts, 0x43);
    ov99_021E6A9C(obj);
    ov99_021E6D34(obj);
    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
    Main_SetVBlankIntrCB(ov99_021E6938, obj);
    return 1;
}
