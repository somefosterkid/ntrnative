#include "global.h"

#include "options.h"
#include "pm_string.h"
#include "sprite.h"
#include "struct_defs/resdat_narc_structs.h"

extern const struct {
    u16 x;
    u16 y;
    u16 animID;
} _021E94AC[1];

void ov52_021E8CBC(Sprite **buttonSprites, int pressedButtonIndex);

void ov52_021E8CBC(Sprite **buttonSprites, int pressedButtonIndex) {
    for (int i = 0; i < 1; i++) {
        if (i == pressedButtonIndex) {
            Sprite_SetAnimCtrlSeq(buttonSprites[i], _021E94AC[i].animID + 1);
        } else {
            Sprite_SetAnimCtrlSeq(buttonSprites[i], _021E94AC[i].animID);
        }
    }
}

void ov52_021E8CDC(Sprite **arg0, s32 arg1);

void ov52_021E8CDC(Sprite **arg0, s32 arg1) {

    if (arg1 == 1) {
        Sprite_SetAnimCtrlSeq((*arg0), 1);
        return;
    }
    Sprite_SetAnimCtrlSeq((*arg0), 0);
}

struct BgConfig;

struct GameStats;

struct MsgData;

struct SpriteResourceCollection;

struct MessageFormat;

struct YesNoPrompt;

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

typedef struct SpriteResourceCollection SpriteResourceCollection;

typedef struct GameStats GameStats;

typedef struct MsgData MsgData;

typedef struct MessageFormat MessageFormat;

typedef struct YesNoPrompt YesNoPrompt;

typedef struct SignatureTouchDataBuffer {
    u8 x[8];
    u8 y[8];
    u8 brushTypeUnused : 4;
    u8 bufferSize : 4;
} SignatureTouchDataBuffer;

typedef struct PreviousTouchDataBuffer {
    u8 lastX;
    u8 lastY;
    u16 bufferSize;
} PreviousTouchDataBuffer;

typedef struct DrawingState {
    s8 previousXDirection;
    s8 previousYDirection;
    s8 currentXDirection;
    s8 currentYDirection;
    u8 padding;
} DrawingState;

typedef struct SignatureApp {
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
    YesNoPrompt *yesNoMenu;
    DrawingState drawingState;
    u8 pad_5CA5[3];
    int lastTouchedX;
    int lastTouchedY;
} SignatureApp;

typedef u32 TextColor;

extern void ov52_021E9158(Window *window, SignatureTouchDataBuffer touchData[], PreviousTouchDataBuffer *prevTouchData, BOOL allowWindowUpdate);

void ov52_021E9218(SignatureApp *unused);

void ov52_021E925C(Window *unused0, int sLotoId, TextColor unused2, SignatureApp *unused3);

void ov52_021E9260(SignatureApp *signatureApp);

void ov52_021E8BDC(SignatureApp *signatureApp);

int ov52_021E8CF4(SignatureApp *signatureApp, int state);

int ov52_021E8CF4(SignatureApp *signatureApp, int state) {
    ov52_021E8BDC(signatureApp);
    ov52_021E925C(signatureApp->dummyWindows, 0, ((TextColor)(((14 & 0xFF) << 16) | ((12 & 0xFF) << 8) | ((15 & 0xFF) << 0))), signatureApp);
    ov52_021E9260(signatureApp);
    ov52_021E9218(signatureApp);
    ov52_021E9158(&signatureApp->signatureWindow, signatureApp->touchDataBuffers, signatureApp->prevTouchData, 1);

    return state;
}
