#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "game_stats.h"
#include "options.h"
#include "pm_string.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sprite.h"
#include "touch_hitbox_controller.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "yes_no_prompt.h"
#include "msgdata.h"
#include "struct_defs/resdat_narc_structs.h"
#include "gf_gfx_planes.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;

typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef struct SpriteResourceCollection SpriteResourceCollection;

typedef struct YesNoButtonAnimData YesNoButtonAnimData;

typedef struct YesNoButtonSprite YesNoButtonSprite;

typedef struct EncodingSeed EncodingSeed;

typedef struct SignatureTouchDataBuffer SignatureTouchDataBuffer;

typedef struct PreviousTouchDataBuffer PreviousTouchDataBuffer;

typedef struct DrawingState DrawingState;

typedef struct SignatureApp SignatureApp;

struct MessageBankEntry {
    u32 offset;
    u32 length;
};

struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
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

struct SpriteResourceCollection {
    GF_2DGfxRawResMan *collection;
    SpriteResource *resources;
    int capacity;
    int count;
    GfGfxResType type;
};

struct YesNoButtonAnimData {
    const fx32 *frameThresholds;
    u32 frameCount;
    fx32 frameProgress;
};

struct YesNoButtonSprite {
    BgConfig *bgConfig;
    enum GFBgLayer bgLayer;
    void *nscrBuffers[4];
    NNSG2dScreenData *screenData[4];
    YesNoButtonAnimData animData;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 animFrame;
};

struct EncodingSeed {
    u16 byteSum;
    u16 modifier;
};

struct GameStats {
    u32 recordsU32[71];
    u16 recordsU16[77];
    u16 padding;
    EncodingSeed seed;
};

struct SignatureTouchDataBuffer {
    u8 x[8];
    u8 y[8];
    u8 brushTypeUnused : 4;
    u8 bufferSize : 4;
};

struct PreviousTouchDataBuffer {
    u8 lastX;
    u8 lastY;
    u16 bufferSize;
};

struct DrawingState {
    s8 previousXDirection;
    s8 previousYDirection;
    s8 currentXDirection;
    s8 currentYDirection;
    u8 padding;
};

struct SignatureApp {
    BgConfig *bgConfig;
    u8 padding[4];
    GameStats *records;
    Options *options;
    MessageFormat *template;
    MsgData *loader;
    String *dummyStrings[5];
    String *buttonText;
    String *descriptionText;
    String *messageText;
    int printerID;
    SpriteList *spriteList;
    G2dRenderer g2DRenderer;
    SpriteResourceCollection *spriteResourceCollection[(GF_GFX_RES_TYPE_ANIM + 1)];
    SpriteResource *spriteResources[2][(GF_GFX_RES_TYPE_ANIM + 1)];
    SpriteResourcesHeader dummyResourcesHeader;
    SpriteResourcesHeader spriteResourcesHeader;
    u8 padding2[112];
    Sprite *sprite[1];
    u8 padding3[32];
    Window dummyWindows[5];
    Window signatureWindow;
    Window messageWindow;
    Window buttonWindow;
    Window descriptionWindow;
    u8 padding4[8];
    int subState;
    u8 padding5[16392];
    u16 glowEffectDegreesUnused;
    u8 brushTypeUnused;
    u8 padding6;
    SignatureTouchDataBuffer touchDataBuffer;
    SignatureTouchDataBuffer touchDataBuffers[5];
    PreviousTouchDataBuffer prevTouchData[5];
    u8 padding7[6144];
    u8 pad_5B96[2];
    u8 *savedSignature;
    u8 buttonPixelBuffer[256];
    struct YesNoPrompt *yesNoMenu;
    DrawingState drawingState;
    u8 pad_5CA5[3];
    int lastTouchedX;
    int lastTouchedY;
};

s32 ov52_021E8CDC(Sprite **, s32);

s32 ov52_021E8D3C(SignatureApp *);

int ov52_021E8DC4(SignatureApp *signatureApp, int state);

int ov52_021E8DC4(SignatureApp *signatureApp, int state) {
    enum YesNoResponse v0;

    v0 = YesNoPrompt_HandleInput((YesNoPrompt *)signatureApp->yesNoMenu);
    switch (v0) {
    case YESNORESPONSE_YES:
        GameStats_AddScore(signatureApp->records, 4);
        GameStats_Inc(signatureApp->records, 0x73);
        ClearFrameAndWindow2(&signatureApp->messageWindow, 1);
        YesNoPrompt_Reset(signatureApp->yesNoMenu);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 0x10, 1, HEAP_ID_39);
        return 2;
    case YESNORESPONSE_NO:
        signatureApp->subState = 4;
        ov52_021E8CDC(signatureApp->sprite, 0);
        ClearFrameAndWindow2(&signatureApp->messageWindow, 1);
        YesNoPrompt_Reset(signatureApp->yesNoMenu);
    default:
        signatureApp->touchDataBuffer.bufferSize = 0;
        ov52_021E8D3C(signatureApp);
        return state;
    }
}

void ov52_021E9488(SignatureApp *signatureApp, s32 param_2);

int ov52_021E8E64(SignatureApp *signatureApp, int state);

int ov52_021E8E64(SignatureApp *signatureApp, int state) {
    YesNoResponse YVar1;

    YVar1 = YesNoPrompt_HandleInput(signatureApp->yesNoMenu);
    switch (YVar1) {
    case YESNORESPONSE_YES:
        signatureApp->subState = 1;
        ClearFrameAndWindow2(&signatureApp->messageWindow, 1);
        YesNoPrompt_Reset(signatureApp->yesNoMenu);
        ov52_021E9488(signatureApp, 0);
        FillWindowPixelBuffer(&signatureApp->signatureWindow, 2);
        CopyWindowToVram(&signatureApp->signatureWindow);
        break;
    case YESNORESPONSE_NO:
        signatureApp->subState = 1;
        ClearFrameAndWindow2(&signatureApp->messageWindow, 1);
        YesNoPrompt_Reset(signatureApp->yesNoMenu);
        ov52_021E9488(signatureApp, 0);
        break;
    }
    return state;
}
