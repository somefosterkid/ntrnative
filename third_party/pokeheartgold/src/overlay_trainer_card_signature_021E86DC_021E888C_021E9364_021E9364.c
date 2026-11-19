#include "global.h"

#include "bg_window.h"
#include "sprite.h"
#include "sys_task.h"
#include "system.h"
#include "unk_02009D48.h"
#include "struct_defs/resdat_narc_structs.h"

typedef struct System System;

struct DrawingState;

struct G2dRenderer;

struct GameStats;

struct MsgData;

struct Options;

struct PreviousTouchDataBuffer;

struct SignatureApp;

struct SignatureTouchDataBuffer;

struct Sprite;

struct SpriteList;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

struct String;

struct MessageFormat;

struct SysTask;

struct SysTaskManager;

struct System;

struct Window;

struct YesNoPrompt;

typedef struct Options Options;

typedef struct String String;

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

void ov52_021E9424(DrawingState *drawingState);

void ov52_021E9364(SignatureApp *signatureApp);

void ov52_021E9364(SignatureApp *signatureApp) {
    int distance;

    if (gSystem.touchX != (u16)-1 && gSystem.touchY != (u16)-1 && signatureApp->lastTouchedX != (u16)-1 && signatureApp->lastTouchedY != (u16)-1) {
        if (signatureApp->lastTouchedX > gSystem.touchX) {
            distance = signatureApp->lastTouchedX - gSystem.touchX;
            signatureApp->drawingState.currentXDirection = -1;
        } else {
            distance = gSystem.touchX - signatureApp->lastTouchedX;
            signatureApp->drawingState.currentXDirection = 1;
        }

        if (3 <= distance && distance <= 40) {
            if (signatureApp->lastTouchedY > gSystem.touchY) {
                distance = signatureApp->lastTouchedY - gSystem.touchY;
                signatureApp->drawingState.currentYDirection = -1;
            } else {
                distance = gSystem.touchY - signatureApp->lastTouchedY;
                signatureApp->drawingState.currentYDirection = 1;
            }

            if (distance <= 40) {
                ov52_021E9424(&signatureApp->drawingState);
            }
        } else if (distance <= 40) {
            if (signatureApp->lastTouchedY > gSystem.touchY) {
                distance = signatureApp->lastTouchedY - gSystem.touchY;
                signatureApp->drawingState.currentYDirection = -1;
            } else {
                distance = gSystem.touchY - signatureApp->lastTouchedY;
                signatureApp->drawingState.currentYDirection = 1;
            }

            if (3 <= distance && distance <= 40) {
                ov52_021E9424(&signatureApp->drawingState);
            }
        }
    }

    signatureApp->lastTouchedX = gSystem.touchX;
    signatureApp->lastTouchedY = gSystem.touchY;
}
