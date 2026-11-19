#include "global.h"

#include "constants/heap.h"

#include "overlay_43_palpad.h"
#include "unk_02005D10.h"

s32 ov43_0222BD18(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx);
s32 ov43_0222BEEC(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, s32 *result);
int ov43_0222BFA4(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx);
void ov43_0222AD20(PalPadGfx *param0, BOOL param1);
void ov43_0222C378(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, s16 param3, u32 param4, u32 param5, enum HeapID heapID);
void ov43_0222C844(PalPadRosterPage *page, PalPadGfx *gfx, u32 param2);
void ov43_0222BFD4(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

int ov43_0222BFA4(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx) {
    s32 v0;
    s32 v1;

    v0 = 0;
    v1 = ov43_0222BEEC(roster, shared, gfx, &v0);
    if (v0 != 0) {
        shared->inputState = 1;
        return v1;
    }
    return ov43_0222BD18(roster, shared, gfx);
}

void ov43_0222BFD4(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID) {
    u16 v0;

    if (param0->scrollDir == 2) {
        v0 = 2;
    } else {
        v0 = 1;
    }

    ov43_0222C378(param0, param1, param2, param0->page, param0->cursor, v0, heapID);
    ov43_0222C844(&param0->pages[0], param2, 10);
    ov43_0222AD20(param2, 0);

    PlaySE(1505);
}
