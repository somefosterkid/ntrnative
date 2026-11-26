#include "global.h"

#include "system.h"
#include "touchscreen.h"
#include "overlay_102_easy_chat_app.h"

s32 ov102_021E8C3C(void *arg0);

s32 ov102_021E8C64(void *);

extern TouchscreenHitbox ov102_021EC5F4;

s32 ov102_021E896C(EasyChatApp *arg0);

s32 ov102_021E896C(EasyChatApp *arg0) {
    TouchscreenHitbox hitbox;
    s32 v2;
    s32 v1;
    s32 v0;
    s32 v3;
    s32 v4;
    s32 v7;
    u16 v5;
    u32 v6;

    if (System_GetTouchNew() == 0) {
        return 0;
    }
    v3 = TouchscreenHitbox_FindRectAtTouchNew(&ov102_021EC5F4);
    switch (v3) {
    case 0:
        return ov102_021E8C3C(&arg0->unk_54);
    case 1:
        return ov102_021E8C64(&arg0->unk_54);
    default:
        v5 = arg0->unk_54.unk_00 * 2;
        v1 = 0;
        v2 = 0;
        v0 = 0x30;
loop_8:
        hitbox.rect.top = (u8)v0;
        v7 = 0;
        v6 = 0x20;
        hitbox.rect.bottom = hitbox.rect.top + 0x10;
loop_9:
        hitbox.rect.left = v6;
        hitbox.rect.right = hitbox.rect.left + 0x48;
        if (TouchscreenHitbox_PointIsIn(&hitbox, (u32)gSystem.touchX, (u32)gSystem.touchY) != 0) {
            *((u16 *)&arg0->unk_60.unk_02) = v5;
            return 5;
        }
        v5 += 1;
        if ((u32)v5 >= (u32) arg0->unk_54.unk_04) {
            v1 = 1;
            goto block_14;
        }
        v7 += 1;
        v6 += 0x68;
        if (v7 >= 2) {
block_14:
            if ((v1 != 0) || (v0 += 0x18, v4 = v2 + 1, v2 = v4, (v4 >= 5))) {
                return 0;
            }
            goto loop_8;
        }
        goto loop_9;
    }
}
