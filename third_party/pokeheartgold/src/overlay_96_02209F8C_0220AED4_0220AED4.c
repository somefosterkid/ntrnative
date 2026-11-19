#include "global.h"

#include "error_handling.h"
#include "sprite_system.h"
#include "unk_struct_ov96_0220b148.h"

s32 ov96_0220AFF8(UnkStruct_ov96_0220B148 *);

s32 ov96_0220B068(UnkStruct_ov96_0220B148 *);

void ov96_0220AED4(UnkStruct_ov96_0220B148 *arg0);

void ov96_0220AED4(UnkStruct_ov96_0220B148 *arg0) {
    int new_var;
    int new_var2;
    if (arg0 == 0L) {
        GF_AssertFail();
    }
    arg0->unk_8 = 0x64;
    new_var2 = 0x78;
    new_var = 0xFFFFF00F;
    new_var = (((arg0->unk_10) & new_var) & 0xFFF00FFF) | 0x1000;
    arg0->unk_10 = (s32)(0x08000000 | new_var);
    ManagedSprite_SetAnim(arg0->unk_0, 1);
    ManagedSprite_SetPositionXY(arg0->unk_0, new_var2, 0);
    ManagedSprite_SetDrawFlag(arg0->unk_0, 1);
    ov96_0220AFF8(arg0);
    ov96_0220B068(arg0);
    ManagedSprite_SetDrawFlag(arg0->unk_4, 1);
}
