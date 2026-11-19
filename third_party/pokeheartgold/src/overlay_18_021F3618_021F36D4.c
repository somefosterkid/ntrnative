#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite.h"
#include "sprite_system.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov18_021F36D4(PokedexAppData *pokedexApp, int a1, int a2, int a3) {
    unsigned int new_var2;
    u32 new_var;
    s16 sStack_1a;
    s16 sStack_1c;
    s32 iStack_18;
    new_var2 = -0xc;
    iStack_18 = a3;
    ManagedSprite_GetPositionXY(pokedexApp->unk_0670[a2], &sStack_1a, &sStack_1c);
    if (a1 == 0) {
        a1 = (s32)sStack_1a;
    } else if (((u32)a1) < 0x34) {
        a1 = 0x34;
    } else if (0xcc < ((u32)a1)) {
        a1 = 0xcc;
    }
    ManagedSprite_GetPositionXY(pokedexApp->unk_0670[a2], &sStack_1a, &sStack_1c);
    ManagedSprite_SetPositionXY(pokedexApp->unk_0670[a2], (s16)a1, sStack_1c);
    ManagedSprite_GetPositionXY(pokedexApp->unk_0670[a2 + 1], &sStack_1a, &sStack_1c);
    ManagedSprite_SetPositionXY(pokedexApp->unk_0670[a2 + 1], (s16)(((u32)((((unsigned int)a1) + (-0x14)) * 0x10000)) >> 0x10), sStack_1c);
    ManagedSprite_SetPositionXY(pokedexApp->unk_0670[a2 + 2], (s16)(((u32)((a1 + new_var2) * 0x10000)) >> 0x10), sStack_1c);
    new_var2 = 0x10000;
    new_var = a1 + 4;
    ManagedSprite_SetPositionXY(pokedexApp->unk_0670[a2 + 3], (s16)(((u32)(new_var * 0x10000)) >> 0x10), sStack_1c);
    ManagedSprite_SetPositionXY(pokedexApp->unk_0670[a2 + 4], (s16)(((u32)((a1 + 0xc) * new_var2)) >> 0x10), sStack_1c);
    if (a3 == 1) {
        ManagedSprite_GetPositionXY(pokedexApp->unk_0670[0], &sStack_1a, &sStack_1c);
        ManagedSprite_SetPositionXY(pokedexApp->unk_0670[0], (s16)a1, sStack_1c);
    }
    return;
}
#else
void ov18_021F36D4(PokedexAppData *pokedexApp, int a1, int a2, int a3) {
    unsigned int new_var2;
    int iVar1;
    u32 new_var;
    s16 sStack_1a;
    s16 sStack_1c;
    s32 iStack_18;
    new_var2 = -0xc;
    iVar1 = a2 * 4;
    iStack_18 = a3;
    ManagedSprite_GetPositionXY(*((ManagedSprite **)(pokedexApp->filler_0770 + iVar1)), &sStack_1a, &sStack_1c);
    if (a1 == 0) {
        a1 = (s32)sStack_1a;
    } else if (((u32)a1) < 0x34) {
        a1 = 0x34;
    } else if (0xcc < ((u32)a1)) {
        a1 = 0xcc;
    }
    ManagedSprite_GetPositionXY(*((ManagedSprite **)(pokedexApp->filler_0770 + iVar1)), &sStack_1a, &sStack_1c);
    ManagedSprite_SetPositionXY(*((ManagedSprite **)(pokedexApp->filler_0770 + iVar1)), (s16)a1, sStack_1c);
    ManagedSprite_GetPositionXY(*((ManagedSprite **)((pokedexApp->filler_0770 + iVar1) + 4)), &sStack_1a, &sStack_1c);
    ManagedSprite_SetPositionXY(*((ManagedSprite **)((pokedexApp->filler_0770 + iVar1) + 4)), (s16)(((u32)((((unsigned int)a1) + (-0x14)) * 0x10000)) >> 0x10), sStack_1c);
    ManagedSprite_SetPositionXY(*((ManagedSprite **)((pokedexApp->filler_0770 + iVar1) + 8)), (s16)(((u32)((a1 + new_var2) * 0x10000)) >> 0x10), sStack_1c);
    new_var2 = 0x10000;
    new_var = a1 + 4;
    ManagedSprite_SetPositionXY(*((ManagedSprite **)((pokedexApp->filler_0770 + iVar1) + 0xc)), (s16)(((u32)(new_var * 0x10000)) >> 0x10), sStack_1c);
    ManagedSprite_SetPositionXY(*((ManagedSprite **)((pokedexApp->filler_0770 + iVar1) + 0x10)), (s16)(((u32)((a1 + 0xc) * new_var2)) >> 0x10), sStack_1c);
    if (a3 == 1) {
        ManagedSprite_GetPositionXY(*((ManagedSprite **)pokedexApp->filler_0770), &sStack_1a, &sStack_1c);
        ManagedSprite_SetPositionXY(*((ManagedSprite **)pokedexApp->filler_0770), (s16)a1, sStack_1c);
    }
    return;
}
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov18_021F37D4(PokedexAppData *pokedexApp, int a1, int a2, int a3) {
    u32 new_var3;
    unsigned int new_var;
    unsigned int new_var2;
    s16 sStack_1a;
    s16 sStack_1c;
    s32 iStack_18;
    iStack_18 = a3;
    ManagedSprite_GetPositionXY(pokedexApp->unk_0670[a2], &sStack_1a, &sStack_1c);
    if (a1 == 0) {
        a1 = (s32)sStack_1a;
    } else if (((u32)a1) < 0x34) {
        a1 = 0x34;
    } else if (0xcc < ((u32)a1)) {
        a1 = 0xcc;
    }
    new_var = 0x10000;
    ManagedSprite_GetPositionXY(pokedexApp->unk_0670[a2], &sStack_1a, &sStack_1c);
    ManagedSprite_SetPositionXY(pokedexApp->unk_0670[a2], (s16)a1, sStack_1c);
    new_var3 = 0x10000;
    ManagedSprite_GetPositionXY(pokedexApp->unk_0670[a2 + 1], &sStack_1a, &sStack_1c);
    new_var2 = a1;
    ManagedSprite_SetPositionXY(pokedexApp->unk_0670[a2 + 1], (s16)(((u32)((new_var2 + (-0x14)) * new_var3)) >> 0x10), sStack_1c);
    ManagedSprite_SetPositionXY(pokedexApp->unk_0670[a2 + 2], (s16)(((u32)((a1 + (-0xc)) * new_var)) >> 0x10), sStack_1c);
    ManagedSprite_SetPositionXY(pokedexApp->unk_0670[a2 + 3], (s16)(((u32)((new_var2 + (-4)) * 0x10000)) >> 0x10), sStack_1c);
    ManagedSprite_SetPositionXY(pokedexApp->unk_0670[a2 + 4], (s16)(((u32)((a1 + 4) * new_var3)) >> 0x10), sStack_1c);
    new_var = 0x10000;
    ManagedSprite_SetPositionXY(pokedexApp->unk_0670[a2 + 5], (s16)(((u32)((a1 + 0x14) * new_var)) >> 0x10), sStack_1c);
    if (a3 == 1) {
        ManagedSprite_GetPositionXY(pokedexApp->unk_0670[0], &sStack_1a, &sStack_1c);
        ManagedSprite_SetPositionXY(pokedexApp->unk_0670[0], (s16)new_var2, sStack_1c);
    }
    return;
}
#else
void ov18_021F37D4(PokedexAppData *pokedexApp, int a1, int a2, int a3) {
    u32 new_var3;
    int iVar1;
    unsigned int new_var;
    unsigned int new_var2;
    s16 sStack_1a;
    s16 sStack_1c;
    s32 iStack_18;
    iVar1 = a2 * 4;
    iStack_18 = a3;
    ManagedSprite_GetPositionXY(*((ManagedSprite **)(pokedexApp->filler_0770 + iVar1)), &sStack_1a, &sStack_1c);
    if (a1 == 0) {
        a1 = (s32)sStack_1a;
    } else if (((u32)a1) < 0x34) {
        a1 = 0x34;
    } else if (0xcc < ((u32)a1)) {
        a1 = 0xcc;
    }
    new_var = 0x10000;
    ManagedSprite_GetPositionXY(*((ManagedSprite **)(pokedexApp->filler_0770 + iVar1)), &sStack_1a, &sStack_1c);
    ManagedSprite_SetPositionXY(*((ManagedSprite **)(pokedexApp->filler_0770 + iVar1)), (s16)a1, sStack_1c);
    new_var3 = 0x10000;
    ManagedSprite_GetPositionXY(*((ManagedSprite **)((pokedexApp->filler_0770 + iVar1) + 4)), &sStack_1a, &sStack_1c);
    new_var2 = a1;
    ManagedSprite_SetPositionXY(*((ManagedSprite **)((pokedexApp->filler_0770 + iVar1) + 4)), (s16)(((u32)((new_var2 + (-0x14)) * new_var3)) >> 0x10), sStack_1c);
    ManagedSprite_SetPositionXY(*((ManagedSprite **)((pokedexApp->filler_0770 + iVar1) + 8)), (s16)(((u32)((a1 + (-0xc)) * new_var)) >> 0x10), sStack_1c);
    ManagedSprite_SetPositionXY(*((ManagedSprite **)((pokedexApp->filler_0770 + iVar1) + 0xc)), (s16)(((u32)((new_var2 + (-4)) * 0x10000)) >> 0x10), sStack_1c);
    ManagedSprite_SetPositionXY(*((ManagedSprite **)((pokedexApp->filler_0770 + iVar1) + 0x10)), (s16)(((u32)((a1 + 4) * new_var3)) >> 0x10), sStack_1c);
    new_var = 0x10000;
    ManagedSprite_SetPositionXY(*((ManagedSprite **)((pokedexApp->filler_0770 + iVar1) + 0x14)), (s16)(((u32)((a1 + 0x14) * new_var)) >> 0x10), sStack_1c);
    if (a3 == 1) {
        ManagedSprite_GetPositionXY(*((ManagedSprite **)pokedexApp->filler_0770), &sStack_1a, &sStack_1c);
        ManagedSprite_SetPositionXY(*((ManagedSprite **)pokedexApp->filler_0770), (s16)new_var2, sStack_1c);
    }
    return;
}
#endif
