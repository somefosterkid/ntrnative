#include "global.h"

#include "pm_string.h"

struct PlayerProfile;

struct UnkStruct_ov66_0223177C_t;

typedef struct {
    union {
        u32 unk_00_val1;
        struct {
            u8 unk_00_val2_unk_00;
            u8 unk_00_val2_unk_01;
            u8 unk_00_val2_unk_02;
            u8 unk_00_val2_unk_03;
        };
    };
} UnkStruct_ov66_02231428;

typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;

typedef struct PlayerProfile PlayerProfile;

typedef struct {
    const UnkStruct_ov66_02231428 *unk_00;
    int unk_04;
} UnkStruct_ov66_02231AB4;

typedef struct {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_ov66_02231D38_sub1;

typedef struct {
    UnkStruct_ov66_02231D38_sub1 unk_00[20];
    u16 unk_50;
    u16 unk_52;
} UnkStruct_ov66_02231D38;

typedef struct UnkStruct_ov66_02231FB0_t {
    u8 unk_00;
    u32 unk_04[3];
    String *unk_10[4];
    u16 unk_20[4];
    u16 unk_28;
    s16 unk_2A;
    struct UnkStruct_ov66_02231FB0_t *unk_2C;
    struct UnkStruct_ov66_02231FB0_t *unk_30;
} UnkStruct_ov66_02231FB0;

typedef struct {
    UnkStruct_ov66_02231FB0 unk_00[8];
    UnkStruct_ov66_02231FB0 unk_188;
} UnkStruct_ov66_02231E94;

struct UnkStruct_ov66_0223177C_t {
    UnkStruct_ov66_02231428 unk_00;
    BOOL unk_04;
    UnkStruct_ov66_02231D38 unk_08;
    UnkStruct_ov66_02231E94 unk_5C;
};

UnkStruct_ov66_02231FB0 *ov45_0222E04C(UnkStruct_ov66_02231E94 *param0, u16 param1);

void ov45_0222E0A4(UnkStruct_ov66_02231E94 *param0, UnkStruct_ov66_02231FB0 *param1);

void ov45_0222E0E0(UnkStruct_ov66_02231FB0 *param0, u32 param1, u32 param2, u32 param3, const PlayerProfile *param4, const PlayerProfile *param5, const PlayerProfile *param6, const PlayerProfile *param7, u16 param8, u16 param9, u16 param10, u16 param11, u32 param12, u32 param13, u32 param14);

void ov45_0222DB98(UnkStruct_ov66_0223177C *param0, const UnkStruct_ov66_02231AB4 *param1);

void ov45_0222DB98(UnkStruct_ov66_0223177C *param0, const UnkStruct_ov66_02231AB4 *param1) {
    UnkStruct_ov66_02231FB0 *v0;

    switch (param1->unk_04) {
    case 0:
    case 1:
    case 2:
    case 4:
        break;
    default:
        return;
    }

    v0 = ov45_0222E04C(&param0->unk_5C, 14);

    if (v0 == NULL) {
        return;
    }

    ov45_0222E0E0(v0, param1->unk_00->unk_00_val1, param1->unk_04, 0, NULL, NULL, NULL, NULL, 0, 0, 0, 0, 900, 14, 6);
    ov45_0222E0A4(&param0->unk_5C, v0);
}
