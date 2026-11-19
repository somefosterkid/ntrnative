#include "global.h"

#include "message_format.h"
#include "msgdata.h"
#include "overlay_48_unk_struct_ov69_0225ef54.h"

String *ov48_0225B0E0(void *arg0, u32 arg1);

String *ov48_0225B0E0(void *arg0, u32 countryId) {
    ReadMsgDataIntoString(*(MsgData **)arg0, 3, ((UnkStruct_ov69_0225EF54 *)arg0)->unk_0C);
    BufferCountryName(((MessageFormat *)((UnkStruct_ov69_0225EF54 *)arg0)->unk_04), 0U, countryId);
    StringExpandPlaceholders(((MessageFormat *)((UnkStruct_ov69_0225EF54 *)arg0)->unk_04), ((UnkStruct_ov69_0225EF54 *)arg0)->unk_08, ((UnkStruct_ov69_0225EF54 *)arg0)->unk_0C);
    return ((UnkStruct_ov69_0225EF54 *)arg0)->unk_08;
}

String *ov48_0225B108(void *arg0, u32 arg1, u32 arg2);

String *ov48_0225B108(void *arg0, u32 countryId, u32 cityId) {
    MessageFormat_ResetBuffers(((MessageFormat *)((UnkStruct_ov69_0225EF54 *)arg0)->unk_04));
    ReadMsgDataIntoString(*(MsgData **)arg0, 3, ((UnkStruct_ov69_0225EF54 *)arg0)->unk_0C);
    BufferCityName(((MessageFormat *)((UnkStruct_ov69_0225EF54 *)arg0)->unk_04), 0U, countryId, cityId);
    StringExpandPlaceholders(((MessageFormat *)((UnkStruct_ov69_0225EF54 *)arg0)->unk_04), ((UnkStruct_ov69_0225EF54 *)arg0)->unk_08, ((UnkStruct_ov69_0225EF54 *)arg0)->unk_0C);
    return ((UnkStruct_ov69_0225EF54 *)arg0)->unk_08;
}
