#include <dwc.h>

#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "field_types_def.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "list_menu.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "pm_string.h"
#include "render_text.h"
#include "save.h"
#include "screen_fade.h"
#include "sys_task.h"
#include "system.h"
#include "unk_020210A0.h"
#include "unk_0203A3B0.h"
#include "vram_transfer_manager.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;


typedef struct ApplicationManager ApplicationManager;

typedef struct ApplicationManagerTemplate ApplicationManagerTemplate;

typedef struct ColoredArrow ColoredArrow;

typedef struct StringList StringList;

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


typedef BOOL (*OverlayFunc)(ApplicationManager *appMan, int *state);

struct ApplicationManagerTemplate {
    OverlayFunc init;
    OverlayFunc main;
    OverlayFunc exit;
    const FSOverlayID overlayID;
};

struct ApplicationManager {
    ApplicationManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    ApplicationManager *parent;
    ApplicationManager *child;
};

typedef u32 TextColor;

struct ColoredArrow {
    TextColor color;
    String *string;
};

struct StringList {
    void *entry;
    union {
        u32 index;
        void *callbackFunction;
    };
};

struct MenuTemplate {
    StringList *choices;
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

typedef struct {
    FieldSystem *fieldSystem;
    SaveData *saveData;
    int unk_08;
    u8 unk_0C;
    u8 padding_0D[3];
} UnkStruct_0203E6C0;

typedef struct {
    UnkStruct_0203E6C0 *unk_00;
    BOOL unk_10;
    ApplicationManager *appMan;
    void *unk_18;
    NNSFndHeapHandle unk_1C;
    DWCInetControl unk_20;
    int unk_88;
    BOOL unk_8C;
} UnkStruct_02017498;

typedef struct {
    SysTask *unk_00;
    BOOL unk_04;
    u16 unk_08[4][16];
    u16 unk_88[21][16];
    s16 unk_328;
    s8 unk_32A;
    u8 unk_32B;
} UnkStruct_ov61_0222C3B0;

typedef struct {
    UnkStruct_02017498 *unk_00;
    BgConfig *unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    StringTemplate *unk_20;
    MsgData *unk_24;
    MsgData *unk_28;
    MsgData *unk_2C;
    MsgData *unk_30;
    String *unk_34;
    String *unk_38;
    String *unk_3C;
    int unk_40;
    Window unk_44;
    Window unk_54;
    Window unk_64;
    Window unk_74;
    Menu *unk_84;
    void *unk_88;
    int unk_8C;
    int unk_90;
    int unk_94;
    int unk_98;
    StringList *unk_9C;
    ListMenu *unk_A0;
    u8 pad_A4[4];
    struct { struct SysTask *unk_00; BOOL unk_04; u16 unk_08[16][4]; u16 unk_88[16][21]; s16 unk_328; s8 unk_32A; u8 unk_32B; int unk_32C; } unk_A4;
} UnkStruct_ov61_0222C664;

void ov39_02228440(BgConfig *param0);

void ov39_022285CC(UnkStruct_ov61_0222C664 *param0);

void ov39_02228418(void *);

s32 ov39_022288A0(UnkStruct_ov61_0222C664 *);

s32 ov39_02228B6C(UnkStruct_ov61_0222C664 *, s32, s32);

s32 ov39_02228140(ApplicationManager *appMan, s32 *unused);

s32 ov39_02228140(ApplicationManager *appMan, s32 *unused) {
    int new_var;
    UnkStruct_02017498 *temp_r1;
    UnkStruct_ov61_0222C664 *temp_r0;
    OverlayManager *new_var4;
    s32 *new_var3;
    s16 *new_var2;
    Main_SetVBlankIntrCB(0L, 0L);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT = (s32)(reg_GX_DISPCNT & 0xFFFFE0FF);
    reg_GXS_DB_DISPCNT = (s32)(0xFFFFE0FF & reg_GXS_DB_DISPCNT);
    new_var3 = &reg_GX_DISPCNT;
    *new_var3 = (s32)((*new_var3) & 0xFFFF1FFF);
    new_var2 = &reg_G2S_DB_BLDCNT;
    reg_GXS_DB_DISPCNT = (new_var = (s32)(new_var = 0xFFFF1FFF & reg_GXS_DB_DISPCNT));
    reg_G2_BLDCNT = 0;
    *new_var2 = 0;
    Heap_Create(HEAP_ID_3, HEAP_ID_124, 0x50000U);
    temp_r0 = OverlayManager_CreateAndGetData(new_var4 = (OverlayManager *)appMan, sizeof(UnkStruct_ov61_0222C664), HEAP_ID_124);
    MI_CpuFill8(temp_r0, 0U, sizeof(UnkStruct_ov61_0222C664));
    temp_r0->unk_00 = OverlayManager_GetArgs(new_var4);
    temp_r0->unk_04 = BgConfig_Alloc(HEAP_ID_124);
    GF_CreateVramTransferManager(0x40U, HEAP_ID_124);
    SetKeyRepeatTimers(4, 8);
    ov39_02228440(temp_r0->unk_04);
    EnableTouchPad();
    InitializeTouchPad(4);
    temp_r0->unk_20 = (struct StringTemplate *)MessageFormat_New_Custom(0xBU, 0x40U, HEAP_ID_124);
    temp_r0->unk_24 = (struct MsgData *)NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x307, HEAP_ID_124);
    temp_r0->unk_28 = (struct MsgData *)NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x30A, HEAP_ID_124);
    temp_r0->unk_2C = (struct MsgData *)NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x320, HEAP_ID_124);
    temp_r0->unk_34 = String_New(0xB4U, HEAP_ID_124);
    temp_r0->unk_3C = String_New(0x100U, HEAP_ID_124);
    temp_r0->unk_38 = (struct String *)((struct MsgData *)NewString_ReadMsgData((struct MsgData *)temp_r0->unk_24, 0x1F));
    ov39_022285CC(temp_r0);
    ov39_022288A0(temp_r0);
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, HEAP_ID_124);
    GfGfx_EngineATogglePlanes(0x10U, 1U);
    GfGfx_EngineATogglePlanes(1U, 1U);
    GfGfx_EngineATogglePlanes(2U, 1U);
    GfGfx_EngineBTogglePlanes(1U, 1U);
    GfGfx_EngineBTogglePlanes(2U, 1U);
    gSystem.screensFlipped = 1;
    GfGfx_SwapDisplay();
    TextFlags_SetCanABSpeedUpPrint(1);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(1);
    Main_SetVBlankIntrCB(ov39_02228418, temp_r0);
    temp_r1 = temp_r0->unk_00;
    if (temp_r1->unk_10) {
        sub_0203A880();
        ov39_02228B6C(temp_r0, 0xD, 8);
    } else if (temp_r1->unk_00->unk_08 != 0) {
        temp_r0->unk_08 = 1;
    } else {
        temp_r0->unk_08 = 0;
    }
    temp_r0->unk_90 = (s32)Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(temp_r0->unk_00->unk_00->saveData));
    temp_r0->unk_00->unk_8C = 0;
    return 1;
}
