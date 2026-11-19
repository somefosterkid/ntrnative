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
BOOL ov45_0222C9D0(const UnkStruct_ov66_022308A0 *param0, u32 param1);
void *ov45_0222C9EC(const UnkStruct_ov66_022308A0 *param0, u32 param1);

BOOL ov45_0222AB48(const WiFiPlazaSystem *param0, u32 param1);
void *ov45_0222AB58(const WiFiPlazaSystem *param0, u32 param1);

BOOL ov45_0222AB48(const WiFiPlazaSystem *param0, u32 param1) {
    return ov45_0222C9D0(&param0->unk_3E4, param1);
}

void *ov45_0222AB58(const WiFiPlazaSystem *param0, u32 param1) {
    return ov45_0222C9EC(&param0->unk_3E4, param1);
}
