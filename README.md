# NTR Native

Decompiled Nintendo DS games rebuilt as native apps, with our own renderers and sound core, for Pokémon Black, White,
Black 2, White 2, Platinum, HeartGold and SoulSilver. "NTR" is the DS's product code. Bring your own ROM: none is
included or will be provided.

## Games

| Game (USA) | SHA-1 |
| --- | --- |
| Black | `26ad0b9967aa279c4a266ee69f52b9b2332399a5` |
| White | `bc696a0dfb448c7b3a8a206f0f8214411a039208` |
| Black 2 | `e51e6dfb8678a3d19dcd2a10691b96a569ca0abb` |
| White 2 | `b5d7490be7b415b8f1e672a53e978a9cc667e56a` |
| Platinum (Rev 1) | `0862ec35b24de5c7e2dcb88c9eea0873110d755c` |
| HeartGold | `4fcded0e2713dc03929845de631d0932ea2b5a37` |
| SoulSilver | `f8dc38ea20c17541a43b58c5e6d18c1732c7e582` |

All four play on Windows, Linux, Android, macOS and iOS, in one app that switches between them. Every
game has been tested playable from a new game to the first gym, by hand and by scripted runs; scripted runs through the
rest of all four games are in progress. Android is tested on the x86_64 emulator only (the arm64 build for phones is
packaged but has not run on a device); iOS on the Simulator only, where Black and Black 2 reach their field. Online play, DS Protect and the DSi-only features are not supported. Black 2 and White 2:
[docs/black2-white2.md](docs/black2-white2.md).

The Generation IV games are further behind. They are built into the Windows, Linux, Android, macOS and iOS builds (the Android APK carries
them, checked on the x86_64 emulator only: each boots from a save and the touch screen works; on macOS and in the iOS
Simulator, Platinum, HeartGold and SoulSilver each reach the field from a save):

- **Platinum** (Windows and Linux; on Android it boots and the Poketch takes touches) plays from a new game to the credits. 218 scripted legs take it through the whole
  story, each leg replayed three times to the same result, and the whole chain from a new game to the credits replays
  with identical frame counts, twice and on two different machines. Windows and Linux run the same frames. Crash sweeps
  cover all 593 maps, every species and every move
  in battle, and every person, sign, hidden item and trigger tile; the crashes left are out of context: rooms that need
  a link session (Union Room, the Underground, the Battle Tower), maps the game never uses, and scenes the sweep enters
  sideways. Testing so far is scripted runs and sweeps. The code is pret's pokeplatinum plus our patches; it is fetched
  when you build, not kept here.
- **HeartGold and SoulSilver**: playable. Known issues: opening the PC box, the Pokedex or the Trainer Card can crash;
  fixes are in progress. Scripted runs take a new game on both from the start into Ecruteak City's Burned Tower and gym:
  69 legs each, through three gyms, the Slowpoke Well, Cut in the Ilex Forest and the Surf machine. Windows matches
  Linux frame for frame on the runs compared so far (Windows is less tested than Linux). Crash sweeps: every map but two
  loads; battles against every species all finish (493 of 493), and 99 of 117 in a sweep of every move. The story after
  Ecruteak and hand play are less tested. The code is pret's pokeheartgold with our 64-bit work, in
  `third_party/pokeheartgold`.

Neither pret project has a license file; see Credits and [THIRD_PARTY.md](THIRD_PARTY.md). No game data is in the
repository: you supply the ROMs.

Coming soon: Pokémon Diamond and Pearl. Later: mods such as Nuzlocke and randomizer modes, which are plain C changes once a
game is decompiled.

## Features

- **Fast**: hundreds of frames a second at native resolution on a desktop CPU.
- **Speed options**: fast forward and Game speed up to 4x or unlimited, with the music kept at normal speed and pitch;
  or speed up only the parts you choose: walking, running, Surf, text, battles, transitions and saving.
