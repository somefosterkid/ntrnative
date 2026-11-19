#include "global.h"
#include "constants/heap.h"
#include "save.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_88_unk_struct_ov66_02231428.h"
#include "overlay_45_unk_struct_ov66_022309a4.h"
#include "overlay_45_unk_struct_ov66_02230a6c.h"
#include "overlay_45_wifi_plaza_system.h"

u32 ov45_0222ECDC(s32);

u32 ov45_0222AD2C(void);

struct PasswordWordBank;
struct PlayerProfile;
struct UnkStruct_0207E060_t;
struct UnkStruct_ov66_0222DFF8_t;
struct UnkStruct_ov66_0223177C_t;
typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;
typedef u16 charcode_t;
typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;
typedef struct PasswordWordBank PasswordWordBank;
BOOL ov45_0222AD58(const WiFiPlazaSystem *param0, u32 param1);

u32 ov45_0222AD2C(void) {
    u32 v0;

    v0 = ov45_0222ECDC(3);
    if (v0 >= 5U) {
        v0 = 0;
    }
    return v0;
}

u32 ov45_0222AD3C(void);

u32 ov45_0222AD3C(void) {
    u32 v0;

    v0 = ov45_0222ECDC(2);
    if (v0 >= 5U) {
        v0 = 0;
    }
    return v0;
}

s32 ov45_0222AD4C(void);

s32 ov45_0222AD4C(void) {
    return ov45_0222ECDC(5) + 0x1E;
}

BOOL ov45_0222AD58(const WiFiPlazaSystem *param0, u32 param1) {
    ((param1 < 20) ? (void)0 : GF_AssertFail());
    return param0->unk_1E8.unk_00[param1];
}
