#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "unk_0208B1AC.h"
#include "filesystem_files_def.h"
#include "ribbon.h"
#include "pokemon_summary_app.h"

s16 sub_0208C0A4(u32, s16, s16);

void sub_0208C250(PokemonSummary *param_1, u8 param_2, s32 param_3);

void sub_0208C0E8(PokemonSummary *arg0) {
    s16 x;
    s16 v0;
    s16 v1;
    s16 v2;
    s16 v3;

    x = sub_0208C0A4(*((u8 *)&arg0->mon.contestStats[0]), 0x58, 0x58);
    Sprite_SetPositionXY(arg0->sprites[47], x, sub_0208C0A4(*((u8 *)&arg0->mon.contestStats[0]), 0x31, 0x49));
    v0 = sub_0208C0A4(*((u8 *)&arg0->mon.contestStats[1]), 0x6E, 0x58);
    Sprite_SetPositionXY(arg0->sprites[48], v0, sub_0208C0A4(*((u8 *)&arg0->mon.contestStats[1]), 0x41, 0x49));
    v1 = sub_0208C0A4(*((u8 *)&arg0->mon.contestStats[2]), 0x67, 0x58);
    Sprite_SetPositionXY(arg0->sprites[49], v1, sub_0208C0A4(*((u8 *)&arg0->mon.contestStats[2]), 0x5C, 0x49));
    v2 = sub_0208C0A4(*((u8 *)&arg0->mon.contestStats[3]), 0x48, 0x57);
    Sprite_SetPositionXY(arg0->sprites[50], v2, sub_0208C0A4(*((u8 *)&arg0->mon.contestStats[3]), 0x5C, 0x49));
    v3 = sub_0208C0A4(*((u8 *)&arg0->mon.contestStats[4]), 0x41, 0x57);
    Sprite_SetPositionXY(arg0->sprites[51], v3, sub_0208C0A4(*((u8 *)&arg0->mon.contestStats[4]), 0x41, 0x49));
    Sprite_SetDrawFlag(arg0->sprites[47], 1);
    Sprite_SetDrawFlag(arg0->sprites[48], 1);
    Sprite_SetDrawFlag(arg0->sprites[49], 1);
    Sprite_SetDrawFlag(arg0->sprites[50], 1);
    Sprite_SetDrawFlag(arg0->sprites[51], 1);
}

void sub_0208C208(PokemonSummary *param_1) {
    Sprite_SetDrawFlag(param_1->sprites[47], 0);
    Sprite_SetDrawFlag(param_1->sprites[48], 0);
    Sprite_SetDrawFlag(param_1->sprites[49], 0);
    Sprite_SetDrawFlag(param_1->sprites[50], 0);
    Sprite_SetDrawFlag(param_1->sprites[51], 0);
    return;
}

void sub_0208C250(PokemonSummary *param_1, u8 param_2, s32 param_3) {
    u32 uVar1;
    s32 idx;

    uVar1 = GetRibbonAttr(param_2, RIBBONDAT_NCGR);
    SpriteSystem_ReplaceCharResObj(param_1->spriteSystem, param_1->spriteManager, NARC_a_0_3_9, uVar1, 0, param_3 + 0x19);
    uVar1 = GetRibbonAttr(param_2, RIBBONDAT_NCLR);
    idx = param_3 + 0x36;
    thunk_Sprite_SetPaletteOverride(param_1->sprites[idx], uVar1 + 7);
    return;
}
