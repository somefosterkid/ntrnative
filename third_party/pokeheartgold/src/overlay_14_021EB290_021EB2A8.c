#include "global.h"

#include "overlay_manager.h"
#include "overlay_14_box_funcs.h"

extern u8 ov14_021F7D50[];

u32 ov14_021EB2A8(UnkStruct_ov14_boxapp *param_1) {
    BOOL BVar1;

    BVar1 = OverlayManager_Run(param_1->unk_14);
    if (BVar1 == 0) {
        return 10;
    }
    OverlayManager_Delete(param_1->unk_14);
    (*(void (**)(UnkStruct_ov14_boxapp *))(ov14_021F7D50 + 4 + (u32)param_1->unk_1E[0] * 0xc))(param_1);
    param_1->unk30 = *(u32 *)(ov14_021F7D50 + 8 + (u32)param_1->unk_1E[0] * 0xc);
    return 0;
}
