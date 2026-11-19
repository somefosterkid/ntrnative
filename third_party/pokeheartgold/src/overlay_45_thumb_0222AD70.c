#include "global.h"
#include "constants/heap.h"
#include "save.h"
#include "overlay_45_wifi_plaza_system.h"

s32 ov45_0222EEF0(s32, s32 *, s32);

void ov45_0222AD70(void *arg0, s32 arg1);

struct PasswordWordBank;
struct PlayerProfile;
struct UnkStruct_0207E060_t;
struct UnkStruct_ov66_0222DFF8_t;
struct UnkStruct_ov66_0223177C_t;
typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;
typedef u16 charcode_t;
typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;
typedef struct PasswordWordBank PasswordWordBank;
int ov45_0222C4E4(const UnkStruct_ov66_022302B0 *param0, u32 param1);
int ov45_0222C4FC(const UnkStruct_ov66_022302B0 *param0, u32 param1);
int ov45_0222AD80(const WiFiPlazaSystem *param0, u32 param1);
int ov45_0222AD90(const WiFiPlazaSystem *param0, u32 param1);

void ov45_0222AD70(void *arg0, s32 arg1) {
    s32 v0;

    v0 = arg1;
    ov45_0222EEF0(5, &v0, 4);
}

int ov45_0222AD80(const WiFiPlazaSystem *param0, u32 param1) {
    return ov45_0222C4E4(&param0->unk_20C, param1);
}

int ov45_0222AD90(const WiFiPlazaSystem *param0, u32 param1) {
    return ov45_0222C4FC(&param0->unk_20C, param1);
}
