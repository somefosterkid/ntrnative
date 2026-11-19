#include "global.h"

#include "bg_window.h"
#include "overlay_96_unk_struct_ov96_021_fab24.h"

void ov96_021FAB24(BgConfig *bgConfig, UnkStruct_ov96_021FAB24 *w);

void ov96_021FAB24(BgConfig *bgConfig, UnkStruct_ov96_021FAB24 *w) {
    s32 v;
    u16 base;
    u16 tile;

    switch (w->state) {
    case 0:
        if (w->timer >= 0x70) {
            base = 2;
            tile = base;
            tile += base << 9;
            FillBgTilemapRect(bgConfig, (u8)w->bg0, 2, w->unk_18 * 10 + 2, 0x32, 1, base, 0x10);
            FillBgTilemapRect(bgConfig, (u8)w->bg0, 3, w->unk_18 * 10 + 3, 0x32, 6, 2, 0x10);
            FillBgTilemapRect(bgConfig, (u8)w->bg0, tile, w->unk_18 * 10 + 9, 0x32, 1, base, 0x10);
            FillBgTilemapRect(bgConfig, (u8)w->bg1, 2, w->unk_18 * 10 + 2, 0x32, 1, base, 0x10);
            FillBgTilemapRect(bgConfig, (u8)w->bg1, 3, w->unk_18 * 10 + 3, 0x32, 6, 2, 0x10);
            FillBgTilemapRect(bgConfig, (u8)w->bg1, tile, w->unk_18 * 10 + 9, 0x32, 1, base, 0x10);
            ScheduleBgTilemapBufferTransfer(bgConfig, (u8)w->bg0);
            ScheduleBgTilemapBufferTransfer(bgConfig, (u8)w->bg1);
            w->state = 1;
            w->unk_1A = 0xff;
        }
        break;
    case 1:
        v = w->timer - 0x10;
        if (v >= 0xE48) {
            v = v / 8;
            v = v % 64;
            v = (0x40 - v) % 64;
            v -= 2;
            if (v < 0) {
                v += 0x40;
            }
            w->unk_1B = v;
            FillBgTilemapRect(bgConfig, (u8)w->bg0, 0x60, w->unk_18 * 10 + 2, w->unk_1B, 1, 2, 0x10);
            FillBgTilemapRect(bgConfig, (u8)w->bg0, 0x61, w->unk_18 * 10 + 3, w->unk_1B, 6, 2, 0x10);
            FillBgTilemapRect(bgConfig, (u8)w->bg0, 0x62, w->unk_18 * 10 + 9, w->unk_1B, 1, 2, 0x10);
            FillBgTilemapRect(bgConfig, (u8)w->bg1, 0x60, w->unk_18 * 10 + 2, w->unk_1B, 1, 2, 0x10);
            FillBgTilemapRect(bgConfig, (u8)w->bg1, 0x61, w->unk_18 * 10 + 3, w->unk_1B, 6, 2, 0x10);
            FillBgTilemapRect(bgConfig, (u8)w->bg1, 0x62, w->unk_18 * 10 + 9, w->unk_1B, 1, 2, 0x10);
            ScheduleBgTilemapBufferTransfer(bgConfig, (u8)w->bg0);
            ScheduleBgTilemapBufferTransfer(bgConfig, (u8)w->bg1);
            w->state = 2;
        }
        break;
    case 2:
        v = w->timer - 0x10;
        if (v >= 0x1048) {
            v = v / 8;
            v = v % 64;
            v = (0x40 - v) % 64;
            v -= 2;
            if (v < 0) {
                v += 0x40;
            }
            w->unk_1B = v;
            base = 2;
            tile = base;
            tile += base << 9;
            FillBgTilemapRect(bgConfig, (u8)w->bg0, 2, w->unk_18 * 10 + 2, w->unk_1B, 1, base, 0x10);
            FillBgTilemapRect(bgConfig, (u8)w->bg0, 3, w->unk_18 * 10 + 3, w->unk_1B, 6, 2, 0x10);
            FillBgTilemapRect(bgConfig, (u8)w->bg0, tile, w->unk_18 * 10 + 9, w->unk_1B, 1, base, 0x10);
            FillBgTilemapRect(bgConfig, (u8)w->bg1, 2, w->unk_18 * 10 + 2, w->unk_1B, 1, base, 0x10);
            FillBgTilemapRect(bgConfig, (u8)w->bg1, 3, w->unk_18 * 10 + 3, w->unk_1B, 6, 2, 0x10);
            FillBgTilemapRect(bgConfig, (u8)w->bg1, tile, w->unk_18 * 10 + 9, w->unk_1B, 1, base, 0x10);
            ScheduleBgTilemapBufferTransfer(bgConfig, (u8)w->bg0);
            ScheduleBgTilemapBufferTransfer(bgConfig, (u8)w->bg1);
            w->state = 3;
        }
        break;
    }
}
