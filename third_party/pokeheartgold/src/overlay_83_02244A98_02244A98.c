#include "global.h"

#include "message_format.h"

typedef struct UnkStruct_ov83_02244A98 {
    u8 unk_0[0x24];
    MessageFormat *unk_24;
} UnkStruct_ov83_02244A98;

void ov83_02244A98(UnkStruct_ov83_02244A98 *arg0, u32 arg1, s32 arg2, u32 arg3, enum PrintingMode arg4);

void ov83_02244A98(UnkStruct_ov83_02244A98 *arg0, u32 idx, s32 num, u32 numDigits, enum PrintingMode strconvmode) {
    BufferIntegerAsString(arg0->unk_24, idx, num, numDigits, strconvmode, 1);
}
