#include "global.h"
struct BattleCastleOpponentApp;

#include "frontier/overlay_80_022340E8.h"

#include "bg_window.h"
#include "unk_02035900.h"
#include "overlay_83_battle_castle_opponent_app.h"

s32 ov83_02244AD8(BattleCastleOpponentApp *param_1, Window *param_2, u32, u32, u8);

s32 ov83_02244B40(BattleCastleOpponentApp *app, Window *window, u32, u32, u8);

s32 ov83_02244DF4(void *, u16 *, u16 *, u16 *, u16 *);

void ov83_02245584(BattleCastleOpponentApp *arg0, Window *arg1);

void ov83_02245584(BattleCastleOpponentApp *arg0, Window *arg1) {
    u16 x, y, px, py, qx, qy;

    ov83_02244DF4(arg0, &px, &py, &qx, &qy);
    if (ov80_02237D8C(arg0->challengeType) == 0) {
        x = px + 4;
        y = py + 1;
        ov83_02244AD8(arg0, arg1, x, y, 0);
    } else {
        if (sub_0203769C() == 0) {
            y = py + 1;
            ov83_02244AD8(arg0, arg1, px, y, 0);
            x = qx + 0;
            y = qy + 1;
            ov83_02244B40(arg0, arg1, x, y, 0);
        } else {
            y = py + 1;
            ov83_02244B40(arg0, arg1, px, y, 0);
            x = qx + 0;
            y = qy + 1;
            ov83_02244AD8(arg0, arg1, x, y, 0);
        }
    }
    ScheduleWindowCopyToVram(arg1);
}
