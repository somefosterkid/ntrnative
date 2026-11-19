#include "global.h"

#include "msgdata.h"
#include "pm_string.h"

typedef struct UnkStruct_ov96_021EE6A0 {
    enum HeapID unk_0;
    u8 unk_4[0x4];
    s32 unk_8;
    u8 unk_C[0x4];
    u8 unk_10;
    u8 unk_11[0xF];
    u8 unk_20;
} UnkStruct_ov96_021EE6A0;

s32 ov96_021EE700(void *, MsgData *, String *, s32, s32);

void ov96_021EE6A0(UnkStruct_ov96_021EE6A0 *arg0);

void ov96_021EE6A0(UnkStruct_ov96_021EE6A0 *arg0) {
    MsgData *msgData;
    String *string;
    u8 v0;

    v0 = (u8) arg0->unk_8;
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x135, arg0->unk_0);
    string = String_New(0x100U, arg0->unk_0);
    ov96_021EE700((void *)&arg0->unk_10, msgData, string, v0 + 0xA6, 4);
    ov96_021EE700((void *)&arg0->unk_20, msgData, string, v0 + 0xBB, 0);
    String_Delete(string);
    DestroyMsgData(msgData);
}
