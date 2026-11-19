#include "global.h"

#include "constants/heap.h"

struct BgConfig;
struct CounterData;
struct PoketchAnimation_AnimatedSpriteData;
struct PoketchAnimation_AnimationManager;
typedef struct PoketchAnimation_AnimationManager PoketchAnimation_AnimationManager;
typedef struct PoketchAnimation_AnimatedSpriteData PoketchAnimation_AnimatedSpriteData;
typedef struct PoketchAnimation_SpriteData {
    void *compressedSprite;
    void *compressedAnim;
    NNSG2dCellDataBank *cell;
    NNSG2dAnimBankData *anim;
    enum HeapID heapID;
} PoketchAnimation_SpriteData;
typedef struct BgConfig BgConfig;
BOOL RemoveWindow(u32 *activeList, u32 taskId);
typedef struct CounterData CounterData;
typedef struct CounterGraphics {
    const CounterData *counterData;
    BgConfig *bgConfig;
    u32 activeTasks[2 + 4];
    PoketchAnimation_AnimationManager *animMan;
    PoketchAnimation_AnimatedSpriteData *buttonAnimation;
    PoketchAnimation_AnimatedSpriteData *digitsAnimation[4];
    PoketchAnimation_SpriteData buttonSprites;
    PoketchAnimation_SpriteData digitSprites;
} CounterGraphics;
enum CounterGraphicsTask {
    COUNTER_GRAPHICS_INIT = 0,
    COUNTER_GRAPHICS_FREE,
    COUNTER_GRAPHICS_UPDATE_BUTTON,
    COUNTER_GRAPHICS_UPDATE_VALUE,
};

BOOL ov43_0222C7EC(CounterGraphics *graphics, enum CounterGraphicsTask taskID);

BOOL ov43_0222C7EC(CounterGraphics *graphics, enum CounterGraphicsTask taskID) {
    return RemoveWindow(graphics->activeTasks, taskID);
}
