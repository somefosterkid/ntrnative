#include "global.h"

#include "msgdata.h"
#include "pm_string.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

u8 ov14_021F4F24(void *, String *, s32, s32, s32, s32, s32);

void ov14_021F4F84(UnkStruct_ov14_boxsys *arg0, MsgData *msgData, s32 arg2, s32 strno, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    String *string;

    string = NewString_ReadMsgData(msgData, strno);
    ov14_021F4F24(&arg0->windows[arg2], string, arg4, arg5, arg6, arg7, arg8);
    String_Delete(string);
}
