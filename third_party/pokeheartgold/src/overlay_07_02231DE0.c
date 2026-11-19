#include "global.h"
#include "overlay_07_battle_anim_system.h"

BOOL ov07_02231AD0(const char *key, u32 *numEntry, BOOL a2, int a3);

BOOL ov07_02231DE0(const char *key, void *numEntry, BOOL a2, int a3);

void sub_02015510(VecFx32 *up);
extern const VecFx32 ov07_02236858;
void ov07_02231DE8(int battler, VecFx32 *up);

struct BattleAnimSystem;
int ov07_0221FAF8(BattleAnimSystem *system, int bg);
void ov07_02231E08(BattleAnimSystem *system, int spriteAlpha, int bgAlpha);

void ov07_02231E44(BattleAnimSystem *system, int planes, int spriteAlpha, int otherAlpha);

BOOL ov07_02231DE0(const char *key, void *numEntry, BOOL a2, int a3) {
    return ov07_02231AD0(key, numEntry, a2, a3);
}

void ov07_02231DE8(int battler, VecFx32 *up) {
    if (battler == 0) {
        sub_02015510(up);
    } else {
        *up = ov07_02236858;
    }
}

void ov07_02231E08(BattleAnimSystem *system, int spriteAlpha, int bgAlpha) {
    if (spriteAlpha == -1) {
        spriteAlpha = 26;
    }

    if (bgAlpha == -1) {
        bgAlpha = 5;
    }

    G2_SetBlendAlpha(
        GX_BLEND_PLANEMASK_NONE,
        (1 << ov07_0221FAF8(system, 2)) | GX_BLEND_PLANEMASK_BG0,
        spriteAlpha,
        bgAlpha);
}

void ov07_02231E44(BattleAnimSystem *system, int planes, int spriteAlpha, int otherAlpha) {
    if (spriteAlpha == -1) {
        spriteAlpha = 26;
    }

    if (otherAlpha == -1) {
        otherAlpha = 5;
    }

    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, planes, spriteAlpha, otherAlpha);
}
