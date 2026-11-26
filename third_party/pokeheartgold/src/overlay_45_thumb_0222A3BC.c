#include "global.h"

#include "overlay_45_wifi_plaza_api.h"
#include "constants/heap.h"
#include "save.h"
#include "overlay_45_wifi_plaza_system.h"

u8 ov45_0222A3BC(WiFiPlazaSystem *arg0);

struct PasswordWordBank;
struct PlayerProfile;
struct UnkStruct_0207E060_t;
struct UnkStruct_ov66_0222DFF8_t;
struct UnkStruct_ov66_0223177C_t;
typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;
typedef u16 charcode_t;
typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;
typedef struct PasswordWordBank PasswordWordBank;
void ov45_0222BCA0(UnkStruct_ov66_0222FBC8 *param0);
void ov45_0222BCA8(UnkStruct_ov66_0222FBC8 *param0);
u32 ov45_0222EC68(s32 param0);
void ov45_0222A430(WiFiPlazaSystem *param0, BOOL param1);
void ov45_0222A450(WiFiPlazaSystem *param0, s32 param1, u32 param2);

u8 ov45_0222A3BC(WiFiPlazaSystem *arg0) {
    if ((s32)arg0->unk_1FC.unk_06 <= 0) {
        return arg0->unk_1FC.unk_01;
    }
    return 1U;
}

u8 ov45_0222A3D4(WiFiPlazaSystem *arg0);

u8 ov45_0222A3D4(WiFiPlazaSystem *arg0) {
    if ((s32)arg0->unk_1FC.unk_06 <= 0) {
        return arg0->unk_1FC.unk_02;
    }
    return 7U;
}

u8 ov45_0222A3EC(WiFiPlazaSystem *arg0);

u8 ov45_0222A3EC(WiFiPlazaSystem *arg0) {
    if ((s32)arg0->unk_1FC.unk_06 <= 0) {
        return arg0->unk_1FC.unk_03;
    }
    return 0xBU;
}

void ov45_0222A404(WiFiPlazaSystem *param0) {
    ov45_0222BCA0(&param0->unk_3A0);
}

void ov45_0222A414(WiFiPlazaSystem *param0) {
    ov45_0222BCA8(&param0->unk_3A0);
}

BOOL ov45_0222A424(const WiFiPlazaSystem *param0) {
    return param0->unk_508.unk_04;
}

void ov45_0222A430(WiFiPlazaSystem *param0, BOOL param1) {
    param0->unk_508.unk_04 = param1;
}

void ov45_0222A43C(WiFiPlazaSystem *param0) {
    MI_CpuClear32(&param0->unk_3DF, sizeof(UnkStruct_ov66_0222E294));
}

void ov45_0222A450(WiFiPlazaSystem *param0, s32 param1, u32 param2) {
    u32 v0;

    ((param2 < 4) ? (void)0 : GF_AssertFail());
    v0 = ov45_0222EC68(param1);
    ((v0 != 0xffffffff) ? (void)0 : GF_AssertFail());

    param0->unk_3DF.unk_00[param2] = v0;
}
