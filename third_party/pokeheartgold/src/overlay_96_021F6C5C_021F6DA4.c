#include "global.h"
#include "overlay_96_unk_struct_ov96_021_f5018.h"
#include "overlay_96_unk_struct_ov96_021_f74a4.h"

s32 ov96_021EB588(void *, s32 *);

void ov96_021F7544(UnkStruct_ov96_021F74A4 *, u8);

extern VecFx32 ov96_0221C0C4;

extern VecFx32 ov96_0221C0D0;

extern VecFx32 ov96_0221C0DC;

void ov96_021F6DA4(UnkStruct_ov96_021F5018 *arg0, u8 arg1);

void ov96_021F6DA4(UnkStruct_ov96_021F5018 *arg0, u8 arg1) {
    VecFx32 sp28;
    s32 i;
    UnkStruct_ov96_021F74A4 **new_var;
    VecFx32 sp1C;
    VecFx32 sp10;
    VecFx32 sp4;
    u8 cnt;
    new_var = &arg0->unk_8C;
    ov96_021F7544(*new_var, arg1);
    sp28 = ov96_0221C0C4;
    ov96_021EB588(arg0->unk_64, (s32 *)(&sp28));
    sp1C = ov96_0221C0DC;
    ov96_021EB588(arg0->unk_68[arg1], (s32 *)(&sp1C));
    sp4 = ov96_0221C0D0;
    i = (cnt = 0);
        do {
        sp10 = sp4;
        if (i != arg1) {
            sp10.y += cnt << 16;
            ov96_021EB588(arg0->unk_68[i], (s32 *)(&sp10));
            cnt += 1;
        }
        i += 1;
    } while (i < 4);
}
