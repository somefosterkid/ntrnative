#include "global.h"

#include "battle/battler_sprite_context.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sprite_resources_header.h"
#include "overlay_07_battle_anim_system.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"
#include "library/spl_particle.h"
#include "library/spl_resource.h"
#include "library/spl_emitter.h"
#include "library/spl_texture.h"
#include "library/spl_manager.h"
#include "bg_window.h"
#include "palette.h"
#include "pokepic.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "sprite_system.h"
#include "camera.h"
#include "particle_system.h"
#include "overlay_07_bg_scroll_context.h"

struct BattleAnimSystem;

struct Pokepic;

struct PokepicDrawParam;

struct SPLEmitter;

struct SysTask;

struct SysTaskQueue;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

void sub_020154E4(ParticleSystem *particleSystem, const VecFx32 *up);

void *sub_02015504(void);

void ov07_02231B90(BattleAnimSystem *system, int battler, VecFx32 *pos);

void ov07_02231DE8(int param0, VecFx32 *param1);

void ov07_02220030(SPLEmitter *emitter);

void ov07_02220030(SPLEmitter *emitter) {
    VecFx32 player1Pos, up;

    BattleAnimSystem *system = sub_02015504();
    int defender = ov07_0221C470(system);
    ParticleSystem *ps = ov07_0221C478(system);

    ov07_02231B90(system, 0, &player1Pos);

    SPL_SetEmitterPositionX(emitter, player1Pos.x);
    SPL_SetEmitterPositionY(emitter, player1Pos.y);
    SPL_SetEmitterPositionZ(emitter, player1Pos.z);

    ov07_02231DE8(defender, &up);
    sub_020154E4(ps, &up);
}

void ov07_0222008C(SPLEmitter *emitter);

void ov07_0222008C(SPLEmitter *emitter) {
    BattleAnimSystem *system = sub_02015504();
    int defender = ov07_0221C470(system);

    VecFx32 pos;
    ov07_02231B90(system, defender, &pos);

    SPL_SetEmitterPositionX(emitter, pos.x);
    SPL_SetEmitterPositionY(emitter, pos.y);
    SPL_SetEmitterPositionZ(emitter, pos.z);
}

void ov07_022200D0(SPLEmitter *emitter);

void ov07_022200D0(SPLEmitter *emitter) {
    BattleAnimSystem *system = sub_02015504();
    int defender = ov07_0221C468(system);

    VecFx32 pos;
    ov07_02231B90(system, defender, &pos);

    SPL_SetEmitterPositionX(emitter, pos.x);
    SPL_SetEmitterPositionY(emitter, pos.y);
    SPL_SetEmitterPositionZ(emitter, pos.z);
}

void ov07_02220114(SPLEmitter *emitter);

void ov07_02220114(SPLEmitter *emitter) {
    BattleAnimSystem *system = sub_02015504();
    int attacker = ov07_0221C468(system);
    (void)(ov07_0221C470(system));

    VecFx32 pos;
    ov07_02231B90(system, attacker, &pos);

    SPL_SetEmitterPositionX(emitter, pos.x);
    SPL_SetEmitterPositionY(emitter, pos.y);
    SPL_SetEmitterPositionZ(emitter, pos.z);
}
