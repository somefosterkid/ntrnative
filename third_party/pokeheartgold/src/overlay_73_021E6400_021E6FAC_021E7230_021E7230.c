#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "unk_02035900.h"
#include "overlay_73_unk_struct_020961_e8_t.h"

s32 ov73_021E71E4(void *, s32, s8);

void ov73_021E7230(UnkStruct_020961E8 *arg0);

void ov73_021E7230(UnkStruct_020961E8 *arg0) {
    String *temp_r0;
    String *temp_r0_2;
    s32 temp_r6;

    if (sub_0203769C() == 0) {
        ov73_021E71E4(arg0, 0, 0);
        temp_r0 = NewString_ReadMsgData(arg0->unk_28, 0x15);
        temp_r6 = 0x40 - FontID_String_GetWidth(FONT_UNK_4, temp_r0, 0U);
        AddTextPrinterParameterizedWithColor(&arg0->unk_2FC[2], FONT_UNK_4, temp_r0, temp_r6 / 2, 0U, 0U, 0x40100U, NULL);
        String_Delete(temp_r0);
    }
    arg0->unk_4A38 = -1;
    ov73_021E71E4(arg0, 1, 0);
    temp_r0_2 = NewString_ReadMsgData(arg0->unk_28, 0x16);
    temp_r6 = 0x40 - FontID_String_GetWidth(FONT_UNK_4, temp_r0_2, 0U);
    AddTextPrinterParameterizedWithColor(&arg0->unk_2FC[3], FONT_UNK_4, temp_r0_2, temp_r6 / 2, 0U, 0U, 0x40100U, NULL);
    String_Delete(temp_r0_2);
    ScheduleBgTilemapBufferTransfer(arg0->unk_00, 1U);
    ScheduleBgTilemapBufferTransfer(arg0->unk_00, 0U);
}
