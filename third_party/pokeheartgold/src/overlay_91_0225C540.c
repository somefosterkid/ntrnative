#include "global.h"

#include "heap.h"
#include "overlay_91.h"
#include "overlay_manager.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"

s32 ov90_02258800(void *, BOOL, void *, BOOL, void *);

s32 ov91_0225CBE4(void *, s32);

s32 ov91_0225C540(OverlayManager *man) {
    void *v0;
    void *v1;

    v1 = OverlayManager_GetArgs(man);
    Heap_Create(HEAP_ID_3, HEAP_ID_106, 0x60000U);
    v0 = OverlayManager_CreateAndGetData(man, 0x88U, HEAP_ID_106);
    memset(v0, 0, 0x88U);
    ov90_02258800(v0 + 8, ((UnkStruct_ov66_02230DBC *)v1)->unk_39, *((void **)&((UnkStruct_ov66_02230DBC *)v1)->unk_34), ((UnkStruct_ov66_02230DBC *)v1)->unk_38, v1);
    ov91_0225CBE4(v0, 0x6A);
    return 1;
}
