#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "list_menu.h"
#include "options.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_02037C94.h"
#include "yes_no_prompt.h"
#include "overlay_75_unk_struct_ov98_02246_e88.h"
#include "overlay_70_gtsapplication_state.h"
#include "overlay_75_unk_struct_ov98_02247704.h"
#include "msgdata.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;


typedef struct ColoredArrow ColoredArrow;

typedef struct MenuTemplate MenuTemplate;

typedef struct Menu Menu;

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


typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

typedef u32 TextColor;

struct ColoredArrow {
    TextColor color;
    String *string;
};

struct MenuTemplate {
    ListMenuItem *choices;
    Window *window;
    u8 fontID;
    u8 xSize;
    u8 ySize;
    u8 lineSpacing : 4;
    u8 suppressCursor : 2;
    u8 loopAround : 2;
};

struct Menu {
    MenuTemplate template;
    ColoredArrow *cursor;
    u32 cancelKeys;
    u8 dummy14;
    u8 cursorPos;
    u8 width;
    u8 xOffset;
    u8 yOffset;
    u8 letterWidth;
    u8 lineHeight;
    u8 lastAction;
    u8 heapID;
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

void ov75_02246BCC(UnkStruct_ov98_02246E88 *, u32, u32);

void ov75_02246BE0(UnkStruct_ov98_02246E88 *, s8);

void ov75_02247854(UnkStruct_ov98_02247704 *, u32, u32);

YesNoPrompt *ov75_02247890(BgConfig *, u32, u32);

void ov75_022494CC(UnkStruct_ov98_02247704 *, MsgData *, u32, u32, u16);

s32 ov75_02249534(s32);

s32 ov75_02248584(UnkStruct_ov98_02247704 *param0);

s32 ov75_02248584(UnkStruct_ov98_02247704 *param0) {
    s32 iVar1;
    YesNoPrompt *pMVar2;
    YesNoResponse YVar3;
    BOOL BVar4;

    iVar1 = ov75_02249534(param0->unk_44);
    if (iVar1 == 1) {
        return 0;
    }
    switch (param0->unk_94) {
    case 0:
        param0->unk_94 = param0->unk_94 + 1;
        break;
    case 1:
        ov75_022494CC(param0, param0->unk_34, 0xb, 1, 0xf0f);
        param0->unk_94 = param0->unk_94 + 1;
        break;
    case 2:
        pMVar2 = ov75_02247890(param0->unk_04, 0x234, 0);
        param0->unk_88 = pMVar2;
        param0->unk_94 = param0->unk_94 + 1;
        break;
    case 3:
        YVar3 = YesNoPrompt_HandleInput(param0->unk_88);
        if (YVar3 == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(param0->unk_88);
            param0->unk_94 = param0->unk_94 + 1;
        } else if (YVar3 == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(param0->unk_88);
            ov75_02247854(param0, 0x22, 0);
        }
        break;
    case 4:
        BVar4 = sub_0203A05C(param0->unk_00->saveData);
        if (BVar4 == 0) {
            ov75_02246BCC(param0->unk_00, 5, 0);
            ov75_02246BE0(param0->unk_00, 0xc);
            param0->unk_08 = 0x1d;
            return 0;
        }
        ov75_02247854(param0, 0x22, 0xc);
    }
    return 0;
}
