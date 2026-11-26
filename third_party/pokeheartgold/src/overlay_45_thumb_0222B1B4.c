#include "global.h"

#include "overlay_45_wifi_plaza_api.h"

#include "player_data.h"
#include "constants/heap.h"
#include "save.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_45_wifi_plaza_system.h"

u32 ov45_0222A53C(const WiFiPlazaSystem *param0);

u32 ov45_0222C900(const UnkStruct_ov66_022307D4 *param0);

u32 ov45_0222B1B4(const WiFiPlazaSystem *arg0);

struct PasswordWordBank;
struct UnkStruct_0207E060_t;
struct UnkStruct_ov66_0222DFF8_t;
struct UnkStruct_ov66_0223177C_t;
typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;
typedef u16 charcode_t;
typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;
typedef struct PasswordWordBank PasswordWordBank;
int ov45_0222CB40(const UnkStruct_ov66_022309A4 *param0);
int ov45_0222CB3C(const UnkStruct_ov66_022309A4 *param0);
PlayerProfile *Save_PlayerData_GetProfile(SaveData *saveData);
UnkStruct_0207E060 *sub_02078DD8(const PlayerProfile *info, enum HeapID heapID);
void sub_02078E28(UnkStruct_0207E060 *param0);
int ov45_0222B1DC(const WiFiPlazaSystem *param0);
int ov45_0222B1EC(const WiFiPlazaSystem *param0);
void ov45_0222B1FC(WiFiPlazaSystem *param0, const UnkStruct_ov68_0225DC74 *param1);
BOOL ov45_0222B21C(const WiFiPlazaSystem *param0, UnkStruct_ov68_0225DC74 *param1);
void ov45_0222B244(WiFiPlazaSystem *param0);
void ov45_0222B270(WiFiPlazaSystem *param0);

BOOL ov45_0222B28C(charcode_t *trainerName, u32 length);

u32 ov45_0222B1B4(const WiFiPlazaSystem *arg0) {
    u32 v0;

    v0 = ov45_0222C900(&arg0->unk_3AB);
    if (ov45_0222AB28(arg0, ov45_0222A53C(arg0)) == 1) {
        v0 += 2;
    }
    return v0;
}

int ov45_0222B1DC(const WiFiPlazaSystem *param0) {
    return ov45_0222CB40(&param0->unk_49C);
}

int ov45_0222B1EC(const WiFiPlazaSystem *param0) {
    return ov45_0222CB3C(&param0->unk_49C);
}

void ov45_0222B1FC(WiFiPlazaSystem *param0, const UnkStruct_ov68_0225DC74 *param1) {
    param0->unk_510.unk_14 = 1;
    param0->unk_510.unk_00 = *param1;
}

BOOL ov45_0222B21C(const WiFiPlazaSystem *param0, UnkStruct_ov68_0225DC74 *param1) {
    if (param0->unk_510.unk_14 == 1) {
        *param1 = param0->unk_510.unk_00;
        return 1;
    }

    return 0;
}

void ov45_0222B244(WiFiPlazaSystem *param0) {
    PlayerProfile *info;

    ((param0->unk_4B8 == 0L) ? (void)0 : GF_AssertFail());

    info = Save_PlayerData_GetProfile(param0->saveData);
    param0->unk_4B8 = sub_02078DD8(info, param0->heapID);
}

void ov45_0222B270(WiFiPlazaSystem *param0) {
    if (param0->unk_4B8 != 0L) {
        sub_02078E28(param0->unk_4B8);
        param0->unk_4B8 = 0L;
    }
}

BOOL ov45_0222B28C(charcode_t *trainerName, u32 length) {
    u32 uVar1;

    uVar1 = 0;
    while (uVar1 < length) {
        if (*trainerName == 0xffff) {
            return 1;
        }
        uVar1 = uVar1 + 1;
        trainerName = trainerName + 1;
    }
    return 0;
}
