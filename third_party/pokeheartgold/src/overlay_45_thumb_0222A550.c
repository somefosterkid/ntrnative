#include "global.h"

#include "constants/heap.h"

#include "save.h"
#include "unk_struct_ov66_02232B20.h"
#include "overlay_45_wifi_plaza_system.h"

struct PasswordWordBank;
struct PlayerProfile;
struct UnkStruct_0207E060_t;
struct UnkStruct_ov66_0222DFF8_t;
struct UnkStruct_ov66_0223177C_t;
typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;
typedef u16 charcode_t;
typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;
void ov45_0222EC10(UnkStruct_ov66_02232B20 *param0);
typedef struct PasswordWordBank PasswordWordBank;
s32 ov45_0222E9E0(void);
const void *ov45_0222EA2C(s32 param0);
BOOL ov45_0222BADC(const UnkStruct_ov66_0222F6C4 *param0, const SaveData *saveData);
BOOL ov45_0222AFF8(const WiFiPlazaSystem *param0);
BOOL ov45_0222B00C(const WiFiPlazaSystem *param0);
s32 ov45_0222EC90(u32 param0);
void ov45_0222BA3C(WiFiPlazaSystem *param0);
void ov45_0222BAC4(UnkStruct_ov66_0222F6C4 *param0, const SaveData *saveData);
void ov45_0222BD5C(UnkStruct_ov66_0222FC68 *param0);
void ov45_0222BE28(WiFiPlazaSystem *param0, s32 param1);
void ov45_0222CB74(UnkStruct_ov66_02230A6C *param0, int param1, s32 param2);
enum PokemonType {
    TYPE_NORMAL = 0,
    TYPE_FIGHTING = 1,
    TYPE_FLYING = 2,
    TYPE_POISON = 3,
    TYPE_GROUND = 4,
    TYPE_ROCK = 5,
    TYPE_BUG = 6,
    TYPE_GHOST = 7,
    TYPE_STEEL = 8,
    TYPE_MYSTERY = 9,
    TYPE_FIRE = 10,
    TYPE_WATER = 11,
    TYPE_GRASS = 12,
    TYPE_ELECTRIC = 13,
    TYPE_PSYCHIC = 14,
    TYPE_ICE = 15,
    TYPE_DRAGON = 16,
    TYPE_DARK = 17,
    NUM_POKEMON_TYPES = 18,
};
void ov45_0222B118(WiFiPlazaSystem *param0, int param1);

BOOL ov45_0222A550(const WiFiPlazaSystem *param0, u32 param1);
const WiFiPlazaProfile *ov45_0222A578(const WiFiPlazaSystem *param0, u32 param1);
WiFiPlazaProfile *ov45_0222A5C0(WiFiPlazaSystem *param0);
void ov45_0222A5E8(WiFiPlazaSystem *param0, int param1);
void ov45_0222A704(WiFiPlazaSystem *param0, int param1, s32 param2);
void ov45_0222A72C(WiFiPlazaSystem *param0, u32 param1);
void ov45_0222A770(WiFiPlazaSystem *param0, int type1, int type2);
void ov45_0222A7DC(WiFiPlazaSystem *param0, u32 param1, u32 param2);

BOOL ov45_0222A550(const WiFiPlazaSystem *param0, u32 param1) {
    UnkStruct_ov66_02232B20 v0;

    ov45_0222EC10(&v0);

    if (v0.unk_04[param1] != -1) {
        return 1;
    }

    return 0;
}

const WiFiPlazaProfile *ov45_0222A578(const WiFiPlazaSystem *param0, u32 param1) {
    s32 v0;
    UnkStruct_ov66_02232B20 v1;
    const WiFiPlazaProfile *v2;

    ((param1 < 20) ? (void)0 : GF_AssertFail());

    ov45_0222EC10(&v1);

    v0 = v1.unk_04[param1];

    if (v0 == -1) {
        return 0L;
    }

    if (v0 == ov45_0222E9E0()) {
        v2 = &param0->unk_108.profile;
    } else {
        v2 = (const WiFiPlazaProfile *)ov45_0222EA2C(v0);
    }

    return v2;
}

WiFiPlazaProfile *ov45_0222A5C0(WiFiPlazaSystem *param0) {
    if (ov45_0222BADC(&param0->unk_108, param0->saveData) == 0) {
        param0->unk_52C = 1;
    }

    return &param0->unk_108.profile;
}

