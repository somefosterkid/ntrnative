#include "global.h"

#include "pokedex.h"
#include "touchscreen.h"

#include "application/pokedex/pokedex_internal.h"


extern u32 ov18_021FB698;

extern u32 ov18_021FB704;


u32 ov18_021F7B94(PokedexAppData *param_1, u32 *param_2) {
    BOOL BVar1;
    s32 iVar2;
    u32 uStack_14;
    u32 uStack_18;

    BVar1 = System_GetTouchNew();
    if (BVar1 == 1) {
        iVar2 = TouchscreenHitbox_FindRectAtTouchNew((TouchscreenHitbox *)&ov18_021FB704);
        if (iVar2 != -1) {
            System_GetTouchNewCoords(&uStack_14, &uStack_18);
            iVar2 = *(s32 *)(&ov18_021FB698 + iVar2);
            if ((iVar2 == 1) && (BVar1 = Pokedex_CheckMonCaughtFlag(param_1->args->pokedex, param_1->curSpecies), BVar1 == 0)) {
                return -1;
            }
            *param_2 = 1;
            return iVar2;
        }
        return -1;
    } else if ((gSystem.newKeys & 2) != 0) {
        *param_2 = 0;
        return 3;
    } else {
        return -1;
    }
}
