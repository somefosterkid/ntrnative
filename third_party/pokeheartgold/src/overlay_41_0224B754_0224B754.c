#include "global.h"

#include "filesystem.h"
#include "overlay_41_unk_struct_ov22_0225b1bc.h"

s32 ov41_0224626C(UnkStruct_ov22_0225A0E4_Head *param0);

s32 ov41_02246594(UnkStruct_ov22_0225A0E4_Head *param0);

s32 ov41_022465C0(UnkStruct_ov22_0225A0E4_Head *param0);

void ov41_0224B754(UnkStruct_ov22_0225B1BC *arg0);

void ov41_0224B754(UnkStruct_ov22_0225B1BC *arg0) {
    ov41_0224626C(&arg0->unk_00);
    ov41_02246594(&arg0->unk_00);
    if (arg0->unk_260 != 0) {
        ov41_022465C0(&arg0->unk_00);
    }
    NARC_Delete(arg0->unk_00.unk_5C);
}
