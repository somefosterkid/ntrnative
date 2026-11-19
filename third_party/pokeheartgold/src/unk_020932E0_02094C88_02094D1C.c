#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "unk_020932E0.h"

void sub_02094D1C(UnkStruct_020932E0 *arg0) {
    MsgData *temp_r0;
    String *temp_r4;

    temp_r0 = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x12A, arg0->unk_4);
    temp_r4 = NewString_ReadMsgData(temp_r0, 0);
    FillWindowPixelBuffer(&arg0->unk_4670, 0U);
    if (arg0->unk_13 != 0) {
        int width = GetWindowWidth(&arg0->unk_4670) << 3;
        AddTextPrinterParameterizedWithColor(&arg0->unk_4670, FONT_UNK_4, temp_r4, FontID_String_GetCenterAlignmentX(FONT_UNK_4, temp_r4, 0U, width), 0U, 0xFFU, 0xF0100U, NULL);
    }
    CopyWindowToVram(&arg0->unk_4670);
    String_Delete(temp_r4);
    DestroyMsgData(temp_r0);
}
