#include "global.h"

#include "constants/heap.h"

#include "overlay_43_palpad.h"
#include "overlay_43_palpad_fns.h"
#include "system.h"

void ov43_0222A148(void *param0);
void ov43_0222D584(PalPadDetails *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);
void ov43_0222A48C(PalPadGfx *gfx, PalPadShared *shared, enum HeapID heapID);
void ov43_0222A154(PalPadWork *work, enum HeapID heapID);
void ov43_0222B0A0(PalPadMenu *menu, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID);
void ov43_0222CD3C(PalPadNaming *naming, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID);
void ov43_0222CE48(PalPadFriendCode *friendCode, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID);

void ov43_0222A154(PalPadWork *work, enum HeapID heapID) {
    ov43_0222A48C(&work->gfx, &work->shared, heapID);

    ov43_0222B0A0(&work->menu, &work->shared, &work->gfx, heapID);
    ov43_0222B944(&work->roster, &work->shared, &work->gfx, heapID);
    ov43_0222CD3C(&work->naming, &work->shared, &work->gfx, heapID);
    ov43_0222CE48(&work->friendCode, &work->shared, &work->gfx, heapID);
    ov43_0222D584(&work->details, &work->shared, &work->gfx, heapID);

    Main_SetVBlankIntrCB(ov43_0222A148, work);
    HBlankInterruptDisable();
}
