#include "global.h"

#include "overworld_anim_manager.h"
#include "sprite.h"
#include "sys_task.h"
#include "overlay_02_hmcut_in.h"

struct HMCutIn;

typedef struct WindParticleAnimData {
    int spriteListPriority;
    BOOL allowWindParticles;
    HMCutIn *cutIn;
    VecFx32 movementDelta;
} WindParticleAnimData;

typedef struct WindParticleAnimEnv {
    int _;
    int animID;
    Sprite *sprite;
    WindParticleAnimData data;
} WindParticleAnimEnv;

Sprite *ov02_0224A3F0(HMCutIn *cutIn, const VecFx32 *position, int listPriority, int animID);

int ov02_0224AA80(OverworldAnimManager *animMan, void *windParticleAnimEnv);

void ov02_0224AAC8(OverworldAnimManager *animMan, void *windParticleAnimEnv);

int ov02_0224AA80(OverworldAnimManager *animMan, void *windParticleAnimEnv) {
    VecFx32 position;
    WindParticleAnimEnv *env = windParticleAnimEnv;
    const WindParticleAnimData *animData = sub_02068D98(animMan);

    env->data = *animData;
    env->animID = sub_02068D90(animMan);

    sub_02068DB8(animMan, &position);

    env->sprite = ov02_0224A3F0(env->data.cutIn, &position, env->data.spriteListPriority, env->animID);
    Sprite_SetDrawFlag(env->sprite, FALSE);

    return 1;
}

void ov02_0224AAC8(OverworldAnimManager *animMan, void *windParticleAnimEnv) {
    WindParticleAnimEnv *env = windParticleAnimEnv;
    Sprite_Delete(env->sprite);
}

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: the funcs table the DS keeps in asm/overlay_02_02248728_0224AC38_022533FC.s (same label), with the context size from the type, not the DS literal.
void ov02_0224AAC8(OverworldAnimManager *animMan, void *windParticleAnimEnv);
void ov02_0224AAD4(OverworldAnimManager *animMan, void *windParticleAnimEnv);
void ov02_0224AB54(void);
extern const OverworldAnimManagerFuncs ov02_02253468;
const OverworldAnimManagerFuncs ov02_02253468 = OVERWORLD_ANIM_MANAGER_FUNCS(WindParticleAnimEnv, ov02_0224AA80, ov02_0224AAC8, ov02_0224AAD4, ov02_0224AB54);
#endif
