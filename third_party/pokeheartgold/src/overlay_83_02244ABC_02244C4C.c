#include "global.h"

#include "options.h"
#include "overlay_83_battle_castle_opponent_app.h"

s32 ov83_02245094(u8 *);

void ov83_02244C4C(u8 *arg0);

void ov83_02244C4C(u8 *arg0) {
    ov83_02245094(arg0 + 0xD0);
}

s32 ov83_02246AA4(void *);

s32 ov83_02246CC0(void *);

s32 ov83_02247944(void *, u32);

void ov83_02244C58(void *arg0);

void ov83_02244C58(void *arg0) {
    ov83_02247944(arg0 + 0xC0, Options_GetFrame(((BattleCastleOpponentApp *)arg0)->options));
    *((s32 *)&((BattleCastleOpponentApp *)arg0)->unk_600) = -2;
    ov83_02246AA4(arg0);
    ov83_02246CC0(arg0);
}

s32 ov83_02246C70(void *);

void ov83_02244C88(u8 *arg0);

void ov83_02244C88(u8 *arg0) {
    ov83_02245094(arg0 + 0xC0);
    ov83_02246C70(arg0);
}

s32 ov83_02246B6C(void *);

s32 ov83_02246D40(void *);

void ov83_02244C9C(void *arg0);

void ov83_02244C9C(void *arg0) {
    ov83_02247944(arg0 + 0xC0, Options_GetFrame(((BattleCastleOpponentApp *)arg0)->options));
    *((s32 *)&((BattleCastleOpponentApp *)arg0)->unk_600) = -2;
    ov83_02246B6C(arg0);
    ov83_02246D40(arg0);
}
