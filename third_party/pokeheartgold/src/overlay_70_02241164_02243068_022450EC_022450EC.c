#include "global.h"

#include "msgdata.h"
#include "pm_string.h"

void ov70_02245084(void *, String *, u32, u32, u32, u32);

extern u8 ov70_0224600C;

void ov70_022450EC(void *param_1, MsgData *param_2, s32 param_3);

void ov70_022450EC(void *param_1, MsgData *param_2, s32 param_3) {
    u32 param_4;
    String *new_var2;
    String *string;
    string = NewString_ReadMsgData(param_2, *((s32 *)((&ov70_0224600C) + (param_3 * 4))));
    ov70_02245084(param_1, string, 0, 0, 0, param_4 = 0x10200);
    new_var2 = string;
    String_Delete(new_var2);
    return;
}
