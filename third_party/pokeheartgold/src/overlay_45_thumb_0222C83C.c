#include "global.h"

#include "constants/heap.h"

#include "save.h"
#include "overlay_45_wifi_plaza_system.h"

struct PasswordWordBank;

struct PlayerProfile;

struct UnkStruct_0207E060_t;

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov66_0223177C_t;

typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;

typedef u16 charcode_t;

typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;

u32 ov45_0222EC68(s32 param0);

typedef struct PasswordWordBank PasswordWordBank;

void ov45_0222C83C(s32 param0, const void *param1, u32 param2, void *param3);

u32 ov45_0222C408(UnkStruct_ov66_022302B0 *param0, u32 param1, int param2);
void ov45_0222C858(s32 param0, const void *param1, u32 param2, void *param3);
void ov45_0222C514(UnkStruct_ov66_022302B0 *param0, u32 param1);
void ov45_0222C88C(s32 param0, const void *param1, u32 param2, void *param3);
void ov45_0222C8AC(UnkStruct_ov66_022307D4 *param0);

void ov45_0222C83C(s32 param0, const void *param1, u32 param2, void *param3) {
    WiFiPlazaSystem *v0 = param3;
    u32 v1 = ov45_0222EC68(param0);

    if (v1 != 0xffffffff) {
        v0->unk_1E8.unk_00[v1] = 1;
    }
}

void ov45_0222C858(s32 param0, const void *param1, u32 param2, void *param3) {
    WiFiPlazaSystem *v0 = param3;
    u32 v1;
    u32 v2;
    int v3;

    v1 = ov45_0222EC68(param0);

    if (v1 != 0xffffffff) {
        for (v3 = 0; v3 < 3; v3++) {
            v2 = ov45_0222C408(&v0->unk_20C, v1, v3);

            if (v2 != 0xffffffff) {
                break;
            }
        }
    }
}

void ov45_0222C88C(s32 param0, const void *param1, u32 param2, void *param3) {
    WiFiPlazaSystem *v0 = param3;
    u32 v1 = ov45_0222EC68(param0);

    if (v1 != 0xffffffff) {
        ov45_0222C514(&v0->unk_20C, v1);
    }
}

void ov45_0222C8AC(UnkStruct_ov66_022307D4 *param0) {
    int v0;

    memset(param0, 0, sizeof(UnkStruct_ov66_022307D4));

    for (v0 = 0; v0 < (27 / 3); v0++) {
        param0->unk_00[v0] = 100;
    }
}
