#include "global.h"
#include "pokemon.h"
#include "constants/heap.h"
#include "overlay_59_juice_feed_app.h"
#include "pokemon_types_def.h"
#include "sprite.h"

void ov59_0223BE18(Ov59JuiceFeedApp *param_1, Ov59JuiceMon *param_2) {
    u32 uVar1;

    CalcMonPokeathlonStars(&param_2->stars, param_2->mon, (s8 *)&param_2->modifiers, param_1->heapId);
    uVar1 = 0;
    do {
        ov59_0223BD4C(param_1, param_2, uVar1 & 0xff);
        uVar1 = uVar1 + 1;
    } while ((s32)uVar1 < 5);
    return;
}

/* DS form: the typed form below compiles to a different register allocation on mwcc */
#if defined(SDK_PORT) || defined(NONMATCHING)
void ov59_0223BE44(Ov59JuiceFeedApp *param_1, s32 param_2, s32 param_3, BOOL param_4) {
    Sprite **iVar1;
    s32 iVar2;

    iVar1 = param_1->sprites;
    iVar2 = param_2 + 8;
    Sprite_SetDrawFlag(iVar1[iVar2], param_4);
    if (param_3 != 0xff) {
        Sprite_SetAnimCtrlSeq(iVar1[iVar2], param_3);
        Sprite_ResetAnimCtrlState(iVar1[iVar2]);
    }
    return;
}
#else
void ov59_0223BE44(Ov59JuiceFeedApp *app, s32 param_2, s32 param_3, BOOL param_4) {
    u8 *param_1 = (u8 *)app;
    u8 *iVar1;
    s32 iVar2;

    iVar2 = (param_2 + 8) * 4;
    iVar1 = param_1 + 0x260;
    Sprite_SetDrawFlag(*(Sprite **)(iVar1 + iVar2), param_4);
    if (param_3 != 0xff) {
        Sprite_SetAnimCtrlSeq(*(Sprite **)(iVar1 + iVar2), param_3);
        Sprite_ResetAnimCtrlState(*(Sprite **)(iVar1 + iVar2));
    }
    return;
}
#endif
