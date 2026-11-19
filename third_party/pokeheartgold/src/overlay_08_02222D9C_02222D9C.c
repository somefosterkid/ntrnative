#include "global.h"

#include "bg_window.h"
#include "overlay_08_battle_bag.h"
#include "render_window.h"

u8 ov08_02222D9C(BattleBag *battleBag);

u8 ov08_02222D9C(BattleBag *battleBag) {
    ClearFrameAndWindow2(&battleBag->messageBoxWindow, 0);
    return 3;
}
