#include "global.h"

#include "application/pokedex/pokedex_internal.h"
#include "sprite_system.h"
#include "unk_02005D10.h"

void ov18_021F5180(PokedexAppData_UnkSub0868_State77_Sub4 *arg0, s32 arg1, s32 arg2) {
    if (arg0->direction == 0) {
        arg0->rotation = (s32)(0 - arg1);
        arg0->rotationStep = arg2;
        return;
    }
    arg0->rotation = arg1;
    arg0->rotationStep = (s32)(0 - arg2);
}

static void ov18_021F5198(PokedexAppData_UnkSub0868_State77_Sub4 *arg0, s32 arg1, s32 arg2, s32 arg3);

static void ov18_021F5198(PokedexAppData_UnkSub0868_State77_Sub4 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (arg0->direction == 0) {
        arg0->rotation = (s32)(0 - arg1);
        arg0->rotationStep = (s32)(0 - arg2);
        arg0->rotationTarget = (s32)(0 - arg3);
        return;
    }
    arg0->rotation = arg1;
    arg0->rotationStep = arg2;
    arg0->rotationTarget = arg3;
}

s32 ov18_021F51BC(void);

s32 ov18_021F51BC(void) {
    PlaySE(0x8EBU);
    return 0;
}

s32 ov18_021F51CC(PokedexAppData_UnkSub0868_State77_Sub4 *arg0);

s32 ov18_021F51CC(PokedexAppData_UnkSub0868_State77_Sub4 *arg0) {
    u32 v1;
    u8 v0;

    v0 = arg0->state;
    switch (v0) {
    case 0:
        PlaySE(0x8EBU);
        ov18_021F5198(arg0, 0x100, -0x20, -0xE0);
        arg0->state = (u8)(arg0->state + 1);
    case 1:
        ManagedSprite_OffsetAffineZRotation(arg0->pokedexApp->unk_0670[7], arg0->rotation);
        ov18_021F5000(arg0->pokedexApp, ManagedSprite_GetRotation(arg0->pokedexApp->unk_0670[7]));
        v1 = arg0->rotation + arg0->rotationStep;
        arg0->rotation = v1;
        if (v1 == arg0->rotationTarget) {
            return 0;
        }
    default:
        return 1;
    }
}

s32 ov18_021F5238(PokedexAppData_UnkSub0868_State77_Sub4 *arg0);

s32 ov18_021F5238(PokedexAppData_UnkSub0868_State77_Sub4 *arg0) {
    u32 v1;
    u8 v0;

    v0 = arg0->state;
    switch (v0) {
    case 0:
        PlaySE(0x8EBU);
        ov18_021F5198(arg0, 0x120, -0x20, -0x100);
        arg0->state = (u8)(arg0->state + 1);
    case 1:
        ManagedSprite_OffsetAffineZRotation(arg0->pokedexApp->unk_0670[7], arg0->rotation);
        ov18_021F5000(arg0->pokedexApp, ManagedSprite_GetRotation(arg0->pokedexApp->unk_0670[7]));
        v1 = arg0->rotation + arg0->rotationStep;
        arg0->rotation = v1;
        if (v1 == arg0->rotationTarget) {
            return 0;
        }
    default:
        return 1;
    }
}

s32 ov18_021F52A4(PokedexAppData_UnkSub0868_State77_Sub4 *arg0);

s32 ov18_021F52A4(PokedexAppData_UnkSub0868_State77_Sub4 *arg0) {
    u32 v1;
    u8 v0;

    v0 = arg0->state;
    switch (v0) {
    case 0:
        PlaySE(0x8EBU);
        ov18_021F5198(arg0, 0x140, -0x20, 0xFFFFFEE0);
        arg0->state = (u8)(arg0->state + 1);
    case 1:
        ManagedSprite_OffsetAffineZRotation(arg0->pokedexApp->unk_0670[7], arg0->rotation);
        ov18_021F5000(arg0->pokedexApp, ManagedSprite_GetRotation(arg0->pokedexApp->unk_0670[7]));
        v1 = arg0->rotation + arg0->rotationStep;
        arg0->rotation = v1;
        if (v1 == arg0->rotationTarget) {
            return 0;
        }
    default:
        return 1;
    }
}

