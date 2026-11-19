#include "global.h"

#include "frontier.h"
#include "constants/heap.h"
#include "frontier/frontier_script_context.h"
#include "bg_window.h"
#include "overlay_42.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "sys_task.h"
#include "overlay_80_unk_struct_ov104_0223_c634.h"
#include "frontier_graphics.h"
#include "overlay_80_field_frontier_dto.h"
#include "overlay_45_unk_struct_ov66_02234798.h"


struct OverlayManager;
struct BattleFrontier;
struct FieldFrontierDTO;
struct FrontierGraphics;
struct FrontierMenuManager;
struct FrontierScriptManager;
struct Menu;
struct MsgData;
struct MessageFormat;
struct UnkStruct_ov63_0222BEC0_t;
typedef struct BattleFrontier BattleFrontier;
typedef struct {
    u8 unk_00;
    u8 unk_01;
    s16 unk_02[7];
} UnkStruct_ov104_0223D8F0;
typedef struct {
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    s16 unk_06;
    s16 unk_08;
    u8 unk_0A;
    u8 unk_0B;
    u8 unk_0C;
    UnkStruct_ov104_0223D8F0 unk_0E;
} UnkStruct_ov104_0223D570;
typedef struct {
    u16 unk_00;
    u8 unk_02;
} UnkStruct_ov104_0223C688;
typedef struct {
    s16 unk_00;
    s16 unk_02;
    u8 unk_04;
    u8 unk_05;
    u16 unk_06_0 : 13;
    u16 unk_06_13 : 1;
    u16 unk_06_14 : 1;
    u16 unk_06_15 : 1;
} UnkStruct_ov104_0223D3B0_sub1;
typedef struct {
    u16 unk_00[8];
    UnkStruct_ov104_0223D3B0_sub1 unk_10[8];
} UnkStruct_ov104_0223D3B0;
typedef void (*BattleFrontierSubAppCallback)(void *);
typedef struct Menu Menu;
typedef struct {
    u16 unk_00[8];
} UnkStruct_ov104_0222E8C8;
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
struct BattleFrontier {
    FieldFrontierDTO *fieldData;
    OverlayManager *appMan;
    void *appArgs;
    BattleFrontierSubAppCallback finishCallback;
    u8 freeArgsAfter;
    FrontierScriptManager *scriptMan;
    FrontierGraphics *graphics;
    u8 unused;
    u8 isGraphicsInitialized;
    u8 changeScript;
    u16 offsetID;
    u8 exitBattleFrontier;
    UnkStruct_ov104_0223C688 unk_24[24];
    UnkStruct_ov104_0223C634 unk_6C[32];
    UnkStruct_ov104_0223D8F0 unk_78C[32];
    UnkStruct_ov104_0223D3B0 unk_98C;
};
UnkStruct_ov104_0223D3B0 *sub_02096878(BattleFrontier *frontier);
void sub_02096884(BattleFrontier *frontier);

s32 sub_0209686C(s32 arg0, s32 arg1) {
    return arg0 + 0x84 + (0x3C * arg1);
}

UnkStruct_ov104_0223D3B0 *sub_02096878(BattleFrontier *frontier) {
    return &frontier->unk_98C;
}

void sub_02096884(BattleFrontier *frontier) {
    MI_CpuClear8(&frontier->unk_98C, sizeof(UnkStruct_ov104_0223D3B0_sub1));

    for (int v0 = 0; v0 < 8; v0++) {
        frontier->unk_98C.unk_00[v0] = 0xffff;
    }
}
