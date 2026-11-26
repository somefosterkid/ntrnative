#include "global.h"

void *ov74_0223105C(void);

void *ov74_022311DC(void);

typedef struct {
    union {
        u8 val1[4];
        u32 val2;
    } unk_00;
} UnkStruct_ov97_022335A8;
typedef struct {
    union {
        u8 val1[2];
        u16 val2;
    } unk_00;
} UnkStruct_ov97_022335C8;
typedef struct {
    u32 unk_00_0 : 8;
    u32 unk_00_8 : 4;
    u32 unk_00_12 : 4;
    u32 unk_00_16 : 16;
} UnkStruct_ov97_02233674_sub1;
typedef struct {
    u32 unk_00_0 : 8;
    u32 unk_00_8 : 4;
    u32 unk_00_12 : 4;
    u32 unk_00_16 : 16;
} UnkStruct_ov97_02233674_sub2;
typedef struct {
    u32 unk_00_0 : 8;
    u32 unk_00_8 : 8;
    u32 unk_00_16 : 16;
} UnkStruct_ov97_02233674_sub3;
typedef struct {
    u32 unk_00_0 : 8;
    u32 unk_00_8 : 24;
} UnkStruct_ov97_02233674_sub4;
typedef struct {
    UnkStruct_ov97_02233674_sub1 unk_00;
    UnkStruct_ov97_02233674_sub2 unk_04;
    UnkStruct_ov97_02233674_sub3 unk_08;
    UnkStruct_ov97_02233674_sub4 unk_0C;
} UnkStruct_ov97_02233674;
UnkStruct_ov97_022335A8 ov74_022311F4(u8 *param0);
UnkStruct_ov97_022335C8 ov74_02231214(u8 *param0);
void ov74_02231238(u32 param0, u32 *param1, u32 *param2);
u32 ov74_02231260(void);
u32 ov74_02231264(void);
u32 ov74_0223127C(u32 param0, u32 param1, void *param2);

void *ov74_022311DC(void) {
    return (u8 *)ov74_0223105C() + 0x40;
}

u8 *ov74_022311E8(void);

u8 *ov74_022311E8(void) {
    return (u8 *)ov74_0223105C() + 0x78;
}

UnkStruct_ov97_022335A8 ov74_022311F4(u8 *param0) {
    u32 v0;
    UnkStruct_ov97_022335A8 v1;

    v1.unk_00.val2 = 0;

    for (v0 = 0; v0 < 4; v0++) {
        v1.unk_00.val1[v0] = param0[v0 + 2];
    }

    return v1;
}

UnkStruct_ov97_022335C8 ov74_02231214(u8 *param0) {
    u32 v0;
    UnkStruct_ov97_022335C8 v1;

    v1.unk_00.val2 = 0;

    for (v0 = 0; v0 < 2; v0++) {
        v1.unk_00.val1[v0] = param0[v0];
    }

    return v1;
}

void ov74_02231238(u32 param0, u32 *param1, u32 *param2) {
    *param1 = param0 % 10000;
    *param2 = (param0 / 10000) % 10000;
}

u32 ov74_02231260(void) {
    return sizeof(UnkStruct_ov97_02233674);
}

u32 ov74_02231264(void) {
    return ((u32)(OS_GetTick() + GX_GetVCount()) & 0xFFFFFFFE) + 1;
}

u32 ov74_0223127C(u32 param0, u32 param1, void *param2) {
    u32 v0;
    u32 *v1 = (u32 *)param2;
    MATHRandContext16 v2;

    MATH_InitRand16(&v2, param0);

    for (v0 = 0; v0 < param1 / sizeof(u32); v0++) {
        (*v1) ^= ((MATH_Rand16(&v2, 0) << 16) | MATH_Rand16(&v2, 0));
        v1++;
    }

    return v2.x;
}
