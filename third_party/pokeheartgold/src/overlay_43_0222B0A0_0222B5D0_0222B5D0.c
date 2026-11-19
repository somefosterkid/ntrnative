#include "global.h"

#include "constants/heap.h"

#include "overlay_43_palpad.h"
#include "screen_fade.h"
#include "system.h"
#include "touchscreen.h"
#include "unk_0202C034.h"
#include "save_frontier.h"
#include "unk_02030A98.h"

void sub_0202C338(WiFiList *wiFiList, int param1);
void sub_020311AC(FrontierSave *frontier, int friendIdx);
void ov43_0222A318(PalPadShared *param0, s8 param1, s8 param2);
void ov43_0222BB20(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);
void ov43_0222BFD4(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);
BOOL ov43_0222C024(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);
void ov43_0222C378(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, s16 param3, u32 param4, u32 param5, enum HeapID heapID);
void ov43_0222C600(PalPadRoster *param0);
int ov43_0222B5D0(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);
void ov43_0222A420(PalPadShared *param0);
u32 ov43_0222BFA4(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2);
void ov43_0222C53C(PalPadRoster *param0, PalPadShared *param1, s16 param2);
u32 ov43_0222C620(const PalPadRoster *param0);
u32 ov43_0222C228(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);
void ov43_0222C148(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);
void ov43_0222A41C(PalPadShared *param0, u32 param1);
void ov43_0222A414(PalPadShared *param0, const u16 *param1);
void ov43_0222BBB8(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2);
void ov43_0222C32C(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);
BOOL ov43_0222C358(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);
void ov43_0222C65C(PalPadRoster *param0, PalPadGfx *param1);
BOOL ov43_0222C714(PalPadRoster *param0, PalPadGfx *param1);
BOOL ov43_0222C2B0(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);
BOOL ov43_0222C5D8(const PalPadRoster *param0);
void ov43_0222C750(PalPadRoster *param0, PalPadGfx *param1);
void ov43_0222AD00(PalPadGfx *param0, BOOL param1);
void ov43_0222C890(PalPadRosterPage *param0, PalPadGfx *param1, s32 param2, u32 param3);
int ov43_0222AE2C(PalPadShared *param0, PalPadGfx *param1);

int ov43_0222B5D0(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID) {
    u32 v0;
    WiFiList *v1;
    u32 v2;

    switch (param1->subState) {
    case 0:
        param0->page = 0;
        param0->prevPage = 0;
        param0->cursor = 0;
        param0->prevCursor = 0;
    case 1:
        ov43_0222A420(param1);
        ov43_0222BB20(param0, param1, param2, heapID);
        param1->subState = 2;
        break;
    case 2:
        v0 = ov43_0222BFA4(param0, param1, param2);

        switch (v0) {
        case 0:
            break;
        case 1:
            ov43_0222C53C(param0, param1, 2);
            break;
        case 2:
            ov43_0222C53C(param0, param1, 3);
            break;
        case 3:
            param1->subState = 23;
            break;
        case 4:
            param0->timer = 4;
            param1->subState = 5;
            break;
        }
        break;
    case 3:
        ov43_0222BFD4(param0, param1, param2, heapID);
        param1->subState = 4;
        break;
    case 4:
        v0 = ov43_0222C024(param0, param1, param2, heapID);

        if (v0 == 1) {
            param1->subState = 2;
        }
        break;
    case 5:
        if (param0->timer-- > 0) {
            break;
        }

        param0->timer = 0;
        ov43_0222A41C(param1, ov43_0222C620(param0));
        ov43_0222C148(param0, param1, param2, heapID);
        param1->subState = 6;
        break;
    case 6:
        param1->subState = ov43_0222C228(param0, param1, param2, heapID);
        break;
    case 7:
        param0->timer = 4;
        param1->subState = 8;
        break;
    case 8:
        param0->timer--;

        if (param0->timer <= 0) {
            ov43_0222BBB8(param0, param1, param2);
            ov43_0222A318(param1, 4, 0);
            return 1;
        }
        break;
    case 9:
        param0->page = param1->selectedFriend / 8;
        param0->cursor = param1->selectedFriend % 8;
        param1->subState = 1;
        break;
    case 10:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, (enum FadeType)16, (enum FadeType)16, (((0) << 10) | ((0) << 5) | (0)), 6, 1, heapID);

        v2 = ov43_0222C620(param0);
        v1 = sub_0202C6F4(param1->saveData);

        ov43_0222A414(param1, sub_0202C254(v1, param1->friends.slots[v2]));
        param1->subState = 11;
        break;
    case 11:
        if (IsPaletteFadeFinished()) {
            ov43_0222BBB8(param0, param1, param2);
            ov43_0222A318(param1, 2, 4);
            return 1;
        }
        break;
    case 12:
        v2 = ov43_0222C620(param0);
        v1 = sub_0202C6F4(param1->saveData);
        sub_0202C270(v1, param1->friends.slots[v2], param1->name);
    case 13:
        ov43_0222BB20(param0, param1, param2, heapID);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, (enum FadeType)17, (enum FadeType)17, (((0) << 10) | ((0) << 5) | (0)), 6, 1, heapID);
        param1->subState = 14;
        break;
    case 14:
        if (IsPaletteFadeFinished()) {
            param1->subState = 2;
        }
        break;
    case 15:
        ov43_0222C32C(param0, param1, param2, heapID);
        param1->subState = 16;
        break;
    case 16:
        v0 = ov43_0222C358(param0, param1, param2, heapID);

        if (v0 == 1) {
            param1->subState = 17;
        }
        break;
    case 17:
        switch (ov43_0222AE2C(param1, param2)) {
        case 1:
            ov43_0222C600(param0);
            param1->subState = 18;
            break;
        case 2:
            ov43_0222C600(param0);
            ov43_0222AD00(param2, 1);
            ov43_0222C890(param0->pages, param2, param0->cursor, 0);
            param1->subState = 2;
            break;
        }
        break;
    case 18:
        ov43_0222C65C(param0, param2);
        param1->subState = 19;
        break;
    case 19:
        if (ov43_0222C714(param0, param2) == 1) {
            v1 = sub_0202C6F4(param1->saveData);
            v2 = ov43_0222C620(param0);

            sub_0202C338(v1, param1->friends.slots[v2]);
            sub_020311AC(Save_Frontier_GetStatic(param1->saveData), param1->friends.slots[v2]);
            ov43_0222A420(param1);
            ov43_0222C378(param0, param1, param2, param0->page, param0->cursor, 0, heapID);
            ov43_0222AD00(param2, 1);
            param1->subState = 20;
        }
        break;
    case 20:
        ov43_0222C750(param0, param2);
        param1->subState = 2;
        break;
    case 21:
        v0 = ov43_0222C2B0(param0, param1, param2, heapID);

        if (v0 == 1) {
            param1->subState = 22;
        } else {
            ov43_0222AD00(param2, 1);
            param1->subState = 2;
        }
        break;
    case 22:
        if (ov43_0222C5D8(param0) == 0) {
            break;
        }

        if (gSystem.newKeys != 0 || System_GetTouchNew()) {
            ov43_0222AD00(param2, 1);
            ov43_0222C890(param0->pages, param2, param0->cursor, 0);
            param1->subState = 2;
            ov43_0222C600(param0);
        }
        break;
    case 23:
        ov43_0222BBB8(param0, param1, param2);
        ov43_0222A318(param1, 0, 2);
        return 1;
    }

    return 0;
}
