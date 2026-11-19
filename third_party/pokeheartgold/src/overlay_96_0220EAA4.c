#include "global.h"

#include "sprite_system.h"
#include "overlay_96_unk_struct_ov96_0220e3f0.h"
#include "overlay_96_app_0221040c.h"

void ov96_021E9C0C(void *param_1);

void ov96_021EA894(void *param_1);

void ov96_0220EAA4(UnkStruct_ov96_0220E3F0 *param_1);

void ov96_0220EAA4(UnkStruct_ov96_0220E3F0 *param_1) {
    ov96_021EA894(param_1->unk_20);
    ov96_021E9C0C(param_1->unk_1C);
    SpriteSystem_FreeResourcesAndManager(param_1->spriteSystem, param_1->spriteManager);
    SpriteSystem_Free(param_1->spriteSystem);
    return;
}

s32 ov96_021EB144(struct Ov96SpriteSet *, s32);

void ov96_0220EAC4(UnkStruct_ov96_0220E3F0 *arg0);

void ov96_0220EAC4(UnkStruct_ov96_0220E3F0 *arg0) {
    UnkStruct_ov96_0220E3F0_HudGroup *v2;
    s32 v1;
    UnkStruct_ov96_0220E3F0 *v0;
    s32 v3;
    s32 v4;
    s32 v5;
    ManagedSprite **v7;

    v0 = arg0;
    ov96_021EB144(arg0->unk_20, 1);
    ov96_0221031C(arg0->unk_C4);
    v4 = 0;
    do {
        ManagedSprite_SetAnimateFlag(v0->hud.slots[v4].sprite, 0);
        v4 += 1;
    } while (v4 < 12);
    v1 = 0;
    v2 = v0->hud.groups;
    do {
        v7 = v2->unk_0;
        v5 = 0;
loop_4:
        ManagedSprite_SetAnimateFlag(v7[0], 0);
        ManagedSprite_SetAnimateFlag(v7[2], 0);
        v5 += 1;
        v7++;
        if (v5 < 2) {
            goto loop_4;
        }
        v2++;
        v3 = v1 + 1;
        v1 = v3;
    } while (v3 < 2);
}
