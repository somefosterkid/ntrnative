#include "global.h"

#include "error_handling.h"
#include "filesystem.h"
#include "heap.h"
#include "overlay_85.h"
#include "overlay_manager.h"
#include "system.h"
#include "unk_020210A0.h"
#include "vram_transfer_manager.h"
#include "overlay_85_ov109_021D0F70.h"

s32 ov85_021E67CC(void *);

s32 ov85_021E75B8(void *);

SysTask *ov85_021E7658(void *);

s32 ov85_021E7D40(void *);

s32 ov85_021E7E78(void *);

s32 ov85_021E7FB0(void *);

s32 ov85_021E8118(void *);

s32 ov85_021E833C(void *);

s32 ov85_021E8418(void *);

s32 ov85_021E5A34(OverlayManager *man) {
    UnkStruct_ov109_021D0F70 *v0;

    v0 = OverlayManager_GetData(man);
    if (DisableTouchPad() != 1) {
        GF_AssertFail();
    }
    ov85_021E7D40(v0);
    ov85_021E7E78(v0);
    ov85_021E7FB0(v0);
    ov85_021E8118(v0);
    ov85_021E833C(v0);
    ov85_021E8418(v0);
    ov85_021E75B8(v0);
    ov85_021E7658(v0);
    ov85_021E67CC(v0);
    Main_SetVBlankIntrCB(NULL, NULL);
    GF_DestroyVramTransferManager();
    NARC_Delete(v0->unk_D80);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_102);
    sub_0201A738(2);
    return 1;
}
