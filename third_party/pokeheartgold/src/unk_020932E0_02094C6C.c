#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "bg_window.h"
#include "font.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "unk_020932E0.h"

void sub_02094C6C(u32 memberIndex, NNSG2dCharacterData **param1, void *param2, NARC *param3) {
    NARC_ReadWholeMember(param3, memberIndex, param2);
    NNS_G2dGetUnpackedBGCharacterData(param2, param1);
}

void sub_02094C88(UnkStruct_020932E0 *arg0, s32 arg1) {
    MsgData *msgData;
    String *string;
    String *v0;

    if (arg1 == 0x12) {
        msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x12A, arg0->unk_4);
        v0 = NewString_ReadMsgData(msgData, 1);
        DestroyMsgData(msgData);
    } else {
        string = String_New(0x40U, arg0->unk_4);
        v0 = string;
        arg0->unk_464C(string, arg0->unk_463C, arg1);
    }
    FillWindowPixelBuffer(&arg0->unk_4650, 0U);
    AddTextPrinterParameterizedWithColor(&arg0->unk_4650, FONT_SYSTEM, v0, FontID_String_GetCenterAlignmentX(FONT_SYSTEM, v0, 0U, 0x60U), 0U, 0xFFU, 0x10200U, NULL);
    CopyWindowToVram(&arg0->unk_4650);
    String_Delete(v0);
}
