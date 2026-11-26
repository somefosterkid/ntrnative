#include "global.h"

#include "msgdata.h"
#include "pm_string.h"
#include "overlay_112_sprite_app.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov112_021F1488(void *, u16 *, u8, u32);
#else
s32 ov112_021F1488(void *, u16 *, s32, u32);
#endif

void ov112_021F1504(Ov112SpriteApp *arg0, u32 arg1);

void ov112_021F1504(Ov112SpriteApp *arg0, u32 arg1) {
    u16 sp0[19];
    String *temp_r0;
    u16 temp_r4;
    u32 var_r1;

    var_r1 = arg1;
    if (var_r1 > 0x218U) {
        var_r1 = 0x218;
    }
    temp_r0 = NewString_ReadMsgData(arg0->msgData[2], (s32)var_r1);
    temp_r4 = String_GetLength(temp_r0) + 1;
    CopyStringToU16Array(temp_r0, sp0, 0x13U);
    ov112_021F1488(arg0, sp0, 4, temp_r4);
    String_Delete(temp_r0);
}

void ov112_021F1548(Ov112SpriteApp *arg0, s32 arg1, s32 arg2);

void ov112_021F1548(Ov112SpriteApp *arg0, s32 arg1, s32 arg2) {
    u16 sp0[11];
    String *temp_r0;
    u16 temp_r7;

    temp_r0 = NewString_ReadMsgData(*((MsgData * *)&arg0->msgData[1]), arg2);
    temp_r7 = String_GetLength(temp_r0) + 1;
    CopyStringToU16Array(temp_r0, sp0, (u32)temp_r7);
    ov112_021F1488(arg0, sp0, arg1, temp_r7);
    String_Delete(temp_r0);
}
