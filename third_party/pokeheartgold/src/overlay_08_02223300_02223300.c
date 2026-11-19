#include "global.h"
struct BattleBag;

#include "bg_window.h"
#include "overlay_08_battle_bag.h"

s32 ov08_02223248(void *, s32);

s32 ov08_022232AC();

s32 ov08_022233DC(void *, u8);

s32 ov08_02223454(void *);

s32 ov08_02223480(void *, s32);

s32 ov08_02223F94(void *, u8);

s32 ov08_02224134(void *, u8);

s32 ov08_0222421C(void *, s32);

s32 ov08_02224A50(void *, s32);

void ov08_02223300(struct BattleBag *arg0, u8 arg1);

void ov08_02223300(BattleBag *arg0, u8 arg1) {
    ov08_022232AC();
    ov08_02223248(arg0, arg1);
    BgFillTilemapBufferAndSchedule(arg0->background, 4U, 0U);
    BgFillTilemapBufferAndSchedule(arg0->background, 5U, 0U);
    ov08_02223454(arg0);
    ov08_022233DC(arg0, arg1);
    ov08_02223480(arg0, arg1);
    ov08_02224A50(arg0, arg1);
    ov08_02224134(arg0, arg1);
    ov08_0222421C(arg0, arg1);
    arg0->currentScreen = arg1;
    ov08_02223F94(arg0, arg0->currentScreen);
}
