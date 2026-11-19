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
#include "overworld_anim_manager.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "overlay_02_hmcut_in.h"

typedef struct WindParticleAnimData {
    int spriteListPriority;
    BOOL allowWindParticles;
    HMCutIn *cutIn;
    VecFx32 movementDelta;
} WindParticleAnimData;

extern const OverworldAnimManagerFuncs ov02_02253468;

void ov02_0224AA44(HMCutIn *cutIn, const VecFx32 *initialPos, const VecFx32 *movementDelta, int spriteListPriority, int param4, BOOL allowWindParticles);

void ov02_0224AA44(HMCutIn *cutIn, const VecFx32 *initialPos, const VecFx32 *movementDelta, int spriteListPriority, int param4, BOOL allowWindParticles) {
    OverworldAnimManager *v0;
    WindParticleAnimData animData;

    animData.spriteListPriority = spriteListPriority;
    animData.allowWindParticles = allowWindParticles;
    animData.cutIn = cutIn;
    animData.movementDelta = *movementDelta;

    v0 = sub_02068B0C(cutIn->animManagerList, &ov02_02253468, initialPos, param4, &animData, 133);
}
