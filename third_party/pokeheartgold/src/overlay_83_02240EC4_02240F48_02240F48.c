#include "global.h"

#include "save.h"
#include "overlay_83_battle_castle_self_app.h"

void ov83_0224777C(SaveData *saveData, u8 challengeType, u8 arg2);

extern const u16 ov83_02247EE0[];

extern const u16 ov83_02247F88[];

u16 ov83_02240F48(BattleCastleSelfApp *app, s32 param_2, s32 param_3);

u16 ov83_02240F48(BattleCastleSelfApp *app, s32 param_2, s32 param_3) {
    ov83_0224777C(app->saveData, app->challengeType, 1);
    if (param_3 == 6) {
        return ov83_02247F88[param_2];
    }
    return ov83_02247EE0[param_2];
}
