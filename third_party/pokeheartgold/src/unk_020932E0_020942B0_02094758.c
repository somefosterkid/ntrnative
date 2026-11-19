#include "global.h"

#include "sprite.h"
#include "unk_020932E0.h"

void sub_02094758(UnkStruct_020932E0_Slot20C *arg0) {
    s32 v0;
    UnkStruct_020932E0_Slot20C *v1;

    v1 = arg0;
    v0 = 0;
    do {
        if (v1->unk_8 != NULL) {
            DC_FlushRange(v1->unk_C, 0x200U);
            GX_LoadOBJ(v1->unk_C, v1->unk_0, 0x200U);
            Sprite_SetDrawFlag(v1->unk_8, 1);
        }
        v0 += 1;
        v1++;
    } while (v0 < 0x1E);
}

s32 sub_02094794(UnkStruct_020932E0 *arg0, s32 arg1) {
    Sprite_SetMatrix(arg0->unk_8C0, Sprite_GetMatrixPtr(arg0->unk_198[arg1].unk_0));
    return sub_020947C0(arg0, arg1);
}
