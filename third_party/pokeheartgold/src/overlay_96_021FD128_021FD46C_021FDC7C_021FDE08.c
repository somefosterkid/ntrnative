#include "global.h"

#include "error_handling.h"
#include "overlay_96_ov96_rower_work.h"
#include "overlay_96_sprite_res_man.h"

void ov96_021EB0A4(void *sprite, s32 x, s32 y, s32 *outX, s32 *outY);

u32 ov96_021EB0CC(Ov96SpriteSetElem *elem, fx32 param_2, fx32 param_3, s32 param_4, s32 param_5);

u32 ov96_021FDE08(Ov96RowerWork *param_1, s32 param_2, s32 param_3, s32 param_4);

u32 ov96_021FDE08(Ov96RowerWork *param_1, s32 param_2, s32 param_3, s32 param_4) {
    s32 iVar1;
    Ov96Rower *rower;
    Ov96SpriteSetElem *elem;
    s32 iStack_18;
    s32 iStack_1c;

    rower = &param_1->rowers[param_2];
    if ((2 + 1) <= rower->subIdx) {
        GF_AssertFail();
    }
    elem = rower->unk00[rower->subIdx];
    ov96_021EB0A4(elem, 0x50, rower->unk80 / 0x1000, &iStack_18, &iStack_1c);
    iVar1 = ov96_021EB0CC(elem, iStack_18 << 0xc, iStack_1c << 0xc, param_3, param_4);
    if (iVar1 != 0) {
        return 1;
    }
    return 0;
}
