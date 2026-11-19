#include "global.h"
#include "library/spl_emitter.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_generic_emitter.h"
#include "particle_system.h"

u8 ov07_02231924(BattleAnimSystem *system, int battler);

u8 sub_02015530(ParticleSystem *particleSystem);

void ov07_02221550(SPLEmitter *emitter, GenericEmitterCallbackData *data);

void ov07_02221550(SPLEmitter *emitter, GenericEmitterCallbackData *data) {
    s32 iVar1;
    s32 unaff_r5;

    ov07_02231924(data->battleAnimSys, data->startBattler);
    iVar1 = ov07_02231924(data->battleAnimSys, data->endBattler);
    switch (data->params[GENERIC_EMITTER_CALLBACK_PARAM_PRIORITY]) {
    case 1:
        if (iVar1 == 0) {
            unaff_r5 = 0x100;
        } else {
            unaff_r5 = -0x1400;
        }
        break;
    case 2:
        if (iVar1 == 0) {
            unaff_r5 = 0;
        } else {
            unaff_r5 = -0x1500;
        }
        break;
    case 0:
    case 3:
        break;
    }
    sub_02015530(data->particleSys);
    *((s32 *)&emitter->emtr_pos.z) = unaff_r5 + *(s32 *)(**((u8 ***)&emitter->p_res) + 0xc);
    return;
}
