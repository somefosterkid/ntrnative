#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "overlay_67_app.h"

void ov67_021E6530(Ov67App *app) {
    FillWindowPixelBuffer(&app->windows[4], 0U);
    BufferIntegerAsString(app->msgFmt, 0U, app->unk_4A0 + 1, 1U, PRINTING_MODE_LEFT_ALIGN, 1);
    ov67_021E61A0(app, 4, 0x14, 8, 0, FONT_SYSTEM, 0x10200, 1);
    ov67_021E6164(app, 4, 0x15, 0x10, 0, FONT_SYSTEM, 0x10200, 2);
    BufferIntegerAsString(app->msgFmt, 0U, 3, 1U, PRINTING_MODE_LEFT_ALIGN, 1);
    ov67_021E61A0(app, 4, 0x14, 0x18, 0, FONT_SYSTEM, 0x10200, 0);
    ScheduleWindowCopyToVram(&app->windows[4]);
}
