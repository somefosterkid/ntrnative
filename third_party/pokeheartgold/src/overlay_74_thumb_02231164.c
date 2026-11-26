#include "global.h"

void *ov74_02231154(void);

void ov74_02231164(void);

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
u32 ov74_022311D8(void);

void ov74_02231164(void) {
    void *dest;

    dest = ov74_02231154();
    MI_CpuFill8(dest, 0U, 0x1C4U);
    *(s16 *)((u8 *)dest + 0x1C0) = 0x118;
}

s32 *ov74_0223105C(void);

u8 *ov74_02231184(void);

u8 *ov74_02231184(void) {
    return (u8 *)ov74_0223105C() + 0x254;
}

void ov74_02231194(void);

void ov74_02231194(void) {
    *(s8 *)(ov74_02231184() + 0x18) = 0;
}

s32 ov74_022311A0(void);

s32 ov74_022311A0(void) {
    return *(s32 *)((u8 *)ov74_0223105C() + 4);
}

u8 ov74_022311AC(void);

u8 ov74_022311AC(void) {
    return (u8) * (s32 *)((u8 *)ov74_0223105C() + 0x40);
}

u32 ov74_022311BC(void);

u32 ov74_022311BC(void) {
    return (u32)(*(s32 *)((u8 *)ov74_0223105C() + 0x40) << 0x14) >> 0x1C;
}

u32 ov74_022311CC(void);

u32 ov74_022311CC(void) {
    return (u32) * (u32 *)((u8 *)ov74_0223105C() + 0x44) >> 0x10;
}

u32 ov74_022311D8(void) {
    return sizeof(UnkStruct_ov97_02233268);
}
