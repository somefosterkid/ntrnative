#include "global.h"

#include "bg_window.h"
#include "overlay_05.h"
#include "battle/trainer_intro_data.h"
#include "overlay_05_unk_struct_ov10_0221_fb28_t.h"

s32 ov05_0221DDEC(void *, void *, u8);

void ov05_0221DE6C(UnkStruct_ov10_0221FB28 *param_1, u8 param_2);

void ov05_0221DE38(UnkStruct_ov10_0221FB28 *arg0) {
    ov05_0221DDEC(arg0, arg0->unk_544[0], 1);
    ov05_0221DDEC(arg0, arg0->unk_544[1], 2);
    ScheduleBgTilemapBufferTransfer(arg0->unk_0C, 1U);
    ScheduleBgTilemapBufferTransfer(arg0->unk_0C, 2U);
}

void ov05_0221DE6C(UnkStruct_ov10_0221FB28 *param_1, u8 param_2) {
    if (param_1->trainerIntroData->playerSide == 0) {
        CopyToBgTilemapRect(param_1->unk_0C, 1, 0x20 - param_2, 0, param_2, 0x18, param_1->unk_544[0], 0, 0, 0x10, 0x18);
        CopyToBgTilemapRect(param_1->unk_0C, 2, 0x20 - param_2, 0, param_2, 0x18, param_1->unk_544[1], 0, 0, 0x10, 0x18);
    } else {
        CopyToBgTilemapRect(param_1->unk_0C, 1, 0, 0, param_2, 0x18, param_1->unk_544[0], 0x10 - param_2, 0, 0x10, 0x18);
        CopyToBgTilemapRect(param_1->unk_0C, 2, 0, 0, param_2, 0x18, param_1->unk_544[1], 0x10 - param_2, 0, 0x10, 0x18);
    }
    ScheduleBgTilemapBufferTransfer(param_1->unk_0C, 1);
    ScheduleBgTilemapBufferTransfer(param_1->unk_0C, 2);
    return;
}
