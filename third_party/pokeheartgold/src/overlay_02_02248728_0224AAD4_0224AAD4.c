#include "global.h"

#include "constants/heap.h"

#include "field/dynamic_terrain_height.h"
#include "field/map_prop_animation.h"
#include "field/model_attributes.h"

#include "bag_cursor.h"
#include "bg_window.h"
#include "camera.h"
#include "field_types_def.h"
#include "map_matrix.h"
#include "options.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "unk_020689C8.h"
#include "overlay_02_hmcut_in.h"
#include "sprite.h"

struct SpriteList;


void Sprite_SetDrawFlag(Sprite *sprite, BOOL draw);

void ov02_0224AB54(void);

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

void ov02_0224AAD4(OverworldAnimManager *animMan, void *windParticleAnimEnv);

void ov02_0224AAD4(OverworldAnimManager *animMan, void *windParticleAnimEnv) {
    VecFx32 newPos;
    WindParticleAnimEnv *env = windParticleAnimEnv;

    sub_02068DB8(animMan, &newPos);

    newPos.x += env->data.movementDelta.x;
    newPos.x %= (FX32_ONE * 512);

    sub_02068DA8(animMan, &newPos);
    Sprite_SetMatrix(env->sprite, &newPos);

    if (env->data.allowWindParticles == TRUE) {
        HMCutIn *v2 = env->data.cutIn;

        if (v2->windParticlesAnimState == 2) {
            BOOL drawFlag = FALSE;
            fx32 v4 = v2->windowY1, v5 = v2->windowY2;

            if ((newPos.y - (FX32_ONE * 2)) >= v4
                && (newPos.y - (FX32_ONE * 2)) <= v5
                && (newPos.y + (FX32_ONE * 2)) >= v4
                && (newPos.y + (FX32_ONE * 2)) <= v5) {
                drawFlag = TRUE;
            }

            Sprite_SetDrawFlag(env->sprite, drawFlag);
        } else if (v2->windParticlesAnimState == 1) {
            Sprite_SetDrawFlag(env->sprite, FALSE);
        }
    }
}

void ov02_0224AB54(void) {
}
