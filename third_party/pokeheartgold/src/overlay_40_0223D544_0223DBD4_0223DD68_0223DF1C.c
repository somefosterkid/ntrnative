#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "error_handling.h"
#include "font.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_40_unk_struct_0208_c06_c.h"
#include "overlay_40_unk_struct_ov62_02241204.h"

void ov40_0223DEB8(UnkStruct_0208C06C *param0);

extern s32 ov40_0224565C;

extern u32 ov40_02245868;

typedef struct Words12 {
    u32 w[12];
} Words12;

typedef struct Words3 {
    u32 w[3];
} Words3;

void ov40_0223DF1C(UnkStruct_0208C06C *param_1, s32 param_2);

void ov40_0223DF1C(UnkStruct_0208C06C *param_1, s32 param_2) {
    UnkStruct_ov62_02241204 *iVar1;
    String *string;
    s32 param1 = 0;
    u32 iVar4;
    s32 *psVar5;
    Window *pWVar6;
    s32 *psVar7;
    s32 iStack_58;
    s32 asStack_50[15];

    iVar1 = param_1->unk_860;
    psVar7 = (s32 *)&ov40_02245868;
    psVar5 = asStack_50 + 3;
    *(Words12 *)psVar5 = *(Words12 *)psVar7;
    *(Words3 *)asStack_50 = *(Words3 *)&ov40_0224565C;
    if (param_2 != 0) {
        GF_AssertFail();
    }
    iVar1->unk_798 = 3;
    iStack_58 = 0;
    psVar5 = asStack_50;
    iVar4 = 0x100;
    pWVar6 = iVar1->unk_6D4;
    psVar7 = asStack_50 + 3;
    do {
        InitWindow(pWVar6);
        AddWindowParameterized(param_1->unk_14.unk_10, pWVar6, 6, (u8)*psVar7, (u8)psVar7[1], (u8)psVar7[2], (u8)psVar7[3], 0xe, (u16)iVar4);
        FillWindowPixelBuffer(pWVar6, 0);
        pWVar6 = pWVar6 + 1;
        iVar4 = iVar4 + psVar7[2] * psVar7[3];
        psVar7 = psVar7 + 4;
        iStack_58 = iStack_58 + 1;
    } while (iStack_58 < 3);
    iVar4 = 0;
    pWVar6 = iVar1->unk_6D4;
    do {
        string = NewString_ReadMsgData(param_1->unk_14.unk_34, *psVar5);
        AddTextPrinterParameterizedWithColor(pWVar6, FONT_SYSTEM, string, 0, 0, 0xff, 0xf0d00, (PrinterCallback_t)0x0);
        ScheduleWindowCopyToVram(pWVar6);
        String_Delete(string);
        pWVar6 = pWVar6 + 1;
        psVar5 = psVar5 + 1;
        iVar4 = iVar4 + 1;
    } while ((s32)iVar4 < 2);
    ov40_0223DEB8(param_1);
    return;
}
