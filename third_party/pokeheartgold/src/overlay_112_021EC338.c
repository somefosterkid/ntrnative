#include "global.h"

#include "math_util.h"
#include "save.h"
#include "sprite_system.h"
#include "text.h"
#include "overlay_112_pokewalker_app.h"
struct Ov112PokewalkerApp;

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov112_021EA7D0(Ov112PokewalkerApp *);
#else
s32 ov112_021EA7D0(Ov112PokewalkerApp *);
#endif

s32 ov112_021EC374(void);
s32 ov112_021EC3F0(void *arg0);
s32 ov112_021EA19C(Ov112PokewalkerApp *arg0);
s32 ov112_021EC404(void *arg0);
void ov112_021EA570(Ov112PokewalkerApp *arg0);
s32 ov112_021EC434(Ov112PokewalkerApp *arg0);

s32 ov112_021EC338(Ov112PokewalkerApp *arg0) {
    u8 *new_var;
    arg0->unk_1F2D0 = (s32)((arg0->unk_1F2D0) + 1);
    new_var = (u8 *)arg0;
    if (((s32)(*((s32 *)(new_var + 0x1F2D0)))) > 0xB4) {
        ov112_021EA670(arg0, 8);
        return 0x1A;
    }
    if (((u32)(*((s32 *)(new_var + 0x1F2D0)))) > ((u32)(MTRandom() % 180U))) {
        ov112_021EA7D0(arg0);
    }
    return 0x19;
}

s32 ov112_021EC374(void) {
    return 0x1B;
}

s32 ov112_021EC378(Ov112PokewalkerApp *arg0) {
    arg0->unk_1E524 = ov112_021EA08C(arg0, 2, 0x1B);
    return 0x1C;
}

s32 ov112_021EC390(Ov112PokewalkerApp *arg0) {
    if (TextPrinterCheckActive((u8) * &arg0->unk_1E524) == 0) {
        return 0x1D;
    }
    return 0x1C;
}

s32 ov112_021EC3B0(Ov112PokewalkerApp *arg0) {
    arg0->unk_1E524 = ov112_021EA08C(arg0, 2, 0xD);
    return 0x1E;
}

s32 ov112_021EC3C8(Ov112PokewalkerApp *arg0) {
    if (TextPrinterCheckActive((u8) arg0->unk_1E524) == 0) {
        ov112_021EA17C(arg0);
        return 0x1F;
    }
    return 0x1E;
}

s32 ov112_021EC3F0(void *arg0) {
    SaveGameNormal(*(SaveData **)((u8 *)arg0 + 0x20));
    Save_ClearStatusFlags(*(SaveData **)((u8 *)arg0 + 0x20));
    return 0x20;
}

s32 ov112_021EC404(void *arg0) {
    ov112_021EA19C(arg0);
    *(s32 *)((u8 *)arg0 + 4) = 0;
    return 0x22;
}

s32 ov112_021EC414(Ov112PokewalkerApp *arg0) {
    s32 v0;

    if (ManagedSprite_IsAnimated(arg0->sprites[0]) == 0) {
        v0 = (s32)arg0->unk_C;
        arg0->unk_8 = v0;
        return v0;
    }
    return 0x21;
}

s32 ov112_021EC434(Ov112PokewalkerApp *arg0) {
    ov112_021EA570(arg0);
    return 0x23;
}

void ov112_021EC440(Ov112PokewalkerApp *arg0, s32 arg1, s32 arg2, s32 arg3) {
    arg0->unk_1EC50 = arg1;
    ov112_021EA51C(arg0, arg2);
    ov112_021EA584(arg0);
    arg0->unk_C = arg3;
}
