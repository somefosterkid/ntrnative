#include "global.h"

#include "constants/heap.h"

#include "frontier/frontier_script_context.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "list_menu.h"
#include "message_format.h"
#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"
#include "string_util.h"
#include "sys_task.h"
#include "text.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;


typedef struct BattleFrontier BattleFrontier;

typedef struct ColoredArrow ColoredArrow;

typedef struct MenuTemplate MenuTemplate;

typedef struct Menu Menu;

typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef struct FrontierScriptManager FrontierScriptManager;

typedef struct FrontierMenuManager FrontierMenuManager;

typedef u16 charcode_t;

struct MessageBankEntry {
    u32 offset;
    u32 length;
};

struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
};


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

typedef struct {
    u16 unk_00[8];
} UnkStruct_ov104_0222E8C8;

struct FrontierMenuManager {
    FrontierScriptManager *scriptMan;
    SysTask *sysTask;
    Window window;
    Window *parent;
    String *choiceStringBuffers[28];
    MsgData *msgLoader;
    MessageFormat *strTemplate;
    u8 sysTaskDelay;
    u8 unused;
    u8 initialCursorPos;
    u8 canExitWithB : 1;
    u8 freeMsgLoaderOnDelete : 1;
    u8 unused2 : 4;
    u8 anchorRight : 1;
    u8 anchorBottom : 1;
    u8 anchorX;
    u8 anchorY;
    u8 unused3;
    u8 optionCount;
    u16 *unused4;
    u16 *selectedOptionPtr;
    MenuTemplate menuTemplate;
    Menu *menu;
    ListMenuItem menuChoiceStrings[28];
    ListMenuTemplate listMenuTemplate;
    ListMenu *listMenu;
    u16 unused5;
    u16 listMenuAltTextIndex;
    ListMenuItem listMenuChoiceStrings[28];
    u16 choicesAltTextEntryIDs[28];
    u16 cursorPos;
};

struct FrontierScriptManager {
    BattleFrontier *frontier;
    FrontierScriptContext *ctx[8];
    UnkStruct_ov104_0222E8C8 unk_24;
    enum HeapID heapID;
    u8 unk_38;
    MsgData *msgLoader;
    u8 *script;
    MessageFormat *strTemplate;
    String *string;
    String *fmtString;
    u8 printerID;
    s8 numSetContexts;
    u16 scene;
    u8 unused[5];
    u8 movementCount;
    u8 isMsgBoxOpen;
    int unused2;
    FrontierMenuManager *menuMan;
    Window msgWindow;
    Menu *menu;
    u16 *unused3;
    void *savingIcon;
    int unused4[10];
    Window *castleHostInfoWin;
    Window *castlePlayer2InfoWin;
    u16 unk_B0;
    u16 unk_B2;
    u16 unk_B4;
    u16 unk_B6;
    u16 *unk_B8;
};

void ov80_02233594(FrontierScriptManager *scriptMan, Window *window, PlayerProfile *trainer, u16 castlePoints);

void ov80_02233594(FrontierScriptManager *scriptMan, Window *window, PlayerProfile *trainer, u16 castlePoints) {
    s32 num;
    PlayerProfile *trainerInfo;
    Window *v0;
    MsgData *msgData;
    String *v1;
    String *v2;

    num = (s32)castlePoints;
    v0 = window;
    trainerInfo = trainer;
    v1 = String_New(0x14U, scriptMan->heapID);
    v2 = String_New(0x14U, scriptMan->heapID);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x1F, HEAP_ID_FIELD2);
    BufferIntegerAsString((MessageFormat *)scriptMan->strTemplate, 0U, num, 4U, PRINTING_MODE_RIGHT_ALIGN, 1);
    ReadMsgDataIntoString(msgData, 2, v1);
    StringExpandPlaceholders((MessageFormat *)scriptMan->strTemplate, v2, v1);
    AddTextPrinterParameterized(v0, FONT_SYSTEM, v2, 0x10U, 0x10U, 0xFFU, NULL);
    BufferPlayersName((MessageFormat *)scriptMan->strTemplate, 0U, (PlayerProfile *)trainer);
    ReadMsgDataIntoString(msgData, 0, v1);
    StringExpandPlaceholders((MessageFormat *)scriptMan->strTemplate, v2, v1);
    AddTextPrinterParameterized(v0, FONT_SYSTEM, v2, 0U, 0U, 0U, NULL);
    String_Delete(v1);
    String_Delete(v2);
    DestroyMsgData(msgData);
    CopyWindowToVram(v0);
}
