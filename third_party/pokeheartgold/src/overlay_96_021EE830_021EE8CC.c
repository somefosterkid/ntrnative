#include "global.h"

#include "overlay_96_text_box.h"

u8 ov96_021EEA08(BgConfig *bgConfig, Window *window, String **string, u32 speed, MessageFormat *msgFormat, MsgData *msgData, u32 strno, u8 bgLayer, enum HeapID heapId);

void ov96_021EE8CC(Ov96TextBox *arg0, s32 arg1);

void ov96_021EE8CC(Ov96TextBox *arg0, s32 arg1) {
    arg0->printerId = ov96_021EEA08(arg0->bgConfig, &arg0->window, &arg0->string, arg0->speed, arg0->msgFormat, arg0->msgData, arg1, arg0->bgLayer, arg0->heapId);
}
