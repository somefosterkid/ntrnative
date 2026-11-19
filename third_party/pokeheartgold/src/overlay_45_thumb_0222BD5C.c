#include "global.h"
#include "constants/heap.h"
#include "save.h"
#include "overlay_45_wifi_plaza_system.h"

void ov45_0222BE54(void *);

void ov45_0222BD5C(u8 *param_1);

struct PasswordWordBank;
struct PlayerProfile;
struct UnkStruct_0207E060_t;
struct UnkStruct_ov66_0222DFF8_t;
struct UnkStruct_ov66_0223177C_t;
typedef u16 charcode_t;
typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;
typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;
void ov45_0222EF4C(u32 param0, s32 param1, const void *param2, u32 param3);
typedef struct PasswordWordBank PasswordWordBank;
BOOL ov45_0222BD74(const UnkStruct_ov66_0222FC68 *param0, u32 param1);
void ov45_0222BD94(UnkStruct_ov66_0222FC68 *param0, u32 param1);
void ov45_0222BDB0(UnkStruct_ov66_0222FC68 *param0, u32 param1);
void ov45_0222BDCC(UnkStruct_ov66_0222FC68 *param0, u32 param1);
void ov45_0222BDE8(UnkStruct_ov66_0222FC68 *param0, u32 param1);
void ov45_0222BE00(UnkStruct_ov66_0222FC68 *param0, u16 param1);
void ov45_0222BE08(WiFiPlazaSystem *param0, s32 param1, BOOL param2);
void ov45_0222BE28(WiFiPlazaSystem *param0, s32 param1);

void ov45_0222BD5C(u8 *param_1) {
    *(u16 *)(param_1 + 4) = 0;
    *(u8 *)(param_1 + 6) = 4;
    ov45_0222BE54(param_1);
    *(u8 *)(param_1 + 10) = 0;
    return;
}

BOOL ov45_0222BD74(const UnkStruct_ov66_0222FC68 *param0, u32 param1) {
    ((param1 < 20) ? (void)0 : GF_AssertFail());

    if (param0->unk_00 & (1 << param1)) {
        return 1;
    }

    return 0;
}

void ov45_0222BD94(UnkStruct_ov66_0222FC68 *param0, u32 param1) {
    ((param1 < 20) ? (void)0 : GF_AssertFail());
    param0->unk_00 |= (1 << param1);
}

void ov45_0222BDB0(UnkStruct_ov66_0222FC68 *param0, u32 param1) {
    ((param1 < 20) ? (void)0 : GF_AssertFail());
    param0->unk_00 &= ~(1 << param1);
}

void ov45_0222BDCC(UnkStruct_ov66_0222FC68 *param0, u32 param1) {
    ((param1 < 20) ? (void)0 : GF_AssertFail());

    if ((param0->unk_0C[param1] + 1) <= 6) {
        param0->unk_0C[param1]++;
    }
}

void ov45_0222BDE8(UnkStruct_ov66_0222FC68 *param0, u32 param1) {
    ((param1 < 20) ? (void)0 : GF_AssertFail());
    param0->unk_0C[param1] = 0;
}

void ov45_0222BE00(UnkStruct_ov66_0222FC68 *param0, u16 param1) {
    param0->unk_20.unk_02 = param0->unk_06;
    param0->unk_20.unk_00 = param1;
}

void ov45_0222BE08(WiFiPlazaSystem *param0, s32 param1, BOOL param2) {
    param0->unk_1C0.unk_20.unk_02 = 2;
    param0->unk_1C0.unk_20.unk_00 = param2;

    ov45_0222EF4C(1, param1, &param0->unk_1C0.unk_20, sizeof(UnkStruct_ov66_022305B0));
}

void ov45_0222BE28(WiFiPlazaSystem *param0, s32 param1) {
    param0->unk_1C0.unk_06 = 4;
    param0->unk_1C0.unk_20.unk_02 = 4;

    ov45_0222EF4C(2, param1, &param0->unk_1C0.unk_20, sizeof(UnkStruct_ov66_022305B0));
}
