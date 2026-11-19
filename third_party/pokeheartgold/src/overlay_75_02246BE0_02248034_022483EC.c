#include <dwc.h>

#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "list_menu.h"
#include "options.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "yes_no_prompt.h"
#include "overlay_75_unk_struct_ov98_02246_e88.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "overlay_70_gtsapplication_state.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "overlay_75_unk_struct_ov98_02247704.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;


typedef struct ColoredArrow ColoredArrow;

typedef struct MenuTemplate MenuTemplate;

typedef struct Menu Menu;

typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

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

int ov75_022480B8(UnkStruct_ov98_02247704 *param0);

s32 ov75_02247854(UnkStruct_ov98_02247704 *, s32, s32);

YesNoPrompt *ov75_02247890(BgConfig *, s32, s32);

s32 ov75_022494CC(UnkStruct_ov98_02247704 *, MsgData *, s32, s32, u16);

s32 ov75_02249534(s32);

s32 ov75_022483EC(UnkStruct_ov98_02247704 *arg0);

s32 ov75_022483EC(UnkStruct_ov98_02247704 *arg0) {
    enum YesNoResponse temp_r0_2;
    enum YesNoResponse temp_r0_4;
    s32 temp_r0_3;
    u32 temp_r0;

    if (ov75_02249534(arg0->unk_44) == 1) {
        return 0;
    }
    temp_r0 = arg0->unk_94;
    switch (temp_r0) {
    case 0:
        arg0->unk_94 = 3;
        break;
    case 1:
        arg0->unk_94 = arg0->unk_94 + 1;
        break;
    case 2:
        temp_r0_2 = (YesNoResponse)((struct YesNoPrompt *)(YesNoPrompt_HandleInput(arg0->unk_88)));
        if (temp_r0_2 == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(arg0->unk_88);
            ov75_022494CC(arg0, arg0->unk_34, 9, 1, 0xF0F);
            arg0->unk_94 = 4;
        } else if (temp_r0_2 == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(arg0->unk_88);
            arg0->unk_94 = arg0->unk_94 + 1;
        }
        break;
    case 3:
        temp_r0_3 = ov75_022480B8(arg0);
        if (temp_r0_3 == 1) {
            ov75_022494CC(arg0, arg0->unk_34, 9, 1, 0xF0F);
            arg0->unk_94 = arg0->unk_94 + 1;
        } else if (temp_r0_3 == 2) {
            ov75_02247854(arg0, 0x22, 0);
        }
        break;
    case 4:
        arg0->unk_88 = ov75_02247890(arg0->unk_04, 0x234, 0);
        arg0->unk_94 = arg0->unk_94 + 1;
        break;
    default:
        temp_r0_4 = YesNoPrompt_HandleInput(arg0->unk_88);
        if (temp_r0_4 == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(arg0->unk_88);
            arg0->unk_08 = 5;
        } else if (temp_r0_4 == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(arg0->unk_88);
            ov75_022494CC(arg0, arg0->unk_34, 0xA, 1, 0xF0F);
            ov75_02247854(arg0, 0x22, 0);
        }
        break;
    }
    return 0;
}
