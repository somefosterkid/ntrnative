#include "global.h"

#include "overlay_01_021EFB38.h"
#include "overlay_115.h"
#include "sprite.h"

s32 ov01_021EFE80(void *);

u32 ov115_0225F0B4(UnkStruct_ov5_021E5128 *eff) {
    s32 i;
    s32 ret = 1;
    s32 done;
    VecFx32 scale;
    if (eff->unk_02 < 4) {
        ret = 0;
        eff->unk_00--;
        if (eff->unk_00 <= ret) {
            eff->unk_00 = 3;
            eff->unk_02++;
        }
    }
    for (i = 0; i < eff->unk_02; i++) {
        done = ov01_021EFE80(&eff->unk_14[i]);
        scale = ov01_021F074C(eff->unk_14[i].currentValue, eff->unk_14[i].currentValue, eff->unk_14[i].currentValue);
        Sprite_SetAffineScale(eff->unk_04[i], &scale);
        Sprite_SetDrawFlag(eff->unk_04[i], 1);
        if (done == 0) {
            ret = 0;
        } else if (i != 3) {
            Sprite_SetDrawFlag(eff->unk_04[i], 0);
        }
    }
    return ret;
}
