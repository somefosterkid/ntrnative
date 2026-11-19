#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "overworld_anim_manager.h"
#include "sys_task.h"
#include "error_handling.h"
#include "heap.h"
#include "overlay_49.h"

struct MsgData;

struct MessageFormat;

struct UnkStruct_ov101_021D13C8_t;

struct UnkStruct_ov101_021D7E48_t;

typedef struct UnkStruct_ov101_021D13C8_t UnkStruct_ov101_021D13C8;

typedef enum {
    UnkEnum_ov101_021D9688_00 = 0,
    UnkEnum_ov101_021D9688_01,
    UnkEnum_ov101_021D9688_02,
    UnkEnum_ov101_021D9688_03,
    UnkEnum_ov101_021D9688_04,
    UnkEnum_ov101_021D9688_05,
    UnkEnum_ov101_021D9688_06,
} UnkEnum_ov101_021D9688;

typedef enum {
    UnkEnum_ov101_021D2BDC_00,
    UnkEnum_ov101_021D2BDC_01,
    UnkEnum_ov101_021D2BDC_02,
    UnkEnum_ov101_021D2BDC_03,
} UnkEnum_ov101_021D2BDC;

typedef enum {
    UnkEnum_ov101_021D4F58_00 = 0,
    UnkEnum_ov101_021D4F58_01,
    UnkEnum_ov101_021D4F58_02,
    UnkEnum_ov101_021D4F58_03,
    UnkEnum_ov101_021D4F58_04,
    UnkEnum_ov101_021D4F58_05,
} UnkEnum_ov101_021D4F58;

typedef enum {
    UnkEnum_ov101_021D394C_00 = 0,
    UnkEnum_ov101_021D394C_01,
    UnkEnum_ov101_021D394C_02,
} UnkEnum_ov101_021D394C;

typedef enum {
    UnkEnum_ov101_021D5814_00 = 0,
    UnkEnum_ov101_021D5814_01,
    UnkEnum_ov101_021D5814_02,
    UnkEnum_ov101_021D5814_03,
    UnkEnum_ov101_021D5814_04,
    UnkEnum_ov101_021D5814_05,
} UnkEnum_ov101_021D5814;

typedef enum {
    UnkEnum_ov101_021D6764_00 = 0,
    UnkEnum_ov101_021D6764_01,
    UnkEnum_ov101_021D6764_02,
    UnkEnum_ov101_021D6764_03,
} UnkEnum_ov101_021D6764;

typedef enum {
    UnkEnum_ov101_021D9934_00 = 0,
    UnkEnum_ov101_021D9934_01,
    UnkEnum_ov101_021D9934_02,
    UnkEnum_ov101_021D9934_03,
    UnkEnum_ov101_021D9934_04,
    UnkEnum_ov101_021D9934_05,
    UnkEnum_ov101_021D9934_06,
} UnkEnum_ov101_021D9934;

typedef enum {
    UnkEnum_ov101_021D9934_1_00 = 0,
    UnkEnum_ov101_021D9934_1_01,
    UnkEnum_ov101_021D9934_1_02,
    UnkEnum_ov101_021D9934_1_03,
} UnkEnum_ov101_021D9934_1;

typedef enum {
    UnkEnum_ov101_021D1894_00,
    UnkEnum_ov101_021D1894_01,
    UnkEnum_ov101_021D1894_02,
    UnkEnum_ov101_021D1894_03,
} UnkEnum_ov101_021D1894;

typedef struct {
    MsgData *unk_00;
    MessageFormat *unk_04;
    Window unk_08[1];
    void *unk_18;
} UnkStruct_ov101_021D148C;

typedef struct {
    u32 unk_00;
    int unk_04;
    UnkEnum_ov101_021D1894 unk_08;
    UnkEnum_ov101_021D1894 unk_0C;
    SysTask *unk_10;
} UnkStruct_ov101_021D1894;

typedef struct {
    u32 unk_00;
    int unk_04;
    fx32 unk_08;
    int unk_0C;
    UnkEnum_ov101_021D2BDC unk_10;
    int unk_14;
    int unk_18;
    SysTask *unk_1C;
    UnkStruct_ov101_021D13C8 *unk_20;
} UnkStruct_ov101_021D2D88;

typedef struct {
    u32 unk_00;
    u32 unk_04;
    int unk_08;
    UnkStruct_ov101_021D13C8 *unk_0C;
    SysTask *unk_10;
    OverworldAnimManager *unk_14;
    OverworldAnimManager *unk_18;
} UnkStruct_ov101_021D4714;

typedef struct {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
    u32 unk_0C;
    int unk_10;
    int unk_14;
    UnkEnum_ov101_021D5814 unk_18;
    UnkEnum_ov101_021D6764 unk_1C;
    UnkStruct_ov101_021D13C8 *unk_20;
    SysTask *unk_24;
    OverworldAnimManager *unk_28;
    OverworldAnimManager *unk_2C;
    OverworldAnimManager *unk_30;
    OverworldAnimManager *unk_34;
    OverworldAnimManager *unk_38;
    OverworldAnimManager *unk_3C;
    OverworldAnimManager *unk_40;
    OverworldAnimManager *unk_44;
} UnkStruct_ov101_021D4764;