s32 ov18_021F5310(PokedexAppData_UnkSub0868_State77_Sub4 *arg0);

s32 ov18_021F5310(PokedexAppData_UnkSub0868_State77_Sub4 *arg0) {
    u32 v1;
    u8 v0;

    v0 = arg0->state;
    switch (v0) {
    case 0:
        PlaySE(0x8EBU);
        ov18_021F5198(arg0, 0x160, -0x20, 0xFFFFFEC0);
        arg0->state = (u8)(arg0->state + 1);
    case 1:
        ManagedSprite_OffsetAffineZRotation(arg0->pokedexApp->unk_0670[7], arg0->rotation);
        ov18_021F5000(arg0->pokedexApp, ManagedSprite_GetRotation(arg0->pokedexApp->unk_0670[7]));
        v1 = arg0->rotation + arg0->rotationStep;
        arg0->rotation = v1;
        if (v1 == arg0->rotationTarget) {
            return 0;
        }
    default:
        return 1;
    }
}

s32 ov18_021F537C(PokedexAppData_UnkSub0868_State77_Sub4 *arg0);

s32 ov18_021F537C(PokedexAppData_UnkSub0868_State77_Sub4 *arg0) {
    u32 v1;
    u8 v0;

    v0 = arg0->state;
    switch (v0) {
    case 0:
        PlaySE(0x8EBU);
        ov18_021F5198(arg0, 0x180, -0x20, 0xFFFFFEA0);
        arg0->state = (u8)(arg0->state + 1);
    case 1:
        ManagedSprite_OffsetAffineZRotation(arg0->pokedexApp->unk_0670[7], arg0->rotation);
        ov18_021F5000(arg0->pokedexApp, ManagedSprite_GetRotation(arg0->pokedexApp->unk_0670[7]));
        v1 = arg0->rotation + arg0->rotationStep;
        arg0->rotation = v1;
        if (v1 == arg0->rotationTarget) {
            return 0;
        }
    default:
        return 1;
    }
}

s32 ov18_021F53E8(PokedexAppData_UnkSub0868_State77_Sub4 *arg0);

s32 ov18_021F53E8(PokedexAppData_UnkSub0868_State77_Sub4 *arg0) {
    u32 v1;
    u8 v0;

    v0 = arg0->state;
    switch (v0) {
    case 0:
        PlaySE(0x8EBU);
        ov18_021F5198(arg0, 0x1C0, -0x40, 0xFFFFFEC0);
        arg0->state = (u8)(arg0->state + 1);
    case 1:
        ManagedSprite_OffsetAffineZRotation(arg0->pokedexApp->unk_0670[7], arg0->rotation);
        ov18_021F5000(arg0->pokedexApp, ManagedSprite_GetRotation(arg0->pokedexApp->unk_0670[7]));
        v1 = arg0->rotation + arg0->rotationStep;
        arg0->rotation = v1;
        if (v1 == arg0->rotationTarget) {
            return 0;
        }
    default:
        return 1;
    }
}

s32 ov18_021F5454(PokedexAppData_UnkSub0868_State77_Sub4 *arg0);

s32 ov18_021F5454(PokedexAppData_UnkSub0868_State77_Sub4 *arg0) {
    u32 v1;
    u8 v0;

    v0 = arg0->state;
    switch (v0) {
    case 0:
        PlaySE(0x8EBU);
        ov18_021F5198(arg0, 0x200, -0x40, 0xFFFFFE80);
        arg0->state = (u8)(arg0->state + 1);
    case 1:
        ManagedSprite_OffsetAffineZRotation(arg0->pokedexApp->unk_0670[7], arg0->rotation);
        ov18_021F5000(arg0->pokedexApp, ManagedSprite_GetRotation(arg0->pokedexApp->unk_0670[7]));
        v1 = arg0->rotation + arg0->rotationStep;
        arg0->rotation = v1;
        if (v1 == arg0->rotationTarget) {
            return 0;
        }
    default:
        return 1;
    }
}
