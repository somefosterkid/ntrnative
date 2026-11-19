#include "global.h"
#include "sys_task.h"

struct ChooseStarterMovement;
struct SoftwareSprite;
struct SoftwareSpriteChars;
struct SoftwareSpritePalette;
struct StarterPreviewAnimation;
struct StarterPreviewWindow;
struct SysTask;
struct SysTaskQueue;
enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
typedef struct SoftwareSprite SoftwareSprite;
void sub_02093594(SoftwareSprite *sprite, BOOL visible);
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

void ov97_0221E88C(StarterPreviewWindow *previewWindow, BOOL show);

void ov97_0221E88C(StarterPreviewWindow *previewWindow, BOOL show)
{
    sub_02093594(previewWindow->sprite, show);
}
