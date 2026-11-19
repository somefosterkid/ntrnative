#include "global.h"

#include "overlay_40_unk_struct_0208_c06_c.h"
#include "sprite_system.h"
#include "unk_02013534.h"

void ov40_0222CFBC(UnkStruct_0208C06C *arg0, s32 arg1);

void ov40_0222CFBC(UnkStruct_0208C06C *arg0, s32 arg1) {
    s32 i;

    i = 0;
    do {
        if (arg0->unk_534.unk_00[i].unk_0C == 0) {
            TextOBJ_SetSpritesDrawFlag(arg0->unk_534.unk_00[i].unk_14, 0);
            ManagedSprite_SetDrawFlag(arg0->unk_534.unk_00[i].unk_00, 0);
        } else {
            TextOBJ_SetSpritesDrawFlag(arg0->unk_534.unk_00[i].unk_14, arg1);
            ManagedSprite_SetDrawFlag(arg0->unk_534.unk_00[i].unk_00, arg1);
        }
        if (arg0->unk_534.unk_C8[i].unk_0C == 0) {
            TextOBJ_SetSpritesDrawFlag(arg0->unk_534.unk_C8[i].unk_14, 0);
            ManagedSprite_SetDrawFlag(arg0->unk_534.unk_C8[i].unk_00, 0);
        } else {
            TextOBJ_SetSpritesDrawFlag(arg0->unk_534.unk_C8[i].unk_14, arg1);
            ManagedSprite_SetDrawFlag(arg0->unk_534.unk_C8[i].unk_00, arg1);
        }
        i += 1;
    } while (i < 5);
}
