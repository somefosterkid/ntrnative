#include "global.h"

#include "player_data.h"
#include "constants/heap.h"
#include "save.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_45_wifi_plaza_system.h"

s32 ov45_0222D79C(u32, u32);

u8 ov45_0222AAA8(void *arg0);

struct PasswordWordBank;
struct UnkStruct_0207E060_t;
struct UnkStruct_ov66_0222DFF8_t;
struct UnkStruct_ov66_0223177C_t;
typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;
typedef u16 charcode_t;
typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;
typedef struct PasswordWordBank PasswordWordBank;
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
BOOL ov45_0222C95C(const UnkStruct_ov66_02232068 *param0, u32 param1);
u32 ov45_0222AAC8(const WiFiPlazaProfile *profile);
BOOL ov45_0222AADC(const WiFiPlazaProfile *profile);
int ov45_0222AAEC(const WiFiPlazaProfile *profile, u32 index);
void ov45_0222AB0C(const WiFiPlazaProfile *profile, UnkStruct_ov66_0222E908 *param1);
PlayerProfile *ov45_0222AB1C(const WiFiPlazaSystem *param0);
BOOL ov45_0222AB28(const WiFiPlazaSystem *param0, u32 param1);

u8 ov45_0222AAA8(void *arg0) {
    if (ov45_0222D79C(*(u16 *)((u8 *)arg0 + 0x3C), *(u8 *)((u8 *)arg0 + 0x3E)) == 0) {
        return 0U;
    }
    return *(u8 *)((u8 *)arg0 + 0x3E);
}

u32 ov45_0222AAC8(const WiFiPlazaProfile *profile) {
    u32 v0;

    if (profile->unk_41 == 0xff) {
        return 0;
    }

    if (profile->unk_41 >= 27) {
        v0 = 0;
    } else {
        v0 = profile->unk_41;
    }

    return v0;
}

BOOL ov45_0222AADC(const WiFiPlazaProfile *profile) {
    if (profile->unk_41 == 0xff) {
        return 0;
    }

    return 1;
}

int ov45_0222AAEC(const WiFiPlazaProfile *profile, u32 index) {
    ((index < 2) ? (void)0 : GF_AssertFail());

    if (profile->unk_88[index] >= NUM_POKEMON_TYPES) {
        return TYPE_NORMAL;
    }

    return profile->unk_88[index];
}

void ov45_0222AB0C(const WiFiPlazaProfile *profile, UnkStruct_ov66_0222E908 *param1) {
    *param1 = profile->unk_8C;
}

PlayerProfile *ov45_0222AB1C(const WiFiPlazaSystem *param0) {
    return param0->unk_508.trainerInfo;
}

BOOL ov45_0222AB28(const WiFiPlazaSystem *param0, u32 param1) {
    return ov45_0222C95C(&param0->unk_3CB, param1);
}
