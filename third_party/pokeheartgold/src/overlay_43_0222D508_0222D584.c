#include "global.h"

#include "constants/heap.h"

#include "overlay_43_palpad.h"

extern const PalPadPanelLayout ov43_0222EE08[3];
extern const PalPadWindowSpec *const ov43_0222F10C[8];
extern const u8 ov43_0222F0D0[8];
extern const u32 ov43_0222EDAC[3];

void ov43_0222D584(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID);
void ov43_0222DD40(PalPadDetailsScreen *screens, PalPadGfx *gfx, enum HeapID heapID);
void ov43_0222DDA0(PalPadDetails *details, PalPadGfx *gfx);
void ov43_0222DDF0(PalPadDetailsSlot *slot, PalPadGfx *gfx, const PalPadPanelLayout *layout, u32 baseTile, enum HeapID heapID);
void ov43_0222DE14(PalPadDetailsSlot *slot, PalPadGfx *gfx, u32 index, u32 count, const PalPadWindowSpec *specs, enum HeapID heapID);

void ov43_0222D584(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID) {
    int v0, v1;

    ov43_0222DD40(details->screens, gfx, heapID);
    ov43_0222DDA0(details, gfx);

    for (v0 = 0; v0 < 3; v0++) {
        ov43_0222DDF0(&details->slots[v0], gfx, &ov43_0222EE08[v0], ov43_0222EDAC[v0], heapID);

        for (v1 = 0; v1 < 8; v1++) {
            ov43_0222DE14(&details->slots[v0], gfx, v1, ov43_0222F0D0[v1], ov43_0222F10C[v1], heapID);
        }
    }
}
