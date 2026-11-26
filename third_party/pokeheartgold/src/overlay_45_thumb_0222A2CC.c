#include "global.h"

#include "overlay_45_wifi_plaza_api.h"

#include "error_handling.h"
#include "constants/heap.h"
#include "save.h"
#include "overlay_45_wifi_plaza_system.h"

s32 ov45_0222ECDC(s32);

void ov45_0222A2CC(void *arg0);

struct PasswordWordBank;
struct PlayerProfile;
struct UnkStruct_0207E060_t;
struct UnkStruct_ov66_0222DFF8_t;
struct UnkStruct_ov66_0223177C_t;
typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;
typedef u16 charcode_t;
typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;
typedef struct PasswordWordBank PasswordWordBank;
BOOL ov45_0222A2E0(const WiFiPlazaSystem *param0);
BOOL ov45_0222A2F8(const WiFiPlazaSystem *param0);
void ov45_0222A310(WiFiPlazaSystem *param0);
BOOL ov45_0222A324(const WiFiPlazaSystem *param0);
BOOL ov45_0222A33C(const WiFiPlazaSystem *param0);
int ov45_0222A35C(const WiFiPlazaSystem *param0);
BOOL ov45_0222A3A0(const WiFiPlazaSystem *param0);

void ov45_0222A2CC(void *arg0) {
    if (arg0 == 0) {
        GF_AssertFail();
    }
    ov45_0222ECDC(4);
}

BOOL ov45_0222A2E0(const WiFiPlazaSystem *param0) {
    ((param0) ? (void)0 : GF_AssertFail());

    return param0->unk_1FC.unk_0F_0;
}

BOOL ov45_0222A2F8(const WiFiPlazaSystem *param0) {
    ((param0) ? (void)0 : GF_AssertFail());
    return param0->unk_1FC.unk_0F_4;
}

void ov45_0222A310(WiFiPlazaSystem *param0) {
    param0->unk_1FC.unk_0F_4 = 1;
}

BOOL ov45_0222A324(const WiFiPlazaSystem *param0) {
    return param0->unk_1FC.unk_00_0;
}

BOOL ov45_0222A330(const WiFiPlazaSystem *param0) {
    return param0->unk_1FC.unk_00_7;
}

BOOL ov45_0222A33C(const WiFiPlazaSystem *param0) {
    if ((param0->unk_1FC.unk_00_1 == 1) && (param0->unk_1FC.unk_04 <= 0)) {
        return 1;
    } else {
        return 0;
    }
}

int ov45_0222A35C(const WiFiPlazaSystem *param0) {
    if (param0->unk_1FC.unk_08 <= 0) {
        return param0->unk_1FC.unk_00_2;
    }

    return 0;
}

BOOL ov45_0222A374(const WiFiPlazaSystem *param0) {
    if ((param0->unk_1FC.unk_00_4 == 1) && (param0->unk_1FC.unk_0A <= 0)) {
        return 1;
    }

    return 0;
}

BOOL ov45_0222A394(const WiFiPlazaSystem *param0) {
    return param0->unk_1FC.unk_00_5;
}

BOOL ov45_0222A3A0(const WiFiPlazaSystem *param0) {
    if ((param0->unk_04.unk_04 == 1) && (param0->unk_1FC.unk_06 <= 0)) {
        return 1;
    }

    return 0;
}
