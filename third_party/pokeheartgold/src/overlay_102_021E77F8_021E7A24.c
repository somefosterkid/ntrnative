#include "global.h"

#include "mail_misc.h"
#include "overlay_manager.h"
#include "touch_hitbox_controller.h"
#include "overlay_102_easy_chat_app.h"

s32 ov102_021E9144(void *);

s32 sub_020918C0(void *);

void ov102_021E7A24(void *arg0, OverlayManager *arg1);

void ov102_021E7A24(void *arg0, OverlayManager *man) {
    sub_02090E5C(*(EasyChatArgs **)arg0, *((enum MenuInputState *)&((EasyChatApp *)arg0)->unk_74));
    TouchHitboxController_Destroy(((TouchHitboxController *)((EasyChatApp *)arg0)->buttonAction));
    ov102_021E9144(*((void **)&((EasyChatApp *)arg0)->unk_14));
    sub_020918C0(*((void **)&((EasyChatApp *)arg0)->unk_18));
    OverlayManager_FreeData(man);
}

void ov102_021E7A4C(EasyChatApp *easyChatApp, EasyChatMainFunc mainFunc) {
    easyChatApp->mainFunc = mainFunc;
    easyChatApp->state = 0;
    *(s32 *)((u8 *)easyChatApp + 0x78) = 0;
}
