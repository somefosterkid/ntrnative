#include "global.h"
#include "constants/heap.h"
#include "save.h"
#include "overlay_45_wifi_plaza_system.h"

s32 ov45_0222EEF0(s32, s32 *, s32);

void ov45_0222AE44(void);

struct PasswordWordBank;
struct PlayerProfile;
struct UnkStruct_0207E060_t;
struct UnkStruct_ov66_0222DFF8_t;
struct UnkStruct_ov66_0223177C_t;
typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;
typedef u16 charcode_t;
typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;
typedef struct PasswordWordBank PasswordWordBank;
void ov45_0222BD4C(UnkStruct_ov66_0222FC68 *param0);
const WiFiPlazaProfile *ov45_0222A578(const WiFiPlazaSystem *param0, u32 param1);
u32 ov45_0222A920(const WiFiPlazaProfile *profile);
s32 ov45_0222EC90(u32 param0);
void ov45_0222EF4C(u32 param0, s32 param1, const void *param2, u32 param3);
void ov45_0222BE00(UnkStruct_ov66_0222FC68 *param0, u16 param1);
void ov45_0222BE48(UnkStruct_ov66_0222FC68 *param0);
void ov45_0222BE28(WiFiPlazaSystem *param0, s32 param1);
BOOL ov45_0222BE74(const UnkStruct_ov66_0222FC68 *param0);
void ov45_0222BE94(UnkStruct_ov66_0222FC68 *param0);
void ov45_0222AE64(WiFiPlazaSystem *param0);
BOOL ov45_0222AE74(WiFiPlazaSystem *param0, u32 param1);
void ov45_0222AED8(WiFiPlazaSystem *param0, u16 param1);
void ov45_0222AF80(WiFiPlazaSystem *param0);
void ov45_0222AFC4(WiFiPlazaSystem *param0);
BOOL ov45_0222AFF8(const WiFiPlazaSystem *param0);
BOOL ov45_0222B00C(const WiFiPlazaSystem *param0);
u32 ov45_0222B020(const WiFiPlazaSystem *param0);
BOOL ov45_0222B028(const WiFiPlazaSystem *param0);
int ov45_0222B034(const WiFiPlazaSystem *param0);
u16 ov45_0222B040(const WiFiPlazaSystem *param0);
BOOL ov45_0222B048(const WiFiPlazaSystem *param0, u32 param1);
BOOL ov45_0222B06C(const WiFiPlazaSystem *param0);
void ov45_0222B094(WiFiPlazaSystem *param0);
void ov45_0222B0A4(WiFiPlazaSystem *param0);
BOOL ov45_0222B0B0(const WiFiPlazaSystem *param0);

void ov45_0222AE44(void) {
    s32 v0;

    ov45_0222EEF0(6, &v0, 4);
}

void ov45_0222AE54(void);

void ov45_0222AE54(void) {
    s32 v0;

    ov45_0222EEF0(7, &v0, 4);
}

void ov45_0222AE64(WiFiPlazaSystem *param0) {
    ov45_0222BD4C(&param0->unk_1C0);
}

BOOL ov45_0222AE74(WiFiPlazaSystem *param0, u32 param1) {
    s32 v0;

    if (param0->unk_1C0.unk_0A != 0) {
        return 0;
    }

    {
        const WiFiPlazaProfile *v1 = ov45_0222A578(param0, param1);
        u32 v2 = ov45_0222A920(v1);

        if (v2 != 1) {
            return 0;
        }
    }

    param0->unk_1C0.unk_04 = param1;
    param0->unk_1C0.unk_06 = 1;
    param0->unk_1C0.unk_0A = 1;

    ov45_0222BE00(&param0->unk_1C0, 0);
    v0 = ov45_0222EC90(param1);
    ov45_0222EF4C(0, v0, &param0->unk_1C0.unk_20, sizeof(UnkStruct_ov66_022305B0));
    ov45_0222BE48(&param0->unk_1C0);

    return 1;
}

