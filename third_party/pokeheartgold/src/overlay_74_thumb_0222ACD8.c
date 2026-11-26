#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "message_format.h"
#include "overlay_manager.h"
#include "overlay_74_mystery_gift_app_data.h"
#include "overlay_74_main_menu_window.h"

s32 ov74_02235414(s32 *, Window *, s32, s32, s32, s32);

s32 ov74_02235464(s32 *, s32, s32, s32);

s32 ov74_0223546C(s32 *, s32, s32);

s32 ov74_02235568(BgConfig *, s32 *, s32, s32, s32);

void ov74_0222ACD8(MysteryGiftAppData *arg0);

u32 ov74_0222ADBC(OverlayManager *param_1, Window *param_2, u32 param_3, u32 param_4);
void ov74_0222AD6C(OverlayManager *param_1);

void ov74_0222ACD8(MysteryGiftAppData *arg0) {
    u32 v0[21];
    MessageFormat *messageFormat;
    s32 v1;

    v1 = *((s32 *)&arg0->wifiCommErrorStringID);
    if (v1 == -1) {
        v1 = 0xB;
    }
    GfGfx_EngineATogglePlanes(0x10U, 0U);
    BgClearTilemapBufferAndCommit(*(BgConfig **)arg0, 0U);
    messageFormat = MessageFormat_New(HEAP_ID_84);
    BufferIntegerAsString(messageFormat, 0U, *((s32 *)&arg0->wifiCommErrorCode), 5U, PRINTING_MODE_LEADING_ZEROS, 1);
    ov74_02235414(((s32 *)v0), &arg0->wifiCommErrorWindow, 1, 0x320, 0x1F, 3);
    ov74_02235464(((s32 *)v0), 0x18, 0x10, 0x64);
    ov74_0223546C(((s32 *)v0), 0, 1);
    (*((MessageFormat **)&((MainMenuWindow *)v0)->strTemplate)) = messageFormat;
    ov74_02235568(*(BgConfig **)arg0, ((s32 *)v0), 4, 4, v1);
    MessageFormat_Delete(messageFormat);
}

void ov74_0222AD6C(OverlayManager *param_1) {
    MysteryGiftAppData *pvVar1;
    s32 iVar2;
    u32 unaff_r4;

    pvVar1 = OverlayManager_GetData(param_1);
    iVar2 = pvVar1->canReceiveGiftStatus;
    if (iVar2 == 3) {
        unaff_r4 = 0x16;
    } else if (iVar2 == 4) {
        unaff_r4 = 0x1a;
    } else if (iVar2 == 2) {
        unaff_r4 = 0x17;
    } else if (iVar2 == 1) {
        unaff_r4 = 0x1c;
    } else if (iVar2 == 5) {
        ov74_0222ADBC(param_1, &pvVar1->messageBox, 9, 0x34);
        return;
    }
    ov74_0222ADBC(param_1, &pvVar1->messageBox, unaff_r4, 0x33);
    return;
}
