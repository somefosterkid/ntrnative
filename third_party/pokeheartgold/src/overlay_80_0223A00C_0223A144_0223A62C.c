#include "global.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "filesystem.h"
#include "palette.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "sprite_system.h"
#include "unk_02013534_font_system.h"
#include "font_types_def.h"
#include "font.h"

struct FontOAM;

struct FrontierBrainEncounterEffectAnimations;

struct NARC;

struct PaletteData;

struct SpriteList;

struct SpriteResourceCollection;

struct SpriteResourceList;

struct SpriteResourceTable;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

struct SysTask;

struct SysTaskManager;

struct TextPrinterTemplate;

struct UnkStruct_02012744_t;

void InitWindow(Window *window);

void AddTextWindowTopLeftCorner(BgConfig *bgConfig, Window *window, u8 width, u8 height, u16 baseTile, u8 fillVal);

void RemoveWindow(Window *window);


typedef struct SpriteResourcesHeaderList SpriteResourcesHeaderList;

enum SpriteResourceType {
    SPRITE_RESOURCE_CHAR = 0,
    SPRITE_RESOURCE_PLTT,
    SPRITE_RESOURCE_CELL,
    SPRITE_RESOURCE_ANIM,
    SPRITE_RESOURCE_MULTI_CELL,
    SPRITE_RESOURCE_MULTI_ANIM,
    SPRITE_RESOURCE_MAX,
};

typedef struct SpriteResourceTable SpriteResourceTable;

typedef struct SpriteResourceList SpriteResourceList;


SpriteList *SpriteManager_GetSpriteList(SpriteManager *spriteMan);

typedef struct {
    const UnkStruct_02012744 *unk_00;
    const Window *unk_04;
    SpriteList *unk_08;
    const NNSG2dImagePaletteProxy *unk_0C;
    const Sprite *unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    int unk_24;
    int unk_28;
    enum HeapID heapID;
} UnkStruct_020127E8;

typedef u16 charcode_t;

typedef BOOL (*TextPrinterCallback)(TextPrinterTemplate *, u16);

typedef struct SysTaskManager SysTaskManager;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

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

typedef u32 TextColor;

u8 AddTextPrinterParameterizedWithColorAndSpacing(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, TextColor color, u32 letterSpacing, u32 lineSpacing, TextPrinterCallback callback);

FontOAM *sub_020135D8(const UnkStruct_020127E8 *param0);

int sub_02013688(const Window *param0, int param1, enum HeapID heapID);

void sub_020136B4(FontOAM *fontOAM, int x, int y);

void sub_020138E0(FontOAM *param0, u32 param1);

typedef struct FrontierBrainEncounterEffectTextWindow {
    Window window;
    u16 widthTiles;
    u16 width;
} FrontierBrainEncounterEffectTextWindow;

typedef struct FrontierBrainNameObject {
    FontOAM *fontOAM;
    UnkStruct_02021AC8 charTransferAllocation;
    u16 width;
} FrontierBrainNameObject;

typedef struct FrontierBrainBannerAnimation {
    s32 top;
    s32 bottom;
    u8 isFinished;
    u8 state;
    u8 left0;
    u8 top0;
    u8 right0;
    u8 bottom0;
    u8 left1;
    u8 top1;
    u8 right1;
    u8 bottom1;
} FrontierBrainBannerAnimation;

typedef struct FrontierBrainMugshotSlideInManager {
    int xOffset;
    int state;
} FrontierBrainMugshotSlideInManager;

typedef struct FrontierBrainEncounterEffectAnimations FrontierBrainEncounterEffectAnimations;

typedef struct FrontierBrainEncounterEffect {
    int state;
    int unused[2];
    FrontierBrainEncounterEffectAnimations *anims;
    BgConfig *bgConfig;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    PaletteData *currentBannerPalette;
    u16 *effectIsFinished;
    NARC *narc;
    s16 xAnchor;
    s16 yAnchor;
    u8 facility;
    UnkStruct_02012744 *unk_30;
    SysTask *vBlankCallback;
    int bannerPaletteTimer;
    int bannerPaletteIdx;
    u16 allPalettes[8 * (16)];
    SysTask *bannerPaletteTask;
    FrontierBrainBannerAnimation bannerAnim;
    FrontierBrainMugshotSlideInManager mugshotSlideIn;
    int mugshotPlttOffset;
    u32 plttOffsets;
} FrontierBrainEncounterEffect;

void ov80_0223A75C(const String *string, int font, int *resultWidth, int *resultWidthTiles);

void ov80_0223A62C(FrontierBrainEncounterEffect *encEffect, FrontierBrainNameObject *brainName, const String *nameString, enum Font font, TextColor textColor, int plttOffset, int plttResourceID, int x, int y, int center, FrontierBrainEncounterEffectTextWindow *textWindow);

void ov80_0223A62C(FrontierBrainEncounterEffect *encEffect, FrontierBrainNameObject *brainName, const String *nameString, enum Font font, TextColor textColor, int plttOffset, int plttResourceID, int x, int y, int center, FrontierBrainEncounterEffectTextWindow *textWindow) {
    UnkStruct_020127E8 v0;

    ((brainName->fontOAM == 0L) ? (void)0 : GF_AssertFail());

    BgConfig *bgConfig = encEffect->bgConfig;
    SpriteManager *spriteMan = encEffect->spriteMan;

    int width, widthTiles;
    if (textWindow == 0L) {
        ov80_0223A75C(nameString, font, &width, &widthTiles);
    } else {
        width = textWindow->width;
        widthTiles = textWindow->widthTiles;
    }

    Window window;
    if (textWindow == 0L) {
        InitWindow(&window);
        AddTextWindowTopLeftCorner(bgConfig, &window, widthTiles, 16 / 8, 0, 0);
        AddTextPrinterParameterizedWithColorAndSpacing(&window, font, nameString, 0, 0, 0xFF, textColor, 0, 0, 0L);
    } else {
        window = textWindow->window;
    }

    UnkStruct_02021AC8 charTransferAllocation;
    int size = sub_02013688(&window, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_101);
    sub_02021AC8(size, 1, NNS_G2D_VRAM_TYPE_2DMAIN, &charTransferAllocation);

    if (center == 1) {
        x -= width / 2;
    }

    y -= 8;

    v0.unk_00 = encEffect->unk_30;
    v0.unk_04 = &window;
    v0.unk_08 = SpriteManager_GetSpriteList(spriteMan);
    v0.unk_0C = SpriteManager_FindPlttResourceProxy(spriteMan, plttResourceID);
    v0.unk_10 = 0L;
    v0.unk_14 = charTransferAllocation.offset;
    v0.unk_18 = x;
    v0.unk_1C = y;
    v0.unk_20 = 0;
    v0.unk_24 = 11;
    v0.unk_28 = NNS_G2D_VRAM_TYPE_2DMAIN;
    v0.heapID = HEAP_ID_101;

    FontOAM *fontOAM = sub_020135D8(&v0);

    sub_020138E0(fontOAM, plttOffset);
    sub_020136B4(fontOAM, x, y);

    if (textWindow == 0L) {
        RemoveWindow(&window);
    }

    brainName->fontOAM = fontOAM;
    brainName->charTransferAllocation = charTransferAllocation;
    brainName->width = width;
}
