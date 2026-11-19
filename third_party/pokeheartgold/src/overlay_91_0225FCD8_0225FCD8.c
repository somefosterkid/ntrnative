#include "global.h"
struct UnkStruct_ov115_02261ADC;

#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "overlay_91_unk_struct_ov115_02261_adc.h"

void ov91_0225FCD8(struct UnkStruct_ov115_02261ADC *arg0, enum HeapID arg1);

void ov91_0225FCD8(UnkStruct_ov115_02261ADC *arg0, enum HeapID heapID) {
    *((MessageFormat * *)&arg0->unk_04) = MessageFormat_New(heapID);
    *((MsgData * *)&arg0->unk_08) = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xE6, heapID);
    arg0->unk_0C = String_New(0x80U, heapID);
    arg0->unk_10 = String_New(0x80U, heapID);
}
