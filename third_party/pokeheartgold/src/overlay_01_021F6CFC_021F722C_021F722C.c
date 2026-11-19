#include "global.h"

#include "message_format.h"
#include "msgdata.h"
#include "overlay_01_021F6CFC.h"
#include "overlay_01_unk_struct_ov5_021_eae78.h"
#include "pm_string.h"

void ov01_021F722C(UnkStruct_ov5_021EAE78 *dest) {
    MI_CpuFill8(dest, 0U, sizeof(UnkStruct_ov5_021EAE78));
    dest->unk_38 = MessageFormat_New(HEAP_ID_FIELD1);
    dest->unk_3C = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x30B, HEAP_ID_FIELD1);
    dest->unk_0C = String_New(0x6EU, HEAP_ID_FIELD1);
    dest->unk_08 = String_New(0x6EU, HEAP_ID_FIELD1);
}
