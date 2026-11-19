#include "global.h"

#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "overlay_49_unk_struct_ov70_0225f208.h"

void ov49_0225B35C(UnkStruct_ov70_0225F208 *arg0);

void ov49_0225B35C(UnkStruct_ov70_0225F208 *arg0) {
    s32 v0;

    MessageFormat_Delete(arg0->unk_00);
    v0 = 0;
    do {
        DestroyMsgData(arg0->unk_04[v0]);
        v0 += 1;
    } while (v0 < 4);
    String_Delete(arg0->unk_14);
    String_Delete(arg0->unk_18);
}

String *ov49_0225B388(UnkStruct_ov70_0225F208 *arg0, s32 arg1, s32 arg2);

String *ov49_0225B388(UnkStruct_ov70_0225F208 *arg0, s32 arg1, s32 strno) {
    ReadMsgDataIntoString(arg0->unk_04[arg1], strno, arg0->unk_18);
    StringExpandPlaceholders(arg0->unk_00, arg0->unk_14, arg0->unk_18);
    return arg0->unk_14;
}

void ov49_0225B3A8(UnkStruct_ov70_0225F208 *arg0, s32 arg1, u32 arg2, u32 arg3, enum PrintingMode arg4);

void ov49_0225B3A8(UnkStruct_ov70_0225F208 *arg0, s32 num, u32 numDigits, u32 idx, enum PrintingMode strconvmode) {
    BufferIntegerAsString(arg0->unk_00, idx, num, numDigits, strconvmode, 1);
}
