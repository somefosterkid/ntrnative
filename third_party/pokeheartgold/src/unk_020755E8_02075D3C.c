#include "global.h"

#include "constants/heap.h"

#include "library/spl_emitter.h"

#include "bag_types_def.h"
#include "bg_window.h"
#include "filesystem.h"
#include "options.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pm_string.h"
#include "sys_task.h"
#include "evolution_data.h"
#include "particle_system.h"
#include "pokemon_types_def.h"
#include "overlay_74_main_menu_util.h"
#include "pokemon_summary_app.h"
#include "pokedex.h"
#include "msgdata.h"
#include "pokepic.h"
#include "game_stats.h"

struct OverlayManager;
struct G3DPipelineBuffers;
struct GameStats;
struct MsgData;
struct PaletteAnimator;
struct ParticleSystem;
struct Party;
struct Pokedex;
struct Pokemon;
struct PokemonAnimManager;
struct Pokepic;
struct PokepicManager;
struct PokepicDrawParam;
struct PokemonSummary;
struct Poketch;
struct MessageFormat;
typedef struct {
    enum HeapID heapID;
    int unk_04;
} UnkStruct_0207C894;
typedef struct {
    UnkStruct_0207C894 unk_00;
    SPLEmitter *unk_08;
    ParticleSystem *unk_0C;
} UnkStruct_0207C8C4;
typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;
typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);
typedef struct PokemonAnimManager PokemonAnimManager;
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
typedef struct Poketch Poketch;
typedef struct PaletteAnimator PaletteAnimator;
typedef void (*UnkFuncPtr_020157E4)(PaletteAnimator *, u16 *);
struct PaletteAnimator {
    SysTask *unk_00;
    UnkFuncPtr_020157E4 unk_04;
    PaletteData *unk_08;
    u16 unk_0C[16];
    u16 unk_2C[16];
    u8 unk_4C;
    u8 unk_4D;
    u8 unk_4E;
    u8 unk_4F;
};
BOOL sub_02075D3C(EvolutionData *evolutionData);

BOOL sub_02075D3C(EvolutionData *evolutionData) {
    return evolutionData->done == 1;
}
