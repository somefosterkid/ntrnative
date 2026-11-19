#include "global.h"

#include "field_system.h"
#include "overlay_01_021E90C0.h"

void ov01_021E90DC(u32 x, u32 y, FieldEnvSubUnk18 *a2) {
    MapPropDoorAnimEnv *env = (MapPropDoorAnimEnv *)a2;

    env->base.direction = x;
    env->unk_08 = y;
}
