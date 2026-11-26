#include "global.h"

#include "heap.h"
#include "overlay_manager.h"
#include "system.h"
#include "overlay_41_unk_struct_ov22_0225_b85_c.h"

s32 ov41_02246150(void);

s32 ov41_02246698(void *);

s32 ov41_0224B57C(void *);

s32 ov41_0224BD8C(void *);

s32 ov41_0224BE34(void *);

s32 AccessoryPortrait_Exit(OverlayManager *man, s32 *state);

s32 AccessoryPortrait_Exit(OverlayManager *man, s32 *state) {
    void *v0;

    v0 = OverlayManager_GetData(man);
    ov41_0224B57C(*((void **)&((UnkStruct_ov22_0225B85C *)v0)->unk_10));
    ov41_0224BD8C(v0);
    ov41_0224BE34(v0);
    ov41_02246698(v0 + 0x14);
    ov41_02246150();
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_13);
    Heap_Destroy(HEAP_ID_14);
    return 1;
}

s32 ov41_022466B8(void *);

SysTask *ov41_0224B5C8(void *);

void ov41_0224BBF0(UnkStruct_ov22_0225B85C *arg0);

void ov41_0224BBF0(UnkStruct_ov22_0225B85C *arg0) {
    ov41_0224B5C8(arg0->unk_10);
    ov41_022466B8(&arg0->unk_14);
}
