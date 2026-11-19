#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "pm_string.h"
#include "sprite.h"
#include "sys_task.h"
#include "touchscreen.h"
#include "overlay_40_unk_struct_ov62_02233310.h"
#include "msgdata.h"

typedef struct BattleRegulation {
    u16 name[12];
    u16 maxTotalLevel;
    u8 teamSize;
    u8 maxLevel;
    s8 heightRestriction;
    s8 weightRestriction;
    u8 evolutionRestriction : 1;
    u8 specialSpeciesRestriction : 1;
    u8 speciesUniqueness : 1;
    u8 itemUniqueness : 1;
    u8 moveRestrictions : 1;
} BattleRegulation;

typedef struct BattleRegulationData {
    BattleRegulation regulations[1];
} BattleRegulationData;

void ov40_0222FA18(BattleRegulationData *regulationData);

void ov40_0222FA18(BattleRegulationData *regulationData) {
    memset(regulationData, 0, sizeof(BattleRegulationData));
}

// ov40_0222F9C0 (overlay_40_0222DD9C_0222F950_0222F9C0.c) is void (UnkStruct_ov62_0223359C *); this is its one-argument
// wrapper (all callers pass &...->unk_14.unk_46C). The struct is a local copy defined below, so void * here.
void ov40_0222F9C0(void *param0);

void ov40_0222FA24(void *param0);

void ov40_0222FA24(void *param0) {
    ov40_0222F9C0(param0);
}

typedef struct {
    int unk_00;
    BOOL unk_04;
    u32 unk_08;
    u32 unk_0C;
    s16 unk_10;
    u8 padding_12[2];
    BOOL unk_14;
    ManagedSprite *unk_18[2];
} UnkStruct_ov62_0223359C;

void ov40_0222FA2C(UnkStruct_ov62_0223359C *param0, u32 param1);

void ov40_0222FA2C(UnkStruct_ov62_0223359C *param0, u32 param1) {
    s32 v0 = param1 - param0->unk_0C;

    if ((v0 * v0) >= (16 * 16)) {
        if (v0 < 0) {
            param0->unk_10 = -1;
        } else {
            param0->unk_10 = +1;
        }
        param0->unk_0C = param1;
    } else {
        param0->unk_10 = 0;
    }
}

struct MsgData;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
} UnkStruct_ov62_02233310_sub1;

typedef struct {
    int unk_00;
    int unk_04;
    u64 unk_08;
} UnkStruct_ov62_02249380;

typedef struct {
    const UnkStruct_ov62_02249380 *unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    u32 unk_24;
    const TouchscreenHitbox *unk_28;
} UnkStruct_ov62_02248CDC;

void ov40_0222FA5C(UnkStruct_ov62_0223359C *param0, UnkStruct_ov62_02233310 *param1);

void ov40_0222FA5C(UnkStruct_ov62_0223359C *param0, UnkStruct_ov62_02233310 *param1) {
    if (param1->unk_48) {
        if (param1->unk_04 < 2) {
            param0->unk_14 = 0;
        } else {
            param0->unk_14 = 1;
        }
    } else {
        if (param1->unk_10 >= param1->unk_04) {
            param0->unk_14 = 0;
        } else {
            param0->unk_14 = 1;
        }
    }
}