typedef struct {
    UnkStruct_ov101_021D13C8 *unk_00;
    u16 unk_04[16];
    u16 unk_24[16];
    u16 unk_44[16];
    u16 unk_64[16];
    u16 unk_84[16];
    u16 unk_A4[5][16];
    u16 unk_144[5][16];
    u16 unk_1E4[5][16];
} UnkStruct_ov101_021D4F58;

typedef struct UnkStruct_ov101_021D7E48_t UnkStruct_ov101_021D7E48;

struct UnkStruct_ov101_021D13C8_t {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
    u32 unk_0C;
    u32 unk_10;
    u32 unk_14;
    u32 unk_18;
    u32 unk_1C;
    u32 unk_20;
    u32 unk_24;
    u32 unk_28;
    u32 unk_2C;
    u32 unk_30;
    u32 unk_34;
    u32 unk_38;
    u32 unk_3C;
    u32 unk_40;
    UnkEnum_ov101_021D9688 unk_44;
    u32 unk_48;
    UnkEnum_ov101_021D9934_1 unk_4C;
    UnkEnum_ov101_021D9934 unk_50;
    u32 unk_54[3];
    int unk_60;
    int unk_64;
    int unk_68;
    fx32 unk_6C;
    int unk_70;
    u32 unk_74;
    u32 unk_78;
    UnkEnum_ov101_021D394C unk_7C[3];
    int unk_88;
    fx32 unk_8C;
    fx32 unk_90[3];
    fx32 unk_9C[3];
    int unk_A8[3];
    UnkStruct_ov101_021D2D88 unk_B4[3];
    UnkStruct_ov101_021D4714 unk_120;
    UnkStruct_ov101_021D4764 unk_13C;
    UnkStruct_ov101_021D4F58 unk_184;
    UnkStruct_ov101_021D148C unk_408;
    UnkStruct_ov101_021D1894 unk_424;
    NARC *unk_438;
    BgConfig *unk_43C;
    NNSG2dScreenData *unk_440;
    NNSG2dCharacterData *unk_444;
    NNSG2dPaletteData *unk_448;
    OverworldAnimManagerList *unk_44C;
    UnkStruct_ov101_021D7E48 *unk_450;
    OverworldAnimManager *unk_454;
    u32 unk_458;
    u32 unk_45C;
    u32 unk_460;
    u32 unk_464;
    u32 unk_468;
    u32 unk_46C;
    u32 unk_470;
    u32 unk_474;
    u32 unk_478;
    u32 unk_47C;
    u32 unk_480;
    u32 unk_484;
    u32 unk_488;
    u32 unk_48C;
    u32 unk_490;
    u32 unk_494;
    u32 unk_498;
    u32 unk_49C;
    u32 unk_4A0;
    u32 unk_4A4;
    u32 unk_4A8;
    u32 unk_4AC;
    u32 unk_4B0;
    u32 unk_4B4;
    u32 unk_4B8;
    u32 unk_4BC;
    u32 unk_4C0;
    int msgBoxFrame;
};

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    fx32 unk_0C;
    UnkEnum_ov101_021D4F58 unk_10;
    fx32 unk_14[5];
    UnkStruct_ov101_021D13C8 *unk_28;
    UnkStruct_ov101_021D4F58 *unk_2C;
} UnkStruct_ov101_021D5388;

void ov49_0225A134(UnkStruct_ov101_021D5388 *param0);

void ov49_02265260(OverworldAnimManager *param0, void *param1);

int ov45_0222B034(void *);
s32 ov45_0222AED8(void *, u16);
s32 ov49_02265698(void *, NARC *, enum HeapID);
s32 ov49_02265738(void *, NARC *, enum HeapID);
s32 ov49_022657B4(void *, NARC *, enum HeapID);
void *ov49_022652E8(void *arg0, void *arg1, void *arg2, void *arg3, enum HeapID arg4, enum HeapID arg5);
s32 ov49_0226571C(void *arg0);
s32 ov49_02265760(void *);
s32 ov49_02265858(void *);
void ov49_0226535C(void *arg0);
s32 ov49_022658E4(void *, void *);
void ov49_02265378(u8 *arg0);
void *ov49_02258DAC(void *);
s32 ov49_02265920(void *, void *);
void ov49_02265398(void *arg0);
void *ov49_02258D70(void *, u32);
s32 ov49_02265890(void *, void *, void *, s32);
void ov49_022653C0(void *arg0, u32 arg1, s32 arg2);
s32 ov49_02265958(void *);
s32 ov49_022653F0(u8 *arg0, s32 arg1);
void ov49_0226540C(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);

