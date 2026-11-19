#include "global.h"

#include "message_format.h"
#include "msgdata.h"
#include "overlay_117.h"
#include "pm_string.h"

String *ov117_0225F470(u32 arg0, enum HeapID heapID) {
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

extern const ExecutiveEncounterParam _0225FACC;

void ov117_0225F4D4(SysTask *task, struct EncounterEffect *encEffect) {
    ov117_0225F524(task, encEffect, &_0225FACC);
}

extern const ExecutiveEncounterParam ov117_0225FAD4;

void ov117_0225F4E4(SysTask *task, struct EncounterEffect *encEffect) {
    ov117_0225F524(task, encEffect, &ov117_0225FAD4);
}

extern const ExecutiveEncounterParam ov117_0225FADC;

void ov117_0225F4F4(SysTask *task, struct EncounterEffect *encEffect) {
    ov117_0225F524(task, encEffect, &ov117_0225FADC);
}

extern const ExecutiveEncounterParam ov117_0225FAE4;

void ov117_0225F504(SysTask *task, struct EncounterEffect *encEffect) {
    ov117_0225F524(task, encEffect, &ov117_0225FAE4);
}

extern const ExecutiveEncounterParam ov117_0225FAEC;

void ov117_0225F514(SysTask *task, struct EncounterEffect *encEffect) {
    ov117_0225F524(task, encEffect, &ov117_0225FAEC);
}
