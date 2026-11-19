#include "global.h"

typedef struct {
    u32 unk_00_0 : 8;
    u32 unk_00_8 : 4;
    u32 unk_00_12 : 4;
    u32 unk_00_16 : 16;
} UnkStruct_ov97_02233268_sub1_sub1;

typedef struct {
    u32 unk_00_0 : 8;
    u32 unk_00_8 : 4;
    u32 unk_00_12 : 4;
    u32 unk_00_16 : 16;
} UnkStruct_ov97_02233268_sub1_sub2;

typedef struct {
    UnkStruct_ov97_02233268_sub1_sub1 unk_00;
    UnkStruct_ov97_02233268_sub1_sub2 unk_04;
} UnkStruct_ov97_02233268_sub1;

typedef struct {
    u16 unk_00[12];
} UnkStruct_ov97_02233268_sub2_sub1;

typedef struct {
    UnkStruct_ov97_02233268_sub2_sub1 unk_00;
    UnkStruct_ov97_02233268_sub2_sub1 unk_18;
} UnkStruct_ov97_02233268_sub2;

typedef struct {
    UnkStruct_ov97_02233268_sub1 unk_00;
    UnkStruct_ov97_02233268_sub2 unk_08;
} UnkStruct_ov97_02233268;

UnkStruct_ov97_02233268 *ov74_022311DC(void);

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

u32 ov74_0223127C(u32 param0, u32 param1, void *param2);

void ov74_022313F0(void *param0);

BOOL ov74_02231424(void *param0);

void ov74_022313F0(void *param0) {
    UnkStruct_ov97_02233674 *v0 = (UnkStruct_ov97_02233674 *)param0;
    UnkStruct_ov97_02233268 *v1 = ov74_022311DC();

    if ((int)v0->unk_00.unk_00_12 == 1) {
        u32 v2;

        v2 = ov74_0223127C(v0->unk_00.unk_00_16, sizeof(UnkStruct_ov97_02233674_sub2) + sizeof(UnkStruct_ov97_02233674_sub3) + sizeof(UnkStruct_ov97_02233674_sub4), &v0->unk_04);
        ov74_0223127C(v2, v0->unk_0C.unk_00_0 + v0->unk_0C.unk_00_8 - sizeof(UnkStruct_ov97_02233674), (void *)((u8 *)param0 + sizeof(UnkStruct_ov97_02233674)));
    }
}

BOOL ov74_02231424(void *param0) {
    UnkStruct_ov97_02233674 *v0 = (UnkStruct_ov97_02233674 *)param0;
    u16 v1 = SVC_GetCRC16(0, ((u8 *)param0) + v0->unk_0C.unk_00_0, v0->unk_0C.unk_00_8);

    if (v0->unk_08.unk_00_16 == v1) {
        return 1;
    } else {
        return 0;
    }
}
