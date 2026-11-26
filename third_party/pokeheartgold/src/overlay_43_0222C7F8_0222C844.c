#include "global.h"

#include "sprite_system.h"
#include "overlay_43_unk_struct_ov64_0222_e21_c.h"
#include "overlay_43_palpad.h"

s32 ov43_0222AD74(void *, s32, s32);

extern const PalPadCursorBox ov43_0222EFA0[9];

void ov43_0222C844(PalPadRosterPage *pages, PalPadGfx *gfx, s32 cursor);

void ov43_0222C844(PalPadRosterPage *pages, PalPadGfx *gfx, s32 cursor) {
    const PalPadCursorBox *box;

    if (cursor == 0xA) {
        ov43_0222AD74(gfx, 0, 0xFF);
        return;
    }
    box = &ov43_0222EFA0[cursor];
    Sprite_SetPositionXY(gfx->cursor, (s16)((box->x + 7) * 8), (s16)((box->y + 2) * 8));
    ov43_0222AD74(gfx, 1, 0xFF);
}
