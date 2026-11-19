#include "global.h"
#include "sprite_resources_header.h"

#include "bg_window.h"
#include "options.h"
#include "pm_string.h"
#include "unk_0200A090.h"
#include "struct_defs/resdat_narc_structs.h"

struct GameStats;

struct MsgData;

struct SpriteList;

struct SpriteResourceCollection;

struct MessageFormat;

struct YesNoPrompt;

typedef struct SpriteList SpriteList;

typedef struct Sprite {
    VecFx32 position;
    VecFx32 affineTranslation;
    VecFx32 affineScale;
    u16 affineZRotation;
    u8 affineOverwriteMode;
    u8 flip;
    u8 overwriteFlags;
    u8 explicitPalette;
    u8 explicitPaletteOffset;
    BOOL explicitMosaic;
    GXOamMode explicitOamMode;
    u8 draw;
    u8 animate;
    fx32 animSpeed;
    SpriteList *list;

    u32 animData[29];
    NNSG2dImageProxy imageProxy;
    NNSG2dImagePaletteProxy paletteProxy;
    u32 type;
    u16 activeAnimID;
    u8 explicitPriority;
    u16 priority;
    enum NNS_G2D_VRAM_TYPE vramType;
    struct Sprite *prev;
    struct Sprite *next;
} Sprite;

struct SpriteList {
    Sprite *sprites;
    int capacity;
    Sprite **freeSprites;
    int freeSpriteHead;
    Sprite sentinelData;
    NNSG2dRendererInstance *renderer;
    void *rawAnimData;
    NNSG2dCellAnimBankData *defaultAnimBank;
    BOOL active;
};

typedef struct SpriteResourceCollection SpriteResourceCollection;

typedef struct G2dRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface mainScreen;
    NNSG2dRenderSurface subScreen;
} G2dRenderer;

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

void ov52_021E8B94(SignatureApp *signatureApp);

void ov52_021E8B94(SignatureApp *signatureApp) {
    for (int i = 0; i < 5; i++) {
        RemoveWindow(&signatureApp->dummyWindows[i]);
    }

    RemoveWindow(&signatureApp->descriptionWindow);
    RemoveWindow(&signatureApp->buttonWindow);
    RemoveWindow(&signatureApp->signatureWindow);
    RemoveWindow(&signatureApp->messageWindow);
}

void ov52_021E8BD8(void);

void ov52_021E8BD8(void) {
}
