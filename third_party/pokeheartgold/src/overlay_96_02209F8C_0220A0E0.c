#include "global.h"

#include "error_handling.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_96_ov96_0220_data.h"

void ov96_0220A0E0(Ov96_0220Data *param_1);

void ov96_0220A0E0(Ov96_0220Data *param_1) {
    s32 iVar1;
    Ov96_0220DataLane *lane;
    s32 iVar4;

    iVar4 = 0;
    lane = param_1->lanes;
    do {
        if (lane->unk_4 == NULL) {
            GF_AssertFail();
        }
        Sprite_DeleteAndFreeResources(lane->unk_4);
        if (lane->unk_8 == NULL) {
            GF_AssertFail();
        }
        Sprite_DeleteAndFreeResources(lane->unk_8);
        iVar1 = 0;
        do {
            if (lane->unk_C[iVar1] == NULL) {
                GF_AssertFail();
            }
            Sprite_DeleteAndFreeResources(lane->unk_C[iVar1]);
            iVar1 = iVar1 + 1;
        } while (iVar1 < 0x14);
        iVar1 = 0;
        do {
            if (lane->unk_5C[iVar1] == NULL) {
                GF_AssertFail();
            }
            Sprite_DeleteAndFreeResources(lane->unk_5C[iVar1]);
            iVar1 = iVar1 + 1;
        } while (iVar1 < 5);
        iVar4 = iVar4 + 1;
        lane++;
    } while (iVar4 < 4);
    return;
}
