#include "global.h"

#include "sys_task.h"

struct SoftwareSprite;

struct SoftwareSpriteChars;

struct SoftwareSpritePalette;

typedef struct SoftwareSpriteChars SoftwareSpriteChars;

typedef struct SoftwareSpritePalette SoftwareSpritePalette;

typedef struct SoftwareSprite SoftwareSprite;

void ManagedSprite_SetDrawFlag(SoftwareSprite *sprite, BOOL visible);

typedef struct ChooseStarterMovement {
    s32 frameCount;
    s32 frameCountIncrement;
    s32 frameCountRange;
    s32 frameCountMax;
} ChooseStarterMovement;

typedef struct StarterPreviewAnimation {
    ChooseStarterMovement x;
    ChooseStarterMovement y;
    ChooseStarterMovement scale;
    int frameCount;
    int frameCountIncrement;
} StarterPreviewAnimation;

typedef struct StarterPreviewWindow {
    SoftwareSpriteChars *spriteChars;
    SoftwareSpritePalette *spritePalette;
    SoftwareSprite *sprite;
    void *charDataPtr;
    void *paletteDataPtr;
    NNSG2dCharacterData *charData;
    NNSG2dPaletteData *paletteData;
    StarterPreviewAnimation previewAnimation;
    SysTask *movementTask;
} StarterPreviewWindow;

void ov40_0222E618(StarterPreviewWindow *previewWindow, BOOL show);

void ov40_0222E618(StarterPreviewWindow *previewWindow, BOOL show) {
    ManagedSprite_SetDrawFlag(previewWindow->sprite, show);
}
