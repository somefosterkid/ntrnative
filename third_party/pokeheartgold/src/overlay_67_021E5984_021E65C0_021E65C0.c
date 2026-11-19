#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "overlay_67_app.h"
#include "string_util.h"
#include "unk_0202D230.h"

void ov67_021E65C0(Ov67App *app) {
    u32 uVar1;
    Window *pWVar2;
    Window *pWVar2_2;
    WifiBattleTowerIndices indices;
    uVar1 = 0;
    pWVar2 = app->windows;
    do {
        FillWindowPixelBuffer(pWVar2, 0);
        uVar1 = uVar1 + 1;
        pWVar2 = pWVar2 + 1;
    } while (uVar1 <= 3);
    ov67_021E61E8(app);
    sub_0202D8D0(app->downloadData, &indices);
    BufferIntegerAsString(app->msgFmt, 0, (u32)indices.rank, 2, PRINTING_MODE_LEFT_ALIGN, 1);
    ov67_021E61A0(app, 1, 0x11, 0, 0, FONT_SYSTEM, 0xf0200, 0);
    BufferIntegerAsString(app->msgFmt, 1, (u32)indices.opponentIdx, 3, PRINTING_MODE_LEADING_ZEROS, 1);
    ov67_021E61A0(app, 2, 0x12, 0x3c, 0, FONT_SYSTEM, 0xf0200, 2);
    ov67_021E6164(app, 3, 0x13, 0, 0, FONT_SYSTEM, 0xf0200, 0);
    uVar1 = 0;
    pWVar2_2 = app->windows;
    do {
        ScheduleWindowCopyToVram(pWVar2_2);
        uVar1 = uVar1 + 1;
        pWVar2_2 = pWVar2_2 + 1;
    } while (uVar1 <= 3);
    ov67_021E6490(app);
    ov67_021E6688(app, 0);
    ov67_021E6530(app);
    return;
}
