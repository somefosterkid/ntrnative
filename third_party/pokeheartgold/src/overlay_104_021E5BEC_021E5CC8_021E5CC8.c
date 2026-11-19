#include "global.h"
#include "sys_task.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "bg_window.h"
#include "camera.h"
#include "field/overlay_01_021FB878.h"
#include "msgdata.h"

struct Bg3DModelAnim;

struct BgConfig;

struct Camera;

struct ClearGamePlayerInfo;

struct DrifloonState;

struct Field3dModel;

struct Field3dObject;

struct EndCredits3DModelManager;

struct EndCreditsApp;

struct EndCreditsDaySceneData;

struct EndCreditsFinSceneData;

struct EndCreditsMemoriesSceneData;

struct EndCreditsMorningSceneData;

struct EndCreditsNightSceneData;

struct EndCreditsPlayerAnimManager;

struct EndCreditsSceneManager;

struct EndCreditsStringsManager;

struct EndCreditsTwinleafSceneData;

struct G3DPipelineBuffers;

struct MagnezoneState;

struct ManagedSprite;

struct MsgData;

struct NARC;

struct PaletteData;

struct SpriteManager;

struct SpriteSystem;

struct SysTask;

struct SysTaskQueue;

struct Window;

struct WingullState;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

typedef struct ClearGamePlayerInfo ClearGamePlayerInfo;

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

void sub_02018030(Field3dModel *model, NARC *narc, u32 memberIndex, u32 heapID);

void sub_020181B0(Field3dObject *obj, Field3dModel *model);

void sub_020182A0(Field3dObject *obj, BOOL visible);

void Field3dObject_SetPosEx(Field3dObject *obj, fx32 x, fx32 y, fx32 z);

void sub_020182C4(Field3dObject *obj, fx32 x, fx32 y, fx32 z);

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

enum EndCreditsManagedSpriteType {
    END_CREDITS_SPRITE_MORNING_PLAYER = 0,
    END_CREDITS_SPRITE_MORNING_SCARF,
    END_CREDITS_SPRITE_MORNING_DRIFLOON_1,
    END_CREDITS_SPRITE_MORNING_DRIFLOON_2,
    END_CREDITS_SPRITE_MORNING_DRIFLOON_3,

    END_CREDITS_SPRITE_DAY_PLAYER = 0,
    END_CREDITS_SPRITE_DAY_SCARF,
    END_CREDITS_SPRITE_DAY_WINGULL_1,
    END_CREDITS_SPRITE_DAY_WINGULL_2,
    END_CREDITS_SPRITE_DAY_WINGULL_3,

    END_CREDITS_SPRITE_NIGHT_PLAYER = 0,
    END_CREDITS_SPRITE_NIGHT_SCARF,
    END_CREDITS_SPRITE_NIGHT_MAGNEZONE,
    END_CREDITS_SPRITE_NIGHT_METEOR_1,
    END_CREDITS_SPRITE_NIGHT_METEOR_2,

    END_CREDITS_SPRITE_COUNT
};

typedef struct Bg3DModelAnim {
    s16 modelCount;
    s16 speed;
    fx32 startXPos;
    fx32 xOffset;
    fx32 startYPos;
    fx32 yOffset;
    fx32 startZPos;
    fx32 zOffset;
} Bg3DModelAnim;

typedef struct EndCredits3DModelManager {
    Bg3DModelAnim bg3DModels[2];
} EndCredits3DModelManager;

typedef struct DrifloonState {
    fx32 xPos;
    fx32 yPos;
    fx32 degrees;
    s32 frame;
    s16 scaleTimer;
    s8 scaleDirection;
} DrifloonState;

typedef struct EndCreditsMorningSceneData {
    fx32 bgXPos;
    fx32 unused_04;
    DrifloonState drifloonStates[3];
} EndCreditsMorningSceneData;

