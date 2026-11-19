#include "global.h"

#include "bg_window.h"
#include "render_window.h"
#include "text.h"
#include "overlay_90.h"
#include "pm_string.h"
#include "overlay_90_unk_struct_ov114_0225d338.h"

void ov90_02259570(UnkStruct_ov114_0225D338 *param0, u32 param1);

void ov90_022594FC(UnkStruct_ov114_0225D338 *param0, u32 param1);

void ov90_02259554(UnkStruct_ov114_0225D338 *param0, u32 param1);

void ov90_022594FC(UnkStruct_ov114_0225D338 *param0, u32 param1) {
    ov90_02259570(param0, param1);

    if (TextPrinterCheckActive(param0->unk_20[param1])) {
        RemoveTextPrinter(param0->unk_20[param1]);
    }

    ClearFrameAndWindow2(&param0->unk_00[param1], 1);
    ClearWindowTilemapAndScheduleTransfer(&param0->unk_00[param1]);
}

s32 ov90_02259538(s32 arg0, s32 arg1) {
    if (TextPrinterCheckActive((u8) * (u16 *)((u8 *)(arg0 + (arg1 * 2)) + 0x20)) == 0) {
        return 1;
    }
    return 0;
}

void ov90_02259554(UnkStruct_ov114_0225D338 *param0, u32 param1) {
    if (param0->unk_2C[param1] == NULL) {
        param0->unk_2C[param1] = WaitingIcon_New(&param0->unk_00[param1], 1);
    }
}
