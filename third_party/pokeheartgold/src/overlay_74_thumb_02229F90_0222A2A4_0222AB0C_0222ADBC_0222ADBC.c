#include "global.h"

#include "message_format.h"
#include "msgdata.h"
#include "overlay_74_mystery_gift_app_data.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "text.h"

s32 ov74_0222AA18(OverlayManager *, Window *, u32);

s32 ov74_0222ADBC(OverlayManager *arg0, Window *arg1, u32 arg2, s32 arg3);

s32 ov74_0222ADBC(OverlayManager *man, Window *arg1, u32 msgno, s32 arg3) {
    MessageFormat *messageFormat;
    MsgData *msgData;
    OverlayManager *overlayManager;
    MessageFormat *v1;
    MsgData *v0;
    MysteryGiftAppData *v2;

    overlayManager = man;
    v2 = OverlayManager_GetData(man);
    if ((arg1 != 0) && (msgno != 0)) {
        v0 = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0xF7, HEAP_ID_84);
        msgData = v0;
        v1 = MessageFormat_New(HEAP_ID_84);
        messageFormat = v1;
        v2->string = ReadMsgData_ExpandPlaceholders(v1, msgData, msgno, HEAP_ID_84);
        v2->msgBoxPrinterDelay = 1;
        ov74_0222AA18(overlayManager, arg1, msgno);
        v2->queuedState = arg3;
        MessageFormat_Delete(v1);
        DestroyMsgData(v0);
    } else {
        if (TextPrinterCheckActive((u8)v2->textPrinterId) == 0) {
            String_Delete(v2->string);
            v2->string = NULL;
            v2->msgBoxPrinterDelay = 0;
            return v2->queuedState;
        }
    }
    return 0x32;
}
