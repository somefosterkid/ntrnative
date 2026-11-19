#include "global.h"
struct BattleHallApp;

#include "bg_window.h"
#include "overlay_82_battle_hall_app.h"
#include "overlay_80_022372D8.h"


s32 ov82_0223E9B0(void);

s32 ov82_0223E9E8(void *);

s32 ov82_0223FCBC(void *, u16, u16);

s32 ov82_0223FCFC(void *, s32);

void ov82_0223E974(struct BattleHallApp *arg0);

void ov82_0223E974(BattleHallApp *arg0) {
    ov82_0223E9B0();
    arg0->bgConfig = BgConfig_Alloc(HEAP_ID_105);
    ov82_0223E9E8(arg0);
    if (ov80_02237920(arg0->cursorPos) == 0xFE) {
        ov82_0223FCFC(*((void **)&arg0->cursorSprite), 2);
        ov82_0223FCBC(*((void **)&arg0->cursorSprite), 0x80, 0xA8);
    }
}
