#include "global.h"

#include "overlay_05.h"
#include "pokemon_types_def.h"
#include "battle/trainer_intro_data.h"
#include "overlay_05_unk_struct_ov10_0221_fb28_t.h"

void ov05_0221DF38(UnkStruct_ov10_0221FB28 *param_1, Party *param_2, u8 param_3);

void ov05_0221E07C(UnkStruct_ov10_0221FB28 *param_1) {
    memset(param_1->unk_214, 0, sizeof(param_1->unk_214));
    ov05_0221DF38(param_1, param_1->trainerIntroData->party[0], 0);
    ov05_0221DF38(param_1, param_1->trainerIntroData->party[2], 3);
    return;
}
