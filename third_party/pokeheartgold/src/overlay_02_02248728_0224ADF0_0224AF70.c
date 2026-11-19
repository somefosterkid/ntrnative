#include "global.h"
#include "sprite_resources_header.h"
#include "field_system_def.h"

#include "overworld_anim_manager.h"
#include "struct_defs/resdat_narc_structs.h"

#include "field/area_data.h"
#include "field/area_light.h"
#include "field/dynamic_terrain_height.h"
#include "field/map_prop_animation.h"
#include "field/model_attributes.h"
#include "field/overlay_01_02204004.h"

#include "bag_cursor.h"
#include "bg_window.h"
#include "camera.h"
#include "gf_3d_loader.h"
#include "igt.h"
#include "math_util.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "sprite.h"
#include "unk_0200A090.h"
#include "overlay_02_hmcut_in.h"



void ov02_0224B2CC(OverworldAnimManager *param0);

int ov02_0224AF70(FlyTaskEnv *env);

int ov02_0224AF70(FlyTaskEnv *env) {
    VecFx32 newPos;
    Sprite *birdSprite = env->birdSprite;

    env->pathParam -= env->pathDelta;

    if (env->pathParam < 0) {
        env->pathParam = 0;
    }

    if (env->pathDelta > 0x800) {
        env->pathDelta -= 0x1800;
    }

    if (env->pathDelta < FX32_ONE) {
        env->pathDelta = FX32_ONE;
    }

    env->pathOffset.x = GF_CosDeg(315) * (env->pathParam / FX32_ONE);
    env->pathOffset.y = GF_SinDeg(env->angleParam / FX32_ONE) * (env->pathParam / FX32_ONE);

    if (env->angleParam / FX32_ONE < 270) {
        env->angleParam += FX32_CONST(4);
    }

    env->birdScale.x += env->scaleDelta;

    if (env->birdScale.x > 0x1800) {
        env->birdScale.x = 0x1800;
    }

    env->birdScale.y += env->scaleDelta;

    if (env->birdScale.y > 0x1800) {
        env->birdScale.y = 0x1800;
    }

    Sprite_SetAffineScale(birdSprite, &env->birdScale);
    env->birdRotation += FX32_CONST(8);

    if (env->birdRotation / FX32_ONE > 360) {
        env->birdRotation = FX32_CONST(360);
    }

    Sprite_SetAffineZRotation(birdSprite, GF_DegreeToSinCosIdx(env->birdRotation / FX32_ONE));

    newPos.x = env->birdStartPos.x + env->pathOffset.x;
    newPos.y = env->birdStartPos.y + env->pathOffset.y;

    Sprite_SetMatrix(birdSprite, &newPos);

    if (env->pathParam == 0) {
        env->descendCount = 0;
        env->state++;
    } else {
        env->descendCount++;
    }

    if (env->descendCount == 12) {
        ov02_0224B2CC(env->unk_60);
    }

    Sprite *playerSprite = env->cutIn->playerSprite;
    const VecFx32 *playerScale = Sprite_GetScalePtr(playerSprite);
    VecFx32 newPlayerScale = *playerScale;

    newPlayerScale.x += 0x80;

    if (newPlayerScale.x > 0x1400) {
        newPlayerScale.x = 0x1400;
    }

    newPlayerScale.y += 0x80;

    if (newPlayerScale.y > 0x1400) {
        newPlayerScale.y = 0x1400;
    }

    Sprite_SetAffineScale(playerSprite, &newPlayerScale);

    return 0;
}
