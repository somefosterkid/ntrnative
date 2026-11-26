#include "global.h"

#include "brightness.h"
#include "touchscreen_list_menu.h"
#include "save.h"
#include "unk_02005D10.h"
#include "unk_0202C034.h"
#include "overlay_43_palpad.h"
#include "overlay_43_palpad_fns.h"
#include "bg_window.h"

s32 ov43_0222AD00(PalPadGfx *, s32);

void ov43_0222C600(PalPadRoster *roster);
void ov43_0222C890(PalPadRosterPage *pages, PalPadGfx *gfx, s32 cursor, s32 unused);

void ov43_0222AAA4(PalPadGfx *gfx, u64 code);
void ov43_0222AB20(PalPadGfx *gfx, SaveData *saveData, s32 slot, u32 heapID);
void ov43_0222C550(PalPadRoster *roster, PalPadGfx *gfx, u32 strno, u32 heapID);
s32 ov43_0222C620(const PalPadRoster *roster);

s32 ov43_0222C228(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx) {
    u32 v0;

    if (ov43_0222C5D8(roster, shared) == 0) {
        return 6;
    }
    v0 = TouchscreenListMenu_HandleInput(roster->menu);
    switch (v0) {
    case -1:
        return 6;
    case 3:
    case -2:
        ov43_0222AD00(gfx, 1);
        v0 = 2;
    default:
        ov43_0222C600(roster);
        shared->inputState = TouchscreenListMenu_WasLastInputTouch(roster->menu);
        TouchscreenListMenu_Destroy(roster->menu);
        if (v0 == 2) {
            ov43_0222AD00(gfx, 1);
            ov43_0222C890(roster->pages, gfx, roster->cursor, 0);
        }
        SetBlendBrightness((fx32)(0), (GXBlendPlaneMask)(0x3D), 1U);
        return v0;
    }
}

BOOL ov43_0222C2B0(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID) {
    WiFiList *a0;
    s32 iVar1;
    DWCFriendData *frienddata;
    u64 uVar2;
    u32 uStack_30;

    a0 = sub_0202C6F4(shared->saveData);
    iVar1 = ov43_0222C620(roster);
    frienddata = sub_0202C23C(a0, shared->friends.slots[iVar1]);
    uVar2 = DWC_GetFriendKey(frienddata);
    if (!uVar2) {
        uStack_30 = 0x42;
    } else {
        uStack_30 = 0x3c;
    }
    PlaySE(0x5dd);
    ov43_0222AAA4(gfx, uVar2);
    ov43_0222AB20(gfx, shared->saveData, shared->friends.slots[iVar1], heapID);
    ov43_0222C550(roster, gfx, uStack_30, heapID);
    return TRUE;
}
