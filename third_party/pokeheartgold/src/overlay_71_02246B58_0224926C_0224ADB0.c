#include "global.h"

#include "bg_window.h"

s32 ov71_02246D54(void *);

void ov71_0224ADB0(void *arg0);

void ov71_0224ADB0(void *arg0) {
    if (*(s32 *)((u8 *)arg0 + 0x14) != 0) {
        ov71_02246D54(*(void **)((u8 *)arg0 + 0x10));
    }
    FreeBgTilemapBuffer(*(BgConfig **)((u8 *)arg0 + 0xC), 2U);
    FreeBgTilemapBuffer(*(BgConfig **)((u8 *)arg0 + 0xC), 6U);
    FreeBgTilemapBuffer(*(BgConfig **)((u8 *)arg0 + 0xC), 3U);
    FreeBgTilemapBuffer(*(BgConfig **)((u8 *)arg0 + 0xC), 7U);
}
