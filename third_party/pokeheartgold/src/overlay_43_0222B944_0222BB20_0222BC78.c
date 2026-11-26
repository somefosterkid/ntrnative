#include "global.h"

#include "unk_02005D10.h"
#include "bg_window.h"
#include "overlay_43_palpad.h"

void ov43_0222AD40(PalPadGfx *gfx, u32 param_2, s32 param_3);
void ov43_0222C890(PalPadRosterPage *pages, PalPadGfx *gfx, s32 cursor, s32 highlight);

s32 ov43_0222BC78(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, s32 action);

s32 ov43_0222BC78(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, s32 action) {
    if (action == 0) {
        PlaySE(SEQ_SE_DP_DECIDE);
        ov43_0222C890(roster->pages, gfx, roster->cursor, 1);
        return 3;
    }
    if (action == 5) {
        if (roster->cursor == 8) {
            PlaySE(SEQ_SE_DP_DECIDE);
            ov43_0222C890(roster->pages, gfx, roster->cursor, 1);
            return 3;
        }
        if (shared->friends.count <= roster->cursor + roster->page * 8) {
            return 0;
        } else {
            PlaySE(SEQ_SE_DP_DECIDE);
            ov43_0222C890(roster->pages, gfx, roster->cursor, 1);
            return 4;
        }
    } else if (action == 1) {
        ov43_0222AD40(gfx, 0, 1);
        return 1;
    } else if (action == 2) {
        ov43_0222AD40(gfx, 1, 1);
        return 2;
    }
    return 0;
}
