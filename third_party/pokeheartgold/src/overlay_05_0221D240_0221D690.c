#include "global.h"

#include "unk_02013534.h"
#include "overlay_05_unk_struct_ov10_0221_fb28_t.h"
#include "battle/trainer_intro_data.h"

void ov05_0221D690(UnkStruct_ov10_0221FB28 *arg0);

void ov05_0221D690(UnkStruct_ov10_0221FB28 *arg0) {
    u32 v0;
    void *v1;

    if (arg0->trainerIntroData->mode != 0) {
        v0 = 0;
        v1 = arg0;
        do {
            FontOAM_Delete(*(TextOBJ **)((u8 *)v1 + 0xB48));
            v0 += 1;
            v1 += 4;
        } while (v0 < 4U);
        sub_020135AC(arg0->unk_B44);
    }
}
