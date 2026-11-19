#include "global.h"

#include "sprite.h"
#include "unk_020932E0.h"

s32 sub_02094860(UnkStruct_020932E0 *arg0, s32 arg1) {
    s32 v0;

    v0 = sub_02094894(arg0, arg1);
    if (arg1 < (s32)arg0->unk_D) {
        Sprite_SetMatrix(arg0->unk_8C0, Sprite_GetMatrixPtr(arg0->unk_7E8[arg1].unk_0));
        return v0;
    } else {
        return v0;
    }
}