typedef struct WingullState {
    fx32 xPos;
    fx32 yPos;
    fx32 unused_08;
    fx32 unused_0C;
    fx32 degrees;
    fx32 xDegrees;
    fx32 yDegrees;
    fx32 flyInOutSpeed;
    fx32 xSpeed;
    fx32 ySpeed;
    fx32 xOffset;
    fx32 yOffset;
    s32 unused_30;
    s16 startTimer;
    s16 rotateTimer;
    s8 rotateDirection;
    u8 phase;
} WingullState;

typedef struct EndCreditsDaySceneData {
    fx32 unused_00;
    fx32 bgYPos;
    u16 bgPaletteBuffers[4][16];
    int currentTransitionFrame;
    int fraction;
    int animDirection;
    WingullState wingullStates[2];
} EndCreditsDaySceneData;

typedef struct MagnezoneState {
    fx32 xPos;
    fx32 yPos;
    fx32 degrees;
    s16 rotateTimer;
    s8 rotateDirection;
} MagnezoneState;

typedef struct EndCreditsNightSceneData {
    fx32 unused_00;
    fx32 unused_04;
    u16 bgPaletteBuffers[8][16];
    int currentTransitionFrame;
    int fraction;
    int unused_110;
    int backgroundState;
    u32 frames;
    u8 meteorLoaded;
    u8 meteorScreen;
    s8 ev1;
    s8 ev2;
    s32 modelToRender;
    MagnezoneState magnezoneStates[1];
} EndCreditsNightSceneData;

typedef struct EndCreditsMemoriesSceneData {
    u32 framesElapsed;
} EndCreditsMemoriesSceneData;

typedef struct EndCreditsTwinleafSceneData {
    u32 framesElapsed;
    u8 currentImage;
    s8 ev1;
    s8 ev2;
    u32 plane1;
} EndCreditsTwinleafSceneData;

typedef struct EndCreditsFinSceneData {
    u32 unused_00;
} EndCreditsFinSceneData;

typedef struct EndCreditsSceneManager {
    int state;
    s16 memoryCooldown;
    s16 unused_06;
    union {
        EndCreditsMorningSceneData morningSceneData;
        EndCreditsDaySceneData daySceneData;
        EndCreditsNightSceneData nightSceneData;
        EndCreditsMemoriesSceneData memoriesSceneData;
        EndCreditsTwinleafSceneData twinleafSceneData;
        EndCreditsFinSceneData finSceneData;
    };
} EndCreditsSceneManager;

typedef struct EndCreditsStringsManager EndCreditsStringsManager;

typedef struct EndCreditsPlayerAnimManager {
    s16 blinkFrames;
    s16 blinkState;
    s16 animCount;
    s16 blinkCount;
} EndCreditsPlayerAnimManager;

typedef struct EndCreditsApp {
    ClearGamePlayerInfo *clearGamePlayerInfo;
    BOOL unused_04;
    BgConfig *bgConfig;
    PaletteData *paletteData;
    G3DPipelineBuffers *g3dPipeline;
    SysTask *sysTask;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    MsgData *messageLoader;
    Window *window;
    Camera *camera;
    Field3dModel bg3DModels[4];
    Field3dObject bg3DObjects[2][16];
    EndCredits3DModelManager modelManager;
    EndCreditsSceneManager sceneManager;
    ManagedSprite *managedSprites[END_CREDITS_SPRITE_COUNT];
    u8 *eyeCharData;
    NARC *narc;
    u32 framesElapsed;
    u8 creditsSkipState;
    u8 currentScene;
    u8 sceneLoaded;
    u8 bg3DModelLoaded;
    u8 bg3DModelRows;
    u8 currentMemory;
    EndCreditsStringsManager *subScreenStringsManager;
    EndCreditsStringsManager *mainScreenStringsManager;
    BOOL creditsScrollDone;
    EndCreditsPlayerAnimManager playerAnimManager;
} EndCreditsApp;

void EndCreditsCommon_Load3DModelStartingPositions(EndCreditsApp *endCreditsApp, int bikeSceneIndex);

void EndCreditsCommon_Set3DModelStartingPositions(EndCreditsApp *endCreditsApp);

typedef struct {
    u16 anim[2];
} ov104_AnimEntry_unused;

