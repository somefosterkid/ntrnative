#include "global.h"

#include "overlay_91.h"
#include "save.h"
#include "unk_02035900.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct PlayerProfile;
struct UnkStruct_ov114_0225D678_t;
struct UnkStruct_ov114_0225E854_t;
struct UnkStruct_ov115_0226095C_t;
struct UnkStruct_ov115_02260BE4_t;
struct UnkStruct_ov115_02260D78_t;
struct UnkStruct_ov66_0222DFF8_t;
typedef struct UnkStruct_ov115_0226095C_t UnkStruct_ov115_0226095C;
typedef struct {
    u16 unk_00;
    u16 unk_02;
    VecFx16 unk_04;
    VecFx16 unk_0A;
    fx16 unk_10;
    s8 unk_12;
    u8 unk_13;
} UnkStruct_ov115_02265AD0;
typedef struct UnkStruct_ov114_0225D678_t UnkStruct_ov114_0225D678;
typedef struct UnkStruct_ov114_0225E854_t UnkStruct_ov114_0225E854;
u32 ov90_0225886C(const UnkStruct_ov114_0225C76C *param0, u32 param1);
typedef struct UnkStruct_ov115_02260BE4_t UnkStruct_ov115_02260BE4;
typedef struct {
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    s32 unk_14;
} UnkStruct_ov115_02262DC4;
typedef struct {
    u32 unk_00;
    UnkStruct_ov115_02262DC4 *unk_04;
    const PlayerProfile *unk_08[4];
    u32 unk_18[4];
} UnkStruct_ov115_02262F50;
void ov91_0225CD74(UnkStruct_ov115_02260BE4 *param0, const UnkStruct_ov115_02265AD0 *param1, u32 param2);
typedef struct UnkStruct_ov115_02260D78_t UnkStruct_ov115_02260D78;
struct UnkStruct_ov115_0226095C_t {
    UnkStruct_ov114_0225D678 *unk_00;
    UnkStruct_ov114_0225E854 *unk_04;
    UnkStruct_ov114_0225C76C unk_08;
    UnkStruct_ov114_0225C9A8 unk_18;
    UnkStruct_ov115_02260BE4 *unk_30;
    UnkStruct_ov115_02260D78 *unk_34;
    BOOL unk_38;
    BOOL unk_3C;
    BOOL unk_40;
    u16 unk_44;
    u16 unk_46;
    UnkStruct_ov115_02262F50 unk_48;
    u32 unk_70;
    u32 unk_74;
    u32 unk_78;
    u32 unk_7C;
    BOOL unk_80;
    u8 unk_84[4];
};
void ov91_0225D2EC(UnkStruct_ov115_02260D78 *param0);
void ov91_0225CD84(UnkStruct_ov115_02260BE4 *param0, u32 param1, u32 param2);
typedef struct {
    u32 unk_00[4];
} UnkStruct_ov115_02265AD4;
void ov91_0225D368(UnkStruct_ov115_02260D78 *param0, const UnkStruct_ov115_02265AD4 *param1);
void ov91_0225D380(UnkStruct_ov115_02260D78 *param0);
void ov91_0225D3B4(UnkStruct_ov115_02260D78 *param0, u32 param1, u32 param2);
void ov91_0225D2D0(UnkStruct_ov115_02260D78 *param0);
BOOL ov91_0225D32C(UnkStruct_ov115_02260D78 *param0, UnkStruct_ov115_02265AD0 *param1);
BOOL ov91_0225CCA8(UnkStruct_ov115_0226095C *param0, int param1, const void *param2, int param3);
s32 ov91_0225D2E8(const UnkStruct_ov115_02260D78 *param0);
u32 ov91_0225D37C(const UnkStruct_ov115_02260D78 *param0);

