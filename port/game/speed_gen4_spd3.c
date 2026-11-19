// The speed options' Gen 4 side, second block (spd3, 2026-11-19): Save speed and the battle intro. Platinum, HeartGold and
// SoulSilver; the first block (Text speed) is port/game/speed_gen4.c. The games' patches (port/subprojects/
// pokeplatinum-speed-spd3.patch, pokeheartgold-speed-spd3.patch) call these at their hook points; compiled into each Gen 4
// module (port/meson.build's main_files), so each game has its own copy. docs/speed-options.md "Platinum and HeartGold/SoulSilver".
// Both are pacing windows, as Black's: they change how fast the machine's tick timeline runs (libntr's SIM_SetSave / SIM_SetBattle
// under the Save speed / Battle speed settings), never a game frame, so an uncapped route run's V-blank counts do not move.
#include <nitro.h>
#include <stdio.h>

extern int SIM_UISettingGet(const char *key);
extern void SIM_SetSave(int speed);
extern void PortOvl_SetEncounter(int on);   // port/src/ovl.c: the encounter effect is inside Battle speed's window

// Save speed: the window while the game saves, after the player's last Yes (the Yes/No itself stays at the player's speed). The hook
// calls it with 1 when the save's messages, its write and its sound start and with 0 when the save's box closes (Platinum:
// the script's ScrCmd_258 .. ScrCmd_CloseSaveInfo; HG/SS: the touch save app's states from the "Saving..." message to its close;
// each frame there, the same value again is a no-op). The setting is SaveSpeed (SIM_UISettingGet "savespeed": 1 = off, 2, 4),
// as Black's port/extern/src/system/game_event.c; the log lines are the same as its.
void PortSpeed4_SaveWindow(int on) {
    static int sOn;
    int speed = on ? SIM_UISettingGet("savespeed") : 0;

    if (on != sOn) {
        fprintf(stderr, "bwport: save %s (frame %u, speed %d)\n", on ? "start" : "end", OS_GetVBlankCount(), speed > 1 ? speed : 1);
        sOn = on;
    }
    SIM_SetSave(speed > 1 ? speed : 0);
}

// Battle intro: the encounter effect (the field task that plays the wild or trainer encounter's screen effect before the battle's
// overlay loads) runs at Battle speed. Called 1 at its start, 0 at its end; port/src/ovl.c adds it to the battle's overlay window.
void PortSpeed4_EncounterWindow(int on) {
    static int sOn;

    if (on != sOn) {
        fprintf(stderr, "bwport: encounter effect %s (frame %u)\n", on ? "start" : "end", OS_GetVBlankCount());
        sOn = on;
    }
    PortOvl_SetEncounter(on);
}
