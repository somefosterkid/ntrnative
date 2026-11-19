#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "error_handling.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_112_sprite_app.h"

void ov112_021F135C(Ov112SpriteApp *param_1, u32 param_2, u32 param_3, u8 param_4, enum Font param_5);

void ov112_021F135C(Ov112SpriteApp *param_1, u32 param_2, u32 param_3, u8 param_4, enum Font param_5) {
    String *string;
    unsigned int new_var;
    if ((3 + 1) <= param_2) {
        GF_AssertFail();
    }
    FillWindowPixelBuffer(&param_1->windows[param_2], 0);
    new_var = param_4;
    string = ReadMsgData_ExpandPlaceholders(param_1->msgFormat, param_1->msgData[0], param_3, param_1->heapID);
    AddTextPrinterParameterizedWithColor(&param_1->windows[param_2], param_5, string, new_var, 0, 0xff, 0x10200, (PrinterCallback_t)0x0);
    String_Delete(string);
    CopyWindowToVram(&param_1->windows[param_2]);
    return;
}
