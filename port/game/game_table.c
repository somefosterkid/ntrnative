// A game's table for the host (port/include/port_game.h): compiled into each game; White's is w_bwgame (on the PC its
// names are renamed w_ anyway; on Apple the one global its relocatable object keeps: port/meson.build), Black 2's
// b2_bwgame and White 2's w2_bwgame (pokebw2's C, the maintainers' notes).
#include "port_game.h"
#include "work_area.h"
// (Black 2 and White 2 carry their overlays' markers and DS layout on every platform: on the PC Black's and White's are the linker
// script's table, port/src/ovl.c)

void NitroMain(void);
extern unsigned char g_GFLSndHeap[];
void GFL_RandomUpdateMT(unsigned seed);
void PortGame_PinLC(void);   // port/extern*/src/system/gfl_use.c
#if defined(BWPORT_OVL_MACHO) || defined(BLACK2) || defined(WHITE2)
extern const unsigned bwovl_count;
extern const unsigned bwovl_ds[][4];
extern unsigned char *const bwovl_dptr[];
extern unsigned char *const bwovl_bptr[];
#endif

#if defined(BLACK2)
const PortGame b2_bwgame = {"IREO",
#elif defined(WHITE2)
const PortGame w2_bwgame = {"IRDO",
#elif defined(WHITE)
const PortGame w_bwgame = {"IRAO",
#else
const PortGame bwgame = {"IRBO",
#endif
    NitroMain, g_GFLSndHeap,
#if defined(BLACK2) || defined(WHITE2)
    WORK_AREA_SIZE(0x9d000),   // pokebw2's src/system/snd_sys.c
#else
    WORK_AREA_SIZE(0x95ad0),   // src/system/snd_sys.c
#endif
    GFL_RandomUpdateMT,
#if defined(BWPORT_OVL_MACHO) || defined(BLACK2) || defined(WHITE2)
    &bwovl_count, bwovl_ds, bwovl_dptr, bwovl_bptr,
#endif
#if defined(BLACK2) || defined(WHITE2)
    .ovlBattle = 167, .ovlEncFirst = 148, .ovlEncLast = 154, .ovlBag = 286, .ovlParty = 287,   // Black 2's, White 2's
#else
    .ovlBattle = 93, .ovlEncFirst = 76, .ovlEncLast = 79, .ovlBag = 197, .ovlParty = 198,
#endif
    .pinLC = PortGame_PinLC,
};
