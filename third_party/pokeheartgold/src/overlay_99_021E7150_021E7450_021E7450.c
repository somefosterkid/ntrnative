#include "global.h"

#include "bg_window.h"
#include "sprite.h"
#include "sprite_system.h"

typedef struct Ov99SpriteArg {
    u32 a : 9;
    u32 b : 5;
    u32 rest : 18;
    u32 unk4;
    u32 unk8;
} Ov99SpriteArg;

void ov98_0221E970(u32, u32, Ov99SpriteArg *, u32, u32);

u8 ov99_021E7158(void *);

u32 ov99_021E71E4(u32 *, u32);

void ov99_021E73E0(u32 *, s32, s32);

void ov99_021E7428(u32 *, u32);

void ov99_021E7450(u32 *param_1);

void ov99_021E7450(u32 *param_1) {
    s32 count = ov99_021E7158(param_1);
    Ov99SpriteArg arg = { 0 };
    s32 i;
    u16 val;
    u32 *p;

    BgClearTilemapBufferAndCommit((BgConfig *)*param_1, 3);
    ov99_021E7428(param_1, 0);
    for (i = 0; i < count; i++) {
        val = *(u16 *)((u8 *)param_1 + (i + ((param_1[0xfd] << 13) >> 27) * 0x1e) * 2 + 0x14);
        arg.a = val;
        arg.b = ov99_021E71E4(param_1, val);
        p = &param_1[i + 0x14];
        ov98_0221E970(param_1[0x101], p[0x102], &arg, 1, 0);
        ManagedSprite_SetDrawFlag((ManagedSprite *)p[0x102], 1);
        ov99_021E73E0(param_1, (u16)i, val);
    }
}
