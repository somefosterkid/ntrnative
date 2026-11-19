#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "overlay_43_palpad.h"
#include "overlay_43_rodata_0222ED5C.h"

void ov43_0222DD40(PalPadDetailsScreen *screens, PalPadGfx *gfx, enum HeapID heapID);

void ov43_0222DD40(PalPadDetailsScreen *screens, PalPadGfx *gfx, enum HeapID heapID) {
    void *pvVar1;
    s32 iVar3;
    u16 *puVar2;
    u16 auStack_20[4];

    auStack_20[0] = _0222ED5C.unk_2E[0];
    auStack_20[1] = _0222ED5C.unk_2E[1];
    auStack_20[2] = _0222ED5C.unk_2E[2];
    auStack_20[3] = _0222ED5C.unk_2E[3];
    puVar2 = auStack_20;
    iVar3 = 0;
    do {
        pvVar1 = GfGfxLoader_GetScrnDataFromOpenNarc(gfx->narc, (u32)*puVar2, 1, &screens->data, heapID);
        screens->file = pvVar1;
        iVar3 = iVar3 + 1;
        screens = screens + 1;
        puVar2 = puVar2 + 1;
    } while (iVar3 < 4);
    return;
}
