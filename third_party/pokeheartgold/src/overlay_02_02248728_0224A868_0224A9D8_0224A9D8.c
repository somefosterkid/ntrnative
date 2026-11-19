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
#include "overlay_02_hmcut_in.h"

typedef struct WindParticleAnimConfig {
    fx32 initialX;
    fx32 initialY;
    fx32 movementDeltaX;
    int spriteListPriority;
    int animID;
} WindParticleAnimConfig;

void ov02_0224AA44(HMCutIn *cutIn, const VecFx32 *initialPos, const VecFx32 *movementDelta, int spriteListPriority, int param4, BOOL allowWindParticles);

extern const WindParticleAnimConfig ov02_022535E4[13];

void ov02_0224A9D8(HMCutIn *cutIn, int allowWindParticles);

void ov02_0224A9D8(HMCutIn *cutIn, int allowWindParticles) {
    int i, v1, v2;
    VecFx32 v3, v4;

    cutIn->windParticlesAnimState = 2;

    for (i = 0; i < 13; i++) {
        v3.x = ov02_022535E4[i].initialX;
        v3.y = ov02_022535E4[i].initialY;
        v3.z = 0;
        v4.x = ov02_022535E4[i].movementDeltaX;
        v4.y = 0;
        v4.z = 0;
        v2 = ov02_022535E4[i].spriteListPriority;
        v1 = ov02_022535E4[i].animID;

        ov02_0224AA44(cutIn, &v3, &v4, v2, v1, allowWindParticles);
        v3.x += (FX32_ONE * 256);
        ov02_0224AA44(cutIn, &v3, &v4, v2, v1, allowWindParticles);
    }
}
