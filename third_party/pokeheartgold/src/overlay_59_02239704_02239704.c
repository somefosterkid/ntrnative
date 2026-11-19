#include "global.h"
#include "overlay_59_apricorn_app.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"

void ov59_02239704(Ov59ApricornApp *arg0);

void ov59_02239704(Ov59ApricornApp *arg0) {
    String *temp_r0;
    s32 i;

    FontID_Alloc(FONT_UNK_4, arg0->heapId);
    arg0->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x15, arg0->heapId);
    arg0->msgFormat = MessageFormat_New_Custom(0x10U, 0x20U, arg0->heapId);
    arg0->unk_64 = String_New(0x100U, arg0->heapId);
    arg0->unk_68 = String_New(0x100U, arg0->heapId);
    arg0->unk_6C = NewString_ReadMsgData(arg0->msgData, 1);
    arg0->unk_74 = NewString_ReadMsgData(arg0->msgData, 0x19);
    for (i = 0; i < 0x16; i++) {
        arg0->unk_78[i] = NewString_ReadMsgData(arg0->msgData, i + 0x1B);
    }
    for (i = 0; i < 7; i++) {
        temp_r0 = NewString_ReadMsgData(arg0->msgData, i + 7);
        arg0->apricornNames[i] = temp_r0;
    }
}
