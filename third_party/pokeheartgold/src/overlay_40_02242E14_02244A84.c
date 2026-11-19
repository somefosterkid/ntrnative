#include "global.h"

#include "msgdata.h"
#include "overlay_40.h"
#include "pm_string.h"

void ov40_02244A84(String *arg0, enum HeapID heapID) {
    MsgData *msgData;

    String_SetEmpty(arg0);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0xD, heapID);
    ReadMsgDataIntoString(msgData, 0x14C, arg0);
    DestroyMsgData(msgData);
}
