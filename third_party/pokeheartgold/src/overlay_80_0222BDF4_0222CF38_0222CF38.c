#include "global.h"
#include "constants/heap.h"
#include "string_util.h"

struct BattleFrontier;

struct BgConfig;

struct FrontierMenuManager;

struct FrontierScriptContext;

struct FrontierScriptManager;

struct ListMenu2D;

struct MsgData;

struct String;

struct MessageFormat;

typedef struct BattleFrontier BattleFrontier;

typedef struct {
    u16 unk_00[8];
} UnkStruct_ov104_0222E8C8;

typedef struct BgConfig BgConfig;

typedef struct Window {
    BgConfig *bgConfig;
    u8 bgLayer;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 width;
    u8 height;
    u8 palette;
    u16 baseTile : 15;
    u16 colorMode : 1;
    void *pixels;
} Window;

typedef struct String String;

enum CharsetMode {

    CHARSET_MODE_JP,

    CHARSET_MODE_EN,
};

typedef struct MsgData MsgData;

typedef struct ListMenu2D ListMenu2D;

typedef struct MessageFormat MessageFormat;

void BufferIntegerAsString(MessageFormat *template, u32 idx, int num, u32 maxDigits, enum PrintingMode paddingMode, enum CharsetMode charsetMode);

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

u16 FrontierScript_ReadVar(FrontierScriptContext *ctx);

u16 ov80_0222CF6C(u32 number);

BOOL FrtCmd_122(FrontierScriptContext *ctx);

BOOL FrtCmd_122(FrontierScriptContext *ctx) {
    u8 templateArg = (*(ctx->scriptPtr++));
    u16 number = FrontierScript_ReadVar(ctx);

    BufferIntegerAsString(ctx->frontierSystem->strTemplate, templateArg, number, ov80_0222CF6C(number), PRINTING_MODE_RIGHT_ALIGN, CHARSET_MODE_EN);
    return 0;
}