typedef struct ov104_ModelEntry {
    u32 narcIdx;
    u16 anims[2];
} ov104_ModelEntry;

typedef struct ov104_Slot {
    NNSG3dRenderObj renderObj;
    NNSG3dResMdl *model;
    void *resource;
    void *animFiles[2];
    NNSG3dAnmObj *animObjs[2];
} ov104_Slot;

extern ov104_ModelEntry ov104_021E60C0[];

extern ov104_ModelEntry ov104_021E60D8[];

extern ov104_ModelEntry ov104_021E6090[];

extern ov104_ModelEntry ov104_021E60F0[];

extern ov104_ModelEntry _021E6060[];

extern ov104_ModelEntry ov104_021E60A8[];

extern ov104_ModelEntry ov104_021E6078[];

extern ov104_ModelEntry ov104_021E6108[];

extern u8 _021E5EF0[];

extern u8 ov104_021E5F08[];

typedef struct ov104_App {
    u8 pad_00[4];
    ov104_Slot slots[3];
    struct NNSFndAllocator allocator;
    u8 pad_b[0x164 - 0x158];
    u8 flagA;
    u8 flagB;
    u8 flagC;
    u8 pad_c;
    u8 pad_d[4];
    u8 *table;
} ov104_App;


void NARC_Delete(NARC *narc);


void HeapExp_FndInitAllocator(NNSFndAllocator *allocator, enum HeapID heapID, int alignment);

void GF3dRender_InitObjFromHeader(void *a, void *b, void *c);

void ov104_021E5CC8(ov104_App *app);

void ov104_021E5CC8(ov104_App *app) {
    u8 i;
    ov104_ModelEntry *list;
    NNSG3dResTex *tex;
    NARC *narc;
    ov104_ModelEntry *entry;
    u8 j;
    ov104_Slot *slot;

    narc = NARC_New(NARC_a_2_3_3, (enum HeapID)149);
    HeapExp_FndInitAllocator(&app->allocator, (enum HeapID)149, 4);
    if (app->flagA == 0) {
        if (app->flagC == 0) {
            if (app->flagB == 0) {
                list = ov104_021E60C0;
            } else {
                list = ov104_021E60D8;
            }
        } else if (app->flagB == 0) {
            list = ov104_021E6090;
        } else {
            list = ov104_021E60F0;
        }
        app->table = _021E5EF0;
    } else {
        if (app->flagC == 0) {
            if (app->flagB == 0) {
                list = _021E6060;
            } else {
                list = ov104_021E60A8;
            }
        } else if (app->flagB == 0) {
            list = ov104_021E6078;
        } else {
            list = ov104_021E6108;
        }
        app->table = ov104_021E5F08;
    }
    for (i = 0; i < 3; i++) {
        slot = &app->slots[i];
        entry = &list[i];
        slot->resource = NARC_AllocAndReadWholeMember(narc, entry->narcIdx, (enum HeapID)149);
        GF3dRender_InitObjFromHeader(&slot->renderObj, &slot->model, &slot->resource);
        tex = NNS_G3dGetTex(app->slots[i].resource);
        if (i == 2) {
            NNS_G3dMdlUseGlbDiff(slot->model);
            NNS_G3dMdlUseGlbAmb(slot->model);
            NNS_G3dMdlUseGlbSpec(slot->model);
            NNS_G3dMdlUseGlbEmi(slot->model);
        }
        for (j = 0; j < 2; j++) {
            void *anm;
            slot->animFiles[j] = NARC_AllocAndReadWholeMember(narc, entry->anims[j], (enum HeapID)149);
            anm = NNS_G3dGetAnmByIdx(slot->animFiles[j], 0);
            slot->animObjs[j] = NNS_G3dAllocAnmObj(&app->allocator, anm, slot->model);
            NNS_G3dAnmObjInit(slot->animObjs[j], anm, slot->model, tex);
            NNS_G3dRenderObjAddAnmObj(&slot->renderObj, slot->animObjs[j]);
        }
    }
    NARC_Delete(narc);
}
