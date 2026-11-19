#include "global.h"
#include "constants/heap.h"
#include "save.h"
#include "overlay_45_wifi_plaza_system.h"

void ov45_0222BD30(s8 *arg0);

void ov45_0222BE54(UnkStruct_ov66_0222FC68 *param0);
void ov45_0222BD40(UnkStruct_ov66_0222FC68 *param0);
void ov45_0222BD4C(UnkStruct_ov66_0222FC68 *param0);

void ov45_0222BD30(s8 *arg0) {
    u32 v1;
    s8 *v0;

    v0 = arg0;
    v1 = 0x14;
    do {
        *v0 = 0;
        v0 += 1;
        v1 -= 1;
    } while (v1 != 0);
}

void ov45_0222BD40(UnkStruct_ov66_0222FC68 *param0) {
    memset(param0, 0, sizeof(UnkStruct_ov66_0222FC68));
}

void ov45_0222BD4C(UnkStruct_ov66_0222FC68 *param0) {
    param0->unk_04 = 0;
    param0->unk_06 = 0;
    param0->unk_0A = 0;

    ov45_0222BE54(param0);
}
