#include "global.h"
#include "overlay_80_unk_struct_ov104_0223_c634.h"
#include "overlay_80_frontier_anim.h"
#include "sys_task.h"
#include "constants/heap.h"
#include "font.h"

struct BattleFrontier;
struct ListMenuCursor;
struct FrontierScriptContext;
struct FrontierScriptManager;
struct ListMenu;
struct ListMenu2D;
struct MsgData;
struct String;
struct MessageFormat;
struct SysTask;
struct SysTaskManager;
struct UnkStruct_ov63_0222BB38_t;
struct UnkStruct_ov63_0222BEC0_t;
struct UnkStruct_ov63_0222D77C_t;
void String_Delete(String *string);
typedef struct BattleFrontier BattleFrontier;
typedef struct {
    u16 unk_00[8];
} UnkStruct_ov104_0222E8C8;
void FillWindowPixelBuffer(Window *window, u8 val);
u8 AddTextPrinterParameterized(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, PrinterCallback_t callback);
typedef struct ListMenuCursor ListMenuCursor;
typedef struct MsgData MsgData;
void ReadMsgDataIntoString(const MsgData *loader, u32 entryID, String *string);
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
typedef struct SysTaskManager SysTaskManager;
struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
};
typedef struct MessageFormat MessageFormat;
void StringExpandPlaceholders(const MessageFormat *template, String *dst, const String *fmtString);
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
void ListMenuGetCurrentItemArrayId(ListMenu *menu, u16 *outPos);
void ov80_0222EE14(FrontierMenuManager *menuManager, u16 entryID, u32 printerDelay);
void ov80_0222EDA0(FrontierMenuManager *menuManager, s32 playSound);
typedef struct UnkStruct_ov63_0222BEC0_t UnkStruct_ov63_0222BEC0;
typedef struct {
    s16 unk_00;
    s16 unk_02;
} UnkStruct_ov63_0222CC3C;
typedef struct {
    UnkStruct_ov63_0222CC3C unk_00;
    u16 unk_04;
    u8 unk_06;
    u8 unk_07;
} UnkStruct_ov63_0222CCB8;
typedef struct UnkStruct_ov63_0222BB38_t UnkStruct_ov63_0222BB38;
typedef struct {
    const UnkStruct_ov63_0222BEC0 *unk_00;
    UnkStruct_ov63_0222BB38 *unk_04;
    u16 unk_08;
    u16 unk_0A;
    u16 unk_0C;
    u16 unk_0E;
    u16 unk_10;
    u8 unk_12;
    u8 unk_13;
} UnkStruct_ov63_0222CE44;
typedef struct UnkStruct_ov63_0222D77C_t UnkStruct_ov63_0222D77C;
void ov80_0222EFD0(UnkStruct_ov63_0222CCB8 *param0, UnkStruct_ov63_0222BEC0 *param1, int param2, int action);
s32 ov42_02228188(const UnkStruct_ov63_0222BEC0 *param0, int param1);
void ov42_02229200(UnkStruct_ov63_0222CE44 *param0, u16 param1);
void ov42_022299C0(UnkStruct_ov63_0222D77C *param0, const UnkStruct_ov63_0222CCB8 *param1);
void Heap_Free(void *ptr);
void SysTask_Destroy(SysTask *task);
extern const u8 ov80_0223BD44[7];

void ov80_0222EE14(FrontierMenuManager *menuManager, u16 entryID, u32 printerDelay);
void ov80_0222EE7C(FrontierMenuManager *menuManager);
void ov80_0222EEAC(FrontierMenuManager *menuManager);
void ov80_0222EEC8(SysTask *task, void *data);

UnkStruct_ov63_0222CC3C ov42_022282DC(const UnkStruct_ov63_0222BEC0 *param0);
UnkStruct_ov63_0222CC3C ov42_022282E8(const UnkStruct_ov63_0222BEC0 *param0);
typedef struct {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_ov104_022419A0;
extern UnkStruct_ov104_022419A0 _0223DC20[32];

void ov80_0222EE14(FrontierMenuManager *menuManager, u16 entryID, u32 printerDelay) {
    String *fmtStr = String_New(80, menuManager->scriptMan->heapID);
    String *displayStr = String_New(80, menuManager->scriptMan->heapID);

    FillWindowPixelBuffer(menuManager->parent, 15);

    ReadMsgDataIntoString(menuManager->msgLoader, entryID, fmtStr);
    StringExpandPlaceholders(menuManager->strTemplate, displayStr, fmtStr);

    AddTextPrinterParameterized(menuManager->parent, FONT_MESSAGE, displayStr, 0, 0, printerDelay, 0L);

    String_Delete(fmtStr);
    String_Delete(displayStr);
}

void ov80_0222EE7C(FrontierMenuManager *menuManager) {
    ListMenuGetCurrentItemArrayId(menuManager->listMenu, &menuManager->listMenuAltTextIndex);

    if (menuManager->choicesAltTextEntryIDs[menuManager->listMenuAltTextIndex] != 0xFF) {
        ov80_0222EE14(menuManager, menuManager->choicesAltTextEntryIDs[menuManager->listMenuAltTextIndex], 0);
    }
}

void ov80_0222EEAC(FrontierMenuManager *menuManager) {
    if (menuManager == 0L) {
        return;
    }

    *menuManager->selectedOptionPtr = -2;
    ov80_0222EDA0(menuManager, 0);
}

void ov80_0222EEC8(SysTask *task, void *data) {
    FrontierAnimData *animData = data;
    UnkStruct_ov63_0222CCB8 v1;
    u16 action = animData->animCmd->action;
    u16 count = animData->animCmd->count;

    switch (animData->state) {
    case 0:
        if (action == 0xFD13) {
            animData->state = 2;
            break;
        }
    case 1:
        if (ov42_02228188(animData->frontierObj->unk_00, 5) == 0) {
            if (action >= 32 && action <= 38) {
                animData->delay++;

                if (animData->delay >= ov80_0223BD44[action - 32]) {
                    animData->delay = 0;
                    animData->animCmd++;
                }
                break;
            } else if (action >= 39 && action < 41) {
                switch (action) {
                case 39:
                    ov42_02229200(animData->frontierObj->sprite, 1);
                    break;
                case 40:
                    ov42_02229200(animData->frontierObj->sprite, 0);
                    break;
                default:
                    ((0) ? (void)0 : GF_AssertFail());
                    break;
                }

                animData->animCmd++;
                break;
            }

            ov80_0222EFD0(&v1, animData->frontierObj->unk_00, animData->localID, action);
            ov42_022299C0(animData->unk_10, &v1);

            animData->elapsed++;

            if (animData->elapsed >= count) {
                animData->elapsed = 0;
                animData->animCmd++;
            }

            animData->state = 0;
        }
        break;
    case 2:
        if (ov42_02228188(animData->frontierObj->unk_00, 5) == 0) {
            (*animData->totalMovementCount)--;
            animData->frontierObj->movementTask = 0L;
            Heap_Free(animData);
            SysTask_Destroy(task);
            return;
        }
        break;
    }
}

void ov80_0222EFD0(UnkStruct_ov63_0222CCB8 *param0, UnkStruct_ov63_0222BEC0 *param1, int param2, int action) {
    param0->unk_07 = param2;
    param0->unk_06 = _0223DC20[action].unk_02;
    param0->unk_04 = _0223DC20[action].unk_00;

    if ((param0->unk_04 == 2) || (param0->unk_04 == 3) || (param0->unk_04 == 6)) {
        param0->unk_00 = ov42_022282E8(param1);
    } else {
        param0->unk_00 = ov42_022282DC(param1);
    }
}
