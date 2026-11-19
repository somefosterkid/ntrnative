#include "global.h"

#include "overlay_96_unk_struct_ov96_0220e3f0.h"

void ov96_0220FBDC(UnkStruct_ov96_0220E3F0_Record *arg0);

void ov96_0220FBDC(UnkStruct_ov96_0220E3F0_Record *arg0) {
    UnkStruct_ov96_0220E3F0_Record *new_var;
    unsigned int new_var2;
    new_var = arg0;
    new_var2 = ~0x1C;
    new_var->unk_4 = (s32)((arg0->unk_4 & (~0x20)) & new_var2);
}
