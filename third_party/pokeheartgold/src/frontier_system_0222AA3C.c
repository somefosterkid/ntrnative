#include "global.h"

#include "filesystem.h"
#include "frontier_system.h"
#include "msgdata.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "heap.h"
#include "pm_string.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_0200A090.h"

void FrontierSystem_deadstripped_0222AA3C(void);

struct BattleFrontier;
struct FrontierMenuManager;
struct FrontierScriptContext;
struct FrontierScriptManager;
struct Menu;
struct MsgData;
struct MessageFormat;
typedef struct BattleFrontier BattleFrontier;
typedef struct {
    u16 unk_00[8];
} UnkStruct_ov104_0222E8C8;
typedef struct Menu Menu;
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
UnkStruct_ov104_0222E8C8 *ov80_0222AAD8(FrontierScriptManager *param0, enum HeapID heapID);
u32 FrontierScriptContext_ReadWord(FrontierScriptContext *ctx);
void sub_0209680C(Sprite *gfxElem);
typedef struct ChooseStarterRotation {
    fx32 verticalOffset;
    fx32 verticalOffsetMultiplier;
    int frameCountMax;
    int frameCount;
} ChooseStarterRotation;
typedef struct ChooseStarterCursor {
    Sprite *sprite;
    SpriteResource *resources[6];
    VecFx32 position;
    SysTask *movementTask;
    ChooseStarterRotation starterRotation;
} ChooseStarterCursor;
UnkStruct_ov104_0222E8C8 *ov80_0222AAD8(FrontierScriptManager *param0, enum HeapID heapID);
void ov80_0222AAF8(FrontierScriptManager *param0, UnkStruct_ov104_0222E8C8 *param1);
void ov80_0222AB14(FrontierScriptContext *ctx, int offsetID);
u16 *ov80_0222AB2C(FrontierScriptManager *param0, int param1);
void FrontierSystem_GetFrontierMap(ChooseStarterCursor *cursor);

void FrontierSystem_deadstripped_0222AA3C(void) {
}

s32 ov80_0222ACA0(s32, s32);

void ov80_0222AA40(void **arg0, MsgData **arg1, s32 arg2, enum HeapID arg3);

void ov80_0222AA40(void **arg0, MsgData **arg1, s32 arg2, enum HeapID heapID) {
    s32 file_id;
    s32 fileId;

    file_id = ov80_0222ACA0(arg2, 1);
    fileId = ov80_0222ACA0(arg2, 2);
    *arg0 = AllocAndReadWholeNarcMemberByIdPair(NARC_a_1_8_2, file_id, heapID);
    *arg1 = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, fileId, heapID);
}

void ov80_0222AA7C(void *arg0, u16 arg1, enum HeapID heapID) {
    MsgData *msgData;
    s32 fileId;
    s32 v1;
    s32 v2;
    void *v0;
    void *v3;

    v1 = ov80_0222ACA0(*(u16 *)((u8 *)arg0 + 0x52), 2);
    fileId = ov80_0222ACA0(arg1, 2);
    if (v1 != fileId) {
        msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, fileId, heapID);
        v2 = 0;
        v3 = arg0;
        do {
            v0 = *(void **)((u8 *)v3 + 4);
            if ((v0 != NULL) && (*(MsgData **)((u8 *)v0 + 0x80) == *(MsgData **)((u8 *)arg0 + 0x3C))) {
                *(MsgData **)((u8 *)v0 + 0x80) = msgData;
            }
            v2 += 1;
            v3 += 4;
        } while (v2 < 8);
        DestroyMsgData(*(MsgData **)((u8 *)arg0 + 0x3C));
        *(MsgData **)((u8 *)arg0 + 0x3C) = msgData;
    }
}

UnkStruct_ov104_0222E8C8 *ov80_0222AAD8(FrontierScriptManager *param0, enum HeapID heapID) {
    UnkStruct_ov104_0222E8C8 *v0 = Heap_Alloc(heapID, sizeof(UnkStruct_ov104_0222E8C8));
    *v0 = param0->unk_24;

    return v0;
}

void ov80_0222AAF8(FrontierScriptManager *param0, UnkStruct_ov104_0222E8C8 *param1) {
    param0->unk_24 = *param1;
    Heap_Free(param1);
}

void ov80_0222AB14(FrontierScriptContext *ctx, int offsetID) {
    ctx->scriptPtr += offsetID * 4;
    ctx->scriptPtr += FrontierScriptContext_ReadWord(ctx);
}

u16 *ov80_0222AB2C(FrontierScriptManager *param0, int param1) {
    return &param0->unk_24.unk_00[param1];
}

void FrontierSystem_GetFrontierMap(ChooseStarterCursor *cursor) {
    sub_0209680C(cursor->sprite);
}
