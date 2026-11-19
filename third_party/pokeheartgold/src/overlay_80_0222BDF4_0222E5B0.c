#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "overlay_42.h"
#include "palette.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "gf_gfx_planes.h"

struct BattleFrontier;
struct ListMenuCursor;
struct FrontierGraphics;
struct FrontierParticleSystem;
struct FrontierScriptContext;
struct FrontierScriptManager;
struct G3DPipelineBuffers;
struct ListMenu;
struct ListMenu2D;
struct MsgData;
struct MessageFormat;
struct UnkStruct_ov63_0222BCE8_t;
struct UnkStruct_ov63_0222BE18_t;
struct UnkStruct_ov63_0222D1C0_t;
struct UnkStruct_ov63_0222D77C_t;
struct UnkStruct_ov63_0222D848_t;
typedef struct BattleFrontier BattleFrontier;
typedef struct {
    u16 unk_00[8];
} UnkStruct_ov104_0222E8C8;
typedef struct ListMenuCursor ListMenuCursor;
typedef struct MsgData MsgData;
typedef struct ListMenuItem {
    void *entry;
    union {
        u32 index;
        void *callbackFunction;
    };
} ListMenuItem;
typedef struct ListMenu ListMenu;
typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);
typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);
typedef struct ListMenuTemplate {
    const ListMenuItem *choices;
    CursorCallback cursorCallback;
    PrintCallback printCallback;
    Window *window;
    u16 count;
    u16 maxDisplay;
    u8 headerXOffset;
    u8 textXOffset;
    u8 cursorXOffset;
    u8 yOffset : 4;
    u8 textColorFg : 4;
    u8 textColorBg : 4;
    u8 textColorShadow : 4;
    u16 letterSpacing : 3;
    u16 lineSpacing : 4;
    u16 pagerMode : 2;
    u16 fontID : 6;
    u16 cursorType : 1;
    void *parent;
} ListMenuTemplate;
struct ListMenu {
    ListMenuTemplate template;
    u8 cursorPal : 4;
    u8 fillValue : 4;
    u8 cursorShadowPal : 4;
    u8 unk_21_4 : 4;
    u8 lettersSpacing : 6;
    u8 unk_23;
    u8 fontId : 7;
    u8 overrideEnabled : 1;
    struct ListMenuCursor *cursor;
    u16 cursorPos;
    u16 itemsAbove;
    u8 unk_30;
    u8 unk_31;
    u8 taskId;
    u8 unk_33;
    u8 heapID;
};
typedef struct ListMenu2DTemplate {
    ListMenuItem *choices;
    Window *window;
    u8 fontID;
    u8 xSize;
    u8 ySize;
    u8 lineSpacing : 4;
    u8 suppressCursor : 2;
    u8 loopAround : 2;
} ListMenu2DTemplate;
typedef struct ListMenu2D ListMenu2D;
ListMenu2D *Create2dMenu(const ListMenu2DTemplate *template, u8 cursorStart, u8 heapID);
typedef struct MessageFormat MessageFormat;
typedef struct FrontierScriptManager FrontierScriptManager;
typedef struct FrontierScriptContext FrontierScriptContext;
typedef BOOL (*FrontierScrCmdFunc)(FrontierScriptContext *);
typedef BOOL (*FrontierShouldResumeScriptFunc)(FrontierScriptContext *);
struct FrontierScriptContext {
    FrontierScriptManager *frontierSystem;
    u8 padding[4];
    u16 unk_08[8];
    u8 stackPointer;
    u8 state;
    const u8 *scriptPtr;
    const u8 *stack[20];
    const FrontierScrCmdFunc *cmdTable;
    u32 cmdTableSize;
    u16 data[4];
    MsgData *msgLoader;
    u8 *scripts;
    FrontierShouldResumeScriptFunc shouldResume;
    u8 unk_8C;
};
typedef struct FrontierMenuManager {
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
    ListMenu2DTemplate menuTemplate;
    ListMenu2D *menu;
    ListMenuItem menuChoiceStrings[28];
    ListMenuTemplate listMenuTemplate;
    ListMenu *listMenu;
    u16 unused5;
    u16 listMenuAltTextIndex;
    ListMenuItem listMenuChoiceStrings[28];
    u16 choicesAltTextEntryIDs[28];
    u16 cursorPos;
} FrontierMenuManager;
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
    ListMenu2D *menu;
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
typedef struct UnkStruct_ov63_0222BCE8_t UnkStruct_ov63_0222BCE8;
typedef struct UnkStruct_ov63_0222BE18_t UnkStruct_ov63_0222BE18;
typedef struct UnkStruct_ov63_0222D1C0_t UnkStruct_ov63_0222D1C0;
typedef struct UnkStruct_ov63_0222D77C_t UnkStruct_ov63_0222D77C;
typedef struct UnkStruct_ov63_0222D848_t UnkStruct_ov63_0222D848;
typedef struct FrontierParticleSystem FrontierParticleSystem;
typedef struct {
    SysTask *unk_00;
    s16 unk_04;
    s16 unk_06;
    s8 unk_08;
    s8 unk_09;
    s8 unk_0A;
    u8 unk_0B;
} UnkStruct_ov104_0223319C;
typedef struct {
    SysTask *unk_00;
    u8 unk_04;
    u8 unk_05;
    u8 unk_06;
    u8 unk_07;
    s16 unk_08;
    s8 unk_0A;
    u8 unk_0B;
} UnkStruct_ov104_022331E8;
typedef struct {
    ManagedSprite *unk_00[8];
    u16 unk_20[8];
    u32 unk_30;
    u16 unk_34[8];
} UnkStruct_ov104_0223D3B0_1;
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
typedef struct FrontierGraphics FrontierGraphics;
struct FrontierGraphics {
    BgConfig *bgConfig;
    PaletteData *plttData;
    BattleFrontier *frontier;
    G3DPipelineBuffers *g3dPipeline;
    FrontierParticleSystem *particleSys;
    UnkStruct_ov63_0222BE18 *unk_14;
    UnkStruct_ov63_0222BCE8 *unk_18;
    UnkStruct_ov44_02232914 unk_1C;
    UnkStruct_ov42_02228EDC *unk_20;
    UnkStruct_ov63_0222D1C0 *unk_24;
    UnkStruct_ov63_0222D1C0 *unk_28;
    UnkStruct_ov63_0222D848 *unk_2C;
    UnkStruct_ov63_0222D77C *unk_30;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteMan;
    UnkStruct_ov104_0223D3B0_1 unk_3C;
    ManagedSprite *unk_80[4];
    void *unk_90;
    SysTask *unk_94;
    SysTask *unk_98;
    SysTask *unk_9C;
    SysTask *dummyExecuteOnVBlank;
    UnkStruct_ov104_0223319C unk_A4;
    UnkStruct_ov104_022331E8 unk_B0;
    u16 unk_BC;
    u16 unk_BE;
    u8 unk_C0;
    u8 sceneID;
    u8 unk_C2;
    u8 pad_C3[1];
};
void DrawFrameAndWindow1(Window *window, u8 skipTransfer, u16 tile, u8 palette);
FrontierGraphics *FrontierSystem_GetFrontierMap(FrontierScriptManager *scriptMan);
u32 ov80_0222E714(FrontierMenuManager *menuManager);
void ov80_0222E754(FrontierMenuManager *menuManager);
void ov80_0222E7C8(SysTask *task, void *data);
void ov80_0222E8FC(FrontierMenuManager *menuManager);
void ReadMsgDataIntoString(const MsgData *loader, u32 entryID, String *string);
void StringExpandPlaceholders(const MessageFormat *template, String *dst, const String *fmtString);

