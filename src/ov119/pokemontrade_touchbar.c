// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov194/pokemontrade_touchbar.c.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/pokemon_trade_local.h"
#include "app/ui/touchbar.h"
#include "constants/sound.h"
#include "gfl/key.h"
#include "nitro/mi.h"

// The trade's touch bar. The ROM names no file between pokemontrade_3d.c and pokemontrade_2d.c (pokebw2's
// pokemontrade_touchbar.c is a descriptive name): its 0x80-byte initializer comes first in .rodata, before the
// smaller objects of pokemontrade_2d.c, which one file's sort by size would put ahead of it

// pokebw2's PokemonTrade_CreateTouchBar
void func_ov119_021e3c20(PokemonTradeWork *wk) {
    TouchBarItem items[] = {
        { 1, { 232, 168 } },
        { TOUCHBAR_ICON_CUSTOM, { 28, 168 } },
        { TOUCHBAR_ICON_CUSTOM + 1, { 204, 168 } },
        { TOUCHBAR_ICON_CUSTOM + 2, { 48, 168 } },
    };
    TouchBarSetup setup;

    MI_CpuFill8(&setup, 0, sizeof(setup));
    setup.items = items;
    setup.count = NELEMS(items);
    setup.unit = wk->clactUnit;
    setup.unk1C = TRUE;
    setup.bgPalette = 7;
    setup.objPalette = 0;
    setup.vramType = 2;
    setup.bg = 4;

    items[1].charRes = wk->objRes[TRADE_OBJRES_CHAR_SUB];
    items[1].plttRes = wk->objRes[TRADE_OBJRES_PLTT_SUB];
    items[1].cellRes = wk->objRes[TRADE_OBJRES_CELL_SUB];
    items[1].anims[0] = 6;
    items[1].anims[1] = 5;
    items[1].anims[2] = 4;
    items[1].key = 0;
    items[1].se = SEQ_SE_DECIDE1;

    items[2].charRes = wk->objRes[TRADE_OBJRES_CHAR_SUB];
    items[2].plttRes = wk->objRes[TRADE_OBJRES_PLTT_SUB];
    items[2].cellRes = wk->objRes[TRADE_OBJRES_CELL_SUB];
    items[2].anims[0] = 9;
    items[2].anims[1] = 8;
    items[2].anims[2] = 7;
    items[2].key = PAD_BUTTON_START;
    items[2].se = SEQ_SE_DECIDE1;

    items[3].charRes = wk->objRes[TRADE_OBJRES_CHAR_SUB];
    items[3].plttRes = wk->objRes[TRADE_OBJRES_PLTT_SUB];
    items[3].cellRes = wk->objRes[TRADE_OBJRES_CELL_SUB];
    items[3].anims[0] = 22;
    items[3].anims[1] = 21;
    items[3].anims[2] = 20;
    items[3].key = PAD_KEY_RIGHT | PAD_KEY_LEFT;
    items[3].se = SEQ_SE_DECIDE1;

    wk->touchBar = func_ov067_021b62e0(&setup, wk->heapId);
    func_ov067_021b6558(wk->touchBar, TOUCHBAR_ICON_CUSTOM + 2, FALSE);
    func_ov067_021b6558(wk->touchBar, TOUCHBAR_ICON_CUSTOM + 1, FALSE);
    func_ov067_021b6520(wk->touchBar, 2);
}
