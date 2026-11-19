#include "global.h"

struct String;

struct PlayerProfile;

struct UnkStruct_ov66_0223177C_t;

struct UnkStruct_ov66_02231FB0_t;

typedef struct {
    u8 pad_0[4];
    u8 unk_4;
    u8 unk_5;
    u16 unk_6;
    u32 unk_8;
} UnkStruct_ov66_02231428;

typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;

typedef struct String String;

typedef struct PlayerProfile PlayerProfile;

enum PlazaMinigame {
    UnkEnum_ov66_022324D0_00,
    UnkEnum_ov66_022324D0_01,
    UnkEnum_ov66_022324D0_02,
    UnkEnum_ov66_022324D0_03,
    UnkEnum_ov66_022324D0_04,
    UnkEnum_ov66_022324D0_05,
    UnkEnum_ov66_022324D0_06,
    UnkEnum_ov66_022324D0_07,
    UnkEnum_ov66_022324D0_08,
    UnkEnum_ov66_022324D0_09,
    UnkEnum_ov66_022324D0_10,
    UnkEnum_ov66_022324D0_11,
    UnkEnum_ov66_022324D0_12 = UnkEnum_ov66_022324D0_02 + 1,
};

typedef struct {
    enum PlazaMinigame unk_00;
    u32 unk_04;
    const PlayerProfile *unk_08;
    const PlayerProfile *unk_0C;
    const PlayerProfile *unk_10;
    const PlayerProfile *unk_14;
    u16 unk_18;
    u16 unk_1A;
    u16 unk_1C;
    u16 unk_1E;
} UnkStruct_ov66_02231B80;

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
    struct { u8 unk_0_anon[4]; } unk_00;
    BOOL unk_04;
    UnkStruct_ov66_02231D38 unk_08;
    UnkStruct_ov66_02231E94 unk_5C;
};

UnkStruct_ov66_02231FB0 *ov45_0222E04C(UnkStruct_ov66_02231E94 *param0, u16 param1);

void ov45_0222E0A4(UnkStruct_ov66_02231E94 *param0, UnkStruct_ov66_02231FB0 *param1);

void ov45_0222E0E0(UnkStruct_ov66_02231FB0 *param0, u32 param1, u32 param2, u32 param3, const PlayerProfile *param4, const PlayerProfile *param5, const PlayerProfile *param6, const PlayerProfile *param7, u16 param8, u16 param9, u16 param10, u16 param11, u32 param12, u32 param13, u32 param14);

BOOL ov45_0222E5B4(enum PlazaMinigame param0, u32 param1);

void ov45_0222DC64(UnkStruct_ov66_0223177C *param0, const UnkStruct_ov66_02231B80 *param1);

void ov45_0222DC64(UnkStruct_ov66_0223177C *param0, const UnkStruct_ov66_02231B80 *param1) {
    UnkStruct_ov66_02231FB0 *v0;
    u32 v1;
    extern const u8 ov45_02254BC8[3];

    if ((param1->unk_00 != UnkEnum_ov66_022324D0_00) && (param1->unk_00 != UnkEnum_ov66_022324D0_01) && (param1->unk_00 != UnkEnum_ov66_022324D0_02)) {
        return;
    }

    if (ov45_0222E5B4(param1->unk_00, param1->unk_04) == 0) {
        return;
    }

    v1 = param1->unk_00 - UnkEnum_ov66_022324D0_00;
    v0 = ov45_0222E04C(&param0->unk_5C, ov45_02254BC8[v1]);

    if (v0 == NULL) {
        return;
    }

    ov45_0222E0E0(v0, v1, param1->unk_04, 0, param1->unk_08, param1->unk_0C, param1->unk_10, param1->unk_14, param1->unk_18, param1->unk_1A, param1->unk_1C, param1->unk_1E, 450, ov45_02254BC8[v1], 8);
    ov45_0222E0A4(&param0->unk_5C, v0);
}
