#include "global.h"

#include "bg_window.h"
#include "unk_02005D10.h"

typedef struct UnkStruct_ov05_0221C6C8 {
    u8 unk_0[0xC];
    BgConfig *unk_C;
    u8 unk_10[0xB72];
    u8 unk_B82;
} UnkStruct_ov05_0221C6C8;

s32 ov05_0221D3AC(void *, u8, s16, s16);

s32 ov05_0221D664(void *, s32, s16, s16);

extern void *ov05_0221EA6C;

extern s32 ov05_0221EA6D;

s32 ov05_0221C6C8(UnkStruct_ov05_0221C6C8 *arg0);

s32 ov05_0221C6C8(UnkStruct_ov05_0221C6C8 *arg0) {
    u8 temp_r1;

    ScheduleSetBgPosText(arg0->unk_C, 2U, BG_POS_OP_ADD_ROT, (s32) * (u8 *)(((u8 *)&ov05_0221EA6C) + (arg0->unk_B82 * 2)));
    ScheduleSetBgPosText(arg0->unk_C, 3U, BG_POS_OP_ADD_ROT, (s32) * (u8 *)(((u8 *)&ov05_0221EA6C) + (arg0->unk_B82 * 2)));
    ov05_0221D3AC(arg0, 0, (s16)(0 - *(u8 *)(((u8 *)&ov05_0221EA6D) + (arg0->unk_B82 * 2))), 0);
    ov05_0221D3AC(arg0, 1, (s16)(0 - *(u8 *)(((u8 *)&ov05_0221EA6D) + (arg0->unk_B82 * 2))), 0);
    ov05_0221D3AC(arg0, 2, (s16)(0 - *(u8 *)(((u8 *)&ov05_0221EA6D) + (arg0->unk_B82 * 2))), 0);
    ov05_0221D3AC(arg0, 3, (s16)(0 - *(u8 *)(((u8 *)&ov05_0221EA6D) + (arg0->unk_B82 * 2))), 0);
    ov05_0221D664(arg0, 0, (s16)(0 - *(u8 *)(((u8 *)&ov05_0221EA6D) + (arg0->unk_B82 * 2))), 0);
    ov05_0221D664(arg0, 1, (s16)(0 - *(u8 *)(((u8 *)&ov05_0221EA6D) + (arg0->unk_B82 * 2))), 0);
    ov05_0221D664(arg0, 2, (s16)(0 - *(u8 *)(((u8 *)&ov05_0221EA6D) + (arg0->unk_B82 * 2))), 0);
    ov05_0221D664(arg0, 3, (s16)(0 - *(u8 *)(((u8 *)&ov05_0221EA6D) + (arg0->unk_B82 * 2))), 0);
    if (arg0->unk_B82 == 0) {
        PlaySE(0x853U);
    }
    temp_r1 = arg0->unk_B82;
    if (temp_r1 == 5) {
        arg0->unk_B82 = 0U;
        return 1;
    }
    arg0->unk_B82 = (u8)(temp_r1 + 1);
    return 0;
}
