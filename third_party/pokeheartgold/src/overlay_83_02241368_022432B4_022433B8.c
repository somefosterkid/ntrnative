#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "overlay_83_battle_castle_opponent_app.h"

s32 ov83_022448E4(BattleCastleOpponentApp *arg0, Window *window);

s32 ov83_022449D4(BattleCastleOpponentApp *arg0, Window *window);

s32 ov83_02244BEC(BattleCastleOpponentApp *param_1);

s32 ov83_022453DC(BattleCastleOpponentApp *arg0, Window *arg1);

s32 ov83_02245584(BattleCastleOpponentApp *arg0, Window *arg1);

void ov83_022433B8(BattleCastleOpponentApp *arg0);

void ov83_022433B8(BattleCastleOpponentApp *arg0) {
    Window *window;

    window = (struct Window *)(&arg0->windows[0]);
    FillWindowPixelBuffer(window, 0U);
    ov83_02245584(arg0, window);
    ov83_022453DC(arg0, window);
    ov83_022448E4(arg0, &arg0->windows[3]);
    ov83_022449D4(arg0, &arg0->windows[2]);
    ov83_02244BEC(arg0);
    GfGfx_BothDispOn();
}
