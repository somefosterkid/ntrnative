#include "global.h"

#include "assert.h"

struct UnkStruct_ov45_0222AB1C;

struct UnkStruct_ov45_0222AB94;

typedef struct UnkStruct_ov45_0222AB1C UnkStruct_ov45_0222AB1C;

typedef struct UnkStruct_ov45_0222AB94 {
    u32 unk0[4];
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13_0 : 7;
    u8 unk13_7 : 1;
} UnkStruct_ov45_0222AB94;

extern u32 ov45_0222EC90(u32 a0);

extern void ov45_0222EEF0(int cmd, void *data, int size);

void ov45_0222ACB8(UnkStruct_ov45_0222AB1C *a0, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6);

void ov45_0222ACB8(UnkStruct_ov45_0222AB1C *a0, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6) {
    UnkStruct_ov45_0222AB94 v0 = { 0 };
    u32 v1;

    switch (a1) {
    case 0:
    case 1:
    case 2:
        v1 = 8;
        break;
    default:
        return;
    }

    v0.unk11 = v1;
    v0.unk0[0] = ov45_0222EC90(a3);
    v0.unk0[1] = ov45_0222EC90(a4);
    v0.unk0[2] = ov45_0222EC90(a5);
    v0.unk0[3] = ov45_0222EC90(a6);
    v0.unk10 = a2;
    v0.unk13_0 = a1;
    v0.unk13_7 = 0;

    ov45_0222EEF0(4, &v0, sizeof(UnkStruct_ov45_0222AB94));
}
