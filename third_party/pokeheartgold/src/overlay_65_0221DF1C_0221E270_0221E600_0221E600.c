#include "global.h"

#include "message_format.h"
#include "unk_02034354.h"

typedef struct UnkStruct_ov65_0221E600 {
    u8 unk_0[0x180];
    s32 unk_180;
    u8 unk_184[0x4E0];
    u8 unk_664;
    u8 unk_665[0xB];
    u8 unk_670;
    u8 unk_671[0x1BAF];
    s32 *unk_2220;
    u8 unk_2224[0x1454];
    u32 unk_3678;
    u8 unk_367C[0x4];
    MessageFormat *unk_3680;
    u8 unk_3684[0x3C];
    u8 unk_36C0;
} UnkStruct_ov65_0221E600;

s32 ov65_0221E330(void *, s32);

u32 ov65_0221FC54(s32, s32, void *, void *, s32);

extern s32 ov65_0221E354;

extern s32 ov65_0221E558;

s32 ov65_0221E600(UnkStruct_ov65_0221E600 *arg0);

s32 ov65_0221E600(UnkStruct_ov65_0221E600 *arg0) {
    u32 v0;

    v0 = (s32)(ov65_0221FC54(arg0->unk_180, (s32)((void *)&arg0->unk_664), (void *)&arg0->unk_670, (void *)&arg0->unk_36C0, 1));
    switch (v0) {
    case 0:
        break;
    case 1:
    case 3:
        arg0->unk_2220 = &ov65_0221E558;
        break;
    case 2:
    case 4:
        BufferPlayersName(arg0->unk_3680, 0U, sub_02034818(arg0->unk_3678));
        ov65_0221E330(arg0, 0x40);
        arg0->unk_2220 = &ov65_0221E354;
        break;
    }
    return 0;
}
