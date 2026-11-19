#include "global.h"

#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F4FBC(UnkStruct_ov14_boxsys *arg0, MsgData *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);

u8 ov14_021F4F24(void *, String *, s32, s32, s32, s32, s32);

void ov14_021F4FBC(UnkStruct_ov14_boxsys *arg0, MsgData *msgData, s32 arg2, s32 strno, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    String *string;

    string = NewString_ReadMsgData(msgData, strno);
    StringExpandPlaceholders(arg0->unk_24, arg0->unk_28, string);
    ov14_021F4F24(&arg0->windows[arg2], arg0->unk_28, arg4, arg5, arg6, arg7, arg8);
    String_Delete(string);
}
