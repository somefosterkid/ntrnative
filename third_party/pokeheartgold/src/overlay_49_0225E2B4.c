#include "global.h"

#include "heap.h"
#include "overlay_49_unk_struct_ov70_02261_e10_t.h"
#include "overlay_49_unk_struct_ov70_022629_fc.h"

typedef struct UnkStruct_ov49_0225E318 {
    u8 unk_0[0x4];
    void *unk_4;
    u8 unk_8[0x240];
    u8 unk_248;
    u8 unk_249[0x11F];
    u8 unk_368;
    u8 unk_369[0x17F];
    u8 unk_4E8;
    u8 unk_4E9[0x11F];
    u8 unk_608;
    u8 unk_609;
    u8 unk_60A[0x1];
    u8 unk_60B;
} UnkStruct_ov49_0225E318;

struct UnkStruct_ov70_0225C894_t;

struct UnkStruct_ov70_02260BB8_t;

struct UnkStruct_ov70_02260E8C_t;

struct UnkStruct_ov70_022610B8_t;

struct UnkStruct_ov70_02261E10_t;

typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;

typedef struct UnkStruct_ov70_02261E10_t UnkStruct_ov70_02261E10;

typedef struct UnkStruct_ov70_02260BB8_t UnkStruct_ov70_02260BB8;

typedef struct UnkStruct_ov70_022610B8_t UnkStruct_ov70_022610B8;

typedef struct UnkStruct_ov70_02260E8C_t UnkStruct_ov70_02260E8C;

void ov49_0225CCC0(UnkStruct_ov70_02260BB8 *param0);

void ov49_0225CE88(UnkStruct_ov70_02260BB8 *param0);

void ov49_0225CF94(UnkStruct_ov70_02260E8C *param0);

void ov49_0225D160(UnkStruct_ov70_02260BB8 *param0, UnkStruct_ov70_022610B8 *param1);

void ov49_0225E2B4(UnkStruct_ov70_02261E10 *param0);

s32 ov49_0225CCF0(UnkStruct_ov70_02260BB8 *);
s32 ov49_0225EB08(void *, UnkStruct_ov70_02260BB8 *);
s32 ov49_0225ECF0(void *, UnkStruct_ov70_02260BB8 *);
s32 ov49_0225ED98(void *, UnkStruct_ov70_02260BB8 *);
void ov49_0225E318(UnkStruct_ov49_0225E318 *arg0);
s32 ov49_0225CD58(UnkStruct_ov70_02260BB8 *);
void ov49_0225E3A0(UnkStruct_ov70_02261E10 *arg0);
s32 ov49_0225CDE8(s32);
void ov49_0225E3AC(void *arg0);

void ov49_0225E2B4(UnkStruct_ov70_02261E10 *param0) {
    {
        int v0;

        for (v0 = 0; v0 < param0->unk_613; v0++) {
            ov49_0225CF94(param0->unk_208[v0]);
        }
    }

    {
        int v1;

        for (v1 = 0; v1 < param0->unk_612; v1++) {
            ov49_0225D160(param0->unk_04, param0->unk_08[v1]);
        }
    }

    ov49_0225CE88(param0->unk_04);
    ov49_0225CCC0(param0->unk_04);

    Heap_Free(param0);
}

void ov49_0225E318(UnkStruct_ov49_0225E318 *arg0) {
    s32 v3;
    s32 v4;
    s32 v5;
    void *v0;
    void *v1;
    void *v2;

    v3 = 0;
    if ((s32) arg0->unk_608 > 0) {
        v0 = (void *)&arg0->unk_248;
        do {
            ov49_0225EB08(v0, arg0->unk_4);
            v3 += 1;
            v0 += 0xC;
        } while (v3 < (s32) arg0->unk_608);
    }
    v4 = 0;
    if ((s32) arg0->unk_609 > 0) {
        v1 = (void *)&arg0->unk_368;
        do {
            ov49_0225ECF0(v1, arg0->unk_4);
            v4 += 1;
            v1 += 0xC;
        } while (v4 < (s32) arg0->unk_609);
    }
    v5 = 0;
    if ((s32) arg0->unk_60B > 0) {
        v2 = (void *)&arg0->unk_4E8;
        do {
            ov49_0225ED98(v2, arg0->unk_4);
            v5 += 1;
            v2 += 0xC;
        } while (v5 < (s32) arg0->unk_60B);
    }
    ov49_0225CCF0(arg0->unk_4);
}

void ov49_0225E3A0(UnkStruct_ov70_02261E10 *arg0) {
    ov49_0225CD58(arg0->unk_04);
}

void ov49_0225E3AC(void *arg0) {
    ov49_0225CDE8(*(s32 *)((u8 *)arg0 + 4));
}
