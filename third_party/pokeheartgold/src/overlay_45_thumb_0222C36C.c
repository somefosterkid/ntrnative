#include "global.h"

#include "player_data.h"
#include "constants/heap.h"
#include "save.h"
#include "overlay_45_unk_struct_ov66_022309a4.h"
#include "overlay_45_unk_struct_ov66_02230a6c.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_45_wifi_plaza_system.h"

void ov45_0222C36C(void);

struct PasswordWordBank;
struct UnkStruct_0207E060_t;
struct UnkStruct_ov66_0222DFF8_t;
struct UnkStruct_ov66_0223177C_t;
typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;
typedef u16 charcode_t;
typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;
typedef struct {
    const UnkStruct_ov66_02231428 *unk_00;
    int unk_04;
} UnkStruct_ov66_02231AB4;
void ov45_0222DB98(UnkStruct_ov66_0223177C *param0, const UnkStruct_ov66_02231AB4 *param1);
typedef struct PasswordWordBank PasswordWordBank;
void ov45_0222C370(WiFiPlazaSystem *param0, int param1);

void ov45_0222C388(void *arg0);

void ov45_0222C3A8(s32 *arg0);

void ov45_0222C36C(void) {
}

void ov45_0222C370(WiFiPlazaSystem *param0, int param1) {
    UnkStruct_ov66_02231AB4 v0;

    v0.unk_00 = &param0->unk_04.unk_D0;
    v0.unk_04 = param1;

    ov45_0222DB98(param0->unk_04.unk_00, &v0);
}

void ov45_0222C388(void *arg0) {
    memset(arg0, 0, 0x194U);
    MIi_CpuClear32(-1U, arg0 + 0x50, 0x6CU);
}

void ov45_0222C3A8(s32 *arg0) {
    *arg0 = 1;
}
