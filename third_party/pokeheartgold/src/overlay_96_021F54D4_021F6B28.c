#include "global.h"
#include "overlay_96_unk_struct_ov96_021_f5018.h"

void ov96_021F6B28(UnkStruct_ov96_021F5018_Ball *arg0, UnkStruct_ov96_021F5018_Row *arg1);

void ov96_021F6B28(UnkStruct_ov96_021F5018_Ball *arg0, UnkStruct_ov96_021F5018_Row *arg1) {
    s32 v0;

    if (arg1->unk_08 > 0) {
        v0 = arg0->unk_08 + arg0->unk_18;
        arg0->unk_1C = v0;
        if (v0 < 0x20000) {
            arg0->unk_1C = 0x20000;
            return;
        }
        if (v0 > 0xDF000) {
            arg0->unk_1C = 0xDF000;
        }
    }
}
