#include "global.h"

#include "msgdata.h"
#include "pm_string.h"
#include "unk_0201956C.h"
#include "unk_struct_ov70_02242e58.h"

s32 ov70_0224190C(void *, s32);

s32 ov70_02242FC4(UnkStruct_0201956C *, void *, String *, s32, s32);

s32 ov70_02243EB8(UnkStruct_0201956C *, MsgData *, void *, s32);

s32 ov70_02243F7C(void *, s32);

s32 ov70_02243FE0(void *, s32);

s32 sub_02019688(UnkStruct_0201956C *, s32, s32, s32, s32);

s32 sub_020197F4(UnkStruct_0201956C *, s32);

void ov70_02242D44(UnkStruct_ov70_02242E58 *arg0, s32 arg1, s32 arg2);

void ov70_02242D44(UnkStruct_ov70_02242E58 *arg0, s32 arg1, s32 arg2) {
    s32 a2;
    s32 v0;
    String *string;
    String *v1;
    s32 v2;
    s32 v3;
    s32 v4;
    s32 v5;

    v0 = arg1;
    a2 = arg2;
    if (arg1 == 6) {
        v3 = 0x21;
    } else {
        v3 = 0x22;
    }
    sub_02019688(arg0->unk_1C, 0, 0x64, v3, 1);
    sub_02019B08(arg0->unk_1C, 0);
    ov70_0224190C(arg0, v0);
    MI_CpuFill8(arg0->unk_64, 1U, 0x1AU);
    v4 = 0;
    v5 = 0;
    do {
        v1 = NewString_ReadMsgData(arg0->unk_24, v4 + 0x6E);
        if (v0 == 4) {
            if (ov70_02243F7C(arg0, v4) == 1) {
                v2 = 0xF0E02;
                arg0->unk_64[v4] = 1;
            } else {
                v2 = 0x80902;
                arg0->unk_64[v4] = 0;
            }
        } else if (ov70_02243FE0(arg0, v4) == 1) {
            v2 = 0xF0E02;
            arg0->unk_64[v4] = 1;
        } else {
            v2 = 0x80902;
            arg0->unk_64[v4] = 0;
        }
#if defined(SDK_PORT) || defined(NONMATCHING)
        ov70_02242FC4(arg0->unk_1C, &arg0->unk_4[v4], v1, 2, v2);
#else
        ov70_02242FC4(arg0->unk_1C, (Window *)((u8 *)arg0->unk_4 + v5), v1, 2, v2);
#endif
        String_Delete(v1);
        v4 += 1;
        v5 += 0x10;
    } while (v4 < 9);
    ov70_02243EB8(arg0->unk_1C, arg0->unk_24, &arg0->unk_4[14], 0x44);
    if (v0 == 6) {
        string = NewString_ReadMsgData(arg0->unk_24, 0xAE);
        ov70_02242FC4(arg0->unk_1C, &arg0->unk_4[15], string, 2, 0xF0E02);
        String_Delete(string);
    }
    sub_020196E8(arg0->unk_1C, 0, (s32)(s8)a2, 0);
    sub_020197F4(arg0->unk_1C, 0);
}

void ov70_02242E58(UnkStruct_ov70_02242E58 *arg0);

void ov70_02242E58(UnkStruct_ov70_02242E58 *arg0) {
    String *string;
    s32 v0;
    s32 v1;

    sub_02019688(arg0->unk_1C, 0, 0x64, 0x1F, 1);
    sub_02019B08(arg0->unk_1C, 0);
    ov70_0224190C(arg0, 1);
    v1 = 0;
    v0 = 0;
    do {
        string = NewString_ReadMsgData(arg0->unk_24, v1 + 0x7C);
#if defined(SDK_PORT) || defined(NONMATCHING)
        ov70_02242FC4(arg0->unk_1C, &arg0->unk_4[v1], string, 2, 0xF0E02);
#else
        ov70_02242FC4(arg0->unk_1C, (Window *)((u8 *)arg0->unk_4 + v0), string, 2, 0xF0E02);
#endif
        String_Delete(string);
        v1 += 1;
        v0 += 0x10;
    } while (v1 < 3);
    ov70_02243EB8(arg0->unk_1C, arg0->unk_24, &arg0->unk_4[14], 0x44);
    sub_020196E8(arg0->unk_1C, 0, 0x20, 0);
    sub_020197F4(arg0->unk_1C, 0);
    sub_020198FC(arg0->unk_1C, 0, -4, 0, 4);
}
