#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "player_data.h"
#include "pm_string.h"
#include "save.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;


typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef struct StringTemplate StringTemplate;

struct MessageBankEntry {
    u32 offset;
    u32 length;
};

struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
};


struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
};

struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
};

struct StringTemplate {
    u32 maxArgs;
    enum HeapID heapID;
    StringTemplateArg *args;
    String *templateBuf;
};

typedef struct {
    StringTemplate *unk_00;
    MsgData *unk_04;
    Window unk_08;
    String *unk_18;
    String *unk_1C;
    u32 unk_20;
    void *unk_24;
    u32 unk_28;
    u32 unk_2C;
} UnkStruct_ov67_0225D210;

void ov46_022592EC(UnkStruct_ov67_0225D210 *param0, BgConfig *param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6, u32 param7, u32 param8, SaveData *saveData, enum HeapID heapID);

void ov46_022592EC(UnkStruct_ov67_0225D210 *param0, BgConfig *param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6, u32 param7, u32 param8, SaveData *saveData, enum HeapID heapID) {
    u8 bVar1;
    MessageFormat *pMVar2;
    MsgData *pMVar3;
    String *pSVar4;
    Options *options;

    pMVar2 = MessageFormat_New(heapID);
    param0->unk_00 = (StringTemplate *)pMVar2;
    pMVar3 = (MsgData *)NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, param3, heapID);
    param0->unk_04 = pMVar3;
    pSVar4 = String_New(0x100, heapID);
    param0->unk_18 = pSVar4;
    pSVar4 = String_New(0x100, heapID);
    param0->unk_1C = pSVar4;
    param0->unk_20 = param2;
    options = Save_PlayerData_GetOptionsAddr(saveData);
    bVar1 = Options_GetTextFrameDelay(options);
    param0->unk_28 = (u32)bVar1;
    param0->unk_2C = 0;
    AddWindowParameterized(param1, &param0->unk_08, 1, (u8)param4, (u8)param5, (u8)param6, (u8)param7, 9, (u16)param8);
    return;
}
