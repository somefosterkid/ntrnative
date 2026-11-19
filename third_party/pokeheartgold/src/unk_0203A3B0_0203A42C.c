#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "heap.h"
#include "network_icon.h"
#include "overworld_anim_manager.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "system.h"
#include "msgdata.h"

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
extern int (*const *const Unk_ov101_021D8774[13])(UnkStruct_ov101_021D4764 *);
void sub_0203A700(NetworkIcon *icon);

s32 sub_0203A4D4(s32, s32, s32, s32);
s32 sub_0203A59C(s32, s32, s32);
void sub_0203A478(NetworkIcon *arg0, s32 arg1, s32 arg2);
extern s8 _0210F930;
void *sub_0203A4AC(enum HeapID arg0);

static inline void ClearOAM(GXOamAttr *oam) {
    G2_SetOBJAttr(oam, 0, 0, 0, GX_OAM_MODE_NORMAL, 0, GX_OAM_EFFECT_NODISPLAY, GX_OAM_SHAPE_16x16, GX_OAM_COLORMODE_16, 0, 0, 0);
}

void sub_0203A42C(SysTask *task, void *param1);
void sub_0203A438(NetworkIcon *icon, int networkStrength);
void sub_0203A444(NetworkIcon *icon);

void sub_0203A42C(SysTask *task, void *param1) {
    NetworkIcon *v0 = param1;
    sub_0203A700(v0);
}

void sub_0203A438(NetworkIcon *icon, int networkStrength) {
    if (networkStrength < 4) {
        icon->strength = networkStrength;
        icon->unused3 = 0;
    }
}

void sub_0203A444(NetworkIcon *icon) {
    SysTask_Destroy(icon->postVBlankTask);
    ClearOAM((GXOamAttr *)HW_OAM);

    if (icon->createdOnSubScreen) {
        ClearOAM((GXOamAttr *)HW_DB_OAM);
    }

    Heap_Free(icon);
}

void sub_0203A478(NetworkIcon *arg0, s32 arg1, s32 arg2) {
    s8 v0;

    sub_0203A4D4(2, arg0->isWifi, 0x1C0, arg2);
    sub_0203A59C(2, arg0->isWifi, arg2);
    if (arg1 != 0) {
        v0 = 1;
    } else {
        v0 = 2;
    }
    *((s8 *)&arg0->screenId) = v0;
    *((s8 *)&arg0->createdOnSubScreen) = 1;
}

void *sub_0203A4AC(enum HeapID heapID) {
    void *v0;

    v0 = Heap_AllocAtEnd(heapID, 0x258U);
    sub_0201A3F8(&_0210F930, &v0);
    DC_FlushRange(v0, 0x258U);
    return v0;
}
