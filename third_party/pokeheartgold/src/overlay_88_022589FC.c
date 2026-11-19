#include "global.h"

#include "heap.h"
#include "overlay_88.h"
#include "overlay_manager.h"
#include "system.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "filesystem.h"
#include "pm_string.h"
#include "gf_gfx_planes.h"
#include "vram_transfer_manager.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_88_unk_struct_ov66_02231428.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_88_wifi_plaza_news.h"

s32 ov88_02258AF4(void *);

s32 ov88_02258D84(void *);

s32 ov88_02258EF8(void *);

s32 ov88_022590D4(void *);

s32 ov88_022593D0(void *);

s32 ov88_022597DC(void *);

struct CounterData;
struct PoketchAnimation_AnimatedSpriteData;
struct PoketchAnimation_AnimationManager;
struct SpriteList;
struct SpriteResourceCollection;
struct UnkStruct_ov66_0222DFF8_t;
struct UnkStruct_ov66_0223177C_t;
typedef struct PoketchAnimation_AnimationManager PoketchAnimation_AnimationManager;
typedef struct PoketchAnimation_AnimatedSpriteData PoketchAnimation_AnimatedSpriteData;
typedef struct PoketchAnimation_SpriteData {
    void *compressedSprite;
    void *compressedAnim;
    NNSG2dCellDataBank *cell;
    NNSG2dAnimBankData *anim;
    enum HeapID heapID;
} PoketchAnimation_SpriteData;
BOOL ov88_02258B20(u32 *activeList, u32 taskId);
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
typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;
void ov88_02258B14(WiFiPlazaNewsGraphics *graphics);
void ov88_02258F60(WiFiPlazaNewsCountdown *countdown, WiFiPlazaNewsDigits *digits, WiFiPlazaNewsGraphics *graphics);
void ov88_02259104(WiFiPlazaNewsAnimState *animState, WiFiPlazaNewsDigits *digits, WiFiPlazaNewsGraphics *graphics, const UnkStruct_ov66_0223177C *playerList, const WiFiPlazaSystem *plazaSys);
void ov88_022594E0(WiFiPlazaNewsNameList *nameList);
void ov88_02259800(WiFiPlazaNewsCaptions *newsCaptions);
BOOL ov88_02258A64(CounterGraphics *graphics, enum CounterGraphicsTask taskID);
void ov88_02258A70(WiFiPlazaNewsData *plazaNewsData, const WiFiPlazaSystem *plazaSys);

s32 ov88_02258B34(void *, enum HeapID);
s32 ov88_02258C98(void *, enum HeapID);
extern GraphicsBanks ov88_02259958;
void ov88_02258ABC(WiFiPlazaNewsGraphics *arg0, enum HeapID arg1);

s32 ov88_022589FC(OverlayManager *man) {
    WiFiPlazaNewsData *data;

    data = OverlayManager_GetData(man);
    OverlayManager_GetArgs(man);
    ov88_022597DC(&data->captions);
    ov88_02258EF8(&data->countdown);
    ov88_022590D4(&data->animState);
    ov88_022593D0(&data->nameList);
    ov88_02258D84(&data->digits);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    ov88_02258AF4(&data->graphics);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_114);
    return 1;
}

BOOL ov88_02258A64(CounterGraphics *graphics, enum CounterGraphicsTask taskID) {
    return ov88_02258B20(graphics->activeTasks, taskID);
}

void ov88_02258A70(WiFiPlazaNewsData *plazaNewsData, const WiFiPlazaSystem *plazaSys) {
    ov88_02258F60(&plazaNewsData->countdown, &plazaNewsData->digits, &plazaNewsData->graphics);
    ov88_02259104(&plazaNewsData->animState, &plazaNewsData->digits, &plazaNewsData->graphics, plazaNewsData->playerList, plazaSys);
    ov88_022594E0(&plazaNewsData->nameList);
    ov88_02259800(&plazaNewsData->captions);
    ov88_02258B14(&plazaNewsData->graphics);
}

void ov88_02258ABC(WiFiPlazaNewsGraphics *arg0, enum HeapID heapID) {
    arg0->narc = NARC_New(NARC_a_1_9_6, heapID);
    GF_CreateVramTransferManager(0x30U, heapID);
    GfGfx_SetBanks(&ov88_02259958);
    ov88_02258B34(arg0, heapID);
    ov88_02258C98(arg0, heapID);
}
