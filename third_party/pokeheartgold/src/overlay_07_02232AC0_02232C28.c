#include "global.h"

#include "constants/heap.h"

#include "library/spl_texture.h"

#include "camera.h"
#include "filesystem.h"
#include "pokemon_types_def.h"
#include "seal_case.h"
#include "sys_task.h"
#include "overlay_07_ball_capsule_seal_effect.h"

void *sub_02015504(void);

void ov07_02232BB0(int param0, VecFx32 *param1);

void ov07_02232C28(SPLEmitter *param0);

void ov07_02232C28(SPLEmitter *param0) {
    int v0;
    BallCapsuleSealEffect *ballCapSealEff = sub_02015504();
    v0 = ballCapSealEff->ballCapCfg.battlerType;

    {
        VecFx32 v2;

        ov07_02232BB0(v0, &v2);
        SPL_SetEmitterPosition(param0, &v2);
    }
}
