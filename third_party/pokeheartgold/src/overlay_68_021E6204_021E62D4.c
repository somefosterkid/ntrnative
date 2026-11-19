#include "global.h"
struct MoveRelearner;

#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "overlay_68_move_relearner.h"
struct String;

void ov68_021E62D4(struct MoveRelearner *param_1, s32 param_2, s32 param_3, u32 param_4, u8 param_5);

void ov68_021E62D4(MoveRelearner *param_1, s32 param_2, s32 param_3, u32 param_4, u8 param_5) {
    String *src;

    src = NewString_ReadMsgData(*((MsgData * *)&param_1->msgData), param_2);
    BufferIntegerAsString(*((MessageFormat * *)&param_1->msgFormat), 0, param_3, param_4, (PrintingMode)((u32)param_5), 1);
    StringExpandPlaceholders(*((MessageFormat * *)&param_1->msgFormat), param_1->string, src);
    String_Delete(src);
    return;
}
