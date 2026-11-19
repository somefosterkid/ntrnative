#include "global.h"

#include "constants/heap.h"

#include "filesystem_files_def.h"
#include "msgdata.h"
#include "pm_string.h"
#include "unk_02031B0C.h"

s32 sub_02031D6C(void *);

String *sub_020322AC(SaveApricornBox *apricornBox, u16 a1, u32 a2) {
    MsgData *temp_r0;
    String *temp_r6;
    u32 temp_r5;

    temp_r6 = String_New(9U, (enum HeapID)a2);
    if ((s32)a1 < 5) {
        temp_r0 = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x15, (enum HeapID)a2);
        ReadMsgDataIntoString(temp_r0, a1 + 0xE, temp_r6);
        DestroyMsgData(temp_r0);
    } else if ((s32)a1 < 8) {
        temp_r5 = (u32)((a1 - 5) << 0x18) >> 0x13;
        if (sub_02031D6C((u8 *)apricornBox + 0x38 + temp_r5) != 0) {
            CopyU16ArrayToString(temp_r6, (const u16 *)((u8 *)apricornBox + 0x28 + temp_r5));
        }
    }
    return temp_r6;
}
