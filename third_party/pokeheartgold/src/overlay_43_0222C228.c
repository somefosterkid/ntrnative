#include "global.h"

#include "brightness.h"
#include "touchscreen_list_menu.h"
#include "save.h"
#include "unk_02005D10.h"
#include "unk_0202C034.h"
#include "overlay_43_unk_struct_ov64_0222_f0_c4.h"
#include "overlay_43_palpad.h"
#include "bg_window.h"

s32 ov43_0222AD00(PalPadGfx *, s32);

s32 ov43_0222C5D8(void *arg0, s32 *arg1);

s32 ov43_0222C600(void *);

s32 ov43_0222C890(void *, BgConfig **, s32, s32);

s32 ov43_0222C228(void *arg0, s32 *arg1, void *arg2);

void ov43_0222AAA4(void *, s32, s32);
void ov43_0222AB20(void *, void *, s32, u32);
void ov43_0222C550(void *, void *, u32, u32);
s32 ov43_0222C620(void *);
u32 ov43_0222C2B0(void *param_1, u8 *param_2, void *param_3, u32 param_4);

s32 ov43_0222C228(void *arg0, s32 *arg1, void *arg2) {
    u32 v0;

    if (ov43_0222C5D8(arg0, arg1) == 0) {
        return 6;
    }
    v0 = TouchscreenListMenu_HandleInput((((PalPadRoster *)arg0)->menu));
    switch (v0) {
    case -1:
        return 6;
    case 3:
    case -2:
        ov43_0222AD00(arg2, 1);
        v0 = 2;
    default:
        ov43_0222C600(arg0);
        *arg1 = (s32)TouchscreenListMenu_WasLastInputTouch(((TouchscreenListMenu *)((PalPadRoster *)arg0)->menu));
        TouchscreenListMenu_Destroy(((TouchscreenListMenu *)((PalPadRoster *)arg0)->menu));
        if (v0 == 2) {
            ov43_0222AD00(arg2, 1);
            ov43_0222C890(arg0 + 8, arg2, *(s16 *)((u8 *)arg0 + 4), 0);
        }
        SetBlendBrightness((fx32)(0), (GXBlendPlaneMask)(0x3D), 1U);
        return v0;
    }
}

u32 ov43_0222C2B0(void *param_1, u8 *param_2, void *param_3, u32 param_4) {
    WiFiList *a0;
    s32 iVar1;
    DWCFriendData *frienddata;
    u64 uVar2;
    u32 uStack_30;

    a0 = sub_0202C6F4(*(SaveData **)(param_2 + 4));
    iVar1 = ov43_0222C620(param_1);
    frienddata = sub_0202C23C(a0, (u32) * (u8 *)(param_2 + 0x18 + iVar1));
    uVar2 = DWC_GetFriendKey(frienddata);
    if (!uVar2) {
        uStack_30 = 0x42;
    } else {
        uStack_30 = 0x3c;
    }
    PlaySE(0x5dd);
    ov43_0222AAA4(param_3, (s32)uVar2, (s32)(uVar2 >> 0x20));
    ov43_0222AB20(param_3, *(void **)(param_2 + 4), *(u8 *)(param_2 + 0x18 + iVar1), param_4);
    ov43_0222C550(param_1, param_3, uStack_30, param_4);
    return 1;
}
