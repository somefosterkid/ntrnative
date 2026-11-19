#include "global.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"

struct Background;

struct BgConfig;

struct CharTransferTemplateWithModes;

struct ListMenuCursor;

struct MailMessage;

struct G2dRenderer;

struct MailViewerApp;

struct MailViewerAppArgs;

struct ManagedSprite;

struct ListMenu2D;

struct ListMenu2DTemplate;

struct MessageBank;

struct MessageBankEntry;

struct MsgData;

struct NARC;

struct Options;

struct PaletteBuffer;

struct PaletteData;

struct PaletteFadeControl;

struct RenderOamTemplate;

struct GF_2DGfxRawResObj;

struct GF_2DGfxRawResMan;

struct Sprite;

struct SpriteList;

struct SpriteManager;

struct SpriteResource;

struct SpriteResourceCollection;

struct GF_2DGfxResObjList;

struct GF_2DGfxResHeader;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

struct ManagedSpriteTemplate;

struct String;

struct ListMenuItem;

struct Window;

typedef struct String {
    u16 maxsize;
    u16 size;
    u32 magic;
    u16 data[0];
} String;

typedef struct MailMessage {
    u16 type;
    u16 id;
    u16 words[2];
} MailMessage;

typedef struct Options {
    u16 textSpeed : 4;
    u16 soundMode : 2;
    u16 battleStyle : 1;
    u16 battleScene : 1;
    u16 buttonMode : 2;
    u16 frame : 5;
    u16 : 1;
} Options;

typedef union MailIconData {
    struct {
        u16 spriteIndex : 12;
        u16 palIndex : 4;
    } asStruct;
    u16 asValue;
} MailIconData;

typedef struct NARC {
    FSFile file;
    u32 fatbStart;
    u32 fimgStart;
    u16 numFiles;
} NARC;

typedef struct PaletteFadeControl {
    u16 unfadedMask;
    u16 wait : 6;
    u16 cur : 5;
    u16 end : 5;
    u16 target : 15;
    u16 sign : 1;
    u16 step : 4;
    u16 waitStep : 6;
    u16 unused : 6;
} PaletteFadeControl;

typedef struct PaletteBuffer {
    u16 *unfaded;
    u16 *faded;
    u32 size;
    PaletteFadeControl selected;
} PaletteBuffer;

typedef struct PaletteData {
    PaletteBuffer buffers[14];
    u16 selectedFlag : 2;
    u16 selectedBuffer : 14;
    u16 transparentBit : 14;
    u16 callbackFlag : 1;
    u16 autoTransparent : 1;
    u8 forceExit;
    u8 unused[3];
} PaletteData;

typedef struct CharTransferTemplateWithModes {
    int maxTasks;
    int sizeMain;
    int sizeSub;
    GXOBJVRamModeChar modeMain;
    GXOBJVRamModeChar modeSub;
} CharTransferTemplateWithModes;

typedef struct RenderOamTemplate {
    int mainOamStart;
    int mainOamCount;
    int mainAffineOamStart;
    int mainAffineOamCount;
    int subOamStart;
    int subOamCount;
    int subAffineOamStart;
    int subAffineOamCount;
} RenderOamTemplate;

void thunk_ClearMainOAM(enum HeapID heapID);

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

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

typedef struct ManagedSprite {
    Sprite *sprite;
    SpriteResourcesHeader *resourceHeader;
    SpriteResourcesHeaderList *resourceHeaderList;
    BOOL vramTransfer;
} ManagedSprite;

typedef struct GF_2DGfxRawResObj {
    int id;
    void *data;
} GF_2DGfxRawResObj;

typedef struct GF_2DGfxRawResMan {
    GF_2DGfxRawResObj *resources;
    int capacity;
    int count;
} GF_2DGfxRawResMan;

typedef struct SpriteResource {
    GF_2DGfxRawResObj *rawResource;
    GfGfxResType type;
    void *data;
} SpriteResource;

