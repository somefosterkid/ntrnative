#include "global.h"
struct EvolutionData;

#include "bg_window.h"
#include "error_handling.h"
#include "sprite.h"
#include "sprite_system.h"
#include "evolution_data.h"

s32 sub_020758D0(struct EvolutionData *arg0);

s32 sub_020759CC(struct EvolutionData *arg0);

u8 sub_02075A04(struct EvolutionData *arg0);

u8 sub_02075A04(EvolutionData *arg0) {
    u8 temp_r1;
    temp_r1 = arg0->unk_8A;
    switch (temp_r1) {
    case 0:
        if (sub_020758D0(arg0) != 0) {
            arg0->unk_8A = (u8)(1 + (arg0->unk_8A));
        }
        break;

    case 1:
        if (sub_020759CC(arg0) != 0) {
            if (arg0->unk_B8 == FALSE) {
                GF_AssertFail();
            }
            if (!(arg0->unk_8B)) {
                GF_AssertFail();
            }
            ToggleBgLayer(5U, 0U);
            ToggleBgLayer(6U, 0U);
            ManagedSprite_SetDrawFlag(arg0->unk_B4, 0);
            return arg0->unk_8B;
        }
        break;

    default:
        GF_AssertFail();
    }

    return 0U;
}
