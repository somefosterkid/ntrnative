#include "global.h"
struct SignatureApp;

#include "sprite.h"
#include "unk_02005D10.h"
#include "signature_app.h"

void ov52_021E9424(s8 *param_1);

void ov52_021E9424(s8 *param_1) {
    BOOL BVar1;

    if (((*param_1 == 0) && (param_1[1] == 0)) && (BVar1 = IsSEPlaying(0x699), BVar1 == 0)) {
        PlaySE(0x699);
    }
    if ((((s32)*param_1 * (s32)param_1[2] < 0) || ((s32)param_1[1] * (s32)param_1[3] < 0)) && (BVar1 = IsSEPlaying(0x699), BVar1 == 0)) {
        PlaySE(0x699);
    }
    *param_1 = param_1[2];
    param_1[1] = param_1[3];
    param_1[2] = 0;
    param_1[3] = 0;
    return;
}

void ov52_021E9488(struct SignatureApp *arg0, s32 arg1);

void ov52_021E9488(SignatureApp *arg0, s32 arg1) {
    if (arg1 == 1) {
        Sprite_SetDrawFlag(arg0->sprite[0], 0);
        return;
    }
    Sprite_SetDrawFlag(arg0->sprite[0], 1);
}
