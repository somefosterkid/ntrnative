#include "global.h"
#include "overlay_90_unk_struct_ov70_0225_ca20_t.h"
#include "bg_window.h"
#include "camera.h"
#include "pm_string.h"
#include "overlay_74_main_menu_util.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_45_unk_struct_ov66_02234798.h"
#include "pokepic.h"
#include "sprite.h"

s32 ov49_02258C1C(void *, s32 *);

void ov49_0225927C(void *arg0, s16 arg1, s8 arg2, s8 arg3);

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
struct SysTask;
struct SysTaskQueue;
struct UnkStruct_ov63_0222BE18_t;
struct UnkStruct_ov63_0222BEC0_t;
struct UnkStruct_ov63_0222D848_t;
struct UnkStruct_ov66_02234548_t;
struct UnkStruct_ov66_02234798_t;
struct UnkStruct_ov70_0225C894_t;
struct UnkStruct_ov70_0225CA20_t;
struct UnkStruct_ov70_0225CC54_t;
struct Window;
u32 MTRandom(void);
typedef struct {
    s16 unk_00;
    s16 unk_02;
} UnkStruct_ov63_0222CC3C;
typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;
typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;
typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;
typedef struct UnkStruct_ov63_0222BE18_t UnkStruct_ov63_0222BE18;
UnkStruct_ov63_0222CC3C ov42_02228270(UnkStruct_ov63_0222CC3C param0, int param1);
UnkStruct_ov63_0222CC3C ov42_022282DC(const UnkStruct_ov63_0222BEC0 *param0);
typedef struct UnkStruct_ov63_0222D848_t UnkStruct_ov63_0222D848;
typedef struct UnkStruct_ov66_02234548_t UnkStruct_ov66_02234548;
u32 ov49_022589B8(const UnkStruct_ov70_0225C894 *param0, u16 param1, u16 param2);
typedef struct {
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    u32 unk_0C;
} UnkStruct_ov70_0225D220;
typedef struct {
    u8 unk_00;
    u8 unk_01;
    s8 unk_02;
    s8 unk_03;
    s16 unk_04;
    s16 unk_06;
} UnkStruct_ov70_0225D93C;
typedef union {
    struct {
        s16 unk_00;
        s16 unk_02;
    } val1;
    struct {
        UnkStruct_ov70_0225D220 unk_00;
        u32 unk_10;
    } val2;
    struct {
        UnkStruct_ov70_0225D220 unk_00;
        u32 unk_10;
        fx32 unk_14;
    } val3;
    struct {
        s32 unk_00;
        VecFx32 unk_04;
    } val4;
} UnkStruct_ov70_0225CC54_sub1;
struct UnkStruct_ov70_0225CC54_t {
    UnkStruct_ov63_0222BEC0 *unk_00;
    UnkStruct_ov66_02234798 *unk_04;
    u16 unk_08;
    u8 unk_0A;
    u8 unk_0B;
    UnkStruct_ov70_0225CC54_sub1 unk_0C;
    void (*unk_24)(struct UnkStruct_ov70_0225CC54_t *, UnkStruct_ov70_0225CA20 *);
};
typedef struct ChooseStarterMovement {
    s32 frameCount;
    s32 frameCountIncrement;
    s32 frameCountRange;
    s32 frameCountMax;
} ChooseStarterMovement;
typedef struct Menu Menu;
typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;
typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);
enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};
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
BOOL ov49_022593FC(const UnkStruct_ov70_0225CC54 *param0);
u8 ov49_02259294(const u8 *param0, u32 param1);
BOOL ov49_022592A8(const UnkStruct_ov70_0225CA20 *param0, const UnkStruct_ov70_0225CC54 *param1, int param2, u32 param3);
void ov49_02259320(ChooseStarterMovement *starterMovement, s32 start, s32 end, s32 frameCountMax);
BOOL ov49_0225932C(UnkStruct_ov70_0225D220 *param0, u32 param1);
enum ChoiceStep ov49_022593BC(ChooseStarterApp *app);
UnkStruct_ov70_0225CC54 *ov49_022593C0(UnkStruct_ov70_0225CA20 *param0);

void ov49_0225927C(void *arg0, s16 arg1, s8 arg2, s8 arg3) {
    s32 v0;

    *(s16 *)(&v0) = arg1;
    *(s8 *)((u8 *)(&v0) + 2) = arg2;
    *(s8 *)((u8 *)(&v0) + 3) = arg3;
    ov49_02258C1C(arg0, &v0);
}

u8 ov49_02259294(const u8 *param0, u32 param1) {
    int v0 = MTRandom() % param1;
    return param0[v0];
}

BOOL ov49_022592A8(const UnkStruct_ov70_0225CA20 *param0, const UnkStruct_ov70_0225CC54 *param1, int param2, u32 param3) {
    UnkStruct_ov63_0222CC3C v0;
    u32 v1;

    v0 = ov42_022282DC(param1->unk_00);
    v0 = ov42_02228270(v0, param2);
    v1 = ov49_022589B8(param0->unk_14, (v0.unk_00) / 16, (v0.unk_02) / 16);

    if (v1 == param3) {
        return 1;
    }

    return 0;
}

void ov49_02259320(ChooseStarterMovement *starterMovement, s32 start, s32 end, s32 frameCountMax) {
    starterMovement->frameCount = start;
    starterMovement->frameCountIncrement = start;
    starterMovement->frameCountRange = end - start;
    starterMovement->frameCountMax = frameCountMax;
}

BOOL ov49_0225932C(UnkStruct_ov70_0225D220 *param0, u32 param1) {
    fx32 v0;

    v0 = FX_MulInline(param0->unk_08, ((fx32)(((param1) > 0) ? ((param1) * (1 << 12) + 0.5f) : ((param1) * (1 << 12) - 0.5f))));
    v0 = FX_Div(v0, ((fx32)(((param0->unk_0C) > 0) ? ((param0->unk_0C) * (1 << 12) + 0.5f) : ((param0->unk_0C) * (1 << 12) - 0.5f))));

    param0->unk_00 = v0 + param0->unk_04;

    if (param1 >= param0->unk_0C) {
        return 1;
    }

    return 0;
}

enum ChoiceStep ov49_022593BC(ChooseStarterApp *app) {
    return app->choiceStep;
}

UnkStruct_ov70_0225CC54 *ov49_022593C0(UnkStruct_ov70_0225CA20 *param0) {
    int v0;

    for (v0 = 0; v0 < param0->unk_0C; v0++) {
        if (ov49_022593FC(&param0->unk_08[v0])) {
            return &param0->unk_08[v0];
        }
    }

    ((0) ? (void)0 : GF_AssertFail());
    return &param0->unk_08[v0];
}
