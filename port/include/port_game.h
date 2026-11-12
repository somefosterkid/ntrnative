// The game the port runs (docs/launcher.md, "More than one game"). Each game's C is linked once into the binary:
// Black's with its own names; White's (when its ROM was there at the build) renamed w_ on the PC (tools/port_rename.py's
// header) and on macOS and iOS as one relocatable object whose globals are all local but its table, w_bwgame; Black 2's
// and White 2's (pokebw2's C) as White's on Apple (port/meson.build). What the host calls in a game goes through its table.
#ifndef BWPORT_PORT_GAME_H
#define BWPORT_PORT_GAME_H

typedef struct {
    const char *code;                        // the ROM header's game code (launcher_core.c's table)
    void (*main)(void);                      // NitroMain: src/system/main.c's main
    unsigned char *sndHeap;                  // g_GFLSndHeap (port/src/acapture.c)
    unsigned sndHeapLen;                     // its size (src/system/snd_sys.c's SND_HEAP_SIZE)
    void (*randomUpdateMT)(unsigned seed);   // GFL_RandomUpdateMT (port/src/bench_pin.c)
    // ld64 builds: the overlays' markers and DS layout (tools/port_overlays.py --macho-files: ovltab.c; port/src/ovl.c)
    const unsigned *ovlCount;
    const unsigned (*ovlDs)[4];
    unsigned char *const *ovlData;
    unsigned char *const *ovlBss;
    // Battle speed's overlays (port/src/ovl.c, the maintainers' notes, 6.): the battle's, the encounter effects' first and
    // last (the battle intro), the battle's Bag and party screen. Black's and White's are 93, 76-79, 197, 198; Black 2's
    // and White 2's 167, 148-154, 286, 287 (config/*/arm9/overlays)
    unsigned ovlBattle, ovlEncFirst, ovlEncLast, ovlBag, ovlParty;
    void (*pinLC)(void);                     // the game's linear congruential generator to the pinned seed (port/src/bench_pin.c)
} PortGame;

extern const PortGame bwgame;       // Black: port/game/game_table.c
extern const PortGame w_bwgame;     // White (BWPORT_WHITE builds): the same file
extern const PortGame b2_bwgame;    // Black 2 (BWPORT_B2 builds; pokebw2's C, the maintainers' notes): the same file
extern const PortGame w2_bwgame;    // White 2 (BWPORT_W2 builds): the same file
extern const PortGame *PORT_Game;   // the game PORT_EnterGame chose; Black until then (port/src/games.c)

const PortGame *PORT_GameByCode(const char *code);   // port/src/games.c: NULL when the build lacks that game
void PORT_SetGame(const char *code);   // games.c: PORT_Game by the game code (not built: Black), and its overlay table

void PortOvl_SetGame(const PortGame *g);   // port/src/ovl.c: the game's overlay table

#endif // BWPORT_PORT_GAME_H
