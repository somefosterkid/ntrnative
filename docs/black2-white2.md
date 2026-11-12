# Black 2 and White 2

Pokémon Black 2 and White 2 (USA) run in the same app as Black and White. Their game code is not ours: it is the C of
[pokebw2](https://github.com/fuddlesworth/pokebw2) (GPL-3.0, a matched decompilation of both games by its contributors).
A patched copy of what the build needs is committed in `third_party/pokebw2` (GPL-3.0, with its LICENSE); the games' data
tables are not in it: they are read from your ROM at build time. The build needs no network.

## Build

1. Put your dumps next to Black's and White's in `orig/`:

   | Game | File | SHA-1 |
   | --- | --- | --- |
   | Black 2 (USA) | `orig/baserom_b2_us.nds` | `e51e6dfb8678a3d19dcd2a10691b96a569ca0abb` |
   | White 2 (USA) | `orig/baserom_w2_us.nds` | `b5d7490be7b415b8f1e672a53e978a9cc667e56a` |

2. Build the port as the README says (`ninja` first, then the platform's command). When the port's build finds a Black 2
   or White 2 ROM in `orig/`, it fills each game's data tables from that ROM (`tools/rom_modules.py --assets`, the rows
   of `config/b2_us/assets.tsv` and `config/w2_us/assets.tsv`, as for Black) and compiles the vendored, patched pokebw2
   code (`third_party/pokebw2`; the patches in `port/subprojects/pokebw2-*.patch` are already applied in it) into one
   module per game (`b2.o`, `w2.o`), beside Black's. No ROM, no Black 2 / White 2 in the build.
3. Start the app and import the ROM (the launcher does it on first start), or pick the game in the games window. All four
   games can be switched in one process.

The DS ROMs of Black 2 and White 2 are not rebuilt here: pokebw2 does that for its own repository.

## How it differs from Black

- The C is pokebw2's, with small patches where the 32-bit DS assumed things 64-bit hosts break (pointer-sized casts,
  work-area sizes, script jumps); each patch's header says what and why. `port/include_b2w2` holds the SDK header names
  pokebw2 uses, `port/src/stub_b2w2.c` the stand-ins for DS Protect and the online library, `port/src/strm_arc.c` the
  title's music stream.
- `tools/port_overlays.py` lays out each game's overlays; `tools/port_relink.py` links each game into one relocatable
  object on Linux (GNU ld), Windows (PE, lld) and Android.
- Online play (Wi-Fi, GTS, Battle Subway servers), the C-Gear's infrared link and the Xtransceiver are stubbed.
- Game data (graphics, sound, text) is read from your ROM when the game starts; the build holds none of it.

## Status

| Platform | Black 2 / White 2 |
| --- | --- |
| macOS, iOS | Playable: Black 2's new game runs through Aspertia City with no open bugs |
| Linux, Windows, Android | Run: new game to the player's room, save and continue, title music, switching between the four games, about 17 of 18 route legs of the first hours of Black 2 (the one flaky leg is an unpinned battle). Tested less than Black and White |

`tools/route.py b2` replays Black 2's first route in legs from saves (`tools/routes/b2.txt`); on Linux
`tools/port_run.py --game b2` (or `w2`) runs the game headless.

## The vendored tree

For maintainers; a build needs none of it.

- `third_party/pokebw2` holds only what the port's build reads (its README lists it): `src`, `include`, the headers of
  eight libraries, the generated headers, and each version's arm9 `delinks.txt` and `symbols.txt`. Message IDs have
  neutral names (`tools/msgid_neutral.py`); each table that is game data includes `asset/<name>.inc`, written from the
  user's ROM. Nothing from a ROM is in the tree.
- To update from a newer pokebw2 commit: set `COMMIT` in `tools/pokebw2_fetch.sh`, run `tools/pokebw2_fetch.sh --vendor`
  (needs network; `POKEBW2_URL` can point at a local clone; it fetches a sparse checkout, applies the patches and copies
  the file set into `third_party/pokebw2`). Then re-run what a fresh copy lacks: the patches that no longer apply are
  fixed, `tools/msgid_neutral.py` renames the message IDs again, and the tables that are game data move to asset rows
  again (`tools/leak_fix.py`; new tables need rows in the two `assets.tsv`). Check `git diff` of `third_party/pokebw2`
  against the previous commit, then rebuild both games and run `tools/rom_leak_check.py`.
