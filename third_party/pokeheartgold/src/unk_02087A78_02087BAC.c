#include "global.h"

#include "frontier/overlay_80.h"

#include "heap.h"
#include "overlay_manager.h"
#include "sound_02004A44.h"
#include "unk_0202FBCC.h"

s32 sub_02087BAC(OverlayManager *arg0);

s32 sub_02087BAC(OverlayManager *man) {
    void *v0;

    v0 = OverlayManager_GetData(man);
    if (sub_0202FC48() == 1) {
        sub_0202FC24();
    }
    Heap_Free(*(void **)((u8 *)v0 + 0x14));
    OverlayManager_FreeData(man);
    GF_SndHandleSetPlayerVolume(1U, 0x7FU);
    GF_SndHandleSetPlayerVolume(7U, 0x7FU);
    Heap_Destroy(HEAP_ID_126);
    return 1;
}
