#include "global.h"

#include "bg_window.h"
#include "pm_string.h"
#include "render_text.h"
#include "overlay_90.h"
#include "render_window.h"
#include "text.h"
#include "overlay_90_unk_struct_ov114_0225d338.h"

void ov90_022594FC(UnkStruct_ov114_0225D338 *param0, u32 param1);

void ov90_02259434(UnkStruct_ov114_0225D338 *param0);

u16 ov90_02259314(u32, u32, u8 *, void *, u32);
void ov90_02259570(void *, s32);
extern u32 ov90_0225C34C;

void ov90_02259434(UnkStruct_ov114_0225D338 *param0) {
    int v0;

    for (v0 = 0; v0 < 2; v0++) {
        ov90_022594FC(param0, v0);
        String_Delete(param0->unk_24[v0]);
        RemoveWindow(&param0->unk_00[v0]);
    }

    TextFlags_EndAutoScroll();
}

void ov90_02259464(u8 *param_1, u32 param_2, u32 param_3, s32 param_4) {
    u8 uVar1;
    u16 uVar2;
    u8 *iVar3;
    s32 iVar4;
    u32 uVar5;
    u16 *puVar6;
    s32 iVar7;

    if (*(s32 *)(param_1 + 0x34) == 1) {
        puVar6 = (u16 *)&ov90_0225C34C;
        uVar5 = 0;
        do {
            if (param_3 == *puVar6) {
                param_3 = (u32)puVar6[1];
            }
            uVar5 = uVar5 + 1;
            puVar6 = puVar6 + 2;
        } while (uVar5 < 0x14);
    }
    iVar3 = param_1 + 0x20;
    iVar4 = param_4 * 2;
    uVar1 = TextPrinterCheckActive((u8) * (u16 *)(iVar3 + iVar4));
    if (uVar1 != 0) {
        RemoveTextPrinter((u8) * (u16 *)(iVar3 + iVar4));
    }
    ov90_02259570(param_1, param_4);
    iVar7 = param_4 * 0x10;
    FillWindowPixelBuffer((Window *)(param_1 + iVar7), 0xf);
    uVar2 = ov90_02259314(param_2, param_3, param_1 + iVar7, *(void **)(param_1 + param_4 * 4 + 0x24), 1);
    *(u16 *)(iVar3 + iVar4) = uVar2;
    DrawFrameAndWindow2((Window *)(param_1 + iVar7), 1, 1, 0xd);
    ScheduleWindowCopyToVram((Window *)(param_1 + iVar7));
    return;
}
