#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_112_sprite_app.h"

void ov112_021F1324(Ov112SpriteApp *param_1);

void ov112_021F1324(Ov112SpriteApp *param_1) {
    s32 iVar1;
    Window *window;

    iVar1 = 0;
    window = &param_1->windows[0];
    do {
        RemoveWindow(window);
        iVar1 = iVar1 + 1;
        window = window + 1;
    } while (iVar1 < 4);
    MessageFormat_Delete(param_1->msgFormat);
    DestroyMsgData(param_1->msgData[0]);
    DestroyMsgData(param_1->msgData[1]);
    DestroyMsgData(param_1->msgData[2]);
    FontID_Release(FONT_UNK_4);
    return;
}
