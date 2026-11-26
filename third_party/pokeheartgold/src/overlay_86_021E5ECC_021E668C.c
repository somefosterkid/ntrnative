#include "global.h"

#include "message_format.h"
#include "msgdata.h"
#include "overlay_86_frontier_records_app.h"
#include "pm_string.h"

void ov86_021E668C(FrontierRecordsApp *arg0, s32 strno) {
    MsgData *msgData;
    String *string;

    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0xED, HEAP_ID_121);
    string = NewString_ReadMsgData(msgData, strno);
    DestroyMsgData(msgData);
    BufferString(arg0->msgFmt, 0U, string, 0, 0, 2);
    String_Delete(string);
}
