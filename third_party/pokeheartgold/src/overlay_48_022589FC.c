#include "global.h"

#include "heap.h"
#include "overlay_48.h"
#include "overlay_manager.h"
#include "system.h"

s32 ov48_022594A8(void *);

s32 ov48_02259868(void *);

s32 ov48_02259984(void *);

s32 ov48_02259C38(void *);

s32 ov48_02259D94(void *);

s32 ov48_02259F14(void *);

s32 ov48_0225A108(void *, void *);

s32 ov48_0225B0A4(void *);

s32 ov48_022589FC(OverlayManager *man, int *state) {
    void *v0;

    v0 = OverlayManager_GetData(man);
    OverlayManager_GetArgs(man);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    ov48_0225A108(v0 + 0xC40C, v0 + 0x20);
    ov48_02259F14(v0 + 0xC700);
    ov48_02259D94(v0 + 0xC3E0);
    ov48_02259C38(v0 + 0xC3CC);
    ov48_02259868(v0 + 0x178);
    ov48_02259984(v0 + 0x224);
    ov48_0225B0A4(v0 + 0x168);
    ov48_022594A8(v0 + 0x20);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_112);
    return 1;
}
