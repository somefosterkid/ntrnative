#include "global.h"
#include "unk_02019BA4.h"
#include "overlay_83_battle_castle_opponent_app.h"

s32 ov83_022469D8(BattleCastleOpponentApp *arg0, s32, s32);

extern s32 ov83_02248544;

void ov83_02247B4C(void *arg0, s32 arg1, s32 arg2);

s32 ov83_02242AC0(void *, s32, s32, s32);
s32 ov83_02242AE0(void *, s32);
extern GridCallbacks ov83_02248520;
extern TouchscreenHitbox ov83_022485C8;
extern DpadMenuBox ov83_02248610;
GridInputHandler *ov83_02247B7C(void *arg0);

void ov83_02247B4C(void *arg0, s32 arg1, s32 arg2) {
    ov83_022469D8(arg0, *(s32 *)(&ov83_02248544 + arg1), *(s32 *)(&ov83_02248544 + arg2));
}

void ov83_02247B64(void *arg0, s32 arg1, s32 arg2);

void ov83_02247B64(void *arg0, s32 arg1, s32 arg2) {
    ov83_022469D8(arg0, *(s32 *)(&ov83_02248544 + arg1), *(s32 *)(&ov83_02248544 + arg2));
}

GridInputHandler *ov83_02247B7C(void *callbackParam) {
    GridInputHandler *gridInputHandler;

    gridInputHandler = GridInputHandler_Create(&ov83_022485C8, &ov83_02248610, &ov83_02248520, callbackParam, 1, 0U, HEAP_ID_107);
    ov83_02242AC0(callbackParam, 0x40, 0x34, 3);
    ov83_02242AE0(callbackParam, 0);
    return gridInputHandler;
}
