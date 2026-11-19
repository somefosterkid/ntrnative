#include "global.h"

#include "msgdata.h"
#include "unk_02031B0C.h"

s32 sub_02032004(ApricornJuice *);

String *sub_02032308(SaveApricornBox *apricornBox, u16 a1, u32 a2) {
    ApricornJuice v0;
    MsgData *msgData;
    String *string;

    sub_02031CEC(apricornBox, a1, &v0);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x15, (enum HeapID)a2);
    string = NewString_ReadMsgData(msgData, sub_02032004(&v0) + 0x13);
    DestroyMsgData(msgData);
    return string;
}

void sub_02032340(void *arg0, s32 arg1);

void sub_02032340(void *arg0, s32 arg1) {
    s32 v0;

    v0 = *(u8 *)((u8 *)arg0 + 0xE) + arg1;
    if (v0 > 0xFF) {
        *(u8 *)((u8 *)arg0 + 0xE) = 0xFFU;
        return;
    }
    *(u8 *)((u8 *)arg0 + 0xE) = (u8)v0;
}
