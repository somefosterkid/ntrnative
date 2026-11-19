#include "global.h"
#include "constants/heap.h"
#include "save.h"
#include "unk_struct_ov66_02232B20.h"
#include "overlay_45_wifi_plaza_system.h"

void ov45_0222EC3C(UnkStruct_ov66_02232B20 *arg0);

s32 ov45_0222AB68(void);

u32 ov45_0222EC68(s32 arg0);
s32 ov45_0222ECA4(u32);
u32 ov45_0222AB78(void *param_1, u32 param_2);

struct PasswordWordBank;
struct PlayerProfile;
struct UnkStruct_0207E060_t;
struct UnkStruct_ov66_0222DFF8_t;
struct UnkStruct_ov66_0223177C_t;
typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;
typedef u16 charcode_t;
typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;
s32 ov45_0222EC90(u32 param0);
void ov45_0222EEF0(u32 param0, const void *param1, u32 param2);
typedef struct {
    s32 unk_00[4];
    u8 unk_10;
    u8 unk_11;
    u8 unk_12;
    u8 unk_13_0 : 7;
    u8 unk_13_7 : 1;
} UnkStruct_ov66_0222E990;
typedef struct PasswordWordBank PasswordWordBank;
void ov45_0222AB94(WiFiPlazaSystem *param0, u32 param1, u32 param2);
void ov45_0222ABD0(WiFiPlazaSystem *param0, u32 param1, u32 param2, u32 param3);

s32 ov45_0222AB68(void) {
    UnkStruct_ov66_02232B20 v0;
    ov45_0222EC3C(&v0);
    return v0.unk_00;
}

u32 ov45_0222AB78(void *param_1, u32 param_2) {
    s32 iVar1;
    u32 uVar2;

    iVar1 = ov45_0222ECA4(param_2);
    uVar2 = 0xffffffff;
    if (iVar1 != -1) {
        uVar2 = ov45_0222EC68(iVar1);
    }
    return uVar2;
}

void ov45_0222AB94(WiFiPlazaSystem *param0, u32 param1, u32 param2) {
    UnkStruct_ov66_0222E990 v0 = { 0 };

    v0.unk_11 = 0;
    v0.unk_00[0] = ov45_0222EC90(param1);
    v0.unk_00[1] = ov45_0222EC90(param2);
    v0.unk_10 = 2;

    ov45_0222EEF0(4, &v0, sizeof(UnkStruct_ov66_0222E990));
}

void ov45_0222ABD0(WiFiPlazaSystem *param0, u32 param1, u32 param2, u32 param3) {
    UnkStruct_ov66_0222E990 v0 = { 0 };

    v0.unk_11 = 1;
    v0.unk_00[0] = ov45_0222EC90(param2);
    v0.unk_00[1] = ov45_0222EC90(param1);
    v0.unk_10 = 2;
    v0.unk_12 = param3;

    ov45_0222EEF0(4, &v0, sizeof(UnkStruct_ov66_0222E990));
}