- **Picture**: resolution 1x to 8x, antialiasing, frame rate 60, 120, your monitor's or unlimited, and Smooth motion
  (in-between frames drawn from the game's own 3D, not blended).
- **Sound**: Enhanced (the default) smooths the DS's samples for cleaner, higher-fidelity audio; DS mode is the
  original output, bit for bit.

## Build and run

Linux or WSL: Python 3.11+, meson 1.3+, ninja, clang, g++, SDL2 and enet ([docs/libntr/build.md](docs/libntr/build.md),
"Dependencies"). The port needs no ROM to build:

| Platform | Command | Doc |
| --- | --- | --- |
| Linux | `meson setup build/port/linux port --native-file port/machine/linux.ini && ninja -C build/port/linux bwport` | [port-build](docs/port-build.md) |
| Windows | `tools/port_win64.sh deploy --out DIR` (cross-builds `bwport.exe` from WSL or Linux with mingw-w64 and puts it, its DLLs and `bwport.bat` in DIR; run `bwport.bat` on Windows) | [port-build](docs/port-build.md) |
| Windows, native | `python tools/port_win64.py fetch`, then `python tools/port_win64.py deploy --out DIR` (on Windows itself, no WSL; Black and White so far) | [port-build](docs/port-build.md) |
| macOS | `tools/ios_deps.sh`, `meson setup build/port/macos-release port --native-file port/machine/macos-release.ini`, `ninja -C build/port/macos-release bwport`, `tools/mac_package.sh` (zip with bwport.app) | [install](#macos-and-ios-install) |
| iOS | `tools/ios_deps.sh`, `meson setup build/port/ios port --native-file port/machine/ios.ini`, `ninja -C build/port/ios bwport`, `tools/ios_package.sh ipa` (unsigned .ipa) | [install](#macos-and-ios-install) |
| Android | `tools/android_deps.sh`, then `tools/android_package.sh --build` (APK for arm64-v8a and x86_64) | [android-parity](docs/android-parity.md) |

### macOS and iOS (install)

- macOS (Apple silicon, macOS 13.3 or later): unzip bwport-macos-REV.zip and move bwport.app to Applications. The app
  is not signed by a developer: on the first start right-click it and choose Open (or System Settings > Privacy &
  Security > Open Anyway). It keeps its games, saves and settings in ~/Library/Application Support/bwport. Import your
  ROM in the app (Add Game, or drop the .nds on the window).
- iOS (iOS 16.3 or later): bwport.ipa is unsigned: sideload it with SideStore or AltStore, which sign it with your own
  Apple ID. Import your ROM in the app.

The Windows build needs SDL2 and enet built for mingw-w64 ([docs/libntr/build.md](docs/libntr/build.md), "win64").
Every game is built in on every platform: Black, White, Black 2, White 2, Platinum, HeartGold and SoulSilver; their data comes
from the ROM the app imports. The Generation IV games' meson options `platinum` and `hgss` are on by default;
`-Dplatinum=false` or `-Dhgss=false` leaves one out. A build directory made before that keeps its old options: delete it,
or `meson configure DIR -Dplatinum=true -Dhgss=true`. Black 2 and White 2 are built from the vendored, patched pokebw2
code in `third_party/pokebw2` (no network). The app imports your ROM on first start (any of the seven); saves and
settings live beside it ([docs/launcher.md](docs/launcher.md)). The exe with every game is about 325 MB unstripped on
Linux (Gen 5 only: 119 MB).

Platinum's first build fetches pret's pokeplatinum (the git submodule `third_party/pokeplatinum`, pinned to one commit;
`git submodule update --init` does the same by hand), a pinned meson and the helper projects pret's header generators
use (metang, yyjson), so it needs the network once. HeartGold's and SoulSilver's first build needs it for the same
meson and helpers; their game code is already in `third_party/pokeheartgold`.

The ROM build (the matching decompilation: it rebuilds the DS ROMs byte for byte) needs your dumps in `orig/`.
`baserom_b_us.nds` (Black) is required there, as the ROM build starts from it; `baserom_w_us.nds` (White) is optional and
rebuilt too when present. The port does not need this step. The port build's release step, which takes the games' data
back out of the binary, runs when every built game's dump is in `orig/`: those two and `baserom_b2_us.nds`,
`baserom_w2_us.nds`, `baserom_plat_us.nds`, `baserom_hg_us.nds`, `baserom_ss_us.nds` ([docs/port-build.md](docs/port-build.md)).

```sh
python3 configure.py
ninja            # build/pokeblack_us.nds (and White's), SHA-1 checked
```

`configure.py` downloads dsd, wibo and the Metrowerks tools on first run.

## How it works

A tour of the code for contributors (layers, the games in one app, the build without game data, testing):
[docs/architecture.md](docs/architecture.md).

- **Decompilation**: about 96% of Black's functions (34,300 of 35,650) are C that compiles to the original bytes with the
  original Metrowerks compiler. The rest is working C that is not yet byte-matched ([docs/nonmatching.md](docs/nonmatching.md)),
  plus Nintendo's online (Wi-Fi) library, which comes from your ROM and is unused: online play is stubbed. White builds from
  the same source ([docs/white.md](docs/white.md)).
- **Generation IV**: Platinum and HeartGold/SoulSilver compile pret's decompiled C on libntr and our NitroSystem, with
  patches of ours for 64-bit hosts (pointer widths, struct layouts).
- **Port**: the game's C runs natively on [libntr](https://github.com/cybervisi0n/libntr), a NitroSDK for PC, with our own
  NitroSystem (`lib/nnsys`, [docs](docs/nnsys-port.md)).
- **Video**: `vcore` is our clean-room CPU renderer of the DS video hardware, exact at 1x and upscaling on the CPU;
  `gpu` upscales on the GPU. OpenGL 4 (Windows, Linux; bundled Mesa software GL when the system
  has none), OpenGL ES 3 (Android).
- **Sound**: `acore` is our DS sound core.
- **Game data** is read from your ROM when the game starts; the Windows, Linux and Android builds hold none of it.

## Legal

NTR Native is a fan project. It is not affiliated with, endorsed or sponsored by Nintendo, Game Freak, Creatures or
The Pokémon Company. Pokémon and all related names are trademarks of their respective owners. This repository holds no
ROMs, game assets or game data: you need your own copy of each game, dumped from a cartridge you own. Requests for ROMs,
or links to them, will be closed.

## Status and license

Work in progress. GPL-3.0, see [LICENSE](LICENSE); it covers our work, not pret's code in `third_party/pokeheartgold` or
the pokeplatinum that Platinum's build fetches (no license file upstream). Every project we use and its license is in
[THIRD_PARTY.md](THIRD_PARTY.md). Contributions: [CONTRIBUTING.md](CONTRIBUTING.md).

## Credits

- **[pokebw2](https://github.com/fuddlesworth/pokebw2)** (GPL-3.0) is the project this one owes most to.
  - Black 2 and White 2: their code is pokebw2's decompilation, included (patched) in `third_party/pokebw2` under GPL-3.0 and compiled when you build.
  - Black: 21,574 of its functions in 1,419 files have a twin in Black 2 / White 2, and many were matched from pokebw2's
    C (`config/b_us/twin_files.tsv`).
  - The build system (`configure.py`, compiler flags, library layout) is adapted from it, and our NitroSystem, SDK and
    Wi-Fi library C is re-ported from it. Many names and struct layouts come from it, and through it from
    [swan](https://github.com/ds-pokemon-hacking/swan).
- **[libntr](https://github.com/cybervisi0n/libntr)** (MIT): the NitroSDK for PC the whole port runs on.
- **[pret](https://github.com/pret)**: Platinum is built from their [pokeplatinum](https://github.com/pret/pokeplatinum)
  and HeartGold/SoulSilver from their [pokeheartgold](https://github.com/pret/pokeheartgold), made to run on 64-bit
  hosts by our patches. All credit for the decompilations goes to pret and its contributors.
- The DS decompilation community: the other pret projects, [zeldaret](https://github.com/zeldaret)'s
  st and ph and the kh decompilations (Metrowerks library C), taxicat1's dsprot, and the authors of dsd and
  decomp-permuter.
- SDL2, Dear ImGui, Mesa, enet, SMAA, stb and the rest: see [THIRD_PARTY.md](THIRD_PARTY.md).
