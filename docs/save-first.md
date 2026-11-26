# Save first (the leave warnings' third choice)

Restart to title and Games leave the game through its own soft reset, and the warning says unsaved progress is lost. Since
2026-11-25 the warning has a third choice, **Save first**: the game saves itself, exactly as its menu's Save does (the same
function, the same file and format, the game's own two slots, footers and checksums), and then the leave goes on. Desktop
(libntr's sim_gui.cpp: the Restart to title and Games popups), iOS (port/src/ios_settings.m: the same two alerts) and Android
(SettingsScreen.java: the same two dialogs, with native calls in port/src/android_settings.c; not built or run yet).

## What runs

- The UI calls `bwport_save_first(then)` (port/src/save_first.c; then 0 = Restart to title, 1 = Games). The phones call it
  once their settings sheet is closed (the sheet stops the game).
- The game's frame hook takes the request on the game thread when the game's own menu would offer Save and the player is
  free, and runs the menu's save without its windows:
  - Platinum: `FieldSystem_Save` (what the start menu's save script's TrySaveGame calls: the map objects and the player's
    place, then `SaveData_Save`), from the field's frame hook (port/game/save_gen4.c, called by PortRoute4_Frame).
  - HeartGold / SoulSilver: `Field_SaveGameNormal` (what the start menu's touch save app calls), the same hook.
  - Black / White: an event of the port's (port/extern/src/system/game_event.c) doing what the field menu's report does after
    Yes: the times-saved record, `func_02012dac` (the game data's save start), `func_02012dd0` each frame until written.
  - Black 2 / White 2: the same with pokebw2's names (`GameData_StartSave`, `GameData_UpdateSave`; port/extern_b2w2).
  Gen 4 saves at once in the hook's frame; Gen 5 writes over a few frames inside its event (the field stops, the soft reset
  is blocked while the game data is saving, as in the menu's save).
- save_first.c's waiter thread presses the leave (the same soft reset as Leave, port/src/restart_press.c) once the game says
  the save is written. If the game does not get free within 180 V-blanks (3 s of its time) nothing is saved, nothing is left,
  and the game goes on. V-blanks, not the wall clock: a paused game (the phones' sheets, desktop's Game > Pause) waits.
- Desktop's popup shows "Saving..." until then; its Cancel drops a request the game has not begun
  (`bwport_save_first_cancel`), and is hidden while the game writes (a write is never stopped). If nothing was saved it says
  so and keeps its choices.

## Where it is refused (the button is greyed out: "the game can't save here right now")

The game's hook says each frame whether its own menu would offer Save (`PortSaveFirst_Can`); the button is enabled when it
said yes within the last 8 V-blanks.
- Everywhere: a battle, an open menu or app, a script or conversation, a map change or cutscene (Gen 4: a field task or the
  field not running; Gen 5: any game event).
- Platinum: the Underground, the Union Room and the colosseums, the Safari Zone, the Pal Park, the Battle Tower's salon,
  the Mystery Zone; a new game that would overwrite another game's save (the save script refuses it).
- HeartGold / SoulSilver: the Union Room and the colosseums, the Safari Zone, the Bug-Catching Contest, the Pal Park, the
  Battle Tower's multi partner room, the Mystery Zone; a save file that belongs to another game.
- Black / White and Black 2 / White 2: where the field menu has no Save (the Union Room and the colosseums, Royal Unova, a
  forced season sync); a new game over another game's save is refused in the event (the menu's report refuses it too).
- The save itself waits for the player to stand (Gen 4: the move state the menu key needs; Gen 5: the field event check's
  "can act" with no direction held), so a save never catches the player between tiles.

## Proof on the Mac (2026-11-25, tools/mac_run.sh from copies of test saves)

| Game | Field: walk, Save first, relaunch (a new process) | Refused (save.bin unchanged) |
|---|---|---|
| SoulSilver (ss-r30) | 550,383 -> 551,383; saved in the request's frame; continues at 551,383 | the start menu open; a test wild battle |
| Platinum (pt-hearth) | 461,726 -> 458,726; saved in the request's frame; continues at 458,726 | the start menu open; a test wild battle |
| Black (route1) | 788,725 -> 786,725; the event writes for 186 V-blanks; continues at 786,725 | the field menu open; asked while walking with the direction held: "nothing saved" after 180 V-blanks |
| Black 2 (b2-route19) | 56,696 -> 54,696; the event writes for 184 V-blanks; continues at 54,696 | the field menu open; a wild battle from the grass |

The relaunch is the checksum proof: each game loads the newest slot whose footer and CRCs pass and falls back to the older
slot otherwise (the original spot); every relaunch continued from the new spot. After the leave the game's soft reset
reached the title each time (and in the in-process restart CONTINUE again showed the new spot). The routes with the hooks in (tools/check.sh; tools/route.py for Black, White, Black 2, White 2, Platinum and SoulSilver): every
leg whose save is in the library passes, except Platinum's r209, which ends one tile off the same way without this change. Not
run here: the phones
(iOS: the alert is compiled, not tapped; Android: not built), and places like the Safari Zone or the Union Room (refused by the
same checks the menus use, read from the code, not run).

## Tests

`savefirst=0|1` in a cfgscript (libntr sim_main.cpp) asks as the button does. The log says `bwport: save first: possible` /
`not possible` as the game's state changes, then `asked`, `saving`, `saved` (or `not saved: why`, `refused`), and
`restart to title` / `the games list` when it leaves.
