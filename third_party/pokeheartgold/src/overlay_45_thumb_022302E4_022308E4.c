#include "global.h"
#include "billboard.h"
#include "overlay_45_unk_struct_ov66_02234798.h"

s32 sub_020182A8(void *, s32, s32, s32);

s32 sub_02023E50(Billboard *billboard, const VecFx32 *pos);

void ov45_022308E4(UnkStruct_ov66_02234798 *param0, const VecFx32 *param1);

void ov45_022308E4(UnkStruct_ov66_02234798 *param0, const VecFx32 *param1) {
    sub_02023E50(param0->unk_08, param1);
    sub_020182A8(&param0->unk_0C, param1->x, 0x2000, param1->z - 0x8000);
}
