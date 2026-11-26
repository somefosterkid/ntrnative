#include "global.h"
#include "overlay_43_palpad.h"
#include "overlay_43_palpad_fns.h"

void ov43_0222C3E8(PalPadRoster *roster, PalPadGfx *gfx, s16 page);
void ov43_0222C7F8(PalPadRosterPage *page, PalPadGfx *gfx, PalPadScreens *screens);
void ov43_0222C844(PalPadRosterPage *page, PalPadGfx *gfx, s32 cursor);
void ov43_0222C918(PalPadRosterPage *page, PalPadShared *shared, PalPadGfx *gfx, struct UnkStruct_ov42_02227060 *icons, PalPadScreens *screens, PalPadFriendList *friends, u32 pageNum, enum HeapID heapID);

BOOL ov43_0222C358(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx) {
    if (ov43_0222C5D8(roster, shared) == 0) {
        return FALSE;
    }
    ov43_0222ADB8(shared, gfx, 1);
    return TRUE;
}

void ov43_0222C378(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, s16 page, s32 cursor, u32 slot, enum HeapID heapID) {
    if (slot == 0) {
        roster->page = page;
        roster->cursor = cursor;
        ov43_0222C3E8(roster, gfx, page);
    }
    ov43_0222C7F8(&roster->pages[slot], gfx, &gfx->screens);
    ov43_0222C918(&roster->pages[slot], shared, gfx, roster->trainerIcons, &gfx->screens, &shared->friends, page, heapID);
    ov43_0222C844(&roster->pages[slot], gfx, cursor);
}