typedef struct SpriteResourceCollection {
    GF_2DGfxRawResMan *collection;
    SpriteResource *resources;
    void *capacity;
    void *count;
    GfGfxResType type;
} SpriteResourceCollection;

typedef struct GF_2DGfxResHeader {
    void *entries;
    int count;
    GfGfxResType type;
    u8 loadFromNARC;
} GF_2DGfxResHeader;

typedef struct GF_2DGfxResObjList {
    SpriteResource **resources;
    int capacity;
    int count;
} GF_2DGfxResObjList;

typedef struct G2dRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface mainScreen;
    NNSG2dRenderSurface subScreen;
} G2dRenderer;

typedef struct SpriteSystem {
    enum HeapID heapID;
    int spriteManagerCount;
    BOOL inUse;
    NNSG2dCellTransferState *cellTransferStates;
    G2dRenderer renderer;
} SpriteSystem;

typedef struct SpriteManager {
    SpriteList *sprites;
    SpriteResourcesHeaderList *resourceHeaders;
    GF_2DGfxResHeader *resourcePaths;
    SpriteResourceCollection *ownedResources[GF_GFX_RES_TYPE_MAX];
    GF_2DGfxResObjList *unownedResources[GF_GFX_RES_TYPE_MAX];
    int loadedResourceCount[GF_GFX_RES_TYPE_MAX];
    int numResourceTypes;
} SpriteManager;

typedef struct ManagedSpriteTemplate {
    s16 x;
    s16 y;
    s16 z;
    u16 animIdx;
    int priority;
    int plttIdx;
    enum NNS_G2D_VRAM_TYPE vramType;
    int resources[GF_GFX_RES_TYPE_MAX];
    int bgPriority;
    BOOL vramTransfer;
} ManagedSpriteTemplate;

typedef union SpriteResourceCapacities {
    int asArray[GF_GFX_RES_TYPE_MAX];
    struct {
        int charCapacity;
        int plttCapacity;
        int cellCapacity;
        int animCapacity;
        int mcellCapacity;
        int manimCapacity;
    } asStruct;
} SpriteResourceCapacities;

SpriteSystem *SpriteSystem_Alloc(enum HeapID heapID);

SpriteManager *SpriteManager_New(SpriteSystem *spriteSys);

BOOL SpriteSystem_Init(SpriteSystem *spriteSys, const RenderOamTemplate *oamTemplate, const CharTransferTemplateWithModes *transferTemplate, int plttCapacity);

BOOL SpriteSystem_InitSprites(SpriteSystem *spriteSys, SpriteManager *spriteMan, int maxSprites);

BOOL SpriteSystem_InitManagerWithCapacities(SpriteSystem *spriteSys, SpriteManager *spriteMan, const SpriteResourceCapacities *capacities);

s8 SpriteSystem_LoadPlttResObj(SpriteSystem *spriteSys, SpriteManager *spriteMan, enum NarcId narcID, int memberIdx, BOOL compressed, int paletteIdx, enum NNS_G2D_VRAM_TYPE vramType, int resourceID);

BOOL SpriteSystem_LoadCellResObj(SpriteSystem *spriteSys, SpriteManager *spriteMan, enum NarcId narcID, int memberIdx, BOOL compressed, int resourceID);

BOOL SpriteSystem_LoadAnimResObj(SpriteSystem *spriteSys, SpriteManager *spriteMan, enum NarcId narcID, int memberIdx, BOOL compressed, int resourceID);

ManagedSprite *SpriteSystem_NewSprite(SpriteSystem *spriteSys, SpriteManager *spriteMan, const ManagedSpriteTemplate *template);

void ManagedSprite_SetDrawFlag(ManagedSprite *managedSprite, BOOL draw);

BOOL SpriteSystem_LoadCharResObjWithHardwareMappingType(SpriteSystem *spriteSys, SpriteManager *spriteMan, enum NarcId narcID, int memberIdx, BOOL compressed, enum NNS_G2D_VRAM_TYPE vramType, int resourceID);

