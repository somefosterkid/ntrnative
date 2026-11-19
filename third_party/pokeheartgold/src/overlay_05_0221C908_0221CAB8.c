#include "global.h"

#include "bg_window.h"
#include "error_handling.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "msgdata.h"
#include "overlay_05.h"
#include "overlay_05_unk_struct_ov10_0221_fb28_t.h"
#include "pm_string.h"
#include "text.h"
#include "unk_020379A0.h"

s32 ov05_0221CAB8(UnkStruct_ov10_0221FB28 *arg0) {
    u8 bVar1;
    s32 iVar2;
    BOOL BVar3;

    switch (arg0->unk_B82) {
    case 0:
        iVar2 = ov05_0221E9F8(arg0);
        if (iVar2 == 0) {
            GfGfx_EngineATogglePlanes(1, 0);
        }
        ov05_0221E9C4(arg0);
        ReadMsgDataIntoString(arg0->unk_BAC, 9, arg0->unk_BB4);
        bVar1 = AddTextPrinterParameterized(&arg0->unk_B88, FONT_MESSAGE, arg0->unk_BB4, 0, 0, 0, (PrinterCallback_t)0x0);
        arg0->unk_BB8 = (u32)bVar1;
        sub_02037AC0(0x3e);
        if ((arg0->unk_BC8 == 0) && (iVar2 = ov05_0221E9F8(arg0), iVar2 == 1)) {
            GfGfx_EngineATogglePlanes(1, 1);
        }
        arg0->unk_B82 = arg0->unk_B82 + 1;
        break;
    case 1:
        BVar3 = sub_02037B38(0x3e);
        if (BVar3 != 0) {
            arg0->unk_B82 = 0;
            return 1;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    ScheduleBgTilemapBufferTransfer(arg0->unk_0C, 0);
    return 0;
}
