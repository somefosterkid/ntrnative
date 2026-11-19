#include "global.h"

#include "error_handling.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_96_ov96_0220_data.h"

void ov96_022099EC(Ov96_0220Data *param_1);

void ov96_022099EC(Ov96_0220Data *param_1) {
    s32 iVar1;

    if (param_1 == 0) {
        GF_AssertFail();
    }
    iVar1 = 0;
    do {
        if (param_1->unk_24[iVar1] == NULL) {
            GF_AssertFail();
        }
        Sprite_DeleteAndFreeResources(param_1->unk_24[iVar1]);
        iVar1 = iVar1 + 1;
    } while (iVar1 < 2);
    return;
}
