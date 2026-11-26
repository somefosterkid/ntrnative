#include "global.h"

#include "overlay_86_frontier_records_app.h"

#include "bg_window.h"

extern const struct {
    const WindowTemplate *templates;
    u32 numTemplates;
} ov86_021E8230[2];

void ov86_021E7DF8(BgConfig *bgConfigs, Window *windows, u8 isOpponentApp) {
    const WindowTemplate *template = ov86_021E8230[isOpponentApp].templates;
    u32 numTemplates = ov86_021E8230[isOpponentApp].numTemplates;

    for (u8 i = 0; i < numTemplates; i++) {
        AddWindow(bgConfigs, &windows[i], &template[i]);
        FillWindowPixelBuffer(&windows[i], 0);
    }
}

void ov86_021E7E40(Window *windows, u8 isOpponentApp) {
    u16 i;
    u32 numTemplates = ov86_021E8230[isOpponentApp].numTemplates;

    for (i = 0; i < numTemplates; i++) {
        RemoveWindow(&windows[i]);
    }
}

void ov86_021E7E68(BgConfig *bgConfig, Window *window) {
    AddWindowParameterized(bgConfig, window, 0U, 0x19U, 0x15U, 6U, 2U, 0xDU, 0x3F4U);
}
