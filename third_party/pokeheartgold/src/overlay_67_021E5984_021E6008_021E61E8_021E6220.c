#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "frontier_data.h"
#include "message_format.h"
#include "overlay_67_app.h"
#include "save_frontier.h"
#include "string_util.h"
#include "unk_0202D230.h"
#include "unk_02030A98.h"

void ov67_021E6220(Ov67App *app) {
    u8 bVar1;
    u16 uVar2;
    u32 uVar3;
    Window *pWVar4;
    u32 uVar5;

    uVar5 = 0;
    pWVar4 = app->windows;
    do {
        FillWindowPixelBuffer(pWVar4, 0);
        uVar5 = uVar5 + 1;
        pWVar4 = pWVar4 + 1;
    } while (uVar5 < 10);
    ov67_021E61E8(app);
    bVar1 = GetWindowWidth(&app->windows[1]);
    ov67_021E6164(app, 1, 2, (bVar1 * 8) / 2, 0, FONT_SYSTEM, 0xf0200, 2);
    ov67_021E6164(app, 2, 5, 0, 0, FONT_SYSTEM, 0x10200, 0);
    ov67_021E6164(app, 3, 8, 0, 0, FONT_SYSTEM, 0x10200, 0);
    ov67_021E6164(app, 4, 0xc, 0, 0, FONT_SYSTEM, 0x10200, 0);
    uVar3 = sub_0202D5DC(app->frontierData, 0xc, 0);
    if (uVar3 == 0) {
        ov67_021E6164(app, 6, 9, 0, 0, FONT_SYSTEM, 0x10200, 0);
    } else {
        ov67_021E6164(app, 6, 10, 0, 0, FONT_SYSTEM, 0x10200, 0);
    }
    ov67_021E6164(app, 8, 0xb, 0, 0, FONT_SYSTEM, 0x10200, 0);
    uVar3 = sub_0202D488(app->frontierData, 0);
    BufferIntegerAsString(app->msgFmt, 0, uVar3, 2, PRINTING_MODE_LEFT_ALIGN, 1);
    bVar1 = GetWindowWidth(&app->windows[5]);
    ov67_021E61A0(app, 5, 0xe, (u32)bVar1 << 3, 0, FONT_SYSTEM, 0x10200, 1);
    uVar2 = FrontierSave_GetStat(app->frontierSave, 9, 0xff);
    BufferIntegerAsString(app->msgFmt, 0, (u32)uVar2, 4, PRINTING_MODE_RIGHT_ALIGN, 1);
    bVar1 = GetWindowWidth(&app->windows[7]);
    ov67_021E61A0(app, 7, 0xf, (bVar1 * 8) / 2, 0, FONT_SYSTEM, 0x10200, 2);
    uVar2 = FrontierSave_GetStat(app->frontierSave, 8, 0xff);
    BufferIntegerAsString(app->msgFmt, 0, (u32)uVar2, 4, PRINTING_MODE_RIGHT_ALIGN, 1);
    bVar1 = GetWindowWidth(&app->windows[9]);
    ov67_021E61A0(app, 9, 0xf, (bVar1 * 8) / 2, 0, FONT_SYSTEM, 0x10200, 2);
    uVar5 = 0;
    pWVar4 = app->windows;
    do {
        ScheduleWindowCopyToVram(pWVar4);
        uVar5 = uVar5 + 1;
        pWVar4 = pWVar4 + 1;
    } while (uVar5 < 10);
    return;
}
