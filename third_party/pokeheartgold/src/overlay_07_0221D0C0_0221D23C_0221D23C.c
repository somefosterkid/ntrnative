#include "global.h"

#include "battle/battler_sprite_context.h"

#include "constants/heap.h"

#include "library/spl_emitter.h"
#include "library/spl_manager.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "filesystem_files_def.h"
#include "particle_system.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "overlay_07_bg_scroll_context.h"
#include "sav_chatot.h"
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

void ov07_0221D23C(BattleAnimSystem *system);

void ov07_0221D23C(BattleAnimSystem *system) {
    u32 **ppuVar1;

    system->scriptPtr = system->scriptPtr + 1;
    ppuVar1 = &system->scriptPtr;
    if ((system->context->isTransformed << 0x1e) >> 0x1f) {
        *ppuVar1 = *ppuVar1 + *system->scriptPtr;
        return;
    }
    *ppuVar1 = *ppuVar1 + 1;
    return;
}
