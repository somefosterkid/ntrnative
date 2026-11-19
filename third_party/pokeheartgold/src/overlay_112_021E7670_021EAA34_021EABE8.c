#include "global.h"

#include "pokemon_icon_idx.h"
#include "sound.h"
#include "sound_02004A44.h"
#include "sprite_system.h"

void ov112_021EABE8(void *arg0, u32 arg1, u32 arg2, u32 arg3, s32 arg4, ManagedSprite *arg5);

void ov112_021EABE8(void *arg0, u32 species, u32 form, u32 isEgg, s32 arg4, ManagedSprite *managedSprite) {
    GX_LoadOBJ(*(void **)((u8 *)arg0 + 0x14), ((arg4 << 5) + 0xC0) << 5, 0x400U);
    ManagedSprite_SetPaletteOverride(managedSprite, GetMonIconPaletteEx(species, form, isEgg) + 5);
}

s32 ov112_021EAC18(void);

s32 ov112_021EAC18(void) {
    Sound_SetSceneAndPlayBGM(0x49U, 0x497U, 0);
    PlayBGM(0x497U);
    return 2;
}
