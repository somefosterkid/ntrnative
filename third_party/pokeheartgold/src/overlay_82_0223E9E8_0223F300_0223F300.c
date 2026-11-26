#include "global.h"

#include "overlay_80_022372D8.h"
#include "system.h"
#include "unk_02005D10.h"
#include "overlay_82_battle_hall_app.h"

extern u16 ov82_0223F558(void *app);

extern u16 ov82_0223F570(void *app);

#if defined(SDK_PORT) || defined(NONMATCHING)
extern void ov82_0223FCBC(void *obj, u16 x, u16 y);
#else
extern void ov82_0223FCBC(void *obj, u32 x, u32 y);
#endif

extern void ov82_0223FCFC(void *obj, u32 mode);

void ov82_0223F300(BattleHallApp *app);

void ov82_0223F300(BattleHallApp *app) {
    int moved = 0;
    u32 keys;
    u8 pos;
    u8 prev;

    keys = gSystem.newKeys;
    if (keys & 0x20) {
        if (ov80_02237920(app->cursorPos) != 0xfe) {
            app->prevCursorPos = app->cursorPos;
        }
        pos = app->cursorPos;
        if (((s32)pos % 4) == 0) {
            app->cursorPos = pos + 3;
        } else {
            if (ov80_02237920(pos) == 0xfe) {
                app->cursorPos = 0x10;
            } else {
                app->cursorPos = app->cursorPos - 1;
            }
        }
        moved = 1;
    }

    keys = gSystem.newKeys;
    if (keys & 0x10) {
        if (ov80_02237920(app->cursorPos) != 0xfe) {
            app->prevCursorPos = app->cursorPos;
        }
        pos = app->cursorPos;
        if (((s32)pos % 4) == 3) {
            app->cursorPos = pos - 3;
        } else {
            if (ov80_02237920(pos) == 0xfe) {
                app->cursorPos = 0x13;
            } else {
                app->cursorPos = app->cursorPos + 1;
            }
        }
        moved = 1;
    }

    keys = gSystem.newKeys;
    if (keys & 0x40) {
        if (ov80_02237920(app->cursorPos) != 0xfe) {
            app->prevCursorPos = app->cursorPos;
        }
        pos = app->cursorPos;
        if (pos < 4) {
            app->cursorPos = pos + 0x10;
        } else {
            if (ov80_02237920(pos) == 0xfe) {
                prev = app->prevCursorPos;
                if (prev == 0x10) {
                    app->cursorPos = 0xd;
                } else if (prev == 0x13) {
                    app->cursorPos = 0xe;
                } else if (prev == 0xd || prev == 1) {
                    app->cursorPos = 0xd;
                } else if (prev == 0xe || prev == 2) {
                    app->cursorPos = 0xe;
                } else {
                    app->cursorPos = 0xd;
                }
            } else {
                app->cursorPos = app->cursorPos - 4;
            }
        }
        moved = 1;
    }

    keys = gSystem.newKeys;
    if (keys & 0x80) {
        if (ov80_02237920(app->cursorPos) != 0xfe) {
            app->prevCursorPos = app->cursorPos;
        }
        pos = app->cursorPos;
        app->cursorPos = (pos >= 0x10) ? pos - 0x10 : pos + 4;
        moved = 1;
    }

    if (moved == 1) {
        u32 x;
        u32 y;
        PlaySE(0x5dc);
        x = ov82_0223F558(app);
        y = ov82_0223F570(app);
        ov82_0223FCBC(((void *)app->cursorSprite), x, y);
    }

    if (ov80_02237920(app->cursorPos) == 0xfe) {
        ov82_0223FCFC(((void *)app->cursorSprite), 2);
        ov82_0223FCBC(((void *)app->cursorSprite), 0x80, 0xa8);
        return;
    }
    ov82_0223FCFC(((void *)app->cursorSprite), 1);
}
