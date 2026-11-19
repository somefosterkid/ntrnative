#include "global.h"

#include "bg_window.h"
#include "mail_message.h"
#include "message_format.h"
#include "overlay_67_app.h"
#include "pm_string.h"
#include "pokemon_types_def.h"

void ov67_021E6688(Ov67App *app, u32 param_2) {
    Window *window;
    String *string;
    u32 uVar1;
    s32 iVar3;

    if (app->unk_4A3 == 0) {
        iVar3 = 5;
    } else {
        iVar3 = 8;
    }
    window = &app->windows[iVar3];
    uVar1 = 0;
    do {
        FillWindowPixelBuffer(window + uVar1, 0);
        uVar1 = uVar1 + 1 & 0xffff;
    } while (uVar1 < 3);
    if (param_2 < 0x1e) {
        BufferString(app->msgFmt, 0, app->wifiTrainers[param_2].name, app->wifiTrainers[param_2].anonNameIdx, 1, 2);
        if (app->wifiTrainers[param_2].anonNameIdx == 0) {
            ov67_021E61A0(app, iVar3, 0x16, 0, 0, FONT_SYSTEM, 0x10200, 0);
        } else {
            ov67_021E61A0(app, iVar3, 0x17, 0, 0, FONT_SYSTEM, 0x10200, 0);
        }
        ov67_021E6164(app, iVar3 + 1, 0x1c, 0, 0, FONT_SYSTEM, 0x10200, 0);
        uVar1 = (u32)app->wifiTrainers[param_2].country;
        if (uVar1 == 0) {
            ov67_021E6164(app, iVar3 + 1, 0x1f, 0, 0x10, FONT_SYSTEM, 0x10200, 0);
        } else {
            BufferCountryName(app->msgFmt, 0, uVar1);
            ov67_021E61A0(app, iVar3 + 1, 0x1d, 0, 0x10, FONT_SYSTEM, 0x10200, 0);
        }
        uVar1 = (u32)app->wifiTrainers[param_2].region;
        if (uVar1 != 0) {
            BufferCityName(app->msgFmt, 0, (u32)app->wifiTrainers[param_2].country, uVar1);
            ov67_021E61A0(app, iVar3 + 1, 0x1e, 0, 0x20, FONT_SYSTEM, 0x10200, 0);
        }
        ov67_021E6164(app, iVar3 + 2, 0x20, 0, 0, FONT_SYSTEM, 0x10200, 0);
        string = MailMsg_GetExpandedString(&app->wifiTrainers[param_2].message, app->heapID);
        ov67_021E6118(window + 2, string, 0, 0x10, FONT_SYSTEM, 0x10200, 0);
        String_Delete(string);
    }
    uVar1 = 0;
    do {
        ScheduleWindowCopyToVram(window + uVar1);
        uVar1 = uVar1 + 1 & 0xffff;
    } while (uVar1 < 3);
    app->unk_4A3 = app->unk_4A3 ^ 1;
    return;
}
