#include "global.h"

#include "overlay_80_022372D8.h"
#include "unk_02005D10.h"
#include "overlay_82_battle_hall_app.h"

extern u8 gSystem[];

extern u16 ov82_0223F558(void *app);

extern u16 ov82_0223F570(void *app);

extern void ov82_0223FCBC(void *obj, u32 x, u32 y);

extern void ov82_0223FCFC(void *obj, u32 mode);

void ov82_0223F300(u8 *app);

void ov82_0223F300(u8 *app) {
    int moved = 0;
    u32 keys;
    u8 pos;
    u8 prev;

    keys = *(u32 *)(gSystem + 0x48);
    if (keys & 0x20) {
        if (ov80_02237920(app[0xd]) != 0xfe) {
            app[0xc] = app[0xd];
        }
        pos = app[0xd];
        if (((s32)pos % 4) == 0) {
            app[0xd] = pos + 3;
        } else {
            if (ov80_02237920(pos) == 0xfe) {
                app[0xd] = 0x10;
            } else {
                app[0xd] = app[0xd] - 1;
            }
        }
        moved = 1;
    }

    keys = *(u32 *)(gSystem + 0x48);
    if (keys & 0x10) {
        if (ov80_02237920(app[0xd]) != 0xfe) {
            app[0xc] = app[0xd];
        }
        pos = app[0xd];
        if (((s32)pos % 4) == 3) {
            app[0xd] = pos - 3;
        } else {
            if (ov80_02237920(pos) == 0xfe) {
                app[0xd] = 0x13;
            } else {
                app[0xd] = app[0xd] + 1;
            }
        }
        moved = 1;
    }

    keys = *(u32 *)(gSystem + 0x48);
    if (keys & 0x40) {
        if (ov80_02237920(app[0xd]) != 0xfe) {
            app[0xc] = app[0xd];
        }
        pos = app[0xd];
        if (pos < 4) {
            app[0xd] = pos + 0x10;
        } else {
            if (ov80_02237920(pos) == 0xfe) {
                prev = app[0xc];
                if (prev == 0x10) {
                    app[0xd] = 0xd;
                } else if (prev == 0x13) {
                    app[0xd] = 0xe;
                } else if (prev == 0xd || prev == 1) {
                    app[0xd] = 0xd;
                } else if (prev == 0xe || prev == 2) {
                    app[0xd] = 0xe;
                } else {
                    app[0xd] = 0xd;
                }
            } else {
                app[0xd] = app[0xd] - 4;
            }
        }
        moved = 1;
    }

    keys = *(u32 *)(gSystem + 0x48);
    if (keys & 0x80) {
        if (ov80_02237920(app[0xd]) != 0xfe) {
            app[0xc] = app[0xd];
        }
        pos = app[0xd];
        app[0xd] = (pos >= 0x10) ? pos - 0x10 : pos + 4;
        moved = 1;
    }

    if (moved == 1) {
        u32 x;
        u32 y;
        PlaySE(0x5dc);
        x = ov82_0223F558(app);
        y = ov82_0223F570(app);
        ov82_0223FCBC(((void *)((BattleHallApp *)app)->cursorSprite), x, y);
    }

    if (ov80_02237920(app[0xd]) == 0xfe) {
        ov82_0223FCFC(((void *)((BattleHallApp *)app)->cursorSprite), 2);
        ov82_0223FCBC(((void *)((BattleHallApp *)app)->cursorSprite), 0x80, 0xa8);
        return;
    }
    ov82_0223FCFC(((void *)((BattleHallApp *)app)->cursorSprite), 1);
}
