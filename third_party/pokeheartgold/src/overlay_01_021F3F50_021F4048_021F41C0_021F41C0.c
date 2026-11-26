#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "field_system.h"
#include "field_types_def.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_01_021F3F50.h"
#include "overlay_01_save_info.h"

void ov01_021F3F50(SaveInfo *, const FieldSystem *);

void ov01_021F3F9C(MessageFormat *, SaveInfo *);

int ov01_021F4044(SaveInfo *);

SaveInfoWindow *Field_SaveStatsPrinter_New(FieldSystem *fieldSystem, enum HeapID x, s32 y) {
    SaveInfoWindow *saveInfoWin;

    saveInfoWin = Heap_Alloc(x, sizeof(SaveInfoWindow));
    saveInfoWin->fieldSystem = fieldSystem;
    saveInfoWin->heapID = x;
    saveInfoWin->bgLayer = (s8)y;
    saveInfoWin->bgConfig = fieldSystem->bgConfig;
    saveInfoWin->strTemplate = MessageFormat_New(x);
    saveInfoWin->msgLoader = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x1A7, x);
    ov01_021F3F50(&saveInfoWin->saveInfo, saveInfoWin->fieldSystem);
    ov01_021F3F9C(saveInfoWin->strTemplate, &saveInfoWin->saveInfo);
    saveInfoWin->width = 0xD;
    saveInfoWin->height = ov01_021F4044(&saveInfoWin->saveInfo);
    return saveInfoWin;
}
