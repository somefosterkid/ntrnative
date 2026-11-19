#include "global.h"

#include "pokedex.h"
#include "pokewalker.h"

extern s32 ov112_021F4138;

s32 ov112_021E9610(void *arg0, u32 arg1);

s32 ov112_021E9610(void *arg0, u32 arg1) {
    int i;
    int count;
    u32 unlockFrame;
    if (Pokedex_GetNatDexFlag(Save_Pokedex_Get(*((SaveData **)(((u8 *)arg0) + 0x20)))) != 0) {
        count = 0x14;
    } else {
        count = 8;
    }
    for (i = 0; i < count; i++) {
        if (Pokewalker_CourseIsUnlocked(*((POKEWALKER **)((u8 *)arg0 + 0x1E440)), i) == 0) {
            unlockFrame = *((u32 *)(((u8 *)&ov112_021F4138) + (i * 0xC0)));
            if (unlockFrame > arg1) {
                return unlockFrame - arg1;
            }
            return 0;
        }
    }
    return -1;
}
