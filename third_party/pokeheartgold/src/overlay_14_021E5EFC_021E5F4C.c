#include "global.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"

#include "bg_window.h"
#include "sprite_system.h"
#include "overlay_14_box_funcs.h"

typedef struct PalFadeState {
    u8 isBg : 1;
    u8 idx : 7;
    u8 pal : 4;
    u8 pal2 : 4;
    u8 step;
    u8 timer;
    u8 x;
    u8 y;
    u8 w;
    u8 h;
} PalFadeState;

s32 ov14_021E5F4C(UnkStruct_ov14_boxapp *arg0) {
    UnkStruct_ov14_boxsys *owner = arg0->sys;
    PalFadeState *st = (PalFadeState *)owner->unk_88D4;

    switch (st->step) {
    case 0:
        if (st->isBg == 0) {
            ManagedSprite_SetPaletteOverride(owner->sprites[st->idx], st->pal);
        } else {
            BgTilemapRectChangePalette(owner->unk_14, st->idx, st->x, st->y, st->w, st->h, st->pal);
            ScheduleBgTilemapBufferTransfer(arg0->sys->unk_14, st->idx);
        }
        st->step++;
        break;
    case 1:
        st->timer++;
        if (st->timer == 4) {
            if (st->isBg == 0) {
                ManagedSprite_SetPaletteOverride(arg0->sys->sprites[st->idx], st->pal2);
            } else {
                BgTilemapRectChangePalette(arg0->sys->unk_14, st->idx, st->x, st->y, st->w, st->h, st->pal2);
                ScheduleBgTilemapBufferTransfer(arg0->sys->unk_14, st->idx);
            }
            st->timer = 0;
            st->step++;
        }
        break;
    case 2:
        st->timer++;
        if (st->timer == 2) {
            return 0;
        }
        break;
    }
    return 1;
}