void ov49_02265260(OverworldAnimManager *param0, void *param1) {
    UnkStruct_ov101_021D5388 *v0 = param1;
    ov49_0225A134(v0);
}

void ov49_0226526C(s16 *arg0) {
    *arg0 = 0;
}

void ov49_02265274(void *arg0, void *arg1, s16 arg2, s16 arg3) {
    if (*(u16 *)arg0 != 0) {
        GF_AssertFail();
    }
    *(u16 *)arg0 = 1U;
    *(s16 *)((u8 *)arg0 + 2) = arg2;
    *(s16 *)((u8 *)arg0 + 4) = arg3;
    *(s16 *)((u8 *)arg0 + 6) = ov45_0222B034(arg1);
}

void ov49_0226529C(void *arg0, void *arg1) {
    s32 v0;

    if (*(u16 *)arg0 != 0) {
        v0 = ov45_0222B034(arg1);
        *(s32 *)((u8 *)arg0 + 8) = v0;
        if (v0 == *(u16 *)((u8 *)arg0 + 4)) {
            ov45_0222AED8(arg1, *(u16 *)((u8 *)arg0 + 2));
            *(u16 *)arg0 = 0U;
            return;
        }
        if (v0 != *(u16 *)((u8 *)arg0 + 6)) {
            *(u16 *)arg0 = 0U;
        }
    }
}

s32 ov49_022652D0(u16 *arg0) {
    if (*arg0 != 1) {
        return 1;
    }
    return 0;
}

u16 ov49_022652E0(void *arg0) {
    return (u16) * (s32 *)((u8 *)arg0 + 8);
}

void *ov49_022652E8(void *arg0, void *arg1, void *arg2, void *arg3, enum HeapID heapID, enum HeapID arg5) {
    void *v1;
    void *v0;
    NARC *narc;
    void *v2;

    v0 = arg2;
    v1 = arg3;
    v2 = Heap_Alloc(heapID, 0x1082CU);
    memset(v2, 0, 0x1082CU);
    *(void **)v2 = arg0;
    *(void **)((u8 *)v2 + 4) = v1;
    *(void **)((u8 *)v2 + 8) = v0;
    *(void **)((u8 *)v2 + 0xC) = arg1;
    narc = NARC_New(NARC_a_2_0_7, heapID);
    HeapExp_FndInitAllocator(v2 + 0x1081C, arg5, 4);
    ov49_02265698(v2, narc, arg5);
    ov49_02265738(v2, narc, arg5);
    ov49_022657B4(v2, narc, arg5);
    NARC_Delete(narc);
    return v2;
}

void ov49_0226535C(void *ptr) {
    ov49_0226571C(ptr);
    ov49_02265760(ptr);
    ov49_02265858(ptr);
    Heap_Free(ptr);
}

void ov49_02265378(u8 *arg0) {
    s32 v0;
    u8 *v1;

    v0 = 0;
    v1 = arg0 + 0x10;
    do {
        ov49_022658E4(arg0, v1);
        v0 += 1;
        v1 += 0xD10;
    } while (v0 < 0x14);
}

void ov49_02265398(void *arg0) {
    s32 v0;
    void *v1;

    ov49_02258DAC(*(void **)((u8 *)arg0 + 4));
    v0 = 0;
    v1 = arg0 + 0x10;
    do {
        ov49_02265920(arg0, v1);
        v0 += 1;
        v1 += 0xD10;
    } while (v0 < 0x14);
}

void ov49_022653C0(void *arg0, u32 arg1, s32 arg2) {
    void *v0;

    if (arg1 >= 0x14U) {
        GF_AssertFail();
    }
    v0 = ov49_02258D70(*(void **)((u8 *)arg0 + 4), arg1);
    if (v0 != 0) {
        ov49_02265890(arg0, arg0 + 0x10 + (0xD10 * arg1), v0, arg2);
    }
}

s32 ov49_022653F0(u8 *arg0, s32 arg1) {
    if (ov49_02265958(arg0 + 0x10 + (0xD10 * arg1)) != 1) {
        return 1;
    }
    return 0;
}

void ov49_0226540C(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    *(s32 *)arg0 = arg7;
    *(s32 *)((u8 *)arg0 + 4) = arg1;
    *(s32 *)((u8 *)arg0 + 0x10) = (s32)(arg2 - arg1);
    *(s32 *)((u8 *)arg0 + 0x14) = arg1;
    *(s32 *)((u8 *)arg0 + 8) = arg3;
    *(s32 *)((u8 *)arg0 + 0x18) = (s32)(arg4 - arg3);
    *(s32 *)((u8 *)arg0 + 0x1C) = arg3;
    *(s32 *)((u8 *)arg0 + 0xC) = arg5;
    *(s32 *)((u8 *)arg0 + 0x20) = (s32)(arg6 - arg5);
    *(s32 *)((u8 *)arg0 + 0x24) = arg5;
}