typedef struct MailViewerAppArgs {
    union {
        u16 writeMode;
        u16 result;
    };
    u8 sentenceIndex;
    u8 horizontalSelectionIndex;
    Options *options;
    u32 trainerID;
    u8 padding;
    u8 language;
    u8 gameVersion;
    u8 mailType;
    String *trainerName;
    u8 pad_14[4];
    MailIconData iconData[3];
    MailMessage sentences[3];
} MailViewerAppArgs;

typedef struct Background {
    void *tilemapBuffer;
    u32 bufferSize;
    u32 baseTile;
    int hOffset;
    int vOffset;
    u8 mode;
    u8 size;
    u8 colorMode;
    u8 tileSize;
    u16 rotation;
    u16 unk22;
    fx32 xScale;
    fx32 yScale;
    int centerX;
    int centerY;
} Background;

typedef struct BgConfig {
    enum HeapID heapID;
    u16 scrollScheduled;
    u16 bufferTransferScheduled;
    struct Background bgs[8];
} BgConfig;

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

typedef u32 TextColor;

typedef struct ListMenuCursor {
    TextColor color;
    String *string;
} ListMenuCursor;

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;

typedef struct ListMenuItem {
    void *entry;
    union {
        u32 index;
        void *callbackFunction;
    };
} ListMenuItem;

typedef struct ListMenu2DTemplate {
    ListMenuItem *choices;
    Window *window;
    u8 fontID;
    u8 xSize;
    u8 ySize;
    u8 lineSpacing : 4;
    u8 suppressCursor : 2;
    u8 loopAround : 2;
} ListMenu2DTemplate;

typedef struct ListMenu2D {
    ListMenu2DTemplate template;
    ListMenuCursor *cursor;
    u32 cancelKeys;
    u8 dummy14;
    u8 cursorPos;
    u8 width;
    u8 xOffset;
    u8 yOffset;
    u8 letterWidth;
    u8 lineHeight;
    u8 lastAction;
    u8 heapID;
} ListMenu2D;

void sub_0203A964(void);

u32 sub_02074490(void);

u32 sub_02074494(void);

u32 sub_020744A0(void);

void GF_CreateVramTransferManager(u32 capacity, enum HeapID heapID);

enum MailViewerMode {
    MAIL_VIEWER_MODE_READ = 0,
    MAIL_VIEWER_MODE_WRITE,
    MAIL_VIEWER_MODE_CONFIRM_EMPTY,
    MAIL_VIEWER_MODE_CANCEL,
};

typedef struct MailViewerApp {
    enum HeapID heapID;
    int state;
    u16 subState;
    u8 unkA;
    u8 frame;
    u8 mode;
    u8 initialMode;
    u8 padding;
    u8 printerID;
    u8 textFrameDelay;
    u8 verticalSelectionIndex;
    u8 horizontalSelectionIndex;
    u8 pauseGlowEffect;
    u8 blendFraction;
    u8 blendDirection;
    u8 blendIndex;
    u8 prevBlendIndex;
    BgConfig *bgConfig;
    MailViewerAppArgs *args;
    struct MsgData *loader;
    void *dummy[1];
    u8 padding2[8];
    PaletteData *paletteData;
    void *unk34;
    void *unk38;
    NNSG2dScreenData *backgroundNSCRBuffer;
    NNSG2dScreenData *interfaceNSCRBuffer;
    struct NNSG2dScreenData *backgroundScreenData;
    struct NNSG2dScreenData *interfaceScreenData;
    Window windows[6];
    struct ListMenu2D *yesNoMenu;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    ManagedSprite *managedSprites[3];
} MailViewerApp;

void ov56_021E6BB4(MailViewerApp *mailViewerApp);

extern const RenderOamTemplate ov56_021E6E38;