void ov91_0225CA74(UnkStruct_ov115_0226095C *param0, const UnkStruct_ov115_02265AD0 *param1, u32 param2);
void ov91_0225CA90(void *param0);
void ov91_0225CA9C(UnkStruct_ov115_0226095C *param0, u32 param1, u32 param2);
void ov91_0225CABC(UnkStruct_ov115_0226095C *param0, const UnkStruct_ov115_02265AD4 *param1, u32 param2);
void ov91_0225CAEC(UnkStruct_ov115_0226095C *param0, u32 param1, u32 param2);
void ov91_0225CB50(UnkStruct_ov115_0226095C *param0);
void ov91_0225CB64(UnkStruct_ov115_0226095C *param0);
void ov91_0225CB98(UnkStruct_ov115_0226095C *param0);

void ov91_0225CA74(UnkStruct_ov115_0226095C *param0, const UnkStruct_ov115_02265AD0 *param1, u32 param2) {
    u32 v0 = ov90_0225886C(&param0->unk_08, param2);
    ov91_0225CD74(param0->unk_30, param1, v0);
}

void ov91_0225CA90(void *param0) {
    UnkStruct_ov115_0226095C *v0 = param0;

    ov91_0225D2EC(v0->unk_34);
}

void ov91_0225CA9C(UnkStruct_ov115_0226095C *param0, u32 param1, u32 param2) {
    u32 v0 = ov90_0225886C(&param0->unk_08, param2);

    if (param0->unk_30 != 0L) {
        ov91_0225CD84(param0->unk_30, param1, v0);
    }
}

void ov91_0225CABC(UnkStruct_ov115_0226095C *param0, const UnkStruct_ov115_02265AD4 *param1, u32 param2) {
    ov91_0225D368(param0->unk_34, param1);
    param0->unk_40 = 1;

    {
        int v0;

        for (v0 = 0; v0 < 4; v0++) {
            param0->unk_18.unk_00[v0] = param1->unk_00[v0];
        }
    }
}

void ov91_0225CAE0(void *param0) {
    UnkStruct_ov115_0226095C *v0 = param0;

    ov91_0225D380(v0->unk_34);
}

void ov91_0225CAEC(UnkStruct_ov115_0226095C *param0, u32 param1, u32 param2) {
    u32 v0;
    int v1;

    v0 = ov90_0225886C(&param0->unk_08, param2);
    ov91_0225D3B4(param0->unk_34, param1, v0);

    param0->unk_84[v0]++;

    if (param0->unk_44 == 0) {
        BOOL v2 = 1;

        for (v1 = 0; v1 < param0->unk_74; v1++) {
            if (param0->unk_7C > param0->unk_84[v1]) {
                v2 = 0;
            }
        }

        if (v2 == 1) {
            param0->unk_7C++;
            sub_02037030(29, 0L, 0);
        }
    }
}

void ov91_0225CB50(UnkStruct_ov115_0226095C *param0) {
    param0->unk_80 = 1;
}

void ov91_0225CB58(void *param0) {
    UnkStruct_ov115_0226095C *v0 = param0;

    ov91_0225D2D0(v0->unk_34);
}

void ov91_0225CB64(UnkStruct_ov115_0226095C *param0) {
    UnkStruct_ov115_02265AD0 v0;

    while (ov91_0225D32C(param0->unk_34, &v0) == 1) {
        ov91_0225CCA8(param0, 24, &v0, sizeof(UnkStruct_ov115_02265AD0));
    }
}

void ov91_0225CB98(UnkStruct_ov115_0226095C *param0) {
    s32 v0;
    u32 v1;
    BOOL v2;

    v0 = ov91_0225D2E8(param0->unk_34);

    if (v0 == 0) {
        return;
    }

    if ((v0 % 30) == 0) {
        if (param0->unk_80 == 1) {
            v1 = ov91_0225D37C(param0->unk_34);
            v2 = ov91_0225CCA8(param0, 28, &v1, sizeof(u32));
            ((v2) ? (void)0 : GF_AssertFail());

            param0->unk_80 = 0;
        }
    }
}
