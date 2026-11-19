#include "global.h"

#include "msgdata.h"

String *ov80_0222D0D4(s32 arg0, enum HeapID arg1);

String *ov80_0222D0D4(s32 strno, enum HeapID heapID) {
    MsgData *msgData;
    String *string;

    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0xED, heapID);
    string = NewString_ReadMsgData(msgData, strno);
    DestroyMsgData(msgData);
    return string;
}
