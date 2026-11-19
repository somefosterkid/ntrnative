#include "global.h"

#include "overlay_05.h"
#include "unk_02013534.h"
#include "battle/trainer_intro_data.h"
#include "pm_string.h"
#include "overlay_05_unk_struct_ov10_0221_fb28_t.h"

void ov05_0221D530(UnkStruct_ov10_0221FB28 *param0, u32 param1, String *param2);

void ov05_0221D5DC(UnkStruct_ov10_0221FB28 *arg0) {
    TrainerIntroData *v0;

    arg0->unk_B44 = FontSystem_NewInit(8, arg0->trainerIntroData->heapID);
    ov05_0221D530(arg0, 0, arg0->trainerIntroData->trainerNames[0]);
    ov05_0221D530(arg0, 2, arg0->trainerIntroData->trainerNames[1]);
    v0 = arg0->trainerIntroData;

    if (v0->playerSide == 1) {
        ov05_0221D530(arg0, 1, v0->trainerNames[2]);
        ov05_0221D530(arg0, 3, arg0->trainerIntroData->trainerNames[3]);
        return;
    }

    ov05_0221D530(arg0, 1, v0->trainerNames[0]);
    ov05_0221D530(arg0, 3, arg0->trainerIntroData->trainerNames[1]);
    TextOBJ_SetSpritesDrawFlag(arg0->unk_B48[1], 0);
    TextOBJ_SetSpritesDrawFlag(arg0->unk_B48[3], 0);
}
