#include "global.h"

#include "msgdata.h"
#include "pm_string.h"
#include "pokemon.h"

void ov80_0222A140(void *arg0, Pokemon *arg1, s32 arg2);

void ov80_0222A140(void *arg0, Pokemon *arg1, s32 arg2) {
    s32 sp1C;
    s32 sp18;
    u8 sp14[4];
    void *sp10;
    MsgData *temp_r0;
    String *temp_r0_2;
    s32 var_r4;
    s32 var_r4_2;
    s32 var_r7;

    var_r4 = arg2;
    ZeroMonData(arg1);
    if (var_r4 == 0x78) {
        var_r4 = 0x32;
    } else if (var_r4 == 0x79) {
        var_r4 = 0x64;
    }
    sp1C = 0x3FFFFFFF & *(s32 *)((u8 *)arg0 + 0x14);
    CreateMon(arg1, (s32)((u32)(*(u16 *)arg0 << 0x15) >> 0x15), var_r4, sp1C, 1, *(s32 *)((u8 *)arg0 + 0x10), 2, 0);
    SetMonData(arg1, 0xAF, &sp1C);
    CalcMonLevelAndStats(arg1);
    *(u8 *)((u8 *)((s32 *)sp14) + 1) = (u8)((u32)(*(u16 *)arg0 << 0x10) >> 0x1B);
    SetMonData(arg1, 0x70, &sp14[1]);
    SetMonData(arg1, 6, arg0 + 2);
    var_r4_2 = 0;
    sp10 = arg0;
    var_r7 = 0;
    do {
        *(u16 *)((u8 *)((s32 *)sp14) + 2) = (u16) * (u16 *)((u8 *)sp10 + 4);
        SetMonData(arg1, var_r4_2 + 0x36, &sp14[2]);
        *(u8 *)((u8 *)((s32 *)sp14) + 1) = (u8)(((s32) * (u8 *)((u8 *)arg0 + 0x1E) >> var_r7) & 3);
        SetMonData(arg1, var_r4_2 + 0x3E, &sp14[1]);
        *(s8 *)((s32 *)sp14) = (s8)GetMonData(arg1, var_r4_2 + 0x42, NULL);
        SetMonData(arg1, var_r4_2 + 0x3A, (s32 *)sp14);
        var_r4_2 += 1;
        var_r7 += 2;
        sp10 += 2;
    } while (var_r4_2 < 4);
    sp18 = *(s32 *)((u8 *)arg0 + 0xC);
    SetMonData(arg1, 7, &sp18);
    *(u8 *)((u8 *)((s32 *)sp14) + 1) = (u8) * (u8 *)((u8 *)arg0 + 0x18);
    SetMonData(arg1, 0xD, &sp14[1]);
    *(u8 *)((u8 *)((s32 *)sp14) + 1) = (u8) * (u8 *)((u8 *)arg0 + 0x19);
    SetMonData(arg1, 0xE, &sp14[1]);
    *(u8 *)((u8 *)((s32 *)sp14) + 1) = (u8) * (u8 *)((u8 *)arg0 + 0x1A);
    SetMonData(arg1, 0xF, &sp14[1]);
    *(u8 *)((u8 *)((s32 *)sp14) + 1) = (u8) * (u8 *)((u8 *)arg0 + 0x1B);
    SetMonData(arg1, 0x10, &sp14[1]);
    *(u8 *)((u8 *)((s32 *)sp14) + 1) = (u8) * (u8 *)((u8 *)arg0 + 0x1C);
    SetMonData(arg1, 0x11, &sp14[1]);
    *(u8 *)((u8 *)((s32 *)sp14) + 1) = (u8) * (u8 *)((u8 *)arg0 + 0x1D);
    SetMonData(arg1, 0x12, &sp14[1]);
    SetMonData(arg1, 0xA, arg0 + 0x20);
    SetMonData(arg1, 9, arg0 + 0x21);
    if (((u32)(*(s32 *)((u8 *)arg0 + 0x14) * 2) >> 0x1F) != 0) {
        temp_r0 = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0xED, HEAP_ID_FIELD1);
        temp_r0_2 = NewString_ReadMsgData(temp_r0, (s32)((u32)(*(u16 *)arg0 << 0x15) >> 0x15));
        SetMonData(arg1, 0x77, temp_r0_2);
        String_Delete(temp_r0_2);
        DestroyMsgData(temp_r0);
    } else {
        SetMonData(arg1, 0x75, arg0 + 0x22);
    }
    SetMonData(arg1, 0xC, arg0 + 0x1F);
    CalcMonLevelAndStats(arg1);
}
