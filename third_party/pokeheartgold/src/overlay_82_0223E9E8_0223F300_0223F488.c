#include "global.h"
struct BattleHallApp;

#include "system.h"
#include "overlay_82_battle_hall_app.h"
#include "overlay_80_022372D8.h"


u16 ov82_0223F558(void *);

u16 ov82_0223F570(void *);

s32 ov82_0223FCBC(void *, s32, s32);

s32 ov82_0223FCFC(void *, s32);

s32 ov82_0223F488(struct BattleHallApp *arg0);

s32 ov82_0223F488(BattleHallApp *arg0) {
    s32 i;
    s32 j;
    s32 a;
    s32 y1;
    s32 x1;
    s32 y;
    s32 tx;
    s32 ty;
    s32 x;

    if (gSystem.touchNew != 0) {
        tx = gSystem.touchX;
        ty = gSystem.touchY;
        for (i = 0, y = 3; i < 5; i++, y += 0x24) {
            y1 = y + 0x23;
            for (j = 0, x = 1; j < 4; j++, x += 0x40) {
                x1 = x + 0x3F;
                if (x <= tx && tx <= x1 && y <= ty && ty <= y1) {
                    arg0->cursorPos = j + i * 4;
                    a = ov82_0223F558(arg0);

                    ov82_0223FCBC(*((void **)&arg0->cursorSprite), a, ov82_0223F570(arg0));
                    if (ov80_02237920(arg0->cursorPos) == 0xFE) {
                        ov82_0223FCFC(*((void **)&arg0->cursorSprite), 2);
                        ov82_0223FCBC(*((void **)&arg0->cursorSprite), 0x80, 0xA8);
                    } else {
                        ov82_0223FCFC(*((void **)&arg0->cursorSprite), 1);
                    }
                    *((u8 *)&arg0->pad_90[0]) = 1;
                    return 1;
                }
            }
        }
    }
    return 0;
}
