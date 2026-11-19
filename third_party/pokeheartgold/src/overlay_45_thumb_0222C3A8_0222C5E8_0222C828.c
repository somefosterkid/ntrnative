#include "global.h"

#include "constants/heap.h"
#include "overlay_45_wifi_plaza_system.h"

struct PasswordWordBank;

struct SaveData;

struct PlayerProfile;

struct UnkStruct_0207E060_t;

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov66_0223177C_t;

struct WiFiPlazaProfile;

typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;

typedef u16 charcode_t;

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

typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;

typedef struct {
    s32 unk_00[4];
    u8 unk_10;
    u8 unk_11;
    u8 unk_12;
    u8 unk_13_0 : 7;
    u8 unk_13_7 : 1;
} UnkStruct_ov66_0222E990;

typedef struct PasswordWordBank PasswordWordBank;

void ov66_0222FDC4(WiFiPlazaSystem *param0, const UnkStruct_ov66_0222E990 *param1);
void ov45_0222BE9C(WiFiPlazaSystem *param0, const UnkStruct_ov66_0222E990 *param1);

void ov66_0222FE40(WiFiPlazaSystem *param0, const UnkStruct_ov66_0222E990 *param1);
void ov45_0222BF18(WiFiPlazaSystem *param0, const UnkStruct_ov66_0222E990 *param1);

void ov66_0222FEC0(WiFiPlazaSystem *param0, const UnkStruct_ov66_0222E990 *param1);
void ov45_0222BF98(WiFiPlazaSystem *param0, const UnkStruct_ov66_0222E990 *param1);

void ov66_0222FFF4(WiFiPlazaSystem *param0, const UnkStruct_ov66_0222E990 *param1);
void ov45_0222C0CC(WiFiPlazaSystem *param0, const UnkStruct_ov66_0222E990 *param1);

void ov66_02230058(WiFiPlazaSystem *param0, const UnkStruct_ov66_0222E990 *param1);
void ov45_0222C130(WiFiPlazaSystem *param0, const UnkStruct_ov66_0222E990 *param1);

void ov66_02230114(WiFiPlazaSystem *param0, const UnkStruct_ov66_0222E990 *param1);
void ov45_0222C1EC(WiFiPlazaSystem *param0, const UnkStruct_ov66_0222E990 *param1);

void ov66_022301D0(WiFiPlazaSystem *param0, const UnkStruct_ov66_0222E990 *param1);
void ov45_0222C2A8(WiFiPlazaSystem *param0, const UnkStruct_ov66_0222E990 *param1);

void ov66_02230294(WiFiPlazaSystem *param0, const UnkStruct_ov66_0222E990 *param1);
void ov45_0222C36C(WiFiPlazaSystem *param0, const UnkStruct_ov66_0222E990 *param1);

WiFiPlazaSystem *ov66_0222DDF0(SaveData *saveData, enum HeapID heapID);

void ov45_0222A844(const WiFiPlazaProfile *profile, PlayerProfile *trainerInfo, enum HeapID heapID);

void ov66_0222EA10(WiFiPlazaSystem *param0, enum PlazaMinigame param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6, BOOL param7);

void ov66_0222EAB4(WiFiPlazaSystem *param0, enum PlazaMinigame param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6);

void ov66_0222F7C8(UnkStruct_ov66_0222F6C4 *param0, SaveData *saveData, enum HeapID heapID);

void ov66_0222FBF0(UnkStruct_ov66_0222FBF0 *param0, enum HeapID heapID);

void ov66_0222FC0C(UnkStruct_ov66_0222FBF0 *param0, const UnkStruct_ov66_0222F6C4 *param1, enum HeapID heapID);

void ov45_0222C828(s32 param0, const void *param1, u32 param2, void *param3);

void ov45_0222C828(s32 param0, const void *param1, u32 param2, void *param3) {
    WiFiPlazaSystem *v0 = param3;
    const UnkStruct_ov66_0222E990 *v1 = param1;

    static void (*const v2[])(WiFiPlazaSystem *, const UnkStruct_ov66_0222E990 *) = {
        ov45_0222BE9C,
        ov45_0222BF18,
        ov45_0222BF98,
        ov45_0222C0CC,
        ov45_0222C130,
        ov45_0222C1EC,
        ov45_0222C36C,
        ov45_0222C36C,
        ov45_0222C2A8
    };

    v2[v1->unk_11](v0, v1);
}
