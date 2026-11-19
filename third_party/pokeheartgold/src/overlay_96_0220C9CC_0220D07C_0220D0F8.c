#include "global.h"

typedef struct UnkStruct_ov96_0220D0F8_Sub {
    u32 unk0_0 : 18;
    u32 unk0_18 : 8;
    u32 unk0_26 : 6;
} UnkStruct_ov96_0220D0F8_Sub;

typedef struct UnkStruct_ov96_0220D0F8 {
    u32 unk0;
    UnkStruct_ov96_0220D0F8_Sub sub[4];
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
} UnkStruct_ov96_0220D0F8;

void ov96_0220D0F8(UnkStruct_ov96_0220D0F8 *p);

void ov96_0220D0F8(UnkStruct_ov96_0220D0F8 *p) {
    UnkStruct_ov96_0220D0F8_Sub *s = &p->sub[p->unk14_30];
    p->unk1C_9 = 0;
    p->unk18_11 = 0;
    p->unk14_16 = 0;
    s->unk0_18 = 10;
}
