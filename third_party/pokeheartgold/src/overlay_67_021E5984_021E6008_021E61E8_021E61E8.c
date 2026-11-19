#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "overlay_67_app.h"

void ov67_021E61E8(Ov67App *app) {
    unsigned short bVar1;
    int new_var;
    bVar1 = GetWindowWidth(&app->windows[0]);
    new_var = (u32)bVar1;
    ov67_021E6164(app, 0, 0x18, (new_var * 8) / 2, 0, FONT_UNK_4, 0xf0100, 2);
    return;
}
