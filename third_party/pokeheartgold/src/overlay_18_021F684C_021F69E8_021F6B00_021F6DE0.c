#include "global.h"

#include "constants/heap.h"

#include "application/pokedex/pokedex_internal.h"

#include "touchscreen.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"

void ov18_021F6E58(PokedexAppData *, s32);

extern GridCallbacks ov18_021FB688;

extern TouchscreenHitbox ov18_021FB878;

extern DpadMenuBox ov18_021FB9F0;

void ov18_021F6DE0(PokedexAppData *pokedexApp, int a1) {
    pokedexApp->unk_1864 = GridInputHandler_Create(&ov18_021FB878, &ov18_021FB9F0, &ov18_021FB688, pokedexApp, 1, (u8)(s32)(s32)a1, HEAP_ID_POKEDEX_APP);
    ov18_021F11C0(pokedexApp, 0, 1);
    ov18_021F6E58(pokedexApp, a1);
}
