#ifndef POKEHEARTGOLD_OVERLAY_01_CHOOSE_STARTER_APP_H
#define POKEHEARTGOLD_OVERLAY_01_CHOOSE_STARTER_APP_H

#include "g2d_renderer.h"

// ChooseStarterApp: merged by tools/hg/promote.py from 1 m2c local copies (1 variants, base
// src/overlay_01_021F1348_021F1478.c) and 4 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct Camera;
struct Menu;
struct NNSFndAllocatorFunc;
struct NNSG2dCharacterData;
struct NNSG2dImagePaletteProxy;
struct NNSG2dImageProxy;
struct NNSG2dPaletteData;
struct NNSG2dPaletteSwapTable;
struct NNSG2dRenderSurface;
struct NNSG2dRndCore2DMtxCache;
struct NNSG2dRndCoreSurface;
struct NNSG3dAnmObj_;
struct NNSG3dJntAnmResult_;
struct NNSG3dMatAnmResult_;
struct NNSG3dResMdlSet_;
struct NNSG3dResMdl_;
struct NNSG3dResTex_;
struct PokemonSprite;
struct PokemonSpriteManager;
struct SoftwareSprite;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
struct Sprite;
struct SpriteList;
struct SpriteResource;
struct SpriteResourceCollection;
struct String;
struct SysTask;
struct Window;

typedef struct ChooseStarterApp {
    u32 choiceStep;
    u32 chooseStarterStep;
    BOOL disableCursorMovement;
    int delayFrameCount;
    struct { struct { s32 frameCount; s32 frameCountIncrement; s32 frameCountRange; s32 frameCountMax; } xRotation; struct { s32 frameCount; s32 frameCountIncrement; s32 frameCountRange; s32 frameCountMax; } distance; struct { s32 frameCount; s32 frameCountIncrement; s32 frameCountRange; s32 frameCountMax; } zTarget; struct Camera *camera; void *target; s32 frameCount; BOOL finished; struct SysTask *movementTask; } cameraMovement;
    u32 cursorPosition;
    int selectionMatrix[3][3];
    int otherSelectionMatrix[2][3];
    struct BgConfig *bgConfig;
    struct Window *messageWindow;
    struct Window *subplaneWindows[3];
    int subplaneWindowIndex;
    struct String *string;
    struct { u8 bgId; u8 left; u8 top; u8 width; u8 height; u8 palette; u16 baseTile; } confirmationMenuWindowTemplate;
    struct Menu *confirmationMenu;
    G2dRenderer g2dRenderer;
    struct SpriteList *spriteList;
    struct SpriteResourceCollection *spriteResourceCollection[6];
    struct PokemonSpriteManager *spriteManager;
    struct PokemonSprite *sprites[3];
    struct { struct PokemonSprite *sprite; struct { struct { s32 frameCount; s32 frameCountIncrement; s32 frameCountRange; s32 frameCountMax; } x; struct { s32 frameCount; s32 frameCountIncrement; s32 frameCountRange; s32 frameCountMax; } y; struct { s32 frameCount; s32 frameCountIncrement; s32 frameCountRange; s32 frameCountMax; } scale; int frameCount; int frameCountIncrement; } previewAnimation; struct SysTask *movementTask; } previewGraphics;
    struct { struct NNSFndAllocatorFunc *pFunc; void *pHeap; u32 heapParam1; u32 heapParam2; } allocator;
    struct { NNSG3dRenderObj renderObj; void *modelRes; struct NNSG3dResMdlSet_ *modelSet; struct NNSG3dResMdl_ *model; struct NNSG3dResTex_ *texture; void *animationRes; void *animation; struct NNSG3dAnmObj_ *animationObj; fx32 animationFrame; BOOL isVisible; struct { fx32 x; fx32 y; fx32 z; } position; struct { fx32 x; fx32 y; fx32 z; } scale; u16 xRotation; u16 yRotation; u16 zRotation; u8 unk_96_tail[0x2]; } starter3DGraphics[6];
    struct Camera *camera;
    struct { fx32 x; fx32 y; fx32 z; } cameraTarget;
    struct { struct Sprite *sprite; struct SpriteResource *resources[6]; struct { fx32 x; fx32 y; fx32 z; } position; struct SysTask *movementTask; struct { fx32 verticalOffset; fx32 verticalOffsetMultiplier; int frameCountMax; int frameCount; } starterRotation; } cursor;
    u16 edgeMarkings[8];
    struct SoftwareSpriteManager *spriteDisplay;
    struct { struct SoftwareSpriteChars *spriteChars; struct SoftwareSpritePalette *spritePalette; struct SoftwareSprite *sprite; void *charDataPtr; void *paletteDataPtr; struct NNSG2dCharacterData *charData; struct NNSG2dPaletteData *paletteData; struct { struct { s32 frameCount; s32 frameCountIncrement; s32 frameCountRange; s32 frameCountMax; } x; struct { s32 frameCount; s32 frameCountIncrement; s32 frameCountRange; s32 frameCountMax; } y; struct { s32 frameCount; s32 frameCountIncrement; s32 frameCountRange; s32 frameCountMax; } scale; int frameCount; int frameCountIncrement; } previewAnimation; struct SysTask *movementTask; } previewWindow;
    int messageFrame;
    u32 textFrameDelay;
    u8 messagePrinterID;
    u8 unk_709[3];
} ChooseStarterApp;

// sizeof(ChooseStarterApp) == 0x6b4

#endif // POKEHEARTGOLD_OVERLAY_01_CHOOSE_STARTER_APP_H
