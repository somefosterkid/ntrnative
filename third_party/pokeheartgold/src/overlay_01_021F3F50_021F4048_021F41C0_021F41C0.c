#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "field_system.h"
#include "field_types_def.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_01_021F3F50.h"

struct SaveStatsPrinter {
    FieldSystem *unk_0;
    enum HeapID unk_4;
    s8 unk_8;
    u8 unk_9[0x3];
    BgConfig *unk_C;
    u8 unk_10[0x4];
    MessageFormat *unk_14;
    MsgData *unk_18;
    u8 unk_1C;
    u8 unk_1D[0xF];
    s32 unk_2C;
    s32 unk_30;
};

s32 ov01_021F3F50(void *, FieldSystem *);

s32 ov01_021F3F9C(MessageFormat *, void *);

s32 ov01_021F4044(void *);

struct SaveStatsPrinter *Field_SaveStatsPrinter_New(FieldSystem *fieldSystem, enum HeapID x, s32 y) {
    struct SaveStatsPrinter *saveStatsPrinter;

    saveStatsPrinter = Heap_Alloc(x, sizeof(struct SaveStatsPrinter));
    saveStatsPrinter->unk_0 = fieldSystem;
    saveStatsPrinter->unk_4 = x;
    saveStatsPrinter->unk_8 = (s8)y;
    saveStatsPrinter->unk_C = (BgConfig *)fieldSystem->bgConfig;
    saveStatsPrinter->unk_14 = MessageFormat_New(x);
    saveStatsPrinter->unk_18 = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x1A7, x);
    ov01_021F3F50((u8 *)&saveStatsPrinter->unk_1C, saveStatsPrinter->unk_0);
    ov01_021F3F9C(saveStatsPrinter->unk_14, (u8 *)&saveStatsPrinter->unk_1C);
    saveStatsPrinter->unk_2C = 0xD;
    saveStatsPrinter->unk_30 = ov01_021F4044((u8 *)&saveStatsPrinter->unk_1C);
    return saveStatsPrinter;
}
