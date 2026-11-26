#include "global.h"
struct TradeRoom;

#include "bg_window.h"
#include "sprite.h"
#include "render_window.h"
#include "overlay_65_trade_room.h"

void ov65_0221F714(struct TradeRoom *arg0);

void ov65_0221F748(Sprite *sprite, int x, int y);

void ov65_0221F760(TradeRoom *arg0);

void ov65_0221F714(TradeRoom *arg0) {
    *((s32 *)&arg0->bgScrollOffset) = (s32)(*((s32 *)&arg0->bgScrollOffset) + 2);
    BgSetPosTextAndCommit(arg0->bgConfig, 6U, BG_POS_OP_SET_ROT, *((s32 *)&arg0->bgScrollOffset));
    BgSetPosTextAndCommit(arg0->bgConfig, 3U, BG_POS_OP_SET_ROT, *((s32 *)&arg0->bgScrollOffset));
}

void ov65_0221F748(Sprite *sprite, int x, int y) {
    VecFx32 pos;

    pos.x = ((fx32)0x0000000000001000L) * x;
    pos.y = ((fx32)0x0000000000001000L) * y;

    Sprite_SetMatrix(sprite, &pos);
}

void ov65_0221F760(TradeRoom *arg0) {
    arg0->waitDialHandle = WaitingIcon_New(&arg0->windows[23], 0x3D9U);
}
