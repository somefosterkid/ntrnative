#include "global.h"

#include "error_handling.h"
#include "field_types_def.h"
#include "follow_mon.h"
#include "map_object.h"
#include "player_avatar.h"
#include "overlay_02_field3d_effects.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov02_0224DF1C(Ov02App_0224DCB0 *env) {
    VecFx32 playerPos;
    VecFx32 followPos;
    int i;
    int j;
    int k;

    env->offsetY = FX32_ONE;
    env->offsetZ = 3 * FX32_ONE;
    PlayerAvatar_CopyPositionVector(env->fieldSystem->playerAvatar, &playerPos);
    if (env->followMonEnabled != 0) {
        MapObject_CopyPositionVector(FollowMon_GetMapObject(env->fieldSystem), &followPos);
        for (i = 0; i < 18; i++) {
            if (env->slots[i].playing == 0) {
                ov02_0224DE10(&env->slots[i], &playerPos, env->offsetY, env->offsetZ);
                break;
            }
        }
        if (i == 18) {
            GF_AssertFail();
        }
        for (j = 0; j < 18; j++) {
            if (env->slots[j].playing == 0) {
                ov02_0224DE10(&env->slots[j], &followPos, env->offsetY, env->offsetZ);
                return;
            }
        }
    } else {
        for (k = 0; k < 18; k++) {
            if (env->slots[k].playing == 0) {
                ov02_0224DE10(&env->slots[k], &playerPos, env->offsetY, env->offsetZ);
                return;
            }
        }
    }
    GF_AssertFail();
}
#else
// DS form: the plain-loop form above allocates the object pointer and the first index to r5/r4 the other way round.
void ov02_0224DF1C(Ov02App_0224DCB0 *arg0) {
    VecFx32 spC;
    VecFx32 sp0;
    s32 var_r5;
    Ov02App_0224DCB0 *var_r4;
    Ov02App_0224DCB0 *var_r2;
    Ov02App_0224DCB0 *var_r2_2;
    s32 var_r0;
    s32 var_r0_2;

    var_r4 = arg0;
    var_r4->offsetY = 0x1000;
    var_r4->offsetZ = 0x3000;
    PlayerAvatar_CopyPositionVector(var_r4->fieldSystem->playerAvatar, &spC);
    if (var_r4->followMonEnabled != 0) {
        MapObject_CopyPositionVector(FollowMon_GetMapObject(var_r4->fieldSystem), &sp0);
        var_r5 = 0;
loop_2:
        if (var_r4->slots[var_r5].playing == 0) {
            ov02_0224DE10(&var_r4->slots[var_r5], &spC, var_r4->offsetY, var_r4->offsetZ);
        } else {
            var_r5 += 1;
            if (var_r5 < 0x12) {
                goto loop_2;
            }
        }
        if (var_r5 == 0x12) {
            GF_AssertFail();
        }
        var_r0 = 0;
        var_r2 = var_r4;
loop_8:
        if (*(u32 *)((u8 *)var_r2 + offsetof(Ov02App_0224DCB0, slots[0].playing)) == 0) {
            ov02_0224DE10(&var_r4->slots[var_r0], &sp0, var_r4->offsetY, var_r4->offsetZ);
            return;
        }
        var_r0 += 1;
        var_r2 = (Ov02App_0224DCB0 *)((u8 *)var_r2 + sizeof(Ov02Field3dEffectSlot));
        if (var_r0 < 0x12) {
            goto loop_8;
        }
        goto fail;
    }
    var_r0_2 = 0;
    var_r2_2 = var_r4;
loop_13:
    if (*(u32 *)((u8 *)var_r2_2 + offsetof(Ov02App_0224DCB0, slots[0].playing)) == 0) {
        ov02_0224DE10(&var_r4->slots[var_r0_2], &spC, var_r4->offsetY, var_r4->offsetZ);
        return;
    }
    var_r0_2 += 1;
    var_r2_2 = (Ov02App_0224DCB0 *)((u8 *)var_r2_2 + sizeof(Ov02Field3dEffectSlot));
    if (var_r0_2 < 0x12) {
        goto loop_13;
    }
    goto fail;
fail:
    GF_AssertFail();
}
#endif
