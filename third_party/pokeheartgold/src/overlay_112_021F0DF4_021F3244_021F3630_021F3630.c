#include "global.h"

#include "save_vars_flags.h"

extern u16 ov112_021FF4D4;

void ov112_021F3630(SaveData *arg0, u8 *arg1);

void ov112_021F3630(SaveData *saveData, u8 *arg1) {
    SaveVarsFlags *varsFlags;
    s32 v0;
    u16 *v1;

    varsFlags = Save_VarsFlags_Get(saveData);
    v1 = &ov112_021FF4D4;
    v0 = 0;
    do {
        if (*(u8 *)(arg1 + v0) != 0) {
            Save_VarsFlags_SetFlagInArray(varsFlags, *v1);
        } else {
            Save_VarsFlags_ClearFlagInArray(varsFlags, *v1);
        }
        v0 += 1;
        v1 += 1;
    } while (v0 < 0xA);
}
