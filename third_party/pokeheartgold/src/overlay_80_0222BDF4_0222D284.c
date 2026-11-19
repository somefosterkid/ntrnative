#include "global.h"

#include "constants/heap.h"

#include "bag_cursor.h"
#include "bg_window.h"
#include "options.h"
#include "overlay_42.h"
#include "party.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "sys_task.h"
#include "use_item_on_mon.h"
#include "overlay_80_field_frontier_dto.h"

struct BattleFrontier;
struct FrontierMenuManager;
struct FrontierScriptContext;
struct FrontierScriptManager;
struct ItemFieldUseContext;
struct JournalEntry;
struct Menu;
struct MessageLoader;
struct StringTemplate;
typedef struct BattleFrontier BattleFrontier;
typedef struct {
    u16 unk_00[8];
} UnkStruct_ov104_0222E8C8;
typedef struct MessageLoader MessageLoader;
typedef struct Menu Menu;
typedef struct StringTemplate StringTemplate;
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
    MessageLoader *msgLoader;
    u8 *scripts;
    FrontierShouldResumeScriptFunc shouldResume;
    u8 unk_8C;
};
typedef struct FrontierMenuManager FrontierMenuManager;
struct FrontierScriptManager {
    BattleFrontier *frontier;
    FrontierScriptContext *ctx[8];
    UnkStruct_ov104_0222E8C8 unk_24;
    enum HeapID heapID;
    u8 unk_38;
    MessageLoader *msgLoader;
    u8 *script;
    StringTemplate *strTemplate;
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
typedef struct JournalEntry JournalEntry;
FieldFrontierDTO *Frontier_GetLaunchArgs(BattleFrontier *frontier);
typedef struct ItemFieldUseContext ItemFieldUseContext;
void *ov80_0222D2AC(void *fieldSystem);
void FrontierScriptContext_Pause(ItemFieldUseContext *usageContext, void *param1);

BOOL FrtCmd_057(FrontierScriptContext *ctx);
BOOL FrtCmd_058(ItemFieldUseContext *usageContext);

BOOL FrtCmd_057(FrontierScriptContext *ctx) {
    FieldFrontierDTO *fieldData = Frontier_GetLaunchArgs(ctx->frontierSystem->frontier);
    HealParty(SaveArray_Party_Get(fieldData->saveData));
    return 0;
}

BOOL FrtCmd_058(ItemFieldUseContext *usageContext) {
    FrontierScriptContext_Pause(usageContext, ov80_0222D2AC);
    return 1;
}
