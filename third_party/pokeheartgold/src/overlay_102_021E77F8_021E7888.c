#include "global.h"
#include "overlay_102_easy_chat_app.h"

#include "mail_misc.h"
#include "overlay_manager.h"
#include "touch_hitbox_controller.h"

void ov102_021E77F8(u32 arg0, u32 arg1, void *arg2);

s32 ov102_021E7934(void *, void *);

s32 ov102_021E8D50(void *, s32 *);

void *ov102_021E909C(void *, s32);

void *sub_02091880(s32, EasyChatArgs *);

extern s32 ov102_021E8D60;

extern TouchscreenHitbox ov102_021EC648;

void *ov102_021E7888(OverlayManager *arg0);

void *ov102_021E7888(OverlayManager *man) {
    EasyChatArgs *args;
    void *v0;

    v0 = OverlayManager_CreateAndGetData(man, 0x7CU, HEAP_ID_34);
    MI_CpuFill8(v0, 0U, 0x7CU);
    args = OverlayManager_GetArgs(man);
    *(EasyChatArgs **)v0 = args;
    *(s32 *)((u8 *)v0 + 4) = (s32)sub_02090D70(args);
    *(s32 *)((u8 *)v0 + 0x74) = sub_02090E44(*(EasyChatArgs **)v0);
    sub_02090D8C(*(EasyChatArgs **)v0, v0 + 0x10, v0 + 8);
    if (*(s32 *)((u8 *)v0 + 4) == 2) {
        ov102_021E7934(v0 + 0x64, v0 + 8);
    }
    *(void **)((u8 *)v0 + 0x18) = sub_02091880(0x22, *(EasyChatArgs **)v0);
    *(void **)((u8 *)v0 + 0x14) = ov102_021E909C(v0, *(s32 *)((u8 *)v0 + 0x18));
    *(TouchHitboxController **)((u8 *)v0 + 0x40) = TouchHitboxController_Create(&ov102_021EC648, 5U, ov102_021E77F8, v0, HEAP_ID_34);
    *(s32 *)((u8 *)v0 + 0x44) = 0;
    *(s32 *)((u8 *)v0 + 0x48) = 0;
    *(s8 *)((u8 *)v0 + 0x6B) = 0;
    *(s32 *)((u8 *)v0 + 0x2C) = 0;
    *(s32 *)((u8 *)v0 + 0x28) = 0;
    *(s16 *)((u8 *)v0 + 0x4E) = 0;
    ov102_021E7A4C(v0, ov102_021E7AA4);
    ov102_021E8D50(v0, &ov102_021E8D60);
    return v0;
}
