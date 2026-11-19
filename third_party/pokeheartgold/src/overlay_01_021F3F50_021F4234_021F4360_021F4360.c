#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "field_types_def.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_01.h"
#include "render_text.h"

void ov01_021F3F50(SaveInfo *, const FieldSystem *);

s32 ov01_021F3F9C(MessageFormat *, void *);

s32 ov01_021F4044(void *);

SaveInfoWindow *ov01_021F4360(FieldSystem *fieldSystem, enum HeapID heapID, u8 a2) {
    SaveInfoWindow *saveInfoWin;

    saveInfoWin = Heap_Alloc(heapID, sizeof(SaveInfoWindow));
    saveInfoWin->fieldSystem = fieldSystem;
    saveInfoWin->heapID = heapID;
    saveInfoWin->bgLayer = a2;
    saveInfoWin->bgConfig = (BgConfig *)fieldSystem->bgConfig;
    saveInfoWin->strTemplate = MessageFormat_New(heapID);
    saveInfoWin->msgLoader = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x1A7, heapID);
    TextFlags_SetCanABSpeedUpPrint(1);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(1);
    ov01_021F3F50(&saveInfoWin->saveInfo, saveInfoWin->fieldSystem);
    ov01_021F3F9C(saveInfoWin->strTemplate, &saveInfoWin->saveInfo);
    saveInfoWin->width = 0x13;
    saveInfoWin->height = ov01_021F4044(&saveInfoWin->saveInfo);
    return saveInfoWin;
}
