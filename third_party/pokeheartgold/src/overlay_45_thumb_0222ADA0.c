#include "global.h"
#include "constants/heap.h"
#include "save.h"
#include "overlay_45_wifi_plaza_system.h"

s32 ov45_0222ADA0(void);

struct PasswordWordBank;
struct PlayerProfile;
struct UnkStruct_0207E060_t;
struct UnkStruct_ov66_0222DFF8_t;
struct UnkStruct_ov66_0223177C_t;
typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;
typedef u16 charcode_t;
typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;
typedef struct PasswordWordBank PasswordWordBank;
int ov45_0222C5B4(const UnkStruct_ov66_022302B0 *param0, u32 param1);
u32 ov45_0222C408(UnkStruct_ov66_022302B0 *param0, u32 param1, int param2);
int ov45_0222C480(const UnkStruct_ov66_022302B0 *param0, u32 param1);
int ov45_0222C514(const UnkStruct_ov66_022302B0 *param0, u32 param1);
int ov45_0222C54C(const UnkStruct_ov66_022302B0 *param0, u32 param1);
int ov45_0222C580(const UnkStruct_ov66_022302B0 *param0, u32 param1);
static inline void inline_ov66_0222EC04(u32 param0, u32 *param1, u32 *param2);
u32 ov45_0222C5E8(UnkStruct_ov66_022302B0 *param0, u32 param1, int param2);
int ov45_0222C658(const UnkStruct_ov66_022302B0 *param0, u32 param1);
int ov45_0222ADA8(const WiFiPlazaSystem *param0, u32 param1);
u32 ov45_0222ADB8(WiFiPlazaSystem *param0, u32 param1, int param2);
int ov45_0222ADC8(const WiFiPlazaSystem *param0, u32 param1);
int ov45_0222ADD8(const WiFiPlazaSystem *param0, u32 param1);
int ov45_0222ADE8(const WiFiPlazaSystem *param0, u32 param1);
int ov45_0222ADF8(const WiFiPlazaSystem *param0, u32 param1);
void ov45_0222AE08(u32 param0, u32 *param1, u32 *param2);
u32 ov45_0222AE24(WiFiPlazaSystem *param0, u32 param1, int param2);
int ov45_0222AE34(const WiFiPlazaSystem *param0, u32 param1);

s32 ov45_0222ADA0(void) {
    return 0x4B0;
}

static inline void inline_ov66_0222EC04(u32 param0, u32 *param1, u32 *param2) {
    *param1 = param0 / 3;
    *param2 = param0 % 3;
}

int ov45_0222ADA8(const WiFiPlazaSystem *param0, u32 param1) {
    return ov45_0222C5B4(&param0->unk_20C, param1);
}

u32 ov45_0222ADB8(WiFiPlazaSystem *param0, u32 param1, int param2) {
    return ov45_0222C408(&param0->unk_20C, param1, param2);
}

int ov45_0222ADC8(const WiFiPlazaSystem *param0, u32 param1) {
    return ov45_0222C480(&param0->unk_20C, param1);
}

int ov45_0222ADD8(const WiFiPlazaSystem *param0, u32 param1) {
    return ov45_0222C514(&param0->unk_20C, param1);
}

int ov45_0222ADE8(const WiFiPlazaSystem *param0, u32 param1) {
    return ov45_0222C54C(&param0->unk_20C, param1);
}

int ov45_0222ADF8(const WiFiPlazaSystem *param0, u32 param1) {
    return ov45_0222C580(&param0->unk_20C, param1);
}

void ov45_0222AE08(u32 param0, u32 *param1, u32 *param2) {
    inline_ov66_0222EC04(param0, param1, param2);
}

u32 ov45_0222AE24(WiFiPlazaSystem *param0, u32 param1, int param2) {
    return ov45_0222C5E8(&param0->unk_20C, param1, param2);
}

int ov45_0222AE34(const WiFiPlazaSystem *param0, u32 param1) {
    return ov45_0222C658(&param0->unk_20C, param1);
}
