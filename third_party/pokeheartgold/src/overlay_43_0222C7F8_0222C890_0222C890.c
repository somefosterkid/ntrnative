#include "global.h"
#include "bg_window.h"
#include "overlay_43_palpad.h"

extern const PalPadCursorBox ov43_0222EFA0[9];

void ov43_0222C890(PalPadRosterPage *pages, PalPadGfx *gfx, s32 cursor, s32 highlight);

void ov43_0222C890(PalPadRosterPage *pages, PalPadGfx *gfx, s32 cursor, s32 highlight) {
    s32 i;
    const PalPadCursorBox *box = ov43_0222EFA0;
    u32 palette;
    u32 dy;
    u32 dx;

    for (i = 0; i < 9; i++, box++) {
        if ((highlight == 1) && (cursor == i)) {
            palette = box->selectedPalette;
        } else {
            palette = box->palette;
        }
        dy = pages[0].layout.y;
        if ((s32)dy < 0) {
            dy = 0 - dy;
        }
        dx = pages[0].layout.x;
        if ((s32)dx < 0) {
            dx = 0 - dx;
        }
        BgTilemapRectChangePalette(gfx->bgConfig, box->bgLayer, (u8)(box->x + dx), (u8)(box->y + dy), box->width, box->height, (u8)palette);
    }
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, 2U);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, 0U);
}
