#include "global.h"

#include "battle/trainer_intro_data.h"
#include "bg_window.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "overlay_05.h"
#include "overlay_05_unk_struct_ov10_0221_fb28_t.h"
#include "pm_string.h"
#include "render_window.h"
#include "unk_0202FBCC.h"

void ov05_0221E8A8(UnkStruct_ov10_0221FB28 *, MsgData *, MessagePrinter *, MessageFormat *, String *, u32);

void ov05_0221E944(UnkStruct_ov10_0221FB28 *arg0) {
    MsgData *msgData;
    MessagePrinter *messagePrinter;
    MessageFormat *messageFormat;
    String *string;
    u32 v0;

    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x12C, arg0->trainerIntroData->heapID);
    messagePrinter = MessagePrinter_New(0xFU, 0xEU, 0U, arg0->trainerIntroData->heapID);
    messageFormat = MessageFormat_New(arg0->trainerIntroData->heapID);
    string = String_New(0x20U, arg0->trainerIntroData->heapID);
    v0 = 0;
    do {
        if (arg0->unk_214[v0].unk_04 != 0) {
            ov05_0221E8A8(arg0, msgData, messagePrinter, messageFormat, string, v0);
        }
        v0 += 1;
    } while (v0 < 6U);
    String_Delete(string);
    DestroyMsgData(msgData);
    MessagePrinter_Delete(messagePrinter);
    MessageFormat_Delete(messageFormat);
}

void ov05_0221E9C4(UnkStruct_ov10_0221FB28 *arg0) {
    FillWindowPixelRect(&arg0->unk_B88, 0xf, 0, 0, 0xd8, 0x20);
    DrawFrameAndWindow2(&arg0->unk_B88, 0, 1, 0xf);
    return;
}

s32 ov05_0221E9F8(UnkStruct_ov10_0221FB28 *arg0) {
    if ((arg0->trainerIntroData->setup->saveData == NULL) || (sub_0202FC48() == 0)) {
        return 0;
    }
    return 1;
}

void ov05_0221EA18(UnkStruct_ov10_0221FB28 *arg0) {
    if (arg0->unk_BC4 == NULL) {
        arg0->unk_BC4 = WaitingIcon_New(&arg0->unk_B88, 1U);
    }
}

void ov05_0221EA38(UnkStruct_ov10_0221FB28 *arg0) {
    WaitingIcon *waitingIcon;

    waitingIcon = arg0->unk_BC4;
    if (waitingIcon != NULL) {
        sub_0200F450(waitingIcon);
        arg0->unk_BC4 = NULL;
    }
}
