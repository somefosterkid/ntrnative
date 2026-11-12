# NTR Native

Decompiled Nintendo DS games rebuilt as native apps, with our own renderers and sound core, for Pokémon Black, White,
Black 2 and White 2. NTR is the DS's product code. Bring your own ROM: none is included or will be provided.

## Games

| Game (USA) | SHA-1 |
| --- | --- |
| Black | `26ad0b9967aa279c4a266ee69f52b9b2332399a5` |
| White | `bc696a0dfb448c7b3a8a206f0f8214411a039208` |
| Black 2 | `e51e6dfb8678a3d19dcd2a10691b96a569ca0abb` |
| White 2 | `b5d7490be7b415b8f1e672a53e978a9cc667e56a` |

All four play on Windows, Linux and Android, in one app that switches between them; macOS and iOS are coming. Every
game has been tested playable from a new game to the first gym, by hand and by scripted runs; scripted runs through the
rest of all four games are in progress. Android is tested on an emulator only. Online play, DS Protect and the DSi-only features are not supported. Black 2 and White 2:
[docs/black2-white2.md](docs/black2-white2.md).

Coming later: Pokémon Diamond, Pearl, Platinum, HeartGold and SoulSilver, in active development, and mods such as Nuzlocke and
randomizer modes, which are plain C changes once a game is decompiled.

## Features

- **Fast**: hundreds of frames a second at native resolution on a desktop CPU.
- **Speed options**: fast forward and Game speed up to 4x or unlimited, with the music kept at normal speed and pitch;
  or speed up only the parts you choose: walking, running, Surf, text, battles, transitions and saving.
- **Picture**: resolution 1x to 8x, antialiasing, frame rate 60, 120, your monitor's or unlimited, and Smooth motion
  (in-between frames drawn from the game's own 3D, not blended).
- **Sound**: Enhanced (the default) smooths the DS's samples for cleaner, higher-fidelity audio; DS mode is the
  original output, bit for bit.

## Build and run

Linux or WSL, Python 3.11+, ninja. Put your dumps in `orig/` as `baserom_b_us.nds`, `baserom_w_us.nds`,
`baserom_b2_us.nds` and `baserom_w2_us.nds` (any you have). First the ROM build, which checks them and writes the
data the C needs:

```sh
python3 configure.py
ninja            # build/pokeblack_us.nds (and White's), SHA-1 checked
```

`configure.py` downloads dsd, wibo and the Metrowerks tools on first run. Then the port:

| Platform | Command | Doc |
| --- | --- | --- |
| Linux | `meson setup build/port/linux port --native-file port/machine/linux.ini && ninja -C build/port/linux bwport` | [port-build](docs/port-build.md) |
| Windows | `tools/port_win64.sh` (cross-builds `bwport.exe` from WSL or Linux into a folder with `bwport.bat`) | [port-build](docs/port-build.md) |
| Android | `tools/android_deps.sh`, then `tools/android_package.sh --build` (APK) | [android-parity](docs/android-parity.md) |

A build includes every game whose ROM is in `orig/`; Black 2 and White 2 are built from the vendored, patched pokebw2
code in `third_party/pokebw2` (no network). The app imports your ROM on first start; saves and settings live beside it
([docs/launcher.md](docs/launcher.md)).

## How it works

- **Decompilation**: about 96% of Black's functions (34,300 of 35,650) are C that compiles to the original bytes with the
  original Metrowerks compiler. The rest is working C that is not yet byte-matched ([docs/nonmatching.md](docs/nonmatching.md)),
  plus Nintendo's online (Wi-Fi) library, which comes from your ROM and is unused: online play is stubbed. White builds from
  the same source ([docs/white.md](docs/white.md)).
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

Work in progress. GPL-3.0, see [LICENSE](LICENSE); every project we use and its license is in
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
- The DS decompilation community: the [pret](https://github.com/pret) projects, [zeldaret](https://github.com/zeldaret)'s
  st and ph and the kh decompilations (Metrowerks library C), taxicat1's dsprot, and the authors of dsd and
  decomp-permuter.
- SDL2, Dear ImGui, Mesa, enet, SMAA, stb and the rest: see [THIRD_PARTY.md](THIRD_PARTY.md).
