#include "global.h"

#include "constants/heap.h"

#include "sys_task.h"
#include "bg_window.h"
#include "camera.h"
#include "pm_string.h"
#include "overlay_74_main_menu_util.h"
#include "overlay_41_sprite_resource_collection.h"
#include "pokepic.h"
#include "sprite.h"
#include "overlay_08_battle_sub_menu_cursor.h"

struct BgConfig;
struct Camera;
struct Menu;
struct Pokepic;
struct PokepicManager;
struct PokepicDrawParam;
struct SoftwareSprite;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
struct SpriteList;
struct SpriteResource;
struct SpriteResourceCollection;
struct String;
struct Window;
void Heap_Free(void *ptr);
typedef struct Menu Menu;
typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;
typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
typedef struct SoftwareSprite SoftwareSprite;
typedef struct SoftwareSpriteManager SoftwareSpriteManager;
enum ChoiceStep {
    CHOICE_STEP_BLEND_BGS = 0,
    CHOICE_STEP_PLAY_BAG_NOISE,
    CHOICE_STEP_SHOW_3D_GRAPHICS,
    CHOICE_STEP_CHOOSE_POKEBALL,
    CHOICE_STEP_CONFIRM_POKEBALL,
    CHOICE_STEP_FINISH,
};
enum ChooseStarterStep {
    CHOOSE_STARTER_STEP_START_CAMERA_MOVEMENT = 0,
    CHOOSE_STARTER_STEP_WAIT_FOR_CAMERA_MOVEMENT_FINISH,
    CHOOSE_STARTER_STEP_UPDATE_CURSOR_POSITION,
    CHOOSE_STARTER_STEP_PRINT_THESE_ARE_POKE_BALLS_TEXT,
    CHOOSE_STARTER_STEP_DELETE_THESE_ARE_POKE_BALLS_TEXT,
    CHOOSE_STARTER_STEP_PRINT_NOW_CHOOSE_TEXT,
    CHOOSE_STARTER_STEP_DELETE_NOW_CHOOSE_TEXT,
    CHOOSE_STARTER_STEP_SHOW_CURSOR,
    CHOOSE_STARTER_STEP_CHANGE_POKEBALL,
};
enum CursorPosition {
    CURSOR_POSITION_LEFT = 0,
    CURSOR_POSITION_CENTER,
    CURSOR_POSITION_RIGHT,
};
typedef struct ChooseStarter3DGraphics {
    NNSG3dRenderObj renderObj;
    void *modelRes;
    NNSG3dResMdlSet *modelSet;
    NNSG3dResMdl *model;
    NNSG3dResTex *texture;
    void *animationRes;
    void *animation;
    NNSG3dAnmObj *animationObj;
    fx32 animationFrame;
    BOOL isVisible;
    VecFx32 position;
    VecFx32 scale;
    u16 xRotation;
    u16 yRotation;
    u16 zRotation;
} ChooseStarter3DGraphics;
typedef struct ChooseStarterMovement {
    s32 frameCount;
    s32 frameCountIncrement;
    s32 frameCountRange;
    s32 frameCountMax;
} ChooseStarterMovement;
typedef struct ChooseStarterCameraMovement {
    ChooseStarterMovement xRotation;
    ChooseStarterMovement distance;
    ChooseStarterMovement zTarget;
    Camera *camera;
    VecFx32 *target;
    s32 frameCount;
    BOOL finished;
    SysTask *movementTask;
} ChooseStarterCameraMovement;
typedef struct ChooseStarterRotation {
    fx32 verticalOffset;
    fx32 verticalOffsetMultiplier;
    int frameCountMax;
    int frameCount;
} ChooseStarterRotation;
typedef struct ChooseStarterCursor {
    Sprite *sprite;
    SpriteResource *resources[6];
    VecFx32 position;
    SysTask *movementTask;
    ChooseStarterRotation starterRotation;
} ChooseStarterCursor;
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
typedef struct StarterPreviewGraphics {
    Pokepic *sprite;
    StarterPreviewAnimation previewAnimation;
    SysTask *movementTask;
} StarterPreviewGraphics;
typedef struct ChooseStarterApp {
    enum ChoiceStep choiceStep;
    enum ChooseStarterStep chooseStarterStep;
    BOOL disableCursorMovement;
    int delayFrameCount;
    ChooseStarterCameraMovement cameraMovement;
    enum CursorPosition cursorPosition;
    int selectionMatrix[3][3];
    int otherSelectionMatrix[3][2];
    BgConfig *bgConfig;
    Window *messageWindow;
    Window *subplaneWindows[3];
    int subplaneWindowIndex;
    String *string;
    WindowTemplate confirmationMenuWindowTemplate;
    Menu *confirmationMenu;
    G2dRenderer g2dRenderer;
    SpriteList *spriteList;
    SpriteResourceCollection *spriteResourceCollection[6];
    PokepicManager *spriteManager;
    Pokepic *sprites[3];
    StarterPreviewGraphics previewGraphics;
    NNSFndAllocator allocator;
    struct { NNSG3dRenderObj renderObj; void *modelRes; struct NNSG3dResMdlSet_ *modelSet; struct NNSG3dResMdl_ *model; struct NNSG3dResTex_ *texture; void *animationRes; void *animation; struct NNSG3dAnmObj_ *animationObj; fx32 animationFrame; BOOL isVisible; VecFx32 position; VecFx32 scale; u16 xRotation; u16 yRotation; u16 zRotation; u8 unk_96_tail[2]; } starter3DGraphics[6];
    Camera *camera;
    VecFx32 cameraTarget;
    ChooseStarterCursor cursor;
    GXRgb edgeMarkings[8];
    SoftwareSpriteManager *spriteDisplay;
    StarterPreviewWindow previewWindow;
    int messageFrame;
    u32 textFrameDelay;
    u8 messagePrinterID;
    u8 unk_709[3];
} ChooseStarterApp;

void ov08_02224B7C(void *ptr);
struct BattleCursor *ov08_02224B84(BattleSubMenuCursor *cursor);

void ov08_02224B7C(void *ptr) {
    Heap_Free(ptr);
}

struct BattleCursor *ov08_02224B84(BattleSubMenuCursor *cursor) {
    return cursor->sprites;
}
