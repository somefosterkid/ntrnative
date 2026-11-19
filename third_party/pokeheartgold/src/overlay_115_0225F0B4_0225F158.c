#include "global.h"

#include "message_format.h"
#include "msgdata.h"
#include "overlay_115.h"
#include "pm_string.h"

String *ov115_0225F158(u32 arg0, enum HeapID heapID) {
    u32 trainerId;
    MessageFormat *messageFormat;
    MsgData *msgData;
    String *string;
    String *dest;

    trainerId = arg0;
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0xBD, heapID);
    messageFormat = MessageFormat_New(heapID);
    dest = String_New(0x80U, heapID);
    string = String_New(0x80U, heapID);
    ReadMsgDataIntoString(msgData, 0, string);
    BufferTrainerName(messageFormat, 0U, trainerId);
    StringExpandPlaceholders(messageFormat, dest, string);
    DestroyMsgData(msgData);
    MessageFormat_Delete(messageFormat);
    String_Delete(string);
    return dest;
}

String *ov115_0225F1BC(SaveData *arg0, enum HeapID heapID) {
    SaveData *saveData;
    MessageFormat *messageFormat;
    MsgData *msgData;
    String *string;
    String *dest;

    saveData = arg0;
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0xBD, heapID);
    messageFormat = MessageFormat_New(heapID);
    dest = String_New(0x80U, heapID);
    string = String_New(0x80U, heapID);
    ReadMsgDataIntoString(msgData, 0, string);
    BufferRivalsName(messageFormat, 0U, saveData);
    StringExpandPlaceholders(messageFormat, dest, string);
    DestroyMsgData(msgData);
    MessageFormat_Delete(messageFormat);
    String_Delete(string);
    return dest;
}
