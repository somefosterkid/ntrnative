#include "global.h"
struct FishingTask;

#include "message_format.h"
#include "msgdata.h"
#include "overlay_01_021FC66C.h"
#include "pm_string.h"
#include "overlay_01_fishing_task.h"

void ov01_021FCB14(void *arg0) {
    *((MsgData **)&((FishingTask *)arg0)->messageLoader) = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x28, HEAP_ID_FIELD1);
    ((FishingTask *)arg0)->formattedFishingMessage = String_New(0x400U, HEAP_ID_FIELD1);
    ((FishingTask *)arg0)->fishingMessage = String_New(0x400U, HEAP_ID_FIELD1);
    *((MessageFormat **)&((FishingTask *)arg0)->strTemplate) = MessageFormat_New_Custom(8U, 0x40U, HEAP_ID_FIELD1);
}

void ov01_021FCB4C(struct FishingTask *arg0);

void ov01_021FCB4C(FishingTask *arg0) {
    MessageFormat_Delete(*((MessageFormat * *)&arg0->strTemplate));
    String_Delete(arg0->formattedFishingMessage);
    String_Delete(arg0->fishingMessage);
    DestroyMsgData(*((MsgData * *)&arg0->messageLoader));
}
