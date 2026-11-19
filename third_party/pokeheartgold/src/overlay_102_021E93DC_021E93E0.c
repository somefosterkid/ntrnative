#include "global.h"

#include "bg_window.h"
#include "sprite.h"
#include "unk_0200B150.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "camera.h"
#include "overlay_07_lock_on_sprite_context.h"
#include "sprite_system.h"
#include "overlay_74_main_menu_util.h"
#include "pokepic.h"
#include "msgdata.h"

typedef struct UnkStruct_ov102_021E93E0_arg {
    u8 unk_0[0x20];
    BgConfig *unk_20;
    SpriteList *unk_24;
} UnkStruct_ov102_021E93E0_arg;

struct BgConfig;

struct Camera;

struct MsgData;

struct PoffinCutscene;

struct PoffinCutsceneData;

struct PoffinCutsceneMessages;

struct LockOnSpriteContext;

struct PoffinCutscenePokemon;

struct PoffinCutsceneTask;

struct PoffinCutsceneTaskManager;

struct PoffinSprite;

struct PoffinSpriteManager;

struct Pokemon;

struct Pokepic;

struct PokepicManager;

struct PokepicShadow;

struct PokepicTemplate;

struct PokepicDrawParam;

struct Sprite;

struct SpriteAnimFrame;

struct SpriteData;

struct SpriteSystem;

struct String;

struct MessageFormat;

struct SysTask;

struct SysTaskQueue;

struct Window;

typedef struct PoffinCutsceneData PoffinCutsceneData;

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

void PokepicManager_HandleLoadImgAndOrPltt(PokepicManager *monSpriteMan);

typedef struct PoffinCutscenePokemon {
    Pokemon *baseMon;
    u16 species;
    u8 nature;
    u8 gender;
    u8 flipSprite;
    u8 poffinPreference;
    String *name;
} PoffinCutscenePokemon;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

typedef struct PoffinSpriteManager PoffinSpriteManager;

typedef struct PoffinSprite PoffinSprite;

void SpriteSystem_TransferOam(void);

void GF_RunVramTransferTasks(void);

enum PoffinCutsceneState {
    STATE_FEED_POFFIN = 0,
    STATE_WAIT_FEED,
    STATE_PLAY_CRY,
    STATE_WAIT_CRY,
    STATE_MOVE_MON_BACK,
    STATE_WAIT_MOVE_MON_BACK,
    STATE_PRINT_MESSAGE,
    STATE_WAIT_MESSAGE_CLOSE,
    STATE_CLOSE_APP,
};

typedef struct PoffinCutsceneMessages {
    MessageFormat *template;
    String *buffer;
    String *strings[3];
} PoffinCutsceneMessages;

typedef struct SpriteData {
    VecFx32 startPos;
    VecFx32 endPos;
    VecFx32 posChangeStep;
    VecFx32 startSize;
    VecFx32 endSize;
    VecFx32 sizeStep;
    fx32 sineStep;
    fx32 unused;
    u16 offset;
    u16 unused2;
    fx32 unused3;
    int unused4;
} SpriteData;

typedef struct PoffinCutsceneTaskManager PoffinCutsceneTaskManager;

typedef struct PoffinCutsceneTask {
    int unused;
    int remainingCycles;
    int unused2;
    fx32 elapsedCycles;
    int unused3;
    PoffinCutsceneTaskManager *taskMan;
    SysTask *task;
} PoffinCutsceneTask;

struct PoffinCutsceneTaskManager {
    int unused;
    int runningTasks;
    int poffinPreference;
    SpriteData poffinSpriteState;
    SpriteData monSpriteState;
    Sprite *poffinSprite;
    Pokepic *monSprite;
    PoffinCutsceneTask tasks[4];
};

typedef struct PoffinCutscene {
    enum HeapID heapID;
    int subState;
    int unused;
    u16 printerID;
    u16 messageClearDelay;
    PoffinCutsceneData *data;
    int unused2;
    MsgData *msgLoader;
    u8 pad_1C[4];
    LockOnSpriteContext monSprite;
    PoffinSpriteManager *poffinSpriteMan;
    PoffinSprite *poffinSprite;
    int unused3;
    BgConfig *bgConfig;
    Window window;
    SpriteSystem *spriteSys;
    PoffinCutsceneTaskManager taskMan;
} PoffinCutscene;

enum PoffinCutsceneState PlayPokemonCry(PoffinCutscene *app, PoffinCutsceneTaskManager *taskMan);

enum PoffinCutsceneState PrintFeedMessage(PoffinCutscene *app);

void ov102_021E93E0(void *unused, UnkStruct_ov102_021E93E0_arg *arg);

void ov102_021E93E0(void *unused, UnkStruct_ov102_021E93E0_arg *arg) {
    DoScheduledBgGpuUpdates(arg->unk_20);
    SpriteList_RenderAndAnimateSprites(arg->unk_24);
    OamManager_ApplyAndResetBuffers();
    OS_SetIrqCheckFlag(1UL << 0);
}
