#include "global.h"

#include "msgdata.h"
#include "pm_string.h"

void ov70_02245084(void *, String *, u32, u32, u32, u32);

extern u32 ov70_022459F4;

extern u32 ov70_02245A4C;

void ov70_0223F38C(void *param_1, MsgData *param_2, s32 param_3, u32 param_4, u32 param_5, u32 param_6, u32 param_7, s32 param_8);

void ov70_0223F38C(void *param_1, MsgData *param_2, s32 param_3, u32 param_4, u32 param_5, u32 param_6, u32 param_7, s32 param_8) {
    String *string;
    u8 *puVar1;

    if (param_3 != -1) {
        if (param_8 == 0) {
            puVar1 = (unsigned char *)(&ov70_02245A4C);
        } else {
            puVar1 = (unsigned char *)(&ov70_022459F4);
        }
        string = NewString_ReadMsgData(param_2, *(s32 *)(puVar1 + param_3 * 8));
        ov70_02245084(param_1, string, param_5, param_6, param_4, param_7);
        String_Delete(string);
    }
    return;
}
