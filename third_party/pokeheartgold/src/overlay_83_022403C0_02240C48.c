#include "global.h"
struct BattleCastleSelfApp;

#include "message_format.h"
#include "overlay_83_battle_castle_self_app.h"

void ov83_02240C48(struct BattleCastleSelfApp *arg0, u32 arg1, s32 arg2, u32 arg3, enum PrintingMode arg4);

void ov83_02240C48(BattleCastleSelfApp *arg0, u32 idx, s32 num, u32 numDigits, enum PrintingMode strconvmode) {
    BufferIntegerAsString(arg0->strTemplate, idx, num, numDigits, strconvmode, 1);
}
