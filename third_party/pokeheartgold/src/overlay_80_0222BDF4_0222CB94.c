#include "global.h"

#include "frontier_launch.h"

#include "constants/heap.h"

#include "bag_cursor.h"
#include "bg_window.h"
#include "options.h"
#include "overlay_42.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "overlay_80_field_frontier_dto.h"

struct BattleFrontier;
struct FrontierMenuManager;
struct FrontierScriptContext;
struct FrontierScriptManager;
struct JournalEntry;
struct Menu;
struct MessageLoader;
struct StringTemplate;
SysTask *NamingScreen_DeleteArgs(SysTaskFunc callback, void *param, u32 priority);
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
void sub_0202FBF0(SaveData *saveData, enum HeapID heapID, int *resultCode);
u16 *FrontierScript_ReadVarPtr(FrontierScriptContext *ctx);
u16 FrontierScript_ReadVar(FrontierScriptContext *ctx);
void FrontierScriptContext_Pause(FrontierScriptContext *ctx, FrontierShouldResumeScriptFunc shouldResume);
u16 FrontierScriptContext_ReadHalfWord(FrontierScriptContext *ctx);
static BOOL ov80_0222CC20(FrontierScriptContext *ctx);
static int ov80_0222CC70(u16 param0, u16 param1);
int sub_0202FE14(SaveData *saveData, int param1, int param2, int param3, u16 *param4, u16 *param5);
typedef enum {
    UnkEnum_0202F510_00,
    UnkEnum_0202F510_01,
    UnkEnum_0202F510_02,
    UnkEnum_0202F510_03,
    UnkEnum_0202F510_04,
    UnkEnum_0202F510_05,
    UnkEnum_0202F510_06,
    UnkEnum_0202F510_07,
    UnkEnum_0202F510_08,
    UnkEnum_0202F510_09,
    UnkEnum_0202F510_10,
    UnkEnum_0202F510_11,
    UnkEnum_0202F510_12,
    UnkEnum_0202F510_13,
    UnkEnum_0202F510_14,
    UnkEnum_0202F510_15,
    UnkEnum_0202F510_16,
    UnkEnum_0202F510_17,
    UnkEnum_0202F510_18,
    UnkEnum_0202F510_19,
    UnkEnum_0202F510_20,
    UnkEnum_0202F510_21,
    UnkEnum_0202F510_22,
    UnkEnum_0202F510_23,
    UnkEnum_0202F510_24,
    UnkEnum_0202F510_25,
    UnkEnum_0202F510_26,
    UnkEnum_0202F510_27,
    UnkEnum_0202F510_28,
    UnkEnum_0202F510_29,
    UnkEnum_0202F510_30,
    UnkEnum_0202F510_31,
    UnkEnum_0202F510_32,
    UnkEnum_0202F510_33 = 0xfa,
    UnkEnum_0202F510_34 = 0xfb,
    UnkEnum_0202F510_35 = 0xfc,
    UnkEnum_0202F510_36 = 0xfd,
    UnkEnum_0202F510_37 = 0xfe,
    UnkEnum_0202F510_38 = 0xff,
} UnkEnum_0202F510;

SysTask *ov80_0222CB94(SysTaskFunc callback, void *param, int priority);
BOOL FrtCmd_108(FrontierScriptContext *ctx);
BOOL FrtCmd_109(FrontierScriptContext *ctx);
static BOOL ov80_0222CC20(FrontierScriptContext *ctx);
static int ov80_0222CC70(u16 param0, u16 param1);

SysTask *ov80_0222CB94(SysTaskFunc callback, void *param, int priority) {
    return NamingScreen_DeleteArgs(callback, param, priority);
}

BOOL FrtCmd_108(FrontierScriptContext *ctx) {
    int resultCode;
    FieldFrontierDTO *fieldData = Frontier_GetLaunchArgs(ctx->frontierSystem->frontier);

    sub_0202FBF0(fieldData->saveData, HEAP_ID_FIELD2, &resultCode);
    return 0;
}

