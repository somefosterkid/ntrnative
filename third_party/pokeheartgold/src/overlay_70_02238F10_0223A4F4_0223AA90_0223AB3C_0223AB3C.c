#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"

void ov70_022391F0(void *, s32, s32);

extern BgTemplate ov70_022454B4;

extern BgTemplate ov70_022454D0;

void ov70_0223AB3C(BgConfig *arg0, s32 arg1);

void ov70_0223AB3C(BgConfig *arg0, s32 arg1) {
    BgTemplate sp1C;
    BgTemplate sp0;

    sp1C = ov70_022454D0;
    InitBgFromTemplate(arg0, 0U, &sp1C, 0U);
    BgClearTilemapBufferAndCommit(arg0, 0U);
    sp0 = ov70_022454B4;
    InitBgFromTemplate(arg0, 1U, &sp0, 0U);
    BG_ClearCharDataRange(0U, 0x20U, 0U, HEAP_ID_61);
    if (GXx_GetMasterBrightness_((u16 *)REG_DB_MASTER_BRIGHT_ADDR) == 0) {
        ov70_022391F0((u8 *)arg0, arg1, 1);
    } else {
        ov70_022391F0((u8 *)arg0, arg1, 0);
    }
    GfGfx_EngineATogglePlanes(4U, 0U);
    GfGfx_EngineATogglePlanes(8U, 0U);
}