void ov80_0222E5B0(FrontierMenuManager *menuManager);
void ov80_0222E690(FrontierMenuManager *menuManager, u32 entryID, u32 altTextEntryID, u32 index);

void ov80_0222E5B0(FrontierMenuManager *menuManager) {
    u32 menuWidth;
    FrontierGraphics *graphics = FrontierSystem_GetFrontierMap(menuManager->scriptMan);

    menuWidth = ov80_0222E714(menuManager);
    if (menuWidth % 8 == 0) {
        menuWidth /= 8;
    } else {
        menuWidth = menuWidth / 8 + 1;
    }

    if (menuManager->anchorRight) {
        menuManager->anchorX -= menuWidth;
    }
    if (menuManager->anchorBottom) {
        menuManager->anchorY -= menuManager->optionCount * 2;
    }

    AddWindowParameterized(graphics->bgConfig, &menuManager->window, GF_BG_LYR_MAIN_1, menuManager->anchorX, menuManager->anchorY, menuWidth, menuManager->optionCount * 2, 14, 1);
    DrawFrameAndWindow1(&menuManager->window, 1, 985, 12);
    ov80_0222E754(menuManager);
    menuManager->menu = Create2dMenu(&menuManager->menuTemplate, menuManager->initialCursorPos, menuManager->scriptMan->heapID);
    ov80_0222E8FC(menuManager);
    menuManager->sysTask = SysTask_CreateOnMainQueue(ov80_0222E7C8, menuManager, 0);
}

void ov80_0222E690(FrontierMenuManager *menuManager, u32 entryID, u32 altTextEntryID, u32 index) {
    String *entryBuf = String_New(80, menuManager->scriptMan->heapID);

    ReadMsgDataIntoString(menuManager->msgLoader, entryID, entryBuf);
    StringExpandPlaceholders(menuManager->strTemplate, menuManager->choiceStringBuffers[menuManager->optionCount], entryBuf);
    menuManager->menuChoiceStrings[menuManager->optionCount].entry = menuManager->choiceStringBuffers[menuManager->optionCount];
    String_Delete(entryBuf);

    menuManager->choicesAltTextEntryIDs[menuManager->optionCount] = altTextEntryID;
    menuManager->menuChoiceStrings[menuManager->optionCount].index = index;
    menuManager->optionCount++;
}