extern const CharTransferTemplateWithModes ov56_021E6DF4;

extern const SpriteResourceCapacities ov56_021E6E08;

void ov56_021E6BB4(MailViewerApp *mailViewerApp) {
    ManagedSpriteTemplate spriteTemplate;

    sub_0203A964();

    if (mailViewerApp->mode == MAIL_VIEWER_MODE_WRITE) {
        return;
    }

    GF_CreateVramTransferManager(32, mailViewerApp->heapID);

    mailViewerApp->spriteSys = SpriteSystem_Alloc(mailViewerApp->heapID);
    mailViewerApp->spriteMan = SpriteManager_New(mailViewerApp->spriteSys);

    RenderOamTemplate oamTemplate = ov56_021E6E38;

    CharTransferTemplateWithModes transferTemplate = ov56_021E6DF4;

    const SpriteResourceCapacities capacities = ov56_021E6E08;

    SpriteSystem_Init(mailViewerApp->spriteSys, &oamTemplate, &transferTemplate, 32);
    SpriteSystem_InitSprites(mailViewerApp->spriteSys, mailViewerApp->spriteMan, 3);
    SpriteSystem_InitManagerWithCapacities(mailViewerApp->spriteSys, mailViewerApp->spriteMan, &capacities);
    thunk_ClearMainOAM(mailViewerApp->heapID);

    SpriteSystem_LoadPlttResObj(mailViewerApp->spriteSys, mailViewerApp->spriteMan, (enum NarcId)20, sub_02074490(), 0, 3, NNS_G2D_VRAM_TYPE_2DMAIN, 0);
    SpriteSystem_LoadCellResObj(mailViewerApp->spriteSys, mailViewerApp->spriteMan, (enum NarcId)20, sub_02074494(), 0, 0);
    SpriteSystem_LoadAnimResObj(mailViewerApp->spriteSys, mailViewerApp->spriteMan, (enum NarcId)20, sub_020744A0(), 0, 0);

    for (int i = 0; i < 3; i++) {
        if (mailViewerApp->args->iconData[i].asValue == 0xFFFF) {
            break;
        }

        SpriteSystem_LoadCharResObjWithHardwareMappingType(mailViewerApp->spriteSys, mailViewerApp->spriteMan, (enum NarcId)20, mailViewerApp->args->iconData[i].asStruct.spriteIndex, 0, NNS_G2D_VRAM_TYPE_2DMAIN, i);
        MI_CpuClear8(&spriteTemplate, sizeof(ManagedSpriteTemplate));

        spriteTemplate.x = 128 - 40 * i;
        spriteTemplate.y = 160;
        spriteTemplate.z = 0;

        spriteTemplate.animIdx = 0;
        spriteTemplate.bgPriority = 2;
        spriteTemplate.plttIdx = mailViewerApp->args->iconData[i].asStruct.palIndex;
        spriteTemplate.vramTransfer = 0;
        spriteTemplate.vramType = NNS_G2D_VRAM_TYPE_2DMAIN;

        spriteTemplate.resources[GF_GFX_RES_TYPE_CHAR] = i;
        spriteTemplate.resources[GF_GFX_RES_TYPE_PLTT] = 0;
        spriteTemplate.resources[GF_GFX_RES_TYPE_CELL] = 0;
        spriteTemplate.resources[GF_GFX_RES_TYPE_ANIM] = 0;
        spriteTemplate.resources[GF_GFX_RES_TYPE_MCEL] = -1;
        spriteTemplate.resources[GF_GFX_RES_TYPE_MANM] = -1;

        mailViewerApp->managedSprites[i] = SpriteSystem_NewSprite(mailViewerApp->spriteSys, mailViewerApp->spriteMan, &spriteTemplate);

        if (mailViewerApp->args->iconData[i].asStruct.spriteIndex == 7) {
            ManagedSprite_SetDrawFlag(mailViewerApp->managedSprites[i], 0);
        }
    }
}