BOOL FrtCmd_109(FrontierScriptContext *ctx) {
    FrontierScriptManager *v0 = ctx->frontierSystem;
    u16 v1 = FrontierScriptContext_ReadHalfWord(ctx);
    u16 v2 = FrontierScript_ReadVar(ctx);
    u16 v3 = FrontierScript_ReadVar(ctx);
    u16 *v4 = FrontierScript_ReadVarPtr(ctx);

    if (v3 > 9999) {
        v3 = 9999;
    }

    v0->unk_B0 = 0;
    v0->unk_B2 = 0;
    v0->unk_B4 = ov80_0222CC70(v1, v2);
    v0->unk_B6 = v3;
    v0->unk_B8 = v4;

    FrontierScriptContext_Pause(ctx, ov80_0222CC20);
    return 1;
}

static BOOL ov80_0222CC20(FrontierScriptContext *ctx) {
    FrontierScriptManager *v0 = ctx->frontierSystem;
    FieldFrontierDTO *fieldData = Frontier_GetLaunchArgs(ctx->frontierSystem->frontier);
    int v2 = sub_0202FE14(fieldData->saveData, v0->unk_B4, v0->unk_B6, 0, &v0->unk_B0, &v0->unk_B2);

    if ((v2 == 2) || (v2 == 3)) {
        if (v2 == 2) {
            *(v0->unk_B8) = 1;
        } else {
            *(v0->unk_B8) = 0;
        }

        return 1;
    }

    return 0;
}

static int ov80_0222CC70(u16 param0, u16 param1) {
    int v0 = (UnkEnum_0202F510_15);

    switch (param0) {
    case 1:
        switch (param1) {
        case 0:
            v0 = (UnkEnum_0202F510_15);
            break;
        case 1:
            v0 = (UnkEnum_0202F510_16);
            break;
        case 2:
        case 3:
        case 6:
            v0 = (UnkEnum_0202F510_17);
            break;
        case 4:
        case 5:
            v0 = (UnkEnum_0202F510_15);
            break;
        default:
            ((0) ? (void)0 : GF_AssertFail());
            break;
        }
        break;
    case 2:
        switch (param1) {
        case 0:
            v0 = (UnkEnum_0202F510_18);
            break;
        case 1:
            v0 = (UnkEnum_0202F510_19);
            break;
        case 2:
        case 3:
            v0 = (UnkEnum_0202F510_20);
            break;
        default:
            ((0) ? (void)0 : GF_AssertFail());
            break;
        }
        break;
    case 3:
        switch (param1) {
        case 0:
            v0 = (UnkEnum_0202F510_21);
            break;
        case 1:
            v0 = (UnkEnum_0202F510_22);
            break;
        case 2:
        case 3:
            v0 = (UnkEnum_0202F510_23);
            break;
        default:
            ((0) ? (void)0 : GF_AssertFail());
            break;
        }
        break;
    case 4:
        switch (param1) {
        case 0:
            v0 = (UnkEnum_0202F510_27);
            break;
        case 1:
            v0 = (UnkEnum_0202F510_28);
            break;
        case 2:
        case 3:
            v0 = (UnkEnum_0202F510_29);
            break;
        default:
            ((0) ? (void)0 : GF_AssertFail());
            break;
        }
        break;
    case 5:
        switch (param1) {
        case 0:
            v0 = (UnkEnum_0202F510_24);
            break;
        case 1:
            v0 = (UnkEnum_0202F510_25);
            break;
        case 2:
        case 3:
            v0 = (UnkEnum_0202F510_26);
            break;
        default:
            ((0) ? (void)0 : GF_AssertFail());
            break;
        }
        break;
    case 6:
        switch (param1) {
        case 0:
            v0 = (UnkEnum_0202F510_30);
            break;
        case 1:
            v0 = (UnkEnum_0202F510_31);
            break;
        case 2:
        case 3:
            v0 = (UnkEnum_0202F510_32);
            break;
        default:
            ((0) ? (void)0 : GF_AssertFail());
            break;
        }
        break;
    }

    return v0;
}
