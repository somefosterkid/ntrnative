#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"

void ov81_02243140(BgConfig *arg0, NARC *arg1, s32 arg2);

void ov81_02243140(BgConfig *arg0, NARC *arg1, s32 arg2) {
    void *buf;
    NNSG2dScreenData *scrn;
    u16 *data;
    u8 h;
    u8 i, j;
    u8 y;

    buf = GfGfxLoader_GetScrnDataFromOpenNarc(arg1, arg2 + 0x87, TRUE, &scrn, HEAP_ID_100);
    if (arg2 == 0) {
        y = 11;
        h = 7;
    } else {
        y = 14;
        h = 4;
    }
    data = (u16 *)scrn;
    data = (u16 *)scrn->rawData;
    for (i = 0; i < h; i++) {
        for (j = 0; j < 32; j++) {
            FillBgTilemapRect(arg0, 5, data[i * 32 + j] + 0x3D9, j, y + i, 1, 1, 9);
        }
    }
    Heap_Free(buf);
}
