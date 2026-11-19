#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "error_handling.h"
#include "filesystem.h"
#include "overlay_48.h"
#include "pm_string.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_48_unk_struct_ov69_0225ef54.h"

typedef struct UnkStruct_ov48_0225B0A4 {
    MsgData *unk_0;
    MessageFormat *unk_4;
    String *unk_8;
    String *unk_C;
} UnkStruct_ov48_0225B0A4;

typedef struct {
    u8 unk_00;
    u8 unk_01;
} UnkStruct_ov69_0225F0E8;

extern const UnkStruct_ov69_0225F0E8 ov48_0225B238[24];

void ov48_0225B068(void *arg0, enum HeapID arg1);
void ov48_0225B0A4(UnkStruct_ov48_0225B0A4 *arg0);
String *ov48_0225B0C4(void *arg0, s32 arg1);

u16 ov48_0225B038(u32 param0) {
    GF_ASSERT(param0 < 24);
    return ov48_0225B238[param0].unk_00;
}

extern const UnkStruct_ov69_0225F0E8 ov48_0225B239[24];

u16 ov48_0225B050(u32 param0) {
    GF_ASSERT(param0 < 24);
    return ov48_0225B239[param0].unk_00;
}

void ov48_0225B068(void *arg0, enum HeapID heapID) {
    *(MsgData **)arg0 = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x321, heapID);
    *((MessageFormat **)&((UnkStruct_ov69_0225EF54 *)arg0)->unk_04) = MessageFormat_New_Custom(8U, 0x40U, heapID);
    ((UnkStruct_ov69_0225EF54 *)arg0)->unk_08 = String_New(0x80U, heapID);
    ((UnkStruct_ov69_0225EF54 *)arg0)->unk_0C = String_New(0x80U, heapID);
}

void ov48_0225B0A4(UnkStruct_ov48_0225B0A4 *arg0) {
    DestroyMsgData(arg0->unk_0);
    MessageFormat_Delete(arg0->unk_4);
    String_Delete(arg0->unk_8);
    String_Delete(arg0->unk_C);
}

String *ov48_0225B0C4(void *arg0, s32 strno) {
    ReadMsgDataIntoString(*(MsgData **)arg0, strno, ((UnkStruct_ov69_0225EF54 *)arg0)->unk_08);
    return ((UnkStruct_ov69_0225EF54 *)arg0)->unk_08;
}
