#include "global.h"
struct MoveRelearner;

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "yes_no_prompt.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "screen_fade.h"
#include "overlay_68_move_relearner.h"
struct String;

extern u32 ov68_021E7DFC;

void ov68_021E5E48(MoveRelearner *param_1);

void ov68_021E5EBC(MoveRelearner *arg0);
void ov68_021E5F18(struct MoveRelearner *arg0);
s32 ov68_021E5F50(MoveRelearner *arg0);

void ov68_021E5E48(MoveRelearner *param_1) {
    YesNoPrompt *pYVar1;
    WindowTemplate *template;
    Window *window;
    u32 uVar2;

    FontID_Alloc(FONT_UNK_4, HEAP_ID_66);
    template = (WindowTemplate *)&ov68_021E7DFC;
    uVar2 = 0;
    window = param_1->windows;
    do {
        AddWindow(param_1->bgConfig, window, template);
        FillWindowPixelBuffer(window, 0);
        uVar2 = uVar2 + 1;
        template = template + 1;
        window = window + 1;
    } while (uVar2 < 0xf);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[8]);
    pYVar1 = YesNoPrompt_Create(HEAP_ID_66);
    param_1->yesNoPrompt = pYVar1;
    return;
}

void ov68_021E5E94(MoveRelearner *param_1);

void ov68_021E5E94(MoveRelearner *param_1) {
    Window *window;
    u32 uVar1;

    YesNoPrompt_Destroy(param_1->yesNoPrompt);
    uVar1 = 0;
    window = param_1->windows;
    do {
        RemoveWindow(window);
        uVar1 = uVar1 + 1;
        window = window + 1;
    } while (uVar1 < 0xf);
    FontID_Release(FONT_UNK_4);
    return;
}

void ov68_021E5EBC(MoveRelearner *arg0) {
    arg0->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x2EB, HEAP_ID_66);
    arg0->msgFormat = MessageFormat_New(HEAP_ID_66);
    arg0->string = String_New(0x100U, HEAP_ID_66);
    arg0->ppLabel = NewString_ReadMsgData(arg0->msgData, 0x19);
    arg0->ppFormat = NewString_ReadMsgData(arg0->msgData, 0x1F);
}

void ov68_021E5F18(MoveRelearner *arg0) {
    DestroyMsgData(*((MsgData * *)&arg0->msgData));
    MessageFormat_Delete(*((MessageFormat * *)&arg0->msgFormat));
    String_Delete(arg0->string);
    String_Delete(*((String * *)&arg0->ppLabel));
    String_Delete(*((String * *)&arg0->ppFormat));
}

s32 ov68_021E5F50(MoveRelearner *arg0) {
    if (IsPaletteFadeFinished() == 1) {
        return arg0->nextState;
    }
    return 0;
}
