#include "global.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "field_system.h"

s32 ov02_0224FC74(FieldSystemUnkSub120 *arg0, FieldSystemUnkSub120_Sub7E4 *arg1);

s32 ov02_0224FC74(FieldSystemUnkSub120 *arg0, FieldSystemUnkSub120_Sub7E4 *arg1) {
    s32 v0;

    v0 = 0;
    arg0->unk86D = 0;
    if (arg1->unk7 != 0) {
        v0 = 1;
    }
    return v0;
}

s32 FollowMon_PlaceholdersSet(void *, MessageFormat *);

void FollowMon_ExpandInteractionMessage(void *arg0, String *arg1, enum HeapID arg2, s32 arg3);

void FollowMon_ExpandInteractionMessage(void *arg0, String *dest, enum HeapID heapID, s32 strno) {
    String *string;
    MessageFormat *messageFormat;
    MsgData *msgData;
    String *v0;

    string = dest;
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x109, heapID);
    messageFormat = MessageFormat_New(heapID);
    FollowMon_PlaceholdersSet(arg0, messageFormat);
    v0 = NewString_ReadMsgData(msgData, strno);
    StringExpandPlaceholders(messageFormat, dest, v0);
    String_Delete(v0);
    MessageFormat_Delete(messageFormat);
    DestroyMsgData(msgData);
}
