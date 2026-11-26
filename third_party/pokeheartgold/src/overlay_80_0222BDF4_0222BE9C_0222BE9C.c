#include "global.h"
#include "overlay_80_frontier_anim.h"
#include "overlay_80_unk_struct_ov104_0223_c634.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "pm_string.h"
#include "msgdata.h"
#include "frontier/frontier_script_context.h"

struct BattleFrontier;


struct FrontierMenuManager;

struct FrontierScriptContext;

struct FrontierScriptManager;

struct ListMenu2D;

struct MsgData;

struct String;

struct MessageFormat;

struct SysTask;

struct SysTaskQueue;

struct UnkStruct_ov63_0222D77C_t;

struct Window;

typedef struct BattleFrontier BattleFrontier;

typedef struct {
    u16 unk_00[8];
} UnkStruct_ov104_0222E8C8;

typedef struct ListMenu2D ListMenu2D;

typedef struct FrontierScriptManager FrontierScriptManager;

typedef struct FrontierMenuManager FrontierMenuManager;

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

u16 *ov80_0222BE24(FrontierScriptContext *ctx, u16 varID);

typedef struct UnkStruct_ov63_0222D77C_t UnkStruct_ov63_0222D77C;


void ov80_0222C7B0(s16 localID, UnkStruct_ov104_0223C634 *object, const FrontierAnimCmd *animCmd, UnkStruct_ov63_0222D77C *param3, u8 *movementCount, enum HeapID heapID);

u16 ov80_0222BE9C(FrontierScriptContext *ctx, u16 varID);

u16 ov80_0222BE9C(FrontierScriptContext *ctx, u16 varID) {
    u16 *var = ov80_0222BE24(ctx, varID);

    if (var == NULL) {
        return varID;
    }

    return *var;
}
