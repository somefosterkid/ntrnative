#include "global.h"

#include "pokedex.h"
#include "pokewalker.h"
#include "overlay_112_pokewalker_app.h"

extern s32 ov112_021F4138;

s32 ov112_021E9610(Ov112PokewalkerApp *arg0, u32 arg1) {
    int i;
    int count;
    u32 unlockFrame;
    if (Pokedex_GetNatDexFlag(Save_Pokedex_Get(arg0->saveData)) != 0) {
        count = 0x14;
    } else {
        count = 8;
    }
    for (i = 0; i < count; i++) {
        if (Pokewalker_CourseIsUnlocked(arg0->pokewalker, i) == 0) {
            unlockFrame = *((u32 *)(((u8 *)&ov112_021F4138) + (i * 0xC0)));
            if (unlockFrame > arg1) {
                return unlockFrame - arg1;
            }
            return 0;
        }
    }
    return -1;
}
