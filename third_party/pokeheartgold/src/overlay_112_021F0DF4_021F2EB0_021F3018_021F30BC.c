#include "global.h"

typedef struct UnkStruct_ov112_021F30BC_Src {
    void *data;
    u32 unk4;
    u16 unk8;
    u8 unkA_0 : 5;
    u8 unkA_5 : 2;
    u8 unkA_7 : 1;
} UnkStruct_ov112_021F30BC_Src;

typedef struct UnkStruct_ov112_021F30BC_Dst {
    s32 unk0;
    u8 unk4[6];
    u16 unkA;
    u8 unkC[0x14];
    u8 unk20[0x5C];
    u32 unk7C;
    u8 unk80[4];
    u8 unk84;
    u8 unk85_0 : 5;
    u8 unk85_5 : 2;
    u8 unk85_7 : 1;
    u8 unk86[2];
} UnkStruct_ov112_021F30BC_Dst;

void ov112_021F30BC(UnkStruct_ov112_021F30BC_Dst *dst, UnkStruct_ov112_021F30BC_Src *src);

void ov112_021F30BC(UnkStruct_ov112_021F30BC_Dst *dst, UnkStruct_ov112_021F30BC_Src *src) {
    MI_CpuFill8(dst, 0, sizeof(UnkStruct_ov112_021F30BC_Dst));
    MI_CpuCopy8(src->data, dst->unk20, 0x16);
    dst->unkA = src->unk8;
    dst->unk85_0 = src->unkA_0;
    dst->unk85_5 = src->unkA_5;
    dst->unk85_7 = src->unkA_7;
    dst->unk7C = src->unk4;
    dst->unk84 = 0x1a;
    dst->unk0 = -1;
}
