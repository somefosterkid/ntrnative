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
typedef struct PasswordWordBank PasswordWordBank;
void ov45_0222BDCC(UnkStruct_ov66_0222FC68 *param0, u32 param1);
void ov45_0222BD94(UnkStruct_ov66_0222FC68 *param0, u32 param1);

void ov45_0222B0D8(WiFiPlazaSystem *param0, u32 param1);
void ov45_0222B0E8(WiFiPlazaSystem *param0, u32 param1);

void ov45_0222B0D8(WiFiPlazaSystem *param0, u32 param1) {
    ov45_0222BDCC(&param0->unk_1C0, param1);
}

void ov45_0222B0E8(WiFiPlazaSystem *param0, u32 param1) {
    ov45_0222BD94(&param0->unk_1C0, param1);
}
