#include "global.h"

#include "message_format.h"
#include "msgdata.h"
#include "overlay_90_unk_struct_ov114_0225_d084.h"

void ov90_0225928C(void *arg0, String *arg1, s32 arg2);

void ov90_0225928C(void *arg0, String *dest, s32 strno) {
    ReadMsgDataIntoString(*(MsgData **)arg0, strno, ((UnkStruct_ov114_0225D084 *)arg0)->unk_0C);
    StringExpandPlaceholders(((MessageFormat *)((UnkStruct_ov114_0225D084 *)arg0)->unk_04), dest, ((UnkStruct_ov114_0225D084 *)arg0)->unk_0C);
}