void ov45_0222AED8(WiFiPlazaSystem *param0, u16 param1) {
    BOOL isTrainerNameValid = 0;
    s32 v1;

    if (param0->unk_1C0.unk_0A == 1) {
        if (param0->unk_1C0.unk_06 != 2) {
            isTrainerNameValid = 1;
        }
    } else {
        if (param0->unk_1C0.unk_0A == 2) {
            if (param0->unk_1C0.unk_06 != 3) {
                isTrainerNameValid = 1;
            }
        }
    }

    if (isTrainerNameValid) {
        v1 = ov45_0222EC90(param0->unk_1C0.unk_04);
        ov45_0222BE28(param0, v1);
        return;
    }

    switch (param0->unk_1C0.unk_0A) {
    case 1:
        param0->unk_1C0.unk_06 = 3;
        break;
    case 2:
        param0->unk_1C0.unk_06 = 2;
        break;
    default:
        v1 = ov45_0222EC90(param0->unk_1C0.unk_04);
        ov45_0222BE28(param0, v1);
        return;
    }

    ov45_0222BE00(&param0->unk_1C0, param1);
    v1 = ov45_0222EC90(param0->unk_1C0.unk_04);
    ov45_0222EF4C(2, v1, &param0->unk_1C0.unk_20, sizeof(UnkStruct_ov66_022305B0));
    ov45_0222BE48(&param0->unk_1C0);
}

void ov45_0222AF80(WiFiPlazaSystem *param0) {
    s32 v0;

    if (param0->unk_1C0.unk_06 == 0) {
        return;
    }

    if (param0->unk_1C0.unk_0A != 1) {
        return;
    }

    param0->unk_1C0.unk_20.unk_02 = 0;
    v0 = ov45_0222EC90(param0->unk_1C0.unk_04);

    ov45_0222EF4C(3, v0, &param0->unk_1C0.unk_20, sizeof(UnkStruct_ov66_022305B0));
    ov45_0222BD4C(&param0->unk_1C0);
}

void ov45_0222AFC4(WiFiPlazaSystem *param0) {
    s32 v0;

    if (param0->unk_1C0.unk_06 == 0) {
        return;
    }

    v0 = ov45_0222EC90(param0->unk_1C0.unk_04);

    param0->unk_1C0.unk_06 = 4;
    param0->unk_1C0.unk_20.unk_02 = 4;

    ov45_0222EF4C(2, v0, &param0->unk_1C0.unk_20, sizeof(UnkStruct_ov66_022305B0));
}

BOOL ov45_0222AFF8(const WiFiPlazaSystem *param0) {
    if (param0->unk_1C0.unk_0A != 0) {
        return 1;
    }

    return 0;
}

BOOL ov45_0222B00C(const WiFiPlazaSystem *param0) {
    if (param0->unk_1C0.unk_0A == 2) {
        return 1;
    }

    return 0;
}

u32 ov45_0222B020(const WiFiPlazaSystem *param0) {
    return param0->unk_1C0.unk_04;
}

BOOL ov45_0222B028(const WiFiPlazaSystem *param0) {
    return param0->unk_1C0.unk_0B;
}

int ov45_0222B034(const WiFiPlazaSystem *param0) {
    return param0->unk_1C0.unk_06;
}

u16 ov45_0222B040(const WiFiPlazaSystem *param0) {
    return param0->unk_1C0.unk_08;
}

BOOL ov45_0222B048(const WiFiPlazaSystem *param0, u32 param1) {
    ((param1 < 20) ? (void)0 : GF_AssertFail());

    if (param0->unk_1C0.unk_0C[param1] >= 6) {
        return 0;
    }

    return 1;
}

BOOL ov45_0222B06C(const WiFiPlazaSystem *param0) {
    if (param0->unk_1C0.unk_06 == 4) {
        return 1;
    }

    if (ov45_0222BE74(&param0->unk_1C0) == 0) {
        return 1;
    }

    return 0;
}

void ov45_0222B094(WiFiPlazaSystem *param0) {
    ov45_0222BE94(&param0->unk_1C0);
}

void ov45_0222B0A4(WiFiPlazaSystem *param0) {
    param0->unk_1C0.unk_07 = 1;
}

BOOL ov45_0222B0B0(const WiFiPlazaSystem *param0) {
    return param0->unk_1C0.unk_07;
}
