#include "global.h"

#include "overlay_07_bg_scroll_context.h"

#include "constants/heap.h"

#include "library/spl_emitter.h"
#include "library/spl_manager.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "heap.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "particle_system.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_bg_switch.h"
#include "overlay_74_main_menu_util.h"
#include "pokepic.h"
#include "sav_chatot.h"
#include "battle/battler_sprite_context.h"
#include "filesystem_files_def.h"
#include "overlay_07_battle_anim_context.h"
#include "overlay_07_battle_bg_anim.h"
#include "overlay_07_battle_anim_tracking.h"

typedef struct SpriteAnimFrame SpriteAnimFrame;

typedef struct BattleBackgroundReference BattleBackgroundReference;

typedef struct BattleAnimScriptLoop BattleAnimScriptLoop;

typedef struct PokemonSpriteDrawContext PokemonSpriteDrawContext;

struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
};

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

struct BattleBackgroundReference {
    int narcID;
    int tilesNarcMemberIdx;
    int paletteNarcMemberIdx;
    int tilemapNarcMemberIdx;
    int paletteDestStart;
    int paletteSrcSize;
};

struct BattleAnimScriptLoop {
    u32 *startPos;
    u8 curIteration;
    u8 maxIterations;
    BOOL isActive;
};

struct PokemonSpriteDrawContext {
    struct BattleAnimSystem *battleAnimSys;
    SpriteManager *spriteManager;
    ManagedSprite *sprite;
    BOOL active;
};


s32 ov07_0221DDB0(BattleBgSwitch *, BattleAnimSystem *, u32);

void ov07_0221E600(SysTask *task, void *param);

BOOL ov07_0221E6D4(BattleBgSwitch *bgSwitch);

BOOL ov07_0221E6D4(BattleBgSwitch *bgSwitch) {
    BattleBgAnim *bgAnim = Heap_Alloc((enum HeapID)bgSwitch->battleAnimSystem->heapID, sizeof(BattleBgAnim));

    bgAnim->bgConfig = bgSwitch->battleAnimSystem->bgConfig;
    bgAnim->offsetX = bgSwitch->battleAnimSystem->scriptVars[2];
    bgAnim->offsetY = bgSwitch->battleAnimSystem->scriptVars[3];
    bgAnim->stepX = bgSwitch->battleAnimSystem->scriptVars[0];
    bgAnim->stepY = bgSwitch->battleAnimSystem->scriptVars[1];
    bgAnim->bg = 3;
    bgAnim->unusedBg = 3;

    if (ov07_0221DDB0(bgSwitch, bgSwitch->battleAnimSystem, 6) == 1) {
        bgAnim->stepX *= -1;
        bgAnim->stepY *= -1;
        bgAnim->offsetX *= -1;
        bgAnim->offsetY *= -1;
    }

    bgAnim->unused = 1;
    bgAnim->cancel = 0;

    ov07_0221DD38(bgSwitch->battleAnimSystem->bgAnim, bgAnim, 1);
    bgSwitch->bgMoveAnimActive = 1;

    SysTask_CreateOnMainQueue(ov07_0221E600, bgAnim, 0x1001);

    return 0;
}
