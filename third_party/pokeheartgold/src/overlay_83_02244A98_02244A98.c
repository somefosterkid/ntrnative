#include "global.h"

#include "message_format.h"
#include "overlay_83_battle_castle_opponent_app.h"

void ov83_02244A98(BattleCastleOpponentApp *app, u32 arg1, s32 arg2, u32 arg3, enum PrintingMode arg4);

void ov83_02244A98(BattleCastleOpponentApp *app, u32 idx, s32 num, u32 numDigits, enum PrintingMode strconvmode) {
    BufferIntegerAsString(app->strTemplate, idx, num, numDigits, strconvmode, 1);
}
