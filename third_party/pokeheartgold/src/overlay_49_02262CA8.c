#include "global.h"
#include "overlay_49_unk_struct_ov70_02266af0.h"

#include "bg_window.h"
#include "filesystem.h"
#include "overworld_anim_manager.h"
#include "sys_task.h"
#include "overlay_49.h"
#include "unk_02037C94.h"

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


typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

s32 ov45_0222AC14(s32, u32, u32, s32, s32, s32, s32, s32);
s32 ov45_0222F314(s32);
u16 ov45_0222F3E8(s32);
s32 ov49_02259FE8(UnkStruct_ov70_0225DEE8 *);
s32 ov49_0225EF8C(struct UnkStruct_ov70_02263344_t *, s32);

void ov49_02262CA8(UnkStruct_ov70_02266AF0 *work, void *param1) {
    UnkStruct_ov101_021D5388 *v0 = param1;
    ov49_0225A134(v0);
}

s32 ov49_02262CB4(UnkStruct_ov70_02266AF0 *work, UnkStruct_ov70_0225DEE8 *arg1, void *arg2, struct UnkStruct_ov70_02263344_t * arg3, s32 arg4) {
    s32 v0;
    s32 v1;
    s16 v2;
    s32 v3;
    s32 v4;

    v0 = ov49_02259FE8(arg1);
    v3 = sub_0203988C();
    v1 = ov45_0222F314(*(u8 *)((u8 *)arg2 + 1));
    if (v3 == 0) {
        *(s8 *)arg2 = 4;
        ov49_0225EF8C(arg3, 0x17);
        return 1;
    }
    if (v3 == 2) {
        work->unk_08 = v1;
        ov49_0225EF8C(arg3, 0x15);
        return 1;
    }
    if (work->unk_07 != v1) {
        work->unk_07 = (u8)v1;
        if (v1 != 4) {
            ov45_0222AC14(v0, *(u8 *)((u8 *)arg2 + 2), v1, arg4, 0, 0, 0, 0);
        }
    }
    work->unk_00 = ov45_0222F3E8(*(u8 *)((u8 *)arg2 + 1));
    v4 = 0;
    v2 = work->unk_00;
    if (v2 == 0) {
        if ((u32)ov45_0222F314(*(u8 *)((u8 *)arg2 + 1)) <= 1U) {
            *(s8 *)arg2 = 3;
            ov49_0225EF8C(arg3, 0x17);
            return 1;
        }
        ov49_0225EF8C(arg3, 0x16);
        return 1;
    }
    if ((s32)v2 <= 0x14A) {
        ov49_0225EF8C(arg3, 0x14);
        v4 = 1;
    }
    return v4;
}
