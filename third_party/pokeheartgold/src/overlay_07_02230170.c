#include "global.h"

#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sys_task.h"
#include "overlay_07_bg_scroll_context.h"
#include "overlay_07_battle_anim_system.h"

struct BattleAnimSystem;
struct BufferManager;
struct CustomBgScrollContext;
struct Pokepic;
struct PokepicDrawParam;
typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);
typedef struct ExtrasensoryContext {
    BattleAnimSystem *battleAnimSys;
    CustomBgScrollContext *bgScroll;
    int state;
    Pokepic *defenderSprite;
    int y;
    int centerY;
    int baseAngle;
    fx32 baseAmplitude;
    int dir;
    int amplitudeStep;
    int delay;
    int count;
    u32 initValue;
} ExtrasensoryContext;

void ov07_02230170(ExtrasensoryContext *ctx);

void ov07_02230170(ExtrasensoryContext *ctx) {
    switch (ctx->count) {
    case 0:
        ctx->baseAngle = (((180) * 0xFFFF) / 360);
        ctx->amplitudeStep = 5;
        ctx->baseAmplitude = ((fx32)(((16) > 0) ? ((16) * (1 << 12) + 0.5f) : ((16) * (1 << 12) - 0.5f)));
        ctx->delay = 16;
        break;
    case 1:
        ctx->baseAngle = (((180) * 0xFFFF) / 360);
        ctx->amplitudeStep = (-5);
        ctx->baseAmplitude = ((fx32)(((-16) > 0) ? ((-16) * (1 << 12) + 0.5f) : ((-16) * (1 << 12) - 0.5f)));
        ctx->delay = 16;
        break;
    case 2:
        ctx->baseAngle = (((180) * 0xFFFF) / 360);
        ctx->amplitudeStep = 10;
        ctx->baseAmplitude = ((fx32)(((20) > 0) ? ((20) * (1 << 12) + 0.5f) : ((20) * (1 << 12) - 0.5f)));
        ctx->delay = 16;
        break;
    }
}
