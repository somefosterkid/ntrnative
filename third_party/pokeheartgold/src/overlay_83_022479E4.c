#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "msgdata.h"
#include "pm_string.h"
#include "sys_task.h"
#include "unk_02019BA4.h"

void ov83_02247998(Window *window, String *string, u32 x, u32 y, u32 fontId, u32 color, s32 alignment);

void ov83_022479E4(Window *window, MsgData *msgData, s32 strno, u32 x, u32 y, u32 fontId, u32 color, s32 alignment);

void GridInputHandler_Free(GridInputHandler *inputHandler);
void ov83_02247A18(GridInputHandler *inputHandler);

void ov83_022479E4(Window *window, MsgData *msgData, s32 strno, u32 x, u32 y, u32 fontId, u32 color, s32 alignment) {
    String *string;

    string = NewString_ReadMsgData(msgData, strno);
    ov83_02247998(window, string, x, y, fontId, color, alignment);
    String_Delete(string);
}

void ov83_02247A18(GridInputHandler *inputHandler) {
    GridInputHandler_Free(inputHandler);
}
