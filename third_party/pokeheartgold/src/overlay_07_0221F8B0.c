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
#include "pokedex.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "overlay_07_battle_anim_system.h"
#include "filesystem_files_def.h"
#include "particle_system.h"
#include "pokepic.h"
#include "overlay_07_bg_scroll_context.h"
#include "mail_misc.h"
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

typedef void (*BattleAnimScriptCmd)(BattleAnimSystem *);

#if defined(SDK_PORT) || defined(NONMATCHING)
// ov07_02234DE8 is asm data of .word relocations (u32 slots at stride 4): the 88 command handlers as C pointers. The handlers are
// declared unprototyped: the table is read as `void (*)(BattleAnimSystem *)`, a few handlers take no parameter or others.
void ov07_0221C6EC();
void ov07_0221C70C();
void ov07_0221C934();
void ov07_0221C974();
void ov07_0221C9C0();
void ov07_0221F098();
void ov07_0221D5A8();
void ov07_0221D5AC();
void ov07_0221F3F4();
void ov07_0221F424();
void ov07_0221D02C();
void ov07_0221D05C();
void ov07_0221C72C();
void ov07_0221D148();
void ov07_0221D2B0();
void ov07_0221D2E4();
void ov07_0221ED44();
void ov07_0221EE74();
void ov07_0221EEC0();
void ov07_0221EF14();
void ov07_0221EF34();
void ov07_0221EF54();
void ov07_0221F0CC();
void ov07_0221F104();
void ov07_0221F120();
void ov07_0221F2DC();
void ov07_0221F340();
void ov07_0221F42C();
void ov07_0221F398();
void ov07_0221F39C();
void ov07_0221D088();
void ov07_0221D5B0();
void ov07_0221D718();
void ov07_0221D0C0();
void ov07_0221F024();
void ov07_0221F1C0();
void ov07_0221F238();
void ov07_0221F084();
void ov07_0221F088();
void ov07_0221F08C();
void ov07_0221DD0C();
void ov07_0221DD10();
void ov07_0221F090();
void ov07_0221F094();
void ov07_0221F0B0();
void ov07_0221D0E8();
void ov07_0221CBC0();
void ov07_0221CC54();
void ov07_0221CDA4();
void ov07_0221CE5C();
void ov07_0221CF14();
void ov07_0221CF54();
void ov07_0221CFA8();
void ov07_0221D000();
void ov07_0221F8AC();
void ov07_0221F814();
void ov07_0221D740();
void ov07_0221D7B8();
void ov07_0221D874();
void ov07_0221DA74();
void ov07_0221DAA0();
void ov07_0221DCC0();
void ov07_0221DCD4();
void ov07_0221DCF0();
void ov07_0221D174();
void ov07_0221F81C();
void ov07_0221F880();
void ov07_0221C74C();
void ov07_0221C8CC();
void ov07_0221C910();
void ov07_0221D1B8();
void ov07_0221D214();
void ov07_0221D270();
void ov07_0221F430();
void ov07_0221F4CC();
void ov07_0221F514();
void ov07_0221F574();
void ov07_0221F5BC();
void ov07_0221F604();
void ov07_0221F710();
void ov07_0221F7C4();
void ov07_0221F7F0();
void ov07_0221DAF4();
void ov07_0221DCA8();
void ov07_0221C6B4();
void ov07_0221D374();
void ov07_0221ED94();
void ov07_0221D23C();

void (*const ov07_02234DE8[88])() = {
    ov07_0221C6EC,
    ov07_0221C70C,
    ov07_0221C934,
    ov07_0221C974,
    ov07_0221C9C0,
    ov07_0221F098,
    ov07_0221D5A8,
    ov07_0221D5AC,
    ov07_0221F3F4,
    ov07_0221F424,
    ov07_0221D02C,
    ov07_0221D05C,
    ov07_0221C72C,
    ov07_0221D148,
    ov07_0221D2B0,
    ov07_0221D2E4,
    ov07_0221ED44,
    ov07_0221EE74,
    ov07_0221EEC0,
    ov07_0221EF14,
    ov07_0221EF34,
    ov07_0221EF54,
    ov07_0221F0CC,
    ov07_0221F104,
    ov07_0221F120,
    ov07_0221F2DC,
    ov07_0221F340,
    ov07_0221F42C,
    ov07_0221F398,
    ov07_0221F39C,
    ov07_0221D088,
    ov07_0221D5B0,
    ov07_0221D718,
    ov07_0221D0C0,
    ov07_0221F024,
    ov07_0221F1C0,
    ov07_0221F238,
    ov07_0221F084,
    ov07_0221F088,
    ov07_0221F08C,
    ov07_0221DD0C,
    ov07_0221DD10,
    ov07_0221F090,
    ov07_0221F094,
    ov07_0221F0B0,
    ov07_0221D0E8,
    ov07_0221CBC0,
    ov07_0221CC54,
    ov07_0221CDA4,
    ov07_0221CE5C,
    ov07_0221CF14,
    ov07_0221CF54,
    ov07_0221CFA8,
    ov07_0221D000,
    ov07_0221F8AC,
    ov07_0221F814,
    ov07_0221D740,
    ov07_0221D7B8,
    ov07_0221D874,
    ov07_0221DA74,
    ov07_0221DAA0,
    ov07_0221DCC0,
    ov07_0221DCD4,
    ov07_0221DCF0,
    ov07_0221D174,
    ov07_0221F81C,
    ov07_0221F880,
    ov07_0221C74C,
    ov07_0221C8CC,
    ov07_0221C910,
    ov07_0221D1B8,
    ov07_0221D214,
    ov07_0221D270,
    ov07_0221F430,
    ov07_0221F4CC,
    ov07_0221F514,
    ov07_0221F574,
    ov07_0221F5BC,
    ov07_0221F604,
    ov07_0221F710,
    ov07_0221F7C4,
    ov07_0221F7F0,
    ov07_0221DAF4,
    ov07_0221DCA8,
    ov07_0221C6B4,
    ov07_0221D374,
    ov07_0221ED94,
    ov07_0221D23C,
};
#else
extern u32 ov07_02234DE8;
#endif

BattleAnimScriptCmd ov07_0221F8B0(u32 id);

struct UnlockedEasyChatWords;
typedef struct UnlockedEasyChatWords UnlockedEasyChatWords;
u32 ov07_0221F8C4(const EasyChatArgs *args);

BattleAnimScriptCmd ov07_0221F8B0(u32 id) {
#if defined(SDK_PORT) || defined(NONMATCHING)
    if (id > 0x57) {
        return NULL;
    }
    return ov07_02234DE8[id];
#else
    if (0x58 < id) {
        return (BattleAnimScriptCmd)0x0;
    }
    return *(BattleAnimScriptCmd *)(&ov07_02234DE8 + id);
#endif
}

u32 ov07_0221F8C4(const EasyChatArgs *args) {
    return args->unk1;
}
