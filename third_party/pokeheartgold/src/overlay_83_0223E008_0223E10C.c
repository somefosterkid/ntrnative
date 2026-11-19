#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "overlay_83_battle_castle_self_app.h"

s32 ov83_02240080(BattleCastleSelfApp *, Window *);

s32 ov83_02240170(BattleCastleSelfApp *, Window *);

s32 ov83_02240290(BattleCastleSelfApp *);

s32 ov83_0224153C(BattleCastleSelfApp *, Window *);

s32 ov83_02241770(BattleCastleSelfApp *, Window *);

void ov83_0223E10C(BattleCastleSelfApp *arg0);

void ov83_0223E10C(BattleCastleSelfApp *arg0) {
    Window *window;

    window = &arg0->windows[0];
    FillWindowPixelBuffer(window, 0U);
    ov83_0224153C(arg0, window);
    ov83_02241770(arg0, window);
    ov83_02240080(arg0, &arg0->windows[3]);
    ov83_02240170(arg0, &arg0->windows[2]);
    ov83_02240290(arg0);
    GfGfx_BothDispOn();
}
