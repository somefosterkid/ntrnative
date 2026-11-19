#include "global.h"
#include "overlay_108_safari_decoration_app.h"

#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_108_021E8850.h"
#include "pm_string.h"

void ov108_021E9C3C(SafariDecorationApp *arg0) {
    s32 strno;
    FontID_Alloc(FONT_UNK_4, arg0->heapID);
    arg0->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x1AE, arg0->heapID);
    arg0->msgFormat = MessageFormat_New_Custom(2U, 0x10U, arg0->heapID);
    arg0->strBuf0 = String_New(0x100U, arg0->heapID);
    arg0->strBuf1 = String_New(0x100U, arg0->heapID);
    arg0->unk_32C = NewString_ReadMsgData(arg0->msgData, 0xC);
    arg0->unk_330 = NewString_ReadMsgData(arg0->msgData, 8);
    strno = 0;
    do {
        arg0->strings[strno] = NewString_ReadMsgData(arg0->msgData, strno);
        strno += 1;
    } while (strno < 4);
}

void ov108_021E9CD0(SafariDecorationApp *arg0) {
    s32 v0;
    v0 = 0;
    do {
        String_Delete(arg0->strings[v0]);
        v0 += 1;
    } while (v0 < 4);
    String_Delete(arg0->unk_330);
    String_Delete(arg0->unk_32C);
    String_Delete(arg0->strBuf1);
    String_Delete(arg0->strBuf0);
    MessageFormat_Delete(arg0->msgFormat);
    DestroyMsgData(arg0->msgData);
    FontID_Release(FONT_UNK_4);
}
