#include "global.h"
struct EvolutionData;

#include "error_handling.h"
#include "sprite.h"
#include "sprite_system.h"
#include "evolution_data.h"

extern u32 _020FFEC4;

extern u32 _020FFEC6;

void sub_020757AC(struct EvolutionData *param_1);

void sub_020757AC(EvolutionData *param_1) {
    s32 unaff_r4;
    if (param_1->unk_B8 == FALSE) {
        GF_AssertFail();
    }
    if (param_1->unk_B4 == NULL) {
        GF_AssertFail();
    }
    switch (param_1->unk_8B) {
    case 1:
        unaff_r4 = 0;
        break;
    case 2:
        unaff_r4 = 1;
        break;
    default:
        GF_AssertFail();
        break;
    }
    ManagedSprite_SetPositionXY(param_1->unk_B4, *((s16 *)((&_020FFEC4) + unaff_r4)), *((s16 *)((&_020FFEC6) + unaff_r4)));
    return;
}
