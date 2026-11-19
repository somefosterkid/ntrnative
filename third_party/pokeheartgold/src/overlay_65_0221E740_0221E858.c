#include "global.h"

#include "message_format.h"
#include "msgdata.h"
#include "unk_0202C034.h"
#include "unk_020379A0.h"
#include "unk_02037C94.h"

typedef struct UnkStruct_ov65_0221E858 {
    u8 unk_0[0x4];
    SaveData *unk_4;
    u8 unk_8[0x17C];
    void *unk_184;
    u8 unk_188[0x8];
    void *unk_190;
    u8 unk_194[0x420];
    u8 unk_5B4;
    u8 unk_5B5[0x1C6B];
    s32 *unk_2220;
    u8 unk_2224[0x13D4];
    u8 unk_35F8;
    u8 unk_35F9[0x87];
    MessageFormat *unk_3680;
    MsgData *unk_3684;
    u8 unk_3688[0x18];
    WiFiList *unk_36A0;
} UnkStruct_ov65_0221E858;

u8 ov65_0221FB90(void *, s32, s32, MsgData *, MessageFormat *);

extern s32 ov65_0221E72C;

extern s32 ov65_0221E740;

s32 ov65_0221E858(UnkStruct_ov65_0221E858 *arg0);

s32 ov65_0221E858(UnkStruct_ov65_0221E858 *arg0) {
    if (sub_0203A1C4(arg0->unk_4, (void *)&arg0->unk_35F8, HEAP_ID_26) == 0) {
        sub_02037AC0(0x13U);
        ov65_0221FB90((void *)&arg0->unk_5B4, 0x1C, 1, arg0->unk_190, arg0->unk_184);
        arg0->unk_2220 = &ov65_0221E72C;
        return 0;
    }
    arg0->unk_3680 = MessageFormat_New(HEAP_ID_26);
    arg0->unk_3684 = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x30B, HEAP_ID_26);
    arg0->unk_36A0 = sub_0202C6F4(arg0->unk_4);
    arg0->unk_2220 = &ov65_0221E740;
    return 0;
}
