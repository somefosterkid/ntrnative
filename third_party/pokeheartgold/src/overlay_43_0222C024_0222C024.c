#include "global.h"

#include "constants/heap.h"

#include "overlay_43_palpad.h"
#include "touchscreen_list_menu.h"
#include "unk_0202C034.h"
#include "unk_02025C44.h"
#include "bg_window.h"
#include "brightness.h"
#include "list_menu.h"
#include "list_menu_items.h"
#include "pm_string.h"
#include "save.h"
#include "sprite.h"

void ov43_0222AD20(PalPadGfx *, u32);
void ov43_0222C378(PalPadRoster *, PalPadShared *, PalPadGfx *, s16, s32, u32, enum HeapID);
void ov43_0222C9A4(PalPadRosterPage *, PalPadGfx *, PalPadScreens *);
extern u32 ov43_0222F0EC;
extern NNSG2dViewRect ov43_0222F0FC;
BOOL ov43_0222C024(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);
s32 ov43_0222AB20(void *, SaveData *, s32, s32);
s32 ov43_0222AD00(void *, s32);
s32 ov43_0222C550(void *, void *, s32, s32);
s32 ov43_0222C620(void *);
void ov43_0222C630(TouchscreenListMenu *menu, u32 cursorPos, void *callbackArg, s32 event);
extern s32 ov43_0222ED9C;
void ov43_0222C148(void *arg0, void *arg1, void *arg2, s32 arg3);

BOOL ov43_0222C024(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID) {
    s32 iVar2;
    NNSG2dViewRect NStack_28;
    enum HeapID HStack_18;

    HStack_18 = heapID;
    if (param0->scrollStep >= 8) {
        ScheduleSetBgPosText(param2->bgConfig, 2, BG_POS_OP_SET_ROT, 0);
        ScheduleSetBgPosText(param2->bgConfig, 3, BG_POS_OP_SET_ROT, 0);
        NStack_28 = ov43_0222F0FC;
        GF_SetG2dRendererSurface(&param2->renderSurface, &NStack_28);
        if (param0->scrollDir == 2) {
            iVar2 = 2;
        } else {
            iVar2 = 1;
        }
        ov43_0222C9A4(param0->pages + iVar2, param2, &param2->screens);
        ov43_0222C378(param0, param1, param2, (s32)param0->page, (s32)param0->cursor, 0, heapID);
        ov43_0222AD20(param2, 1);
        return 1;
    }
    iVar2 = param0->scrollStep * 0x100;
    iVar2 = (s32)(iVar2 + ((u32)(iVar2 >> 2) >> 0x1d)) >> 3;
    if (param0->scrollDir == 2) {
        iVar2 = -iVar2;
    }
    ScheduleSetBgPosText(param2->bgConfig, 2, BG_POS_OP_SET_ROT, iVar2);
    ScheduleSetBgPosText(param2->bgConfig, 3, BG_POS_OP_SET_ROT, iVar2);
    NStack_28 = ov43_0222F0FC;
    NStack_28.posTopLeft.x += iVar2 * 0x1000;
    GF_SetG2dRendererSurface(&param2->renderSurface, &NStack_28);
    iVar2 = (s32)param0->scrollStep;
    if (iVar2 % 2 != 0 && *(void (**)())(&ov43_0222F0EC + (iVar2 / 2)) != NULL) {
        (**(void (**)())(&ov43_0222F0EC + (iVar2 / 2)))(param0, param2, (s32)param0->page, (s32)param0->prevPage);
    }
    param0->scrollStep = param0->scrollStep + 1;
    return 0;
}

void ov43_0222C148(void *arg0, void *arg1, void *arg2, s32 arg3) {
    TouchscreenListMenuHeader sp28;
    s32 sp24;
    s32 sp20;
    WiFiList *sp1C;
    s32 sp18;
    WiFiList *temp_r0;
    void *temp_r7;

    sp18 = arg3;
    temp_r0 = sub_0202C6F4(*(SaveData **)((u8 *)arg1 + 4));
    sp1C = temp_r0;
    sp20 = ov43_0222C620(arg0);
    temp_r7 = arg1 + 0x18;
    sp24 = sub_0202C090(temp_r0, (s32) * (u8 *)(temp_r7 + sp20), 8);
    MI_CpuFill8(&sp28, 0U, sizeof(TouchscreenListMenuHeader));
    {
        u16 *src = (u16 *)&ov43_0222ED9C;
        u16 *dst = (u16 *)&sp28;
        dst[0] = src[2];
        dst[1] = src[3];
        dst[2] = src[4];
        dst[3] = src[5];
        dst[4] = src[6];
        dst[5] = src[7];
    }
    if (sp24 == 2) {
        sp28.listMenuItems = *(ListMenuItem **)((u8 *)arg0 + 0xE8);
    } else {
        sp28.listMenuItems = *(ListMenuItem **)((u8 *)arg0 + 0xE4);
    }
    sp28.bgConfig = *(BgConfig **)arg2;
    sp28.numWindows = 4;
    *(TouchscreenListMenu **)((u8 *)arg0 + 0xEC) = TouchscreenListMenu_CreateWithCallback(*(TouchscreenListMenuSpawner **)((u8 *)arg2 + 0x5C), &sp28, (u8) * (s32 *)arg1, 0xDU, 5U, 0x11U, 0U, (TouchscreenListMenuCallback)(ov43_0222C630), NULL, 1);
    ov43_0222AB20(arg2, *(SaveData **)((u8 *)arg1 + 4), *(u8 *)(temp_r7 + sp20), sp18);
    ov43_0222C550(arg0, arg2, 0xC, sp18);
    ov43_0222AD00(arg2, 0);
    StartBrightnessTransition(4U, 8, 0, (GXBlendPlaneMask)(0x3D), 1U);
}
