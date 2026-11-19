#include "global.h"
struct UnkStruct_ov5_021EAE78;

#include "message_format.h"
#include "text.h"
#include "unk_02034354.h"
#include "overlay_01_unk_struct_ov5_021_eae78.h"

s32 ov01_021F6ED8(struct UnkStruct_ov5_021EAE78 *arg0);

s32 ov01_021F6ED8(UnkStruct_ov5_021EAE78 *arg0) {
    if (TextPrinterCheckActive((u8) *((s32 *)&arg0->unk_40)) == 0) {
        *((s16 *)&arg0->unk_98) = 0;
        *((s32 *)&arg0->unk_44) = 5;
    }
    return 0;
}

s32 ov01_021F6CA0(UnkStruct_ov5_021EAE78 *arg0);

s32 ov01_021F71C4(void *, s32);

s32 ov01_021F6EFC(struct UnkStruct_ov5_021EAE78 *arg0);

s32 ov01_021F6EFC(UnkStruct_ov5_021EAE78 *arg0) {
    if (ov01_021F6CA0(arg0) == 0) {
        if (*((s32 *)&arg0->unk_90) == 0) {
            *((s16 *)&arg0->unk_9A) = 0;
            *((s32 *)&arg0->unk_44) = 8;
        } else {
            BufferPlayersName(*((MessageFormat * *)&arg0->unk_38), 0U, sub_02034818(*((u32 *)&arg0->unk_6C[7])));
            ov01_021F71C4(arg0, 0x40);
            *((s32 *)&arg0->unk_44) = 6;
        }
    }
    return 0;
}

s32 ov01_021F6F44(struct UnkStruct_ov5_021EAE78 *arg0);

s32 ov01_021F6F44(UnkStruct_ov5_021EAE78 *arg0) {
    if (TextPrinterCheckActive((u8) *((s32 *)&arg0->unk_40)) == 0) {
        *((s16 *)&arg0->unk_98) = 0;
        *((s32 *)&arg0->unk_44) = 7;
    }
    return 0;
}

s32 ov01_021F6F68(struct UnkStruct_ov5_021EAE78 *arg0);

s32 ov01_021F6F68(UnkStruct_ov5_021EAE78 *arg0) {
    if (ov01_021F6CA0(arg0) == 0) {
        if (*((s32 *)&arg0->unk_90) == 0) {
            *((s32 *)&arg0->unk_44) = 0xC;
        } else {
            ov01_021F71C4(arg0, 0x3F);
            *((s32 *)&arg0->unk_44) = 4;
        }
    }
    return 0;
}
