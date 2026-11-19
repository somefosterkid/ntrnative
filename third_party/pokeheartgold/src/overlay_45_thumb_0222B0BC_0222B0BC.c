#include "global.h"

#include "overlay_45_wifi_plaza_system.h"

BOOL ov45_0222AFF8(const WiFiPlazaSystem *param0);

void ov45_0222BDCC(UnkStruct_ov66_0222FC68 *param0, u32 param1);

void ov45_0222B0BC(WiFiPlazaSystem *param0);

void ov45_0222B0BC(WiFiPlazaSystem *param0) {
    s32 iVar1;

    iVar1 = ov45_0222AFF8(param0);
    if (iVar1 == 1) {
        ov45_0222BDCC(&param0->unk_1C0, param0->unk_1C0.unk_04);
    }
    return;
}
