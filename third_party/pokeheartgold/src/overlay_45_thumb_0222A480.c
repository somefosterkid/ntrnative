#include "global.h"

#include "overlay_45_wifi_plaza_api.h"
#include "constants/heap.h"
#include "save.h"
#include "overlay_45_wifi_plaza_system.h"

s32 ov45_0222A450(s32, s32, s32);

s32 ov45_0222E9E0(void);

void ov45_0222A480(s32 arg0, s32 arg1);

struct PasswordWordBank;
struct PlayerProfile;
struct UnkStruct_0207E060_t;
struct UnkStruct_ov66_0222DFF8_t;
struct UnkStruct_ov66_0223177C_t;
typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;
typedef u16 charcode_t;
typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;
typedef struct PasswordWordBank PasswordWordBank;
void ov45_0222BD24(UnkStruct_ov66_0222FBC8 *param0);
void ov45_0222BD2C(UnkStruct_ov66_0222FBC8 *param0);
void ov45_0222A4A8(WiFiPlazaSystem *param0);
void ov45_0222A4C8(WiFiPlazaSystem *param0, BOOL param1);

void ov45_0222A480(s32 arg0, s32 arg1) {
    ov45_0222A450(arg0, ov45_0222E9E0(), arg1);
}

void ov45_0222A498(const WiFiPlazaSystem *param0, UnkStruct_ov66_0222E294 *param1) {
    MI_CpuCopy8(&param0->unk_3DF, param1, sizeof(UnkStruct_ov66_0222E294));
}

void ov45_0222A4A8(WiFiPlazaSystem *param0) {
    ov45_0222BD24(&param0->unk_3A0);
}

void ov45_0222A4B8(WiFiPlazaSystem *param0) {
    ov45_0222BD2C(&param0->unk_3A0);
}

void ov45_0222A4C8(WiFiPlazaSystem *param0, BOOL param1) {
    param0->unk_1FC.unk_0C = param1;
}
