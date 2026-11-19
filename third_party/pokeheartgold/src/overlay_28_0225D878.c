#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "overlay_28.h"
#include "unk_02009D48.h"

void ov28_0225D878(DowsingMchnApp *app) {
    u32 uVar1;
    Window *window;

    uVar1 = 0;
    window = app->windows;
    do {
        RemoveWindow(window);
        uVar1 = uVar1 + 1;
        window = window + 1;
    } while (uVar1 < 5);
    FontID_Release(FONT_UNK_4);
    return;
}

void ov28_0225D898(DowsingMchnApp *app, NARC *narc) {
    app->spriteList = G2dRenderer_Init(8, &app->renderer, HEAP_ID_8);
    G2dRenderer_SetSubSurfaceCoords(&app->renderer, 0, 0x100000);
    ov28_0225D8EC(app);
    ov28_0225DA74(app);
    ov28_0225DB8C(app, narc);
}
