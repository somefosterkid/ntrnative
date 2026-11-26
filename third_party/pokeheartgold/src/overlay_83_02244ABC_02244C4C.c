#include "global.h"

#include "options.h"
#include "overlay_83_battle_castle_opponent_app.h"

s32 ov83_02245094(Window *window);

void ov83_02244C4C(BattleCastleOpponentApp *arg0);

void ov83_02244C4C(BattleCastleOpponentApp *arg0) {
    ov83_02245094(&arg0->windows[8]);
}

void ov83_02246AA4(BattleCastleOpponentApp *app);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov83_02246CC0(BattleCastleOpponentApp *param_1);
#else
void ov83_02246CC0(BattleCastleOpponentApp *param_1);
#endif

s32 ov83_02247944(Window *window, u32);

void ov83_02244C58(BattleCastleOpponentApp *arg0);

void ov83_02244C58(BattleCastleOpponentApp *arg0) {
    ov83_02247944(&arg0->windows[7], Options_GetFrame(arg0->options));
    *((s32 *)&arg0->listSelection) = -2;
    ov83_02246AA4(arg0);
    ov83_02246CC0(arg0);
}

s32 ov83_02246C70(BattleCastleOpponentApp *arg0);

void ov83_02244C88(BattleCastleOpponentApp *arg0);

void ov83_02244C88(BattleCastleOpponentApp *arg0) {
    ov83_02245094(&arg0->windows[7]);
    ov83_02246C70(arg0);
}

void ov83_02246B6C(BattleCastleOpponentApp *app);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov83_02246D40(BattleCastleOpponentApp *param_1);
#else
void ov83_02246D40(BattleCastleOpponentApp *param_1);
#endif

void ov83_02244C9C(BattleCastleOpponentApp *arg0);

void ov83_02244C9C(BattleCastleOpponentApp *arg0) {
    ov83_02247944(&arg0->windows[7], Options_GetFrame(arg0->options));
    *((s32 *)&arg0->listSelection) = -2;
    ov83_02246B6C(arg0);
    ov83_02246D40(arg0);
}