void ov45_0222A5E8(WiFiPlazaSystem *param0, int param1) {
    s32 v0;

    ((param1 < 15) ? (void)0 : GF_AssertFail());

    if (ov45_0222BADC(&param0->unk_108, param0->saveData) == 0) {
        param0->unk_52C = 1;
        return;
    }

    if (param0->unk_108.profile.unk_43 == param1) {
        return;
    }

    if (param1 != 9) {
        if (ov45_0222AFF8(param0) == 1) {
            if (ov45_0222B00C(param0) == 0) {
                v0 = ov45_0222EC90(param0->unk_1C0.unk_04);
                ov45_0222BE28(param0, param0->unk_1C0.unk_04);
            }

            ov45_0222BD5C(&param0->unk_1C0);
        }
    }

    switch (param0->unk_108.profile.unk_43) {
    case 2:
        ov45_0222CB74(&param0->unk_4BC, 16, -1);
        break;
    case 3:
        ov45_0222CB74(&param0->unk_4BC, 17, -1);
        break;
    case 4:
        ov45_0222CB74(&param0->unk_4BC, 18, -1);
        break;
    case 5:
        ov45_0222CB74(&param0->unk_4BC, 19, -1);
        break;
    case 6:
        ov45_0222CB74(&param0->unk_4BC, 20, -1);
        break;
    case 7:
        ov45_0222CB74(&param0->unk_4BC, 21, -1);
        break;
    case 8:
        ov45_0222CB74(&param0->unk_4BC, 22, -1);
        break;

    default:
        break;
    }

    param0->unk_108.profile.unk_43 = param1;

    ov45_0222BAC4(&param0->unk_108, param0->saveData);
    ov45_0222BA3C(param0);
}

void ov45_0222A704(WiFiPlazaSystem *param0, int param1, s32 param2) {
    ((param1 < 24) ? (void)0 : GF_AssertFail());

    ov45_0222CB74(&param0->unk_4BC, param1, param2);
    ov45_0222BA3C(param0);
}

void ov45_0222A72C(WiFiPlazaSystem *param0, u32 param1) {
    ((param1 < 27) ? (void)0 : GF_AssertFail());

    if (ov45_0222BADC(&param0->unk_108, param0->saveData) == 0) {
        param0->unk_52C = 1;
        return;
    }

    param0->unk_108.profile.unk_41 = param1;

    ov45_0222BAC4(&param0->unk_108, param0->saveData);
    ov45_0222BA3C(param0);
}

void ov45_0222A770(WiFiPlazaSystem *param0, int type1, int type2) {
    ((type1 < NUM_POKEMON_TYPES) ? (void)0 : GF_AssertFail());
    ((type2 < NUM_POKEMON_TYPES) ? (void)0 : GF_AssertFail());

    if (ov45_0222BADC(&param0->unk_108, param0->saveData) == 0) {
        param0->unk_52C = 1;
        return;
    }

    if (type1 >= NUM_POKEMON_TYPES) {
        return;
    }

    if (type2 >= NUM_POKEMON_TYPES) {
        return;
    }

    if (type1 == TYPE_NORMAL) {
        param0->unk_108.profile.unk_88[0] = type2;
        param0->unk_108.profile.unk_88[1] = TYPE_NORMAL;
    } else {
        param0->unk_108.profile.unk_88[0] = type1;
        param0->unk_108.profile.unk_88[1] = type2;
    }

    ov45_0222BAC4(&param0->unk_108, param0->saveData);
    ov45_0222BA3C(param0);
}

void ov45_0222A7DC(WiFiPlazaSystem *param0, u32 param1, u32 param2) {
    ((param2 < 3) ? (void)0 : GF_AssertFail());

    if (ov45_0222BADC(&param0->unk_108, param0->saveData) == 0) {
        param0->unk_52C = 1;
        return;
    }

    if (param2 >= 3) {
        return;
    }

    param0->unk_108.profile.unk_8C.questionNumber = param1;
    param0->unk_108.profile.unk_8C.selectedAnswer = param2;
    param0->unk_1FC.unk_0F_0 = 1;

    ov45_0222B118(param0, 6);
    ov45_0222BAC4(&param0->unk_108, param0->saveData);
    ov45_0222BA3C(param0);
}
