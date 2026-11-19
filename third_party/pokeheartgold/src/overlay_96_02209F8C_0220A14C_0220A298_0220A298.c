#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "overlay_96_ov96_0220_data.h"

void ov96_0220B6EC(void *arg0);

u32 ov96_0220B730(void *arg0, s32, s32);

s32 ov96_0220B744(void *arg0);

u32 ov96_0220B788(void *arg0);

extern TouchscreenHitbox ov96_0221CC04;

void ov96_0220A298(Ov96_0220Data *arg0, s32 arg1, s32 arg2);

void ov96_0220A298(Ov96_0220Data *arg0, s32 arg1, s32 arg2) {
    u32 spC;
    u32 sp8;
    u32 sp4;
    u32 sp0;
    s32 var_r1;
    s32 var_r2;
    s32 var_r3;
    s32 var_r4;
    Ov96_0220DataDrag *st;

    ov96_0220B730(arg0->unk_4C, arg1, arg2);
    st = &arg0->drag;
    if (System_GetTouchNew() != 0) {
        System_GetTouchNewCoords(&spC, &sp8);
        var_r1 = 0;
        var_r2 = 0;
        var_r3 = 0;
        if ((spC >= 0x60U) && (spC <= 0xB0U)) {
            var_r3 = 1;
        }
        if ((var_r3 != 0) && (sp8 >= 0x48U)) {
            var_r2 = 1;
        }
        if ((var_r2 != 0) && (sp8 <= 0x98U)) {
            var_r1 = 1;
        }
        if ((arg1 != 0) || (var_r1 != 0)) {
            st->y1 = sp8;
            st->y0 = sp8;
            st->count = 1;
            st->active = 1;
        }
    } else if ((st->active != 0) && (System_GetTouchHeld() == 0)) {
        if (((s32)st->y1 - (s32)st->y0 >= 0x20) && (ov96_0220B744(arg0->unk_4C) != 0)) {
            ov96_0220B6EC(arg0->unk_4C);
        }
        st->active = 0;
    } else if (st->active != 0) {
        System_GetTouchHeldCoords(&sp4, &sp0);
        st->y0 = sp0;
        st->count = st->count + 1;
        if (st->count >= 0xA) {
            st->active = 0;
        }
    }
    if ((System_GetTouchNew() != 0) || (System_GetTouchHeld() <= 0)) {
        if (TouchscreenHitbox_FindRectAtTouchNew(&ov96_0221CC04) != -1) {
            if (ov96_0220B744(arg0->unk_4C) != 0) {
                var_r4 = 0x14;
                PlaySE(0x89BU);
                ov96_0220B6EC(arg0->unk_4C);
            } else {
                var_r4 = 0x16;
                PlaySE(0x89CU);
            }
        } else if (ov96_0220B744(arg0->unk_4C) == 0) {
            var_r4 = 0x15;
        } else if (ov96_0220B788(arg0->unk_4C) != 0) {
            var_r4 = 0x17;
        } else {
            var_r4 = 0x13;
        }
        ManagedSprite_SetAnimNoRestart(arg0->unk_24[1], var_r4);
    }
}
