#include "global.h"

#include "system.h"
#include "bg_window.h"

s32 ov112_021EBA8C(void);

int ov112_021EBAB0(void);

s32 ov112_021EA64C(void *);
s32 ov112_021EA688(void *, s32);
s32 ov112_021EBAB4(void *arg0);

s32 ov112_021EBA8C(void) {
    if ((1 & gSystem.newKeys) || (gSystem.touchNew != 0)) {
        return 3;
    }
    return 2;
}

int ov112_021EBAB0(void) {
    return sizeof(u32);
}

s32 ov112_021EBAB4(void *arg0) {
    FillBgTilemapRect(*(BgConfig **)((u8 *)arg0 + 0x18), 4U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    FillBgTilemapRect(*(BgConfig **)((u8 *)arg0 + 0x18), 5U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    FillBgTilemapRect(*(BgConfig **)((u8 *)arg0 + 0x18), 1U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    FillBgTilemapRect(*(BgConfig **)((u8 *)arg0 + 0x18), 2U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    ov112_021EA64C(arg0);
    ov112_021EA688(arg0, 7);
    BgCommitTilemapBufferToVram(*(BgConfig **)((u8 *)arg0 + 0x18), 4U);
    BgCommitTilemapBufferToVram(*(BgConfig **)((u8 *)arg0 + 0x18), 5U);
    BgCommitTilemapBufferToVram(*(BgConfig **)((u8 *)arg0 + 0x18), 1U);
    BgCommitTilemapBufferToVram(*(BgConfig **)((u8 *)arg0 + 0x18), 2U);
    return 2;
}
