# Credits and third-party licenses

bw-port is licensed GPL-3.0 as a whole (`LICENSE`). Everything below is either code or data we took, a library the
port links, a tool the build downloads, or a project we only read. License texts are in `licenses/`, and beside the
code they cover (`lib/nitro/LICENSE-*`, `lib/msl/LICENSE-*`, `port/gpu/LICENSE-SMAA`, `port/src/LICENSE-imgui`). A file taken
from another project says so in its header, with the project, the commit and the license.

## Thanks

- **pokebw2** ([fuddlesworth/pokebw2](https://github.com/fuddlesworth/pokebw2), GPL-3.0): the Pokémon Black 2 / White 2
  decompilation. We adapted its build system (`configure.py`, the compiler flags and the library layout) and used its C as
  the source for Black's functions that have an identical twin in Black 2 / White 2: `config/b_us/twin_files.tsv` lists
  21,574 of Black's functions with a twin in 1,419 of its files (`config/b_us/pokebw2.txt` pins the commit). Many names and
  struct layouts come from it, and through it from [swan](https://github.com/ds-pokemon-hacking/swan) (GPL-3.0).
- **cybervisi0n**'s [libntr](https://github.com/cybervisi0n/libntr) (MIT): the drop-in NitroSDK/TwlSDK for PC that the whole
  port is built on, and the source of a large part of `lib/nitro`. (Its companion libntrsystem, NitroSystem for PC, was
  the port's NitroSystem until 2026-11-07; it had no licence and is no longer used: the port builds `lib/nnsys`, our own.)
- The DS decompilation community: the [pret](https://github.com/pret) projects (names and structure; Platinum and
  HeartGold/SoulSilver are also built from theirs, see below), zeldaret's
  [st](https://github.com/zeldaret/st) and [ph](https://github.com/zeldaret/ph), khdays-decomp, kh-recoded-decomp,
  pictochat-decomp, taxicat1's [dsprot](https://github.com/taxicat1/dsprot), the dsd and decomp-permuter authors.

## Taken into this repository

| Project | License | What we took, where | Notice kept |
| --- | --- | --- | --- |
| [pokebw2](https://github.com/fuddlesworth/pokebw2) | GPL-3.0 | Toolchain adapted (`configure.py`, `config/compiler_flags.toml`, `.claude/hooks/guard.py`); C for Black functions with a Black 2 twin (src/, lib/, include/); Black 2 / White 2 themselves: its C is vendored in `third_party/pokebw2` (commit 921123bed, patched by `port/subprojects/pokebw2-*.patch`, GPL-3.0, with LICENSE; the games' data tables removed: they are read from the user's ROM) and compiled; symbol names; `config/b_us/twins_b_us*.tsv`, `twin_files.tsv` | file header lines; same license as ours |
| [pret/pokeheartgold](https://github.com/pret/pokeheartgold) | none upstream (no license file) | Vendored with our changes in `third_party/pokeheartgold` (`PIN` names the commit of our working fork, 79ce66778 at this release): pret's HeartGold / SoulSilver decompilation plus our decompilation of functions it still has in assembly (`port/nonmatching`) and 64-bit typed data (`port/asmdata`); only what the port's build reads: `main.lsf.in`, `include/`, `src/` (C), library headers (among them 11 DS Protect headers in `lib/dsprot/include`, as pret carries them; the dsprot project has no license either) and libspl's C, the data-only `.s` it links, the script headers of `files/`, and the generated message-ID / NARC-index headers (`gen/`). Removed: every asset (`files/`: graphics, sound, text, NARC sources), the assembly with functions, pret's tools, docs and build files; the game's text quoted in comments; the game's data is read from the user's ROM. Made by `tools/hg_vendor.py`; the port's patches (`port/subprojects/pokeheartgold-*.patch`) apply to a build-dir copy (`build/pokeheartgold`). All credit for the decompilation to pret and its contributors | none: pret has no license file to keep |
| [pret/pokeplatinum](https://github.com/pret/pokeplatinum) | none upstream (no license file) | **Not in this repository**: `third_party/pokeplatinum` is a git submodule that points at pret's repository (commit 1776eba4c), fetched by `git submodule update --init` or `tools/pokeplat_gen.py`. Ours: the port's patches to it (`port/subprojects/pokeplatinum-*.patch`, GPL-3.0), applied in a build-dir copy (`build/pokeplatinum`) | none needed: nothing of pret's is copied here (the patches say in their headers what they change) |
| [swan](https://github.com/ds-pokemon-hacking/swan) | GPL-3.0 | Names of functions, scripts and structs, through pokebw2 (`include/app/**`, notes in headers) | header notes |
| [libntr](https://github.com/cybervisi0n/libntr) | MIT | Headers and sources ported as they are: `lib/nitro/sdk`, `lib/nitro/include`, `lib/nitro/src` | `lib/nitro/LICENSE-libntr`, `lib/nitro/sdk/LICENSE-libntr`, `licenses/libntr-MIT.txt` |
| [zeldaret/st](https://github.com/zeldaret/st) | CC0-1.0 | Metrowerks Standard Library C: `lib/msl/include`, `lib/msl/src` | `lib/msl/LICENSE-zeldaret-st` |
| [khdays-decomp](https://github.com/Yokimitsuro/khdays-decomp) | CC0-1.0 | `__strtoul`, `strtol` (`lib/msl/src/strtoul.c`) | header |
| [kh-recoded-decomp](https://github.com/kh-recoded-decomp/kh-recoded-decomp) | MIT (`eu/`: CC0) | qsort (`eu/`, CC0); names and the layout of the card library's internals (`lib/nitro/src/card`, MIT) | `lib/nitro/LICENSE-kh-recoded-decomp` |
| [SMAA](https://github.com/iryoku/smaa) (Jimenez et al.) | MIT | Edge, weight and blend passes ported to GLSL / C: `port/gpu/shaders/g08_smaa*`, `port/gpu/src/g08_smaa*` | file headers, `port/gpu/LICENSE-SMAA` |
| [MMPX](https://casual-effects.com/research/McGuire2021PixelArt/) (McGuire and Gagiu) | MIT | The 2D filter's MMPX rules ported to GLSL / C: `port/gpu/shaders/g11_up_mmpx.glsl`, `port/gpu/tools/up2d_ref.c` | file headers, `licenses/MMPX-MIT.txt` |
| [Dear ImGui](https://github.com/ocornut/imgui) | MIT | Metal backend: `port/src/imgui_impl_metal.{h,mm}` (unmodified but an include) | file headers, `port/src/LICENSE-imgui` |
| [stb](https://github.com/nothings/stb) | MIT or public domain | `port/src/stb_image.h`, `stb_image_write.h` | in the files |
| Khronos GL headers | MIT-style (Khronos) | `port/gpu/include/{GLES3,KHR}`, `port/bridge/glinc` | in the files |
| [Winpthreads / MinGW-w64](https://www.mingw-w64.org/) | MIT-style | Linked into the Windows exe (the DLL is shipped, see below) | `licenses/winpthreads-COPYING.txt` |

Read, not copied. These projects were read for names, structure and behaviour; the C in this repository is written from them,
not copied: pret's pokediamond and pmd-sky (no license; pokeplatinum is a submodule and pokeheartgold is vendored: both above), taxicat1/dsprot (no license), Martin
Korth's GBATEK (the hardware reference for `port/vcore`, `port/gpu` and `port/acore`). No C or header in `lib/`, `src/` or
`include/` is derived from [ntrtwl](https://github.com/ntrtwl)'s repositories any more (no license): see "Provenance".

## Fetched when you build (not in this repository)

| Project | License | Used for |
| --- | --- | --- |
| [ds-decomp (dsd)](https://github.com/fuddlesworth/ds-decomp), fork of [AetiasHax/ds-decomp](https://github.com/AetiasHax/ds-decomp) | MIT | Extracts, delinks and relinks the ROM (`configure.py` downloads it) |
| [wibo](https://github.com/decompals/wibo) | MIT | Runs the Windows compiler under Linux (downloaded) |
| Metrowerks CodeWarrior ARM tools (mwccarm) | proprietary | The original compiler; downloaded by `configure.py` from a third-party host, never stored here |
| [libntr](https://github.com/cybervisi0n/libntr) | MIT | Not stored here: `port/subprojects/libntr.wrap` fetches upstream commit f6b3a69, then meson applies our changes to it (`port/subprojects/packagefiles/libntr-ours.patch`) |
| [m2c](https://github.com/matt-kempster/m2c) (GPL-3.0), [decomp-permuter](https://github.com/simonlindholm/decomp-permuter) (MIT), [Ghidra](https://github.com/NationalSecurityAgency/ghidra) (Apache-2.0) | as listed | Optional helpers of the decompilation tools (`tools/`) |
| Temurin JDK 17 (GPL-2.0 + Classpath Exception), Gradle (Apache-2.0), Android NDK / SDK | as listed | Build tools of the Android package only |
| [meson](https://github.com/mesonbuild/meson) 1.12.0 (Apache-2.0), [metang](https://github.com/lhearachel/metang) (Apache-2.0), [yyjson](https://github.com/ibireme/yyjson) (MIT) | as listed | Run by pret's header generators when Platinum or HeartGold/SoulSilver is built (`tools/pokeplat_gen.py`, `tools/pokehg_gen.py`); nothing of them is in the binaries. The wrap files of pret's projects also name ntrtwl's NitroSDK and NitroSystem; the generators put empty stand-ins there and never fetch them |

## Inside the port's binaries

What a Windows, Android or Apple build contains. libntr compiles its bundled sources into the executable.

| Component | License | Where it is | Notice goes |
| --- | --- | --- | --- |
| libntr | MIT | static, all platforms | `licenses/libntr-MIT.txt` |
| Dear ImGui, ImGuiFileDialog (bundled by libntr) | MIT | static | `licenses/imgui-MIT.txt`, `ImGuiFileDialog-MIT.txt` |
| blip-buf (bundled by libntr) | LGPL-2.1 | static | `licenses/blip-buf-LGPL-2.1.txt`; the source is this repository, libntr's pinned commit |
| fireflower (bundled by libntr) | BSD-3-Clause | static | `licenses/fireflower-BSD-3-Clause.txt` |
| glad (bundled by libntr) | generated code: public domain / CC0 | static | none needed |
| [SDL2](https://libsdl.org) 2.32.x | zlib | `SDL2.dll`, `libSDL2.so`, static on Apple | `licenses/SDL2-zlib.txt` |
| [enet](http://enet.bespin.org) 1.3.x | MIT | `enet.dll`, static on Android | `licenses/enet-MIT.txt` |
| [Mesa](https://www.mesa3d.org) llvmpipe and Zink (OpenGL on the system's Vulkan: the graphics driver's `vulkan-1.dll`, not shipped), from [pal1000/mesa-dist-win](https://github.com/pal1000/mesa-dist-win) (MIT) | MIT; contains LLVM 22.1.8 (Apache-2.0 with LLVM exceptions), zstd 1.5.7 (BSD-3-Clause) and zlib 1.3.2 (zlib), all static | `mesa\opengl32.dll`, `mesa\libgallium_wgl.dll` (Windows only, used when the system has no usable OpenGL: on the graphics card through Zink, else on the CPU through llvmpipe) | `licenses/Mesa-MIT.txt`, `mesa-dist-win-MIT.txt`, `zstd-BSD-3-Clause.txt`; LLVM's exception text is in `LLVM-libcxx-Apache-2.0-with-LLVM-exception.txt` |
| DXIL signer `dxil.dll` 1.9.2609.5 (Microsoft; byte-identical to `bin\x64\dxil.dll` of the [DirectXShaderCompiler](https://github.com/microsoft/DirectXShaderCompiler) release v1.9.2609, as shipped in mesa-dist-win) | Microsoft Software License Terms for the DirectX Shader Compiler (`LICENSE-MS.txt` of that release): NOT open source. Distribution only as "distributable code" under conditions (see the release report); contains LLVM code (University of Illinois/NCSA) | **not shipped** (optional, supplied by the user: if `mesa\dxil.dll` is there, Mesa's d3d12 driver is the fallback when Zink/Vulkan is not; `tools/port_win64.sh --dxil` puts it in a private folder, never in a public zip) | `licenses/DirectXShaderCompiler-dxil-Microsoft-LICENSE-MS.txt`, `licenses/DirectXShaderCompiler-LLVM-NCSA.txt` (for private folders made with `--dxil`; nothing of it is in a public zip) |
| libwinpthread | MIT-style | `libwinpthread-1.dll` (Windows) | `licenses/winpthreads-COPYING.txt` |
| LLVM libc++ | Apache-2.0 with LLVM exceptions | `libc++_shared.so` (Android) | `licenses/LLVM-libcxx-Apache-2.0-with-LLVM-exception.txt` |
| SMAA | MIT | static (`port/gpu`) | `licenses/SMAA-MIT.txt` |
| MMPX | MIT | static (`port/gpu`) | `licenses/MMPX-MIT.txt` |
| kh-recoded-decomp (names and layout of the card library's internals) | MIT | static (`lib/nitro/src/card`) | `licenses/kh-recoded-decomp-MIT.txt` |
| pret's pokeplatinum and pokeheartgold C (the Platinum and HeartGold/SoulSilver builds only) | none upstream | static, one module object per game, with our patches | none: pret has no license file to keep |

`tools/port_win64.sh` copies `licenses/`, `THIRD_PARTY.md` and `LICENSE` into the Windows folder; `tools/android_package.sh` puts them
into the APK's assets; `tools/ios_package.sh` puts the same files in the app bundle's `licenses/` (added 2026-11-13: it did not before).

## Not shipped, but used

melonDS (GPL-3.0), DeSmuME (GPL-2.0-or-later) and NooDS (GPL-3.0) are used as test oracles only, outside this repository.
libretro-thumbnails box art is downloaded by the launcher on the user's own device when they ask for covers, not distributed.

## Provenance of lib/ (every ntrtwl or "libraries/" mention in lib/, src/ and include/)

`grep -rIiE ntrtwl lib src include` finds nothing. `grep -rIE "libraries/" lib src include` finds 102 files, all in `lib/nitro`
and `lib/twl`, and every one is a path in libntr (cybervisi0n, MIT, credited in the header: "From libntr (MIT, ...) at 1555d91,
libraries/<module>/src/<file>.c: ported as it is"), or a comment that says where in libntr a declaration is. Where a commit message
of such a file says "ntrtwl-NitroSDK" (the early ports, rtc/external.c's second commit), the file is libntr's text: libntr
carries the same SDK code under the MIT licence, and `tools/port_lib.py` takes libntr's file first. The audit of all 473 files
of `lib/nitro` and `lib/twl` (header, first commit and every commit whose message names a reference or `tools/port_lib.py`):

| Source named in the header | Files | Licence |
|---|---|---|
| libntr (cybervisi0n), alone or with pokebw2 / zeldaret next to it | 355 | MIT |
| pokebw2 | 92 | GPL-3.0, same as this repository |
| zeldaret (st, ph), khdays-decomp, kh-recoded-decomp (libc/MSL headers in `lib/nitro/sdk`) | 14 | CC0 / MIT |
| none: umbrella headers (`ioreg.h`, `memorymap.h`, `fx_mtx.h`: includes only), `ltdmain_begin.h`/`_end.h`, `fx_vec.c` and `os_irqHandler_dtcm.c` (ours, from Black's code and pokebw2's names), `dsp_g711_component.c` (an asset include) | 12 | ours |
| derived from ntrtwl | 0 | n/a |

The online libraries were handled in docs/online-libs.md (59 sources deleted, headers rewritten); `lib/dwc`, `lib/dwcutil`,
`lib/nitrowifi` and `lib/poke_net` now name ntrtwl nowhere. Where a header says "NitroDWC's" or "NitroWiFi's", that is the name of
a type or file the SDK has (names and boundaries are facts), not text taken from it.

## Open questions (for the maintainer)

- **libntrsystem (no licence): removed 2026-11-07.** The port's NitroSystem is `lib/nnsys` (re-ported from pokebw2, GPL-3) on
  libntr; docs/nnsys-port.md.
- **ntrtwl: settled 2026-11-07.** Its repositories have no license, so the sources derived from them were re-ported from
  pokebw2 (lib/nnsys, lib/nitrowifi's WCM) or deleted and left as ROM code (lib/dwc, lib/dwcutil, lib/nitrowifi: docs/online-libs.md);
  the headers that still carried its text were rewritten on pokebw2's. "Provenance" lists what is left.
- **No license: pret, taxicat1/dsprot.** From pret's other projects and from dsprot only names and layouts were taken, and
  the file headers say so. pokeplatinum's code is not in this repository (a submodule: the build fetches it from pret; our
  patches to it carry a few of its lines as diff context). pokeheartgold's code is vendored with our changes, without a
  license from pret: the maintainer's decision.
- **Metrowerks MSL** (`lib/msl`) is a reconstruction of a proprietary library, published by zeldaret and others as CC0; their license is what we rely on.
