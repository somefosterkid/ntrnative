#include "global.h"

typedef struct UnkStruct_ov96_0220D07C_Sub {
    u32 unk0_0 : 18;
    u32 unk0_18 : 8;
    u32 unk0_26 : 6;
} UnkStruct_ov96_0220D07C_Sub;

typedef struct UnkStruct_ov96_0220D07C {
    u32 unk0;
    UnkStruct_ov96_0220D07C_Sub sub[4];
    u32 unk14_0 : 16;
    u32 unk14_16 : 8;
    u32 unk14_24 : 6;
    u32 unk14_30 : 2;
    u32 unk18_0 : 11;
    u32 unk18_11 : 11;
    u32 unk18_22 : 10;
    u32 unk1C_0 : 9;
    u32 unk1C_9 : 1;
    u32 unk1C_10 : 22;
} UnkStruct_ov96_0220D07C;

void ov96_0220D0F8(UnkStruct_ov96_0220D07C *p);

void ov96_0220D07C(UnkStruct_ov96_0220D07C *p);

void ov96_0220D07C(UnkStruct_ov96_0220D07C *p) {
    if (p->unk1C_9) {
        p->unk18_11++;
        if (p->unk18_11 >= 90) {
            ov96_0220D0F8(p);
        }
    } else if (p->unk14_16 != 0) {
        p->unk18_11++;
        if (p->unk18_11 >= 2) {
            p->unk18_11 = 0;
            p->unk14_16--;
        }
    }
}
