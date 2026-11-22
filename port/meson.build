# The PC build of Pokemon Black (docs/port-build.md): the decomp's game C, compiled as it is, on libntr (a NitroSDK and
# TwlSDK for PC, a meson subproject: port/subprojects/libntr.wrap) and our NitroSystem (lib/nnsys, below). The sources
# are tools/port_sources.py's: src/, lib/gfl, lib/poke_net and lib/spl minus port/exclude.txt, plus every port/src/*.c.
# lib/nitro and lib/twl (libntr's in their place), lib/dwc*, lib/nitrowifi, lib/dpw,
# lib/dsprot (online and anti-piracy: port/src stubs) and lib/msl (the host's libc) are not compiled.
#
#   meson setup build/port/linux port --native-file port/machine/linux.ini
#   meson setup build/port/win64 port --native-file port/machine/win64.ini -Dbuild_target=win64
#   ninja -C build/port/linux bwport
project('bwport', 'c', 'cpp',
  default_options: {'default_library': 'static', 'buildtype': 'debug', 'warning_level': '0'},
  meson_version: '>=1.3.0')

fs = import('fs')
repo = fs.parent(meson.current_source_dir())
python = find_program('python3')
build_target = get_option('build_target')
# macOS (build_target linux, Apple clang): ld64 has no linker scripts and no --wrap (the maintainers' notes, "Mac")
is_mac = host_machine.system() == 'darwin'
# iOS (device or Simulator; port/machine/ios.ini, ios-sim.ini): the macOS build with SDL's UIKit and OpenGL ES (the maintainers' notes, "iOS")
is_ios = is_mac and meson.get_compiler('c', native: true).get_define('TARGET_OS_IOS', prefix: '#include <TargetConditionals.h>') == '1'
# Android (port/machine/android-arm64.ini, android-x86_64.ini): the Linux build with the NDK's clang; the game is SDL's main
# library, libmain.so (the maintainers' notes, "Android")
is_android = meson.get_compiler('c', native: true).get_define('__ANDROID__') != ''
if build_target not in ['linux', 'win64']
  error('build_target: linux or win64')
endif

# Ubuntu's compilers turn _FORTIFY_SOURCE on at -O2, which aborts in libsim's G3 clip matrix copy
# (docs/libntr/build.md). Global arguments reach the subprojects; libntr's PC targets are all native: true.
# (Apple's headers want =0: with -U their memset/memcpy macros collide with lib/msl's declarations)
add_global_arguments(is_mac ? '-D_FORTIFY_SOURCE=0' : '-U_FORTIFY_SOURCE', language: ['c', 'cpp'], native: true)
if build_target == 'win64'
  # mingw lays out bitfields and the 64-bit integers next to them as MSVC does unless told not to: with
  # -mno-ms-bitfields the boot path's structs (5 save blocks among them) match the DS's layout, as Linux's do.
  # Global, so that libntr's side of a struct agrees.
  add_global_arguments('-mno-ms-bitfields', language: ['c', 'cpp'], native: true)
  # __thread as mingw gcc (libntr's C++) does it, through libgcc's __emutls_get_address: clang's native TLS (gs:0x58,
  # _tls_index; Ubuntu's mingw-w64 11 CRT, GNU ld or lld) faults on the first read here, even in a 3-line main
  # (port/src/main_threads.c's tDepth in PORT_RuntimeInit, before the game: docs/libntr/build.md)
  add_global_arguments('-femulated-tls', language: 'c', native: true)
endif

# No absolute build paths in a shipped binary (__FILE__ of the assert strings, debug info): the checkout root becomes "."
# (tools/release_check.py scans the packages for what is left). ccache leaves these three options out of its hash.
# The build dir is under the root, so its generated files map to ./build/...
add_global_arguments('-ffile-prefix-map=' + repo + '=.', language: ['c', 'cpp'], native: true)
if is_mac   # (the Metal core and bridge, apple_thermal.m, the settings sheet; metal_present.mm): here, as meson takes global arguments only before the first target
  add_languages('objc', 'objcpp', native: true)
  add_global_arguments('-ffile-prefix-map=' + repo + '=.', language: ['objc', 'objcpp'], native: true)
endif
if get_option('nitro_header')
  add_global_arguments('-include', repo / 'port/pch/force_nitro.h', language: 'c', native: true)
endif
# dev_fast (meson.options): function names and lines in a backtrace are all a dev build needs of the debug info (250 of the exe's 328 MB)
dev_fast = get_option('dev_fast')
if dev_fast
  foreach lang : ['c', 'cpp']   # (the C++ compiler can be gcc beside clang)
    add_global_arguments(meson.get_compiler(lang, native: true).get_id() == 'clang' ? '-gline-tables-only' : '-g1',
                         language: lang, native: true)
  endforeach
endif

nitrosdk_dep = dependency('NitroSDK', fallback: ['libntr', 'nitrosdk_dep'], native: true)
# Our NitroSystem (lib/nnsys, docs/nnsys-port.md; libntrsystem's place until 2026-11-07). Its headers here (the umbrella
# <nnsys.h> port_prefix.h includes is port/include_nnsys/nnsys.h), the library below the game's flags
nitrosystem_dep = declare_dependency(compile_args: ['-I' + repo / 'port/include_nnsys', '-I' + repo / 'lib/nnsys/include'])

sources = run_command(python, repo / 'tools/port_sources.py', check: true).stdout().strip().split('\n')

# The second game, Pokemon White (docs/launcher.md "More than one game" and both "Step 3 as built", docs/white.md): the PC
# (Linux, Windows, Android) links it renamed (the w_ header below), macOS and iOS as one relocatable object (white.o below)

# The game's include dirs in configure.py's order (INCLUDE_DIRS), with libntr's and libntrsystem's (from the
# dependencies) where lib/nitro's, lib/twl's and lib/nnsys's were. port/include first: the SDK header names pokebw2
# uses and libntr lacks (nitro/fx.h ...) and port_prefix.h, which every file includes first (as libntr's
# include/pch/nitro_pch.h has its users do). MSL after the host's libc: libntr's thread.h includes SDL2, which
# includes the host's headers, and MSL's stdio.h, stdarg.h ... can't stand in for them.
game_includes = ['-I' + repo / 'port/include', '-I' + repo / 'include']
foreach lib : ['dpw', 'dsprot', 'dwc', 'dwcutil', 'nitrowifi', 'poke_net', 'spl']
  game_includes += '-I' + repo / 'lib' / lib / 'include'
endforeach
# asset/<name>.inc: placeholders (tools/port_assets.py: every assets.tsv row as zeros of the host's layout, the
# pointer fields' names from config/b_us/asset_ptrs.tsv), so the build reads no ROM; port/src/asset_load.c copies each row
# from the player's ROM at start (every platform; Apple since 2026-11-17). With the ROMs in orig/ (Apple: tools/mac_rom.sh's
# dir; -Drom_dir names another) the link also goes through tools/port_romfill.py (the bwport target below).
asset_inc_dir = meson.current_build_dir() / 'asset_ph'
black_asset_inc = '-I' + asset_inc_dir
white_asset_inc = '-I' + asset_inc_dir / 'w_us'
game_includes += [black_asset_inc]
asset_deps = []
asset_tool = repo / 'tools/port_assets.py'
# White's config (config/w_us, not committed: Black's and the delta; configure.py writes it too, but a port-only build
# never runs configure.py): its assets.tsv for the rows below, its overlays' layout further down
run_command(python, repo / 'tools/version_config.py', 'w_us', check: true)
asset_tsvs = [repo / 'config/b_us/assets.tsv', repo / 'config/w_us/assets.tsv', repo / 'config/b2_us/assets.tsv',
              repo / 'config/w2_us/assets.tsv']
# (HeartGold's and SoulSilver's: tools/hg_assets.py's, in the vendored tree; rows only, no placeholders step)
asset_row_tsvs = asset_tsvs + [repo / 'third_party/pokeheartgold/port/assets_hg.tsv', repo / 'third_party/pokeheartgold/port/assets_ss.tsv']
# each version's rows for asset_load.c (names, DS ranges, layouts: no ROM bytes; every platform links the loader)
asset_rows_c = custom_target('asset_rows',
  output: 'asset_rows.c',
  depend_files: [asset_tool, repo / 'port/asset_rows_fnv.tsv'] + asset_row_tsvs,
  command: [python, asset_tool, 'rows', '@OUTPUT@'])
# the placeholders (the rows' shapes, every number 1) in asset_ph/asset and asset_ph/w_us/asset
asset_deps += declare_dependency(sources: custom_target('asset_placeholders',
  output: 'asset_placeholders.stamp',
  depend_files: [asset_tool, repo / 'tools/asset_table.py', repo / 'config/b_us/asset_ptrs.tsv',
                 repo / 'config/b2_us/asset_ptrs.tsv'] + asset_tsvs,
  command: [python, asset_tool, 'incs', asset_inc_dir, '@OUTPUT@']))
# the sources that include an asset are compiled from rewritten copies (const dropped, a constructor registers each table);
# a row added or moved changes assets.tsv, which makes meson run this list again (configure_file's input)
foreach t : asset_tsvs
  configure_file(input: t, output: 'asset_list_' + fs.name(fs.parent(t)) + '.tsv', copy: true)
endforeach
asset_files = run_command(python, asset_tool, 'list', check: true).stdout().strip().split('\n')
asset_gen = generator(python,
  output: '@PLAINNAME@',
  depfile: '@PLAINNAME@.d',
  arguments: [asset_tool, 'src', '@INPUT@', '@OUTPUT@', '@DEPFILE@'])
# -Dgames=black (port/meson.options): the exe holds only the listed games; each per-game option below still applies as well
games_wanted = get_option('games')
build_white = get_option('white').allowed() and 'white' in games_wanted   # (its data: placeholders, filled from White's ROM at start)
# the ROMs the whole-image fill reads (below; none: the link as it is): orig/, on Apple tools/mac_rom.sh's dir (the main
# checkout's orig/: a worktree has none), or -Drom_dir (an empty folder: a build that sees no ROM)
b2_rom_dir = get_option('rom_dir') != '' ? get_option('rom_dir') : repo / 'orig'
if is_mac and get_option('rom_dir') == ''
  b2_rom_dir = fs.parent(run_command(repo / 'tools/mac_rom.sh', check: true).stdout().strip())
endif
# Black 2 and White 2 (GNU ld, lld, PE, ld64: tools/port_relink.py), with or without their ROMs: their data rows are
# placeholders filled from the player's ROM at start, as Black's (the sha1s are checked at run time:
# port/src/launcher_core.c); -Db2w2=disabled leaves them out
build_b2 = get_option('b2w2').allowed() and 'b2w2' in games_wanted
build_w2 = build_b2

# Platinum's module (below): on by default, with or without its ROM (without it the overlays' DS layout comes from the
# player's ROM at start: asset_load.c PORT_AssetRomOverlays)
build_plat = get_option('platinum') and 'platinum' in games_wanted
# HeartGold's and SoulSilver's (below Platinum's): on by default, win64 too (port/subprojects/pokeheartgold-zwin64.patch),
# macOS and iOS too (ld64: port_relink.py --macho, pokehg_asm.py's Mach-O names)
build_hg = get_option('hgss') and 'hgss' in games_wanted

# A precompiled prefix per target (docs/port-build.md "Precompiled prefix"): the forced prefix header (port_prefix.h) is ~75k lines of
# libntr, SDL2 and the host's libc, parsed again by each of the 17k compiles (1.3 s of a 1.3-1.9 s compile, measured). meson's c_pch
# makes one clang .pch per target from the target's own flags and adds -include-pch to its compiles (the -include of the prefix stays in
# the flags: it finds its guard defined). The objects are the same but for debug info.
# NOT through c_pch on the targets that get `-include gnu_*_section.h` (Black 2 / White 2 / Platinum / HG/SS): clang bakes the PCH-time
# `-include` list into the .pch and skips the same names at use, while `#pragma clang section` is not stored in a .pch, so their variables
# landed in .bss/.data instead of .plovlbss/.hgovldata... (885 objects differed, tb3b 11-20; docs/port-build.md "Precompiled prefix"). They
# get a probe library below (tb3c): its .pch is made without the section include, theirs comes after `-include-pch`.
# Clang only (apt's clang 21 here), Linux for now: -Dpch=disabled leaves it off; the ccache .ini adds -fno-pch-timestamp.
use_pch = get_option('pch').enabled() or (get_option('pch').auto() and build_target == 'linux' and not is_mac and not is_android
                                          and meson.get_compiler('c', native: true).get_id() == 'clang')
pch_bw = use_pch ? ['pch/bw_prefix.h'] : []
pch_gen4 = use_pch ? ['pch/gen4_prefix.h'] : []
# The Gen 4 and Black 2 / White 2 targets: their own PCH from a probe library (one empty source, the target's flags MINUS the
# `-include gnu_*_section.h`), and the real targets pass `-include-pch` of it themselves, their section -include after it, which
# clang then applies (a `#pragma clang section` is not stored in a .pch and a .pch skips the -include files it was made with).

game_args = [
  '-fms-extensions',       # a function prototyped extern and defined static (86 files): MWCC takes it, clang only so
  '-fsigned-char',         # MWCC -char signed
  '-fcommon',              # tentative definitions in several files, as MWCC links them
  '-fno-strict-aliasing',
  '-fwrapv',
  '-fno-delete-null-pointer-checks',   # the DS reads address 0 quietly, so a pointer may be tested after use: -O2 would drop the test
  '-w',
  '-include', 'port_prefix.h',
] + game_includes + ['-idirafter', repo / 'lib/msl/include']
# a local the C reads before it writes is 0, as on every run, not the host stack's leftovers: most of the sanitizer's
# uninitialised reads are the original game's own (admin-79 hk129: 54 of 75 on HG/SS), whose DS stack held what the
# last call left; zero makes the run deterministic. Every game module (Black, White, Black 2/White 2 and Platinum, whose
# pl_args take game_args) and port/src's stand-ins built with game_args.
game_args += '-ftrivial-auto-var-init=zero'
if build_target == 'win64'
  # win64's s32 and u32 are long and unsigned long (libntr): a callback of int where s32 is expected is the same
  # 32 bits there, but clang calls the pointer types incompatible
  game_args += '-Wno-error=incompatible-function-pointer-types'
endif
if is_mac
  game_args += '-D_FORTIFY_SOURCE=0'   # after libntr's global -U, which would leave Apple's headers fortifying
endif
if build_white
  game_args += '-DBWPORT_WHITE'   # port/src: White's table (port/include/port_game.h), its sha1 in launcher_core.c
endif
if build_b2
  game_args += '-DBWPORT_B2'   # port/src: Black 2's table (port/include/port_game.h), its sha1 in launcher_core.c
endif
if build_w2
  game_args += '-DBWPORT_W2'   # port/src: White 2's table and sha1
endif
if build_plat
  game_args += '-DBWPORT_PLAT'   # port/src: Platinum's launcher row (games.c) and its sha1 (launcher_core.c)
endif
if build_hg
  game_args += '-DBWPORT_HG'   # port/src: HeartGold's table (games.c)
endif
game_args += '-DBWPORT_OVL_SCRIPT'   # port/src/ovl.c: the overlays' data is gathered (a linker script, tools/port_overlays.py; on macOS BWPORT_OVL_MACHO)
machine_args = is_mac ? ['-fno-common', '-include', meson.current_source_dir() / 'machine/macos_machine_section.h'] : []

# Our NitroSystem: lib/nnsys's C with the game's flags (its data the machine's: libnnsys.a is in
# tools/port_overlays.py's MACH_ARCHIVES, and on Apple the machine's sections) and port/include_nnsys/nnsys_lib.h first
# (libntr's member names, the DS registers it writes: port/src/nnsys_g3stage.c); the port's side (port/src/nnsys_*.c,
# tools/port_sources.py --nnsys-port) with the game's flags only. One library for every game in the exe
nnsys_libs = []
if true
  game_args += '-DBWPORT_NNSYS'   # port/src/machine.c: the restart's reset of port/src/nnsys_*.c
  nnsys_kw = {'dependencies': [nitrosdk_dep.partial_dependency(compile_args: true, includes: true, sources: true),
                               nitrosystem_dep],
              'override_options': ['c_std=gnu99'], 'implicit_include_directories': false, 'native': true, 'c_pch': pch_bw}
  nnsys_libs += static_library('nnsys',
    run_command(python, repo / 'tools/port_sources.py', '--nnsys', check: true).stdout().strip().split('\n'),
    c_args: game_args + machine_args + ['-include', repo / 'port/include_nnsys/nnsys_lib.h'] +
            (is_mac ? ['-DBWPORT_ROM_SHARED'] : []),   # (its read-only data in group s: machine/macos_rom_section.h)
    kwargs: nnsys_kw)
  nnsys_libs += static_library('nnsys_port',
    run_command(python, repo / 'tools/port_sources.py', '--nnsys-port', check: true).stdout().strip().split('\n'),
    c_args: game_args, kwargs: nnsys_kw)
endif

# The game's entry, NitroMain on the DS, is src/system/main.c's main(); libntr's main() (sim_main.cpp) sets up the
# simulated DS and calls NitroMain(). That one file is built apart, with main renamed.
entry_c = repo / 'src/system/main.c'
game_sources = []
foreach s : sources
  if s != entry_c
    game_sources += s
  endif
endforeach
# macOS and iOS: the game's C (and port/src's stand-ins for the DS's side; the rest of port/src is the host's) puts its
# writable data in the machine's sections (machine_args, above), which port/src/machine.c puts back at an in-process
# restart (docs/launcher.md, section 1)
entry = static_library('entry', entry_c,
  c_args: game_args + machine_args + ['-DBWPORT_ENTRY'], c_pch: pch_bw,
  # the headers and defines only: the link gets libntr's libraries and crt0 objects once, from the executable
  dependencies: [nitrosdk_dep.partial_dependency(compile_args: true, includes: true, sources: true),
                 nitrosystem_dep.partial_dependency(compile_args: true, includes: true)] + asset_deps,
  override_options: ['c_std=gnu99'],
  implicit_include_directories: false,
  native: true)

# macOS (ld64 has no linker script): the overlay files are their own archive, with each overlay's data and bss in
# __DATA,__ovldata / __ovlbss, behind a marker object per overlay (tools/port_overlays.py --macho-layout; port/src/ovl.c)
macho_ovl_libs = []
if is_mac
  ovl_layout = run_command(python, repo / 'tools/port_overlays.py', '--macho-layout', game_sources, check: true).stdout().strip().split('\n')
  # written at configure time (meson puts generated sources first in an archive; plain files keep their order)
  ovl_gen_dir = meson.current_build_dir() / 'ovlgen'
  run_command(python, repo / 'tools/port_overlays.py', '--macho-files', ovl_gen_dir, check: true)
  ovl_members = []
  ovl_files = []
  foreach l : ovl_layout
    if l.startswith('mark ')
      ovl_members += ovl_gen_dir / ('ovlmark' + l.substring(5).to_int().to_string(fill: 3) + '.c')
    else
      ovl_members += l
      ovl_files += l
    endif
  endforeach
  non_ovl = []
  foreach s : game_sources
    if s not in ovl_files
      non_ovl += s
    endif
  endforeach
  game_sources = non_ovl + [ovl_gen_dir / 'ovltab.c']   # ovltab.c: the markers' addresses
  w_ovl_members = []
  if build_white   # White's overlays: the same files in White's layout (its DS addresses and sizes); macOS only
    w_ovl_gen_dir = meson.current_build_dir() / 'ovlgen_w'
    run_command(python, repo / 'tools/port_overlays.py', '--version', 'w_us', '--macho-files', w_ovl_gen_dir, check: true)
    foreach l : run_command(python, repo / 'tools/port_overlays.py', '--version', 'w_us', '--macho-layout', ovl_files, check: true).stdout().strip().split('\n')
      w_ovl_members += l.startswith('mark ') ? w_ovl_gen_dir / ('ovlmark' + l.substring(5).to_int().to_string(fill: 3) + '.c') : l
    endforeach
    w_ovl_members = asset_gen.process(w_ovl_members, preserve_path_from: repo)
  endif
  # every member through the asset rewrite, in order (the archive's member order is the overlays' layout, and meson puts a
  # target's generated objects before its others), as Black 2's
  macho_ovl_libs += static_library('game_ovl', asset_gen.process(ovl_members, preserve_path_from: repo),
    c_args: game_args + ['-fno-common', '-include', meson.current_source_dir() / 'machine/macos_ovl_section.h'],
    dependencies: [nitrosdk_dep.partial_dependency(compile_args: true, includes: true, sources: true),
                   nitrosystem_dep.partial_dependency(compile_args: true, includes: true)] + asset_deps,
    override_options: ['c_std=gnu99'],
    implicit_include_directories: false,
    native: true)
  game_args += '-DBWPORT_OVL_MACHO'   # port/src/ovl.c
endif

# The host's files (all of port/src but the DS-side stand-ins) are their own archive on every platform: the machine's
# range is gathered by archive (macOS: by section pragma in the machine's files; elsewhere the linker script,
# tools/port_overlays.py machine_script)
host_libs = []
if true
  mach_sources = []
  host_sources = []
  foreach s : game_sources
    # port/src is the host's, but for the DS-side stand-ins: the SDK's (sdk_*.c), the stubs, the arenas' memory
    f = s.split('/')[-1]
    if s.startswith(repo / 'port/src/') and not (f.startswith('sdk_') or f.startswith('stub_') or f in ['linker_syms.c', 'boot_exit.c'])
      host_sources += s
    else
      mach_sources += s
    endif
  endforeach
  game_sources = mach_sources
  host_libs += static_library('porthost', host_sources, asset_rows_c,
    c_args: game_args,   # (with BWPORT_WHITE, BWPORT_B2, BWPORT_W2: games.c, launcher_core.c, the games in the exe)
    dependencies: [nitrosdk_dep.partial_dependency(compile_args: true, includes: true, sources: true),
                   nitrosystem_dep.partial_dependency(compile_args: true, includes: true)] + asset_deps,
    override_options: ['c_std=gnu99'],
    implicit_include_directories: false,
    native: true)
endif

game_sources += meson.current_source_dir() / 'game/game_table.c'   # the host's way into the game (port/include/port_game.h)
# the files with asset tables through the rewrite (tools/port_assets.py), the rest as they are
game_plain = []
game_assets = []
foreach s : game_sources
  if s in asset_files
    game_assets += s
  else
    game_plain += s
  endif
endforeach
game_gen = [asset_gen.process(game_assets, preserve_path_from: repo)]
if entry_c in asset_files
  error('src/system/main.c includes an asset: port/meson.build compiles it apart (entry), without the rewrite')
endif
game = static_library('game', game_plain, game_gen,
  c_args: game_args + machine_args, c_pch: pch_bw,
  dependencies: [nitrosdk_dep.partial_dependency(compile_args: true, includes: true, sources: true),
                 nitrosystem_dep.partial_dependency(compile_args: true, includes: true)] + asset_deps,
  override_options: ['c_std=gnu99'],
  implicit_include_directories: false,
  native: true)

# White on the PC (GNU ld, lld, mingw): the game's C again (port/src's stand-ins and the files both games share left out:
# tools/port_rename.py --shared), with -DWHITE, White's asset dir in place of Black's and the rename header first (its
# PortGame table, port/game/game_table.c, is w_bwgame by its own #if). Its objects keep Black's member names, so the
# overlay script puts each overlay's White data beside Black's in the same range (one reset clears both; the DS layout table
# stays Black's: White's overlays have the same sizes but ov088's, 32 bytes longer, and the same overlap relations), and its
# archives are in the machine's range (tools/port_overlays.py MACH_ARCHIVES).
white_libs = []
if build_white and not is_mac
  nm = find_program('llvm-nm', 'nm', native: true)
  shared = []
  foreach f : run_command(python, repo / 'tools/port_rename.py', '--shared', check: true).stdout().strip().split('\n')
    shared += repo / f
  endforeach
  white_sources = []
  white_assets = []
  foreach s : game_sources
    if not s.startswith(repo / 'port/src/') and s not in shared
      if s in asset_files
        white_assets += s
      else
        white_sources += s
      endif
    endif
  endforeach
  rename_h = custom_target('w_rename',
    output: 'w_rename.h',
    depend_files: repo / 'tools/port_rename.py',
    command: [python, repo / 'tools/port_rename.py', '--nm', nm, '--prefix', 'w_', '-o', '@OUTPUT@', game, entry])
  white_args = ['-include', meson.current_build_dir() / 'w_rename.h', '-DWHITE']
  foreach a : game_args
    white_args += a == black_asset_inc ? white_asset_inc : a
  endforeach
  white_libs += static_library('game_w', white_sources, rename_h,
    asset_gen.process(white_assets, preserve_path_from: repo),
    c_args: white_args + machine_args, c_pch: pch_bw,
    dependencies: [nitrosdk_dep.partial_dependency(compile_args: true, includes: true, sources: true),
                   nitrosystem_dep.partial_dependency(compile_args: true, includes: true)] + asset_deps,
    override_options: ['c_std=gnu99'],
    implicit_include_directories: false,
    native: true)
  white_libs += static_library('entry_w', entry_c, rename_h,
    c_args: white_args + machine_args + ['-DBWPORT_ENTRY'], c_pch: pch_bw,
    dependencies: [nitrosdk_dep.partial_dependency(compile_args: true, includes: true, sources: true),
                   nitrosystem_dep.partial_dependency(compile_args: true, includes: true)],
    override_options: ['c_std=gnu99'],
    implicit_include_directories: false,
    native: true)
endif

# White on macOS and iOS (ld64): the same C with WHITE defined (include/constants/version.h's GAME_VERSION) and White's assets, but for the files
# both games share (the SDK's code in the game's dirs, which libntr's libraries call, and port/src's DS-side stand-ins).
# Its objects are linked into one relocatable object that keeps one global, its table w_bwgame: every other global
# becomes local, so none clashes with Black's, and White's references bind to White's own definitions or to what both
# share (libntr, the shared files, the host). No rename header, no second set of names.
white_objs = []
if build_white and is_mac
  shared_game = [repo / 'src/main/main_0207e3d0.c', repo / 'src/main/main_02083644.c']
  w_sources = []
  w_assets = []
  foreach s : game_sources
    if s == ovl_gen_dir / 'ovltab.c'
      w_sources += w_ovl_gen_dir / 'ovltab.c'
    elif s.startswith(repo / 'port/src/') or s in shared_game
    elif s in asset_files
      w_assets += s
    else
      w_sources += s
    endif
  endforeach
  w_args = ['-DWHITE']
  foreach a : game_args
    w_args += a == black_asset_inc ? white_asset_inc : a   # White's placeholders
  endforeach
  w_kw = {'dependencies': [nitrosdk_dep.partial_dependency(compile_args: true, includes: true, sources: true),
                           nitrosystem_dep.partial_dependency(compile_args: true, includes: true)] + asset_deps,
          'override_options': ['c_std=gnu99'], 'implicit_include_directories': false, 'native': true}
  w_libs = [static_library('w_game', w_sources, asset_gen.process(w_assets, preserve_path_from: repo), c_args: w_args + machine_args, kwargs: w_kw),
            static_library('w_entry', entry_c, c_args: w_args + machine_args + ['-DBWPORT_ENTRY'], kwargs: w_kw),
            static_library('w_game_ovl', w_ovl_members,
              c_args: w_args + ['-fno-common', '-include', meson.current_source_dir() / 'machine/macos_ovl_section.h'], kwargs: w_kw)]
  # (clang, not ccache, with the build's target flags: ld64 wants the platform)
  w_ld = []
  foreach a : meson.get_compiler('c', native: true).cmd_array() + get_option('c_args')
    w_ld += a == 'ccache' ? [] : [a]
  endforeach
  white_objs += custom_target('white_o',
    output: 'white.o',
    input: w_libs,
    command: w_ld + ['-r', '-nostdlib', '-Wl,-exported_symbol,_w_bwgame', '-o', '@OUTPUT@', '-Wl,-all_load', '@INPUT@'])
endif

# Black 2 and White 2: pokebw2's C (GPL-3; third_party/pokebw2, its vendored copy with the port's patches, the
# maintainers' notes; tools/pokebw2_fetch.sh --vendor updates it), compiled as Black's is but with its own include dirs, BLACK2 or WHITE2 defined and the
# version's overlay layout, and linked as White is into one relocatable object each (b2.o, w2.o) whose only global is
# its table (b2_bwgame, w2_bwgame). What it doesn't define binds to the shared code (libntr, port/src's stand-ins, the
# host) or to the undefined stubs below.
b2w2_objs = []
b2w2_games = []
if build_b2
  b2w2_games += [['b2', 'BLACK2', 'b2_us']]
endif
if build_w2
  b2w2_games += [['w2', 'WHITE2', 'w2_us']]
endif
if b2w2_games.length() > 0
  b2_root = repo / 'third_party/pokebw2'
  b2_sources = run_command(python, repo / 'tools/port_sources.py', '--b2w2', b2_root, check: true).stdout().strip().split('\n')
  b2_entry_c = b2_root / 'src/system/main.c'
  b2_asset_files = run_command(python, asset_tool, 'list', 'b2w2', check: true).stdout().strip().split('\n')
  if b2_entry_c in b2_asset_files
    error('pokebw2\'s src/system/main.c includes an asset: port/meson.build compiles it apart (entry), without the rewrite')
  endif
  # Black's flags without Black's include dirs; pokebw2's include/ after the host's (it has MSL's stdio.h, string.h ...
  # at its top, which libntr's and SDL's own includes must not find)
  b2_args = ['-I' + repo / 'port/include', '-I' + repo / 'port/include_b2w2']   # (pokebw2's SDK header names)
  foreach lib : ['dpw', 'dsprot', 'dwc', 'dwcutil', 'irc', 'nitrowifi', 'poke_net', 'spl']
    b2_args += '-I' + b2_root / 'lib' / lib / 'include'
  endforeach
  b2_args += ['-I' + b2_root / 'generated', '-idirafter', b2_root / 'include',
              '-include', 'work_area.h']   # (port/include_b2w2: WORK_AREA_SIZE, which the patches use)
  b2_late = ['-include', 'port_b2w2.h']   # after port_prefix.h
  skip = false
  foreach a : game_args
    if skip
      skip = false
    elif a == '-idirafter'
      skip = true
    elif a not in game_includes
      b2_args += a
    endif
  endforeach
endif
if b2w2_games.length() > 0 or build_plat or build_hg   # (Platinum's and HeartGold's modules below too)
  # Platinum's and HeartGold's writable data in sections of their own: ld64's by pragma (with their read-only data's group,
  # machine/macos_rom_section.h), the others' by the linker script
  gen4_mach = meson.current_source_dir() / (is_mac ? 'machine/macos_machine_section.h' : 'machine/gnu_machine_section.h')
  gen4_ovlh = meson.current_source_dir() / (is_mac ? 'machine/macos_ovl_section.h' : 'machine/gnu_ovl_section.h')
  b2_kw = {'dependencies': [nitrosdk_dep.partial_dependency(compile_args: true, includes: true, sources: true),
                            nitrosystem_dep.partial_dependency(compile_args: true, includes: true)],
           'override_options': ['c_std=gnu99'], 'implicit_include_directories': false, 'native': true}
  b2_ld = []
  foreach a : meson.get_compiler('c', native: true).cmd_array() + get_option('c_args')
    b2_ld += a == 'ccache' ? [] : [a]
  endforeach
  # (PE: GNU objcopy and nm: llvm-objcopy has no --keep-global-symbol for COFF)
  b2_objcopy = disabler()
  b2_nm = find_program('nm', native: true)   # (Apple's: llvm-nm; port_relink.py --macho reads its -m)
  # the relocatable links: ld64's -r with -exported_symbol (port_relink.py --macho), elsewhere the linker and objcopy
  relink = [python, repo / 'tools/port_relink.py', '--cc', ' '.join(b2_ld), '--nm', b2_nm.full_path()]
  if is_mac
    relink += ['--macho']
  else
    if build_target == 'win64'
      # (built on Windows, tools/port_win64.py: WinLibs' binutils carry no target prefix)
      b2_objcopy = find_program('x86_64-w64-mingw32-objcopy', 'objcopy', native: true)
      b2_nm = find_program('x86_64-w64-mingw32-nm', 'nm', native: true)
      relink = [python, repo / 'tools/port_relink.py', '--cc', ' '.join(b2_ld), '--nm', b2_nm.full_path()]
    else
      b2_objcopy = find_program('llvm-objcopy', 'objcopy', native: true)
      b2_nm = find_program('llvm-nm', 'nm', native: true)
      relink = [python, repo / 'tools/port_relink.py', '--cc', ' '.join(b2_ld), '--nm', b2_nm.full_path()]
    endif
    relink += ['--objcopy', b2_objcopy.full_path()]
  endif
  foreach g : b2w2_games
    tag = g[0]
    gen_dir = meson.current_build_dir() / 'ovlgen_' + tag
    run_command([python, repo / 'tools/port_overlays.py', '--version', g[2], '--root', b2_root, '--macho-files', gen_dir] + (is_mac ? [] : ['--gnu-tag', tag]), check: true)
    ovl_members = []
    ovl_files = []
    foreach l : run_command(python, repo / 'tools/port_overlays.py', '--version', g[2], '--root', b2_root, '--macho-layout', b2_sources, check: true).stdout().strip().split('\n')
      if l.startswith('mark ')
        ovl_members += gen_dir / ('ovlmark' + l.substring(5).to_int().to_string(fill: 3) + '.c')
      else
        ovl_members += l
        ovl_files += l
      endif
    endforeach
    main_files = [meson.current_source_dir() / 'game/game_table.c', gen_dir / 'ovltab.c']
    foreach s : b2_sources
      if s != b2_entry_c and s not in ovl_files
        main_files += s
      endif
    endforeach
    # the files with asset tables through the rewrite (placeholders, filled from the player's ROM at start), as Black's
    b2_plain = []
    b2_rewrite = []
    foreach s : main_files
      if s in b2_asset_files
        b2_rewrite += s
      else
        b2_plain += s
      endif
    endforeach
    main_files = b2_plain + [asset_gen.process(b2_rewrite, preserve_path_from: repo)]
    # every overlay file through it, in order: the archive's member order is the overlays' layout (the markers between
    # them) and meson puts a target's generated sources' objects before its others
    ovl_members = [asset_gen.process(ovl_members, preserve_path_from: repo)]
    kw = b2_kw + {'dependencies': b2_kw['dependencies'] + asset_deps}
    args = ['-D' + g[1]] + b2_args + ['-I' + asset_inc_dir / g[2]] + b2_late
    kw_pch = kw
    pch_args = []
    if use_pch
      probe = static_library(tag + '_pchprobe', 'pch/pch_probe.c', c_args: args + ['-fno-common'], kwargs: kw + {'c_pch': pch_gen4})
      pch_stamp = custom_target(tag + '_pch', output: tag + '_pch.stamp', depends: [probe],
        command: [python, '-c', 'import sys; open(sys.argv[1], "w").close()', '@OUTPUT@'])
      kw_pch = kw + {'dependencies': kw['dependencies'] + declare_dependency(sources: pch_stamp)}
      pch_args = ['-include-pch', 'lib' + tag + '_pchprobe.a.p/gen4_prefix.h.pch']
    endif
    # the writable data in sections of its own (the machine's range, the overlays'): ld64's by pragma, the others' by
    # port/machine/gnu_*_section.h and the linker script (tools/port_overlays.py)
    b2_mach = is_mac ? machine_args : ['-fno-common', '-include', meson.current_source_dir() / 'machine/gnu_machine_section.h']
    b2_ovlh = meson.current_source_dir() / (is_mac ? 'machine/macos_ovl_section.h' : 'machine/gnu_ovl_section.h')
    libs = [static_library(tag + '_game', main_files, c_args: args + pch_args + b2_mach, kwargs: kw_pch),
            static_library(tag + '_entry', b2_entry_c, c_args: args + b2_mach + ['-DBWPORT_ENTRY'], kwargs: kw),
            static_library(tag + '_game_ovl', ovl_members,
              c_args: args + pch_args + ['-fno-common', '-include', b2_ovlh], kwargs: kw_pch)]
    b2w2_objs += custom_target(tag + '_o',
      output: tag + '.o',
      input: libs,
      command: relink + ['--keep', tag + '_bwgame', '-o', '@OUTPUT@', '--ovl-tag', tag]
               + (build_target == 'win64' ? ['--tag', tag] : []) + ['@INPUT@'])
  endforeach
endif

# The Gen 4 games' working copies (build/pokeplatinum, build/pokeheartgold) are made by tools/pokeplat_gen.py and tools/pokehg_gen.py,
# which meson runs at configure time, from the pinned tree plus every port/subprojects/pokeplatinum-*.patch / pokeheartgold-*.patch.
# Reading each patch here makes an edit to one reconfigure an existing build dir (so the generators run again); before this a build
# dir kept its old tree when a patch changed or a patch file was added (the speed options' patches, 2026-11-19). A NEW patch file
# is only seen at the next configure: touch this file (or any meson.build input) when a commit adds one.
foreach p : run_command(python, '-c', 'import glob, sys; print("\\n".join(sorted(glob.glob(sys.argv[1] + "/port/subprojects/poke[ph]*-*.patch"))))', repo, check: true).stdout().strip().split('\n')
  fs.read(p, encoding: 'latin-1')
endforeach

# Platinum (docs/platinum.md): pret's pokeplatinum (third_party/pokeplatinum; the port compiles its working copy build/pokeplatinum,
# the port's patches applied and pret's generated headers made: tools/pokeplat_gen.py), compiled as Black 2 is: Black's flags
# without Black's include dirs and prefix, then Platinum's (tools/pokeplat_cc.py --meson-args), PLATINUM defined; its overlay
# layout from pret's linker spec and the ROM's overlay table (tools/port_overlays.py --plat); one relocatable object, pl.o, whose
# only global is pl_bwgame (port/game/game_table.c), linked into the exe with BWPORT_PLAT defined (its launcher row,
# port/src/games.c). Behind the option `platinum` (on by default: Linux, Windows and Android, an Android APK carries it, no ROM at
# build time; not Apple)
pl_objs = []
plat_rom = b2_rom_dir / 'baserom_plat_us.nds'   # (orig/, Apple: tools/mac_rom.sh's dir; optional: without it the overlays' DS layout comes from the player's ROM at start)
if build_plat
  run_command(python, repo / 'tools/pokeplat_gen.py', check: true)
  pl_root = repo / 'build/pokeplatinum'
  pl_ovl = [python, repo / 'tools/port_overlays.py', '--plat', plat_rom, '--root', pl_root]
  pl_sources = run_command(pl_ovl + ['--sources'], check: true).stdout().strip().split('\n')
  pl_args = ['-DPLATINUM', '-I' + repo / 'port/include_plat', '-I' + repo / 'port/include_gen4']
  skip = false
  foreach a : game_args
    if skip
      skip = false
    elif a in ['-idirafter', '-include']
      skip = true   # (Black's MSL headers and prefix, port_prefix.h: Platinum's is port_plat.h)
    elif a not in game_includes
      pl_args += a
    endif
  endforeach
  pl_args += run_command(python, repo / 'tools/pokeplat_cc.py', '--meson-args', check: true).stdout().strip().split('\n')
  # what tools/pokeplat_cc.py's count takes as warnings and clang as errors (the pointer pass's, pl1's: docs/platinum.md)
  # (and MWCC's implicit int: `static const v1[9] = {...}`, 5 files)
  # A call with no prototype in scope is an error (as clang 16+'s default; explicit for every host compiler, and -w does not
  # silence it): the callee is taken to return int, so a pointer returned loses its upper 32 bits on every 64-bit target and
  # a u8/u16 return is read with garbage above it on win64, arm64 and clang 21 (impl1: Platinum had none, HG/SS one,
  # port/subprojects/pokeheartgold-zimplicit.patch)
  pl_args += ['-Werror=implicit-function-declaration', '-Wno-error=int-conversion', '-Wno-error=incompatible-pointer-types',
              '-Wno-error=implicit-int']
  # (-ftrivial-auto-var-init=zero: from game_args, above)
  gen_dir = meson.current_build_dir() / 'ovlgen_pl'
  run_command(pl_ovl + ['--macho-files', gen_dir] + (is_mac ? [] : ['--gnu-tag', 'pl']), check: true)
  pl_args += ['-I' + gen_dir]   # port_ovids.h (port_plat.h's FS_OVERLAY_ID)
  ovl_members = []
  ovl_files = []
  foreach l : run_command(pl_ovl + ['--macho-layout'] + pl_sources, check: true).stdout().strip().split('\n')
    if l.startswith('mark ')
      ovl_members += gen_dir / ('ovlmark' + l.substring(5).to_int().to_string(fill: 3) + '.c')
    else
      ovl_members += l
      ovl_files += l
    endif
  endforeach
  main_files = [meson.current_source_dir() / 'game/game_table.c', meson.current_source_dir() / 'game/plat_online.c',
                meson.current_source_dir() / 'game/route_gen4.c', meson.current_source_dir() / 'game/speed_gen4.c',
                gen_dir / 'ovltab.c']
  main_files += meson.current_source_dir() / 'game/speed_gen4_spd3.c'   # Save speed, battle intro (spd3)
  foreach s : pl_sources
    if s not in ovl_files
      main_files += s
    endif
  endforeach
  # the tables the C includes (asset/<name>.inc, config/plat_us/assets.tsv) from the ROM into build/plat_us
  pl_assets = custom_target('pl_assets',
    output: 'pl_assets.stamp',
    command: [python, repo / 'tools/rom_modules.py', '--assets', b2_rom_dir, '--version', 'plat_us', '--stamp', '@OUTPUT@'],
    build_always_stale: true)
  kw = b2_kw + {'dependencies': b2_kw['dependencies'] + declare_dependency(sources: pl_assets)}
  pch_args = []
  if use_pch   # (the probe, see "The Gen 4 and Black 2" above)
    probe = static_library('pl_pchprobe', 'pch/pch_probe.c', c_args: pl_args + ['-fno-common'], kwargs: kw + {'c_pch': pch_gen4})
    pch_stamp = custom_target('pl_pch', output: 'pl_pch.stamp', depends: [probe],
      command: [python, '-c', 'import sys; open(sys.argv[1], "w").close()', '@OUTPUT@'])
    kw = kw + {'dependencies': kw['dependencies'] + declare_dependency(sources: pch_stamp)}
    pch_args = ['-include-pch', 'libpl_pchprobe.a.p/gen4_prefix.h.pch']
  endif
  libs = [static_library('pl_game', main_files,
            c_args: pl_args + pch_args + ['-fno-common', '-include', gen4_mach], kwargs: kw),
          static_library('pl_game_ovl', ovl_members,
            c_args: pl_args + pch_args + ['-fno-common', '-include', gen4_ovlh], kwargs: kw)]
  # the battle AI's script (gTrainerAITable): data pret writes in assembly with its macros (asm/macros/aicmd.inc), assembled
  # here from the working copy as pret's tools/scripts/make_script_bin.sh does (the C preprocessor, pret's enumproc for the
  # enums of its headers, then the assembler: the build's C compiler, whose words are the DS's, little-endian: the same 42,428
  # bytes as the ROM's overlay 14). pret's file says `.text`: objcopy makes the section read-only data (.rodata, PE .rdata) so it
  # is in group pl's and tools/port_romfill.py fills it from the player's ROM (as .text it was 42 KB of the ROM in the package)
  pl_cc = ' '.join(b2_ld)
  pl_ro = build_target == 'win64' ? '.rdata' : '.rodata'
  # (Mach-O: no objcopy there, the section is named in the text, and the table's name is the C's _gTrainerAITable; the
  # script's own labels are made assembler-local (L...): Mach-O resolves `(Label - FlagTable) / 4` at assembly time only
  # between those. No backslashes: meson turns them into slashes)
  if is_mac
    pl_ai_cmd = ['sh', '-c', pl_cc + ' -E -x assembler-with-cpp -I' + pl_root / 'include' + ' -I' + pl_root / 'asm' + ' -I' + pl_root / 'gen'
                 + ' "$1" | "$2" | sed -e "s/^[[:space:]]*[.]text$/.section __DATA,__bwrom_pl/" -e "s/gTrainerAITable/_gTrainerAITable/g"'
                 + ' | perl -0pe "$4" | '
                 + pl_cc + ' -x assembler-with-cpp -c -o "$3" -',
                 'sh', '@INPUT@', pl_root / 'hbuild/tools/enumproc/enumproc', '@OUTPUT@',
                 'my %l; $l{$1} = 1 while /^[[:space:]]*([A-Za-z][A-Za-z0-9_]*):/mg;'
                 + ' s/(?<![A-Za-z0-9_.])([A-Za-z][A-Za-z0-9_]*)(?![A-Za-z0-9_])/exists $l{$1} ? "L$1" : $1/ge']
  else
    pl_ai_cmd = ['sh', '-c', pl_cc + ' -E -x assembler-with-cpp -I' + pl_root / 'include' + ' -I' + pl_root / 'asm' + ' -I' + pl_root / 'gen'
                + ' "$1" | "$2" | ' + pl_cc + ' -x assembler-with-cpp' + (build_target == 'win64' ? '' : ' -Wa,--noexecstack') + ' -c -o "$3" -'
                + ' && "$4" --rename-section .text=' + pl_ro + ',alloc,load,readonly,data,contents "$3"',
                'sh', '@INPUT@', pl_root / 'hbuild/tools/enumproc/enumproc', '@OUTPUT@', b2_objcopy.full_path()]
  endif
  pl_ai = custom_target('pl_ai_script',
    input: pl_root / 'src/battle/trainer_ai/script.s',
    output: 'pl_ai_script.o',
    depend_files: pl_root / 'asm/macros/aicmd.inc',
    command: pl_ai_cmd)
  pl_objs += custom_target('pl_o',
    output: 'pl.o',
    input: libs + [pl_ai],
    command: relink + ['--keep', 'pl_bwgame', '-o', '@OUTPUT@', '--ovl-tag', 'pl']
              + (build_target == 'win64' ? ['--tag', 'pl'] : []) + ['@INPUT@'])
endif

# HeartGold (docs/hgss.md): our decompilation on pret's pokeheartgold (third_party/pokeheartgold; the port compiles its
# working copy build/pokeheartgold, the patches applied and the headers made: tools/pokehg_gen.py), as Platinum's module: Black's
# flags without Black's include dirs and prefix, then HeartGold's (tools/pokehg_cc.py --meson-args), HEARTGOLD defined; its
# overlays from main.lsf.in (tools/port_overlays.py --hgss; the ROM, when it is in orig/, gives their DS addresses); one
# relocatable object, hg.o, whose only global is hg_bwgame (port/game/game_table.c), with BWPORT_HG defined for the exe. The data
# pret keeps in assembly (4,395 .s files) is hg_data.o (tools/pokehg_asm.py). The files that do not compile on the pin
# (port/hgss_exclude.txt) are left out: their functions are the link's undefined symbols (the exe's stub step). Behind the option
# `hgss` (on by default: Linux, Windows and Android; not Apple)
hg_objs = []
hg_rom = b2_rom_dir / 'baserom_hg_us.nds'
ss_rom = b2_rom_dir / 'baserom_ss_us.nds'   # SoulSilver's (the same module: the version is a value, docs/hgss.md "SoulSilver")
if build_hg
  run_command(python, repo / 'tools/pokehg_gen.py', check: true)
  hg_root = repo / 'build/pokeheartgold'
  hg_ovl = [python, repo / 'tools/port_overlays.py', '--hgss', hg_rom, '--root', hg_root]
  hg_skip = []
  foreach s : fs.read(meson.current_source_dir() / 'hgss_exclude.txt').strip().split('\n')
    if s != '' and not s.startswith('#')
      hg_skip += hg_root / s.split(' ')[0]
    endif
  endforeach
  hg_sources = []
  foreach s : run_command(hg_ovl + ['--sources'], check: true).stdout().strip().split('\n')
    if s not in hg_skip
      hg_sources += s
    endif
  endforeach
  hg_args = ['-DHEARTGOLD', '-I' + repo / 'port/include_hgss', '-I' + repo / 'port/include_gen4']
  skip = false
  foreach a : game_args
    if skip
      skip = false
    elif a in ['-idirafter', '-include']
      skip = true   # (Black's MSL headers and prefix, port_prefix.h: HeartGold's is port_hgss.h)
    elif a not in game_includes
      hg_args += a
    endif
  endforeach
  hg_args += run_command(python, repo / 'tools/pokehg_cc.py', '--meson-args', check: true).stdout().strip().split('\n')
  # what tools/pokehg_cc.py's count takes as warnings and clang as errors (as Platinum's)
  # (and `return;` in a function that returns a value: 6 files of overlays 41, 43, 51, 70, whose asm-side decompiled bodies are stubs)
  # (a call with no prototype in scope is an error, as Platinum's, above)
  hg_args += ['-Werror=implicit-function-declaration', '-Wno-error=int-conversion', '-Wno-error=incompatible-pointer-types',
              '-Wno-error=implicit-int', '-Wno-error=return-type']
  # a local the C reads before it writes is 0, as on every run, not the host stack's leftovers (as Platinum's module,
  # above: 54 of the sanitizer's 75 uninitialised reads on HG/SS are the original's own, admin-79 hk129)
  hg_args += ['-ftrivial-auto-var-init=zero']
  # the crash sweep's function coverage (-Dhgss_cov=true, its own build dir: tools/hg_sweep.py; docs/hgss.md "Crash sweep"):
  # every module unit gets an inline flag per function (no call: trace-pc-guard's clobbered the argument registers),
  # port/game/hg_cov.c (not instrumented, linked in every HG build) writes the set ones
  hg_cov = get_option('hgss_cov') ? ['-fsanitize-coverage=func,inline-bool-flag,pc-table'] : []
  gen_dir = meson.current_build_dir() / 'ovlgen_hg'
  run_command(hg_ovl + ['--macho-files', gen_dir] + (is_mac ? [] : ['--gnu-tag', 'hg']), check: true)
  ovl_members = []
  ovl_files = []
  # (the sources relative to the tree: 10,000 absolute paths are more than macOS's 1 MB of arguments)
  hg_rel = []
  foreach s : hg_sources
    hg_rel += s.replace(hg_root + '/', '')
  endforeach
  foreach l : run_command(hg_ovl + ['--macho-layout'] + hg_rel, check: true).stdout().strip().split('\n')
    if l.startswith('mark ')
      ovl_members += gen_dir / ('ovlmark' + l.substring(5).to_int().to_string(fill: 3) + '.c')
    else
      ovl_members += hg_root / l
      ovl_files += hg_root / l
    endif
  endforeach
  main_files = [meson.current_source_dir() / 'game/game_table.c', meson.current_source_dir() / 'game/route_gen4.c',
                meson.current_source_dir() / 'game/speed_gen4.c', gen_dir / 'ovltab.c']
  main_files += meson.current_source_dir() / 'game/speed_gen4_spd3.c'   # Save speed, battle intro (spd3)
  foreach s : run_command(python, repo / 'tools/pokehg_cc.py', '--nonmatching', check: true).stdout().strip().split('\n')
    if s not in hg_skip
      main_files += s
    endif
  endforeach
  # the decomp's PC-only typed copies of asm data holding pointers (port/asmdata, hk73): their labels are local in hg_data.o
  # (tools/pokehg_asm.py's C_DATA reads port/asmdata/LABELS.txt)
  foreach s : run_command(python, repo / 'tools/pokehg_cc.py', '--asmdata', check: true).stdout().strip().split('\n')
    if s != '' and s not in hg_skip
      main_files += s
    endif
  endforeach
  foreach s : hg_sources
    if s not in ovl_files
      main_files += s
    endif
  endforeach
  hg_kw = b2_kw
  hg_pch = []
  if use_pch   # (the probe, see "The Gen 4 and Black 2" above)
    probe = static_library('hg_pchprobe', 'pch/pch_probe.c', c_args: hg_args + hg_cov + ['-fno-common'], kwargs: b2_kw + {'c_pch': pch_gen4})
    pch_stamp = custom_target('hg_pch', output: 'hg_pch.stamp', depends: [probe],
      command: [python, '-c', 'import sys; open(sys.argv[1], "w").close()', '@OUTPUT@'])
    hg_kw = b2_kw + {'dependencies': b2_kw['dependencies'] + declare_dependency(sources: pch_stamp)}
    hg_pch = ['-include-pch', 'libhg_pchprobe.a.p/gen4_prefix.h.pch']
  endif
  # the overlay files in archives of 2,500 members, in order (the relink takes them in order: the layout stays one run): the
  # one archive's `ar` line was 1.2 MB, more than macOS's posix_spawn takes (Apple's ar reads no @file)
  hg_ovl_libs = []
  part = []
  foreach m : ovl_members
    part += m
    if part.length() == 2500
      hg_ovl_libs += static_library('hg_game_ovl' + (hg_ovl_libs.length() > 0 ? hg_ovl_libs.length().to_string() : ''), part,
        c_args: hg_args + hg_cov + hg_pch + ['-fno-common', '-include', gen4_ovlh], kwargs: hg_kw)
      part = []
    endif
  endforeach
  if part.length() > 0
    hg_ovl_libs += static_library('hg_game_ovl' + (hg_ovl_libs.length() > 0 ? hg_ovl_libs.length().to_string() : ''), part,
      c_args: hg_args + hg_cov + hg_pch + ['-fno-common', '-include', gen4_ovlh], kwargs: hg_kw)
  endif
  libs = [static_library('hg_game', main_files,
            c_args: hg_args + hg_cov + hg_pch + ['-fno-common', '-include', gen4_mach], kwargs: hg_kw)] + hg_ovl_libs + [
          # the online stubs (port/game/hg_online.c: Platinum's, whose headers are port/include_plat's)
          static_library('hg_online', meson.current_source_dir() / 'game/hg_online.c',
            c_args: hg_args + ['-I' + repo / 'port/include_plat', '-fno-common', '-include',
                               gen4_mach], kwargs: b2_kw)]
  libs += static_library('hg_cov', meson.current_source_dir() / 'game/hg_cov.c', kwargs: b2_kw)
  hg_data = custom_target('hg_data',
    output: 'hg_data.o',
    depend_files: [repo / 'tools/pokehg_asm.py'] + (fs.exists(hg_root / 'port/asmdata/LABELS.txt') ? [hg_root / 'port/asmdata/LABELS.txt'] : []),
    command: [python, repo / 'tools/pokehg_asm.py', '--cc', ' '.join(b2_ld) + (build_target == 'win64' ? '' : ' -Wa,--noexecstack'),
              '-o', '@OUTPUT@'])
  hg_objs += custom_target('hg_o',
    output: 'hg.o',
    input: libs + [hg_data],
    command: relink + ['--keep', 'hg_bwgame,ss_bwgame', '-o', '@OUTPUT@', '--ovl-tag', 'hg']
              + (build_target == 'win64' ? ['--tag', 'hg'] : []) + ['@INPUT@'])
endif

# The overlays' writable data gathered into ranges port/src/ovl.c resets at each FS_LoadOverlay, with the DS's overlay
# table (tools/port_overlays.py: an INSERT script for GNU ld, ELF and PE)
ovl_ld = custom_target('ovl_ld',
  output: 'ovl.ld',
  depend_files: [repo / 'tools/port_overlays.py', repo / 'tools/port_romfill.py'],   # (the machine's ranges are in it too; the GAMES table's groups)
  command: [python, repo / 'tools/port_overlays.py', '--target', build_target, '-o', '@OUTPUT@'])
ovl_link_args = is_mac ? ['-Wl,-order_file,' + meson.current_source_dir() / 'machine/macos.order'] : ['-Wl,-T,' + ovl_ld.full_path().replace('\\', '/')]   # (built on Windows: a backslash in a response file is an escape)
# The Linux exe's two links (bwport-nostubs for tools/port_undefined.py, bwport-full) with lld where it is installed: 3.5 s
# against GNU ld's 7.8 s, the same script, --wrap and map (tools/port_romfill.py reads either; Android links with lld too).
# Not mingw's (win64) nor the -r relinks of tools/port_relink.py (1.7 s). -Dlld=disabled: GNU ld.
if build_target == 'linux' and not is_mac and not is_android and not get_option('lld').disabled()
  if meson.get_compiler('c', native: true).has_link_argument('-fuse-ld=lld') and meson.get_compiler('cpp', native: true).has_link_argument('-fuse-ld=lld')
    ovl_link_args += ['-fuse-ld=lld', '-Wl,--error-limit=0']
  elif get_option('lld').enabled()
    error('-Dlld=enabled: the compilers cannot link with -fuse-ld=lld (apt install lld)')
  endif
endif
if build_target == 'linux' and not is_mac
  # port/src/lstep_dump.c (BWPORT_LSTEP, docs/lockstep.md): the end of a game frame, the seed and the clock
  ovl_link_args += ['-Wl,--wrap=PORT_WaitIrq', '-Wl,--wrap=OS_WaitVBlankIntr', '-Wl,--wrap=GFL_VBlankUpdate', '-Wl,--wrap=buildSeed', '-Wl,--wrap=time']
endif

# port/vcore (docs/vcore.md), the DS video core that draws the picture by default: its sources as a static library
# (port/vcore is also its own meson project, for its unit tests and tools: build/vcore), optimized in any build type
# (a software renderer at -O0 costs the frame rate). port/bridge/vcore_bridge.c hands it each frame's state from libntr
# and gives libntr the screens (libntr-vbridge.patch); linked whole, as its constructor registers it. Both are C11 with
# vcore's own headers, apart from the game's C (whose types.h differs from vcore's on win64).
vcore_srcs = run_command(python, repo / 'port/vcore/list_sources.py', repo / 'port/vcore/src',
                         check: true).stdout().strip().split('\n')
vcore_inc = include_directories('vcore/include', 'vcore/src')
vcore_opts = ['c_std=c11', 'optimization=2', 'warning_level=0']
# vcore renders the 3D, engine B and engine A on threads, the bridge pipelines frames on one more (p20_frame.c, vcore_bridge.c)
vcore_threads = dependency('threads', native: true)
vcore_lib = static_library('vcore', vcore_srcs,
  include_directories: vcore_inc,
  override_options: vcore_opts,
  implicit_include_directories: false,
  dependencies: vcore_threads,
  native: true)
vbridge = static_library('vbridge', 'bridge/vcore_bridge.c',
  include_directories: vcore_inc,
  override_options: vcore_opts,
  implicit_include_directories: false,
  native: true)

# port/acore (docs/acore.md), the DS sound core: its sources as a static library (also its own meson project, for its
# unit tests and tools: build/acore). port/bridge/acore_bridge.c is its side of the live path (Sound=acore in
# sim_config.ini, piece 18; port/src/acore_glue.c is the game's side): acore's own headers, apart from the game's C.
acore_srcs = run_command(python, repo / 'port/acore/list_sources.py', repo / 'port/acore/src',
                         check: true).stdout().strip().split('\n')
acore_inc = include_directories('acore/include', 'acore/src')
acore_opts = ['c_std=c11', 'optimization=2', 'warning_level=0']
acore_lib = static_library('acore', acore_srcs,
  include_directories: acore_inc,
  override_options: acore_opts,
  # Apple: its ARM7 tables in group s's section, for tools/port_romfill.py (port/machine/macos_rom_section.h)
  c_args: is_mac ? ['-DBWPORT_ROM_SHARED', '-include', meson.current_source_dir() / 'machine/macos_rom_section.h'] : [],
  implicit_include_directories: false,
  native: true)
abridge = static_library('abridge', 'bridge/acore_bridge.c',
  include_directories: acore_inc,
  override_options: acore_opts,
  dependencies: vcore_threads,
  implicit_include_directories: false,
  native: true)

# port/gpu (docs/gpu.md), the GPU path that upscales the 3D (sim_config.ini Renderer=gpu, Scale=N): its sources as a static
# library (port/gpu is also its own meson project with the headless EGL context for its tools: build/gpu; the port has
# libntr's context instead, so gpu_egl.c is left out) and port/bridge/gpu_bridge.c, which hands libntr a GL texture
# (libntr-gpu.patch). The GL functions are loaded by name; the Khronos GLES 3 headers are vendored in bridge/glinc for
# their types (mingw has none). Linked whole like the vcore bridge: its constructor registers the GL renderer.
gpu_dir = repo / 'port/gpu'
gpu_srcs = []
foreach f : run_command(python, repo / 'port/vcore/list_sources.py', gpu_dir / 'src', check: true).stdout().strip().split('
')
  if not f.endswith('gpu_egl.c')
    gpu_srcs += f
  endif
endforeach
gpu_glsl = run_command(python, gpu_dir / 'tools/embed_glsl.py', '--list', gpu_dir / 'shaders', check: true).stdout().strip().split('
')
gpu_shaders_c = custom_target('gpu_shaders',
  input: gpu_glsl,
  output: 'gpu_shaders.c',
  command: [python, gpu_dir / 'tools/embed_glsl.py', '@OUTPUT@', '@INPUT@'])
gpu_inc = include_directories('gpu/include', 'gpu/src', 'bridge/glinc', 'vcore/include', 'vcore/src')
# macOS / iOS: the Metal core too (port/gpu/src/gpu_mtl.m, its MSL generated from the GLSL into port/gpu/msl and committed;
# gpu_msl.py embed fails the build when a GLSL input changed since: docs/gpu-backends.md M1) and the bridge's Metal half
# (bridge/gpu_bridge_mtl.m: under Present=metal the GPU renderer draws with it). Other platforms: as before.
gpu_mtl_srcs = []
gbridge_srcs = ['bridge/gpu_bridge.c']
gpu_mtl_args = []
if is_mac
  add_languages('objc', native: true)
  gpu_msl = run_command(python, gpu_dir / 'tools/gpu_msl.py', '--list', check: true).stdout().strip().split('
')
  gpu_mtl_srcs = [gpu_dir / 'src/gpu_mtl.m', custom_target('gpu_msl',
    input: gpu_msl + gpu_glsl + [gpu_dir / 'tools/gpu_msl.py', gpu_dir / 'src/gpu_uni.h'],
    output: 'gpu_msl.c',
    command: [python, gpu_dir / 'tools/gpu_msl.py', 'embed', '@OUTPUT@'])]
  gbridge_srcs += 'bridge/gpu_bridge_mtl.m'
  gpu_mtl_args = ['-DGPU_HAVE_METAL']
endif
gpu_lib = static_library('gpu', gpu_srcs, gpu_shaders_c, gpu_mtl_srcs,
  include_directories: gpu_inc,
  override_options: vcore_opts,
  implicit_include_directories: false,
  dependencies: vcore_threads,
  c_args: gpu_mtl_args,
  objc_args: gpu_mtl_args + ['-fobjc-arc'],
  native: true)
gbridge = static_library('gbridge', gbridge_srcs,
  include_directories: gpu_inc,
  override_options: vcore_opts,
  implicit_include_directories: false,
  c_args: is_mac ? ['-DBR_METAL'] : [],
  objc_args: ['-fobjc-arc'],
  native: true)

# win64 builds as the build machine's (native: true), so meson would give the exe no suffix
exe_suffix = build_target == 'win64' ? 'exe' : []
exe_deps = [declare_dependency(link_with: nnsys_libs), nitrosdk_dep, nitrosystem_dep, vcore_threads]
if is_mac and not is_ios
  ovl_link_args += ['-Wl,-framework,Foundation', '-lobjc']    # port/src/apple_thermal.m
  ovl_link_args += ['-Wl,-framework,Metal', '-Wl,-framework,QuartzCore', '-Wl,-framework,CoreVideo', '-Wl,-framework,AppKit']   # port/src/metal_present.mm
  ovl_link_args += ['-lcurl']   # the SDK's libcurl: port/src/http_get.c (the launcher's cover art)
endif
if build_target == 'win64'
  ovl_link_args += ['-lwinhttp']   # port/src/http_get.c (the launcher's cover art)
  # launcher_gui.cpp (std::function ...) pulls libstdc++.a members that meson's own -lstdc++ (the import library) then defines again
  ovl_link_args += ['-Wl,-Bstatic', '-lstdc++', '-Wl,-Bdynamic']
  ovl_link_args += ['-lole32', '-luuid']   # port/src/launcher_gui.cpp: the Open dialog (IFileOpenDialog)
endif
if is_android
  # SDLActivity loads the game as libmain.so and calls its SDL_main (port/src/android_main.c): the same link as a shared
  # object (tools/android_package.sh copies bwport into the APK as lib/<abi>/libmain.so)
  # --rosegment: the read-only data (.rodata, the .bwrom_* groups) in a segment of its own, not in the executable one (the NDK's
  # lld merges them: one R E segment). tools/port_romfill.py's start-up fill (asset_load.c FillProtect) makes the groups writable
  # and then read-only again; for a page of the executable segment that is a write and then an exec of the APK's file mapping,
  # which SELinux's execmod denies ("can't restore the protection": Black's log had it too, and the data stayed writable)
  ovl_link_args += ['-shared', '-Wl,-soname,libmain.so', '-Wl,--rosegment']
endif
if is_ios
  # SDL's static library needs its frameworks; port/src/ios_main.c's main() starts SDL's UIKit app
  foreach fw : ['CoreVideo', 'CoreAudio', 'AudioToolbox', 'AVFoundation', 'CoreBluetooth', 'CoreGraphics', 'CoreMotion', 'Foundation',
                'GameController', 'Metal', 'OpenGLES', 'QuartzCore', 'UIKit', 'CoreHaptics', 'ImageIO', 'MobileCoreServices', 'UniformTypeIdentifiers',
                'PhotosUI']   # (PhotosUI: the launcher's Set Artwork, PHPickerViewController)
    ovl_link_args += ['-Wl,-framework,' + fw]
  endforeach
endif

# The link without the stubs: never built by ninja itself. tools/port_undefined.py runs its command from build.ninja
# (the undefined references as warnings) to list the undefined symbols, and writes undefined_stubs.c: a trap for each
# undefined function (it prints the name and aborts) and a zeroed block for each undefined datum. The exe links
# with them, so it runs until it reaches one. -Dallow_undefined=false links without them (the link fails while any
# symbol is undefined).
buildinfo = []
if is_mac
  # the heat state (and iOS's Low Power Mode) in the log: port/src/apple_thermal.m
  add_languages('objc', native: true)
  buildinfo += static_library('thermal', 'src/apple_thermal.m', native: true, objc_args: ['-fobjc-arc'])
  # the Metal present (Present=metal: libntr-ios-metal.patch) and Dear ImGui's Metal backend: port/src/metal_present.mm
  add_languages('objcpp', native: true)
  buildinfo += static_library('metalpresent', 'src/metal_present.mm', 'src/imgui_impl_metal.mm', native: true,
    objcpp_args: ['-fobjc-arc'], dependencies: nitrosdk_dep.partial_dependency(includes: true))
endif
if not is_ios and not is_android
  # the desktop's launcher, the Games window in libntr's ImGui (libntr-ios-launcher.patch): port/src/launcher_gui.cpp
  buildinfo += static_library('launchergui', 'src/launcher_gui.cpp', native: true,
    include_directories: subproject('libntr').get_variable('libsim_includes'),   # (ImGuiFileDialog.h, imgui_internal.h)
    dependencies: nitrosdk_dep.partial_dependency(includes: true))
endif
if is_ios
  # the build line (git hash, date) port/src/ios_log.c writes at the top of each log
  buildinfo_c = custom_target('buildinfo',
    output: 'buildinfo.c',
    command: [python, repo / 'tools/ios_buildinfo.py', '@OUTPUT@'],
    build_always_stale: true)
  buildinfo += [static_library('buildinfo', buildinfo_c, native: true)]
  # the safe-area insets (UIKit) for the touch layout: port/src/ios_safearea.m
  add_languages('objc', native: true)
  buildinfo += static_library('iossafe', 'src/ios_safearea.m', native: true, objc_args: ['-fno-objc-arc'])
  # the native settings sheet (UIKit): port/src/ios_settings.m
  buildinfo += static_library('iossettings', 'src/ios_settings.m', native: true, objc_args: ['-fobjc-arc'])
  # the input latency and the main run loop's turns (UIKit): port/src/ios_input.m
  buildinfo += static_library('iosinput', 'src/ios_input.m', native: true, objc_args: ['-fobjc-arc'])
  # the launcher (UIKit): the games, Add Game, the saves, before the game boots: port/src/ios_launcher.m
  buildinfo += static_library('ioslauncher', 'src/ios_launcher.m', native: true, objc_args: ['-fobjc-arc'])
  # PORT_HttpGet through NSURLSession, for the launcher's cover art: port/src/ios_http.m
  buildinfo += static_library('ioshttp', 'src/ios_http.m', native: true, objc_args: ['-fobjc-arc'])
endif

# One ninja target per game (`ninja -C build/port/linux game-hgss`): its objects, with no other game's and no link. A compile check of
# one game in a build dir that holds all of them (a cold dir: 17k compile steps for the six, about a third of them Black's)
alias_target('game-black', game, entry)
if white_libs.length() + white_objs.length() > 0
  alias_target('game-white', white_libs, white_objs)
endif
if b2w2_objs.length() > 0
  alias_target('game-b2w2', b2w2_objs)
endif
if pl_objs.length() > 0
  alias_target('game-platinum', pl_objs)
endif
if hg_objs.length() > 0
  alias_target('game-hgss', hg_objs)
endif

# The link is C++'s (not objcpp: port/src/metal_present.mm). On win64 meson picks it itself: link_language: 'cpp' makes it
# add -lstdc++, which finds libstdc++.dll.a before the static libstdc++ and the link fails on duplicate symbols
link_kw = build_target == 'win64' ? {} : {'link_language': 'cpp'}
bwport_nostubs = executable('bwport-nostubs', white_objs, b2w2_objs, pl_objs, hg_objs,
  name_suffix: exe_suffix,
  link_whole: [game, entry, vbridge, abridge, gbridge] + white_libs + host_libs + macho_ovl_libs + buildinfo,
  link_with: [gpu_lib, vcore_lib, acore_lib],
  link_args: ovl_link_args,
  link_depends: ovl_ld,
  dependencies: exe_deps,
  kwargs: link_kw,
  build_by_default: false,
  native: true)

stub_sources = []
if get_option('allow_undefined')
  # What bwport-nostubs links without being a library of the lists above (the stub step runs its link command itself, ninja knows only
  # `depends`): libinit, whose crt0 object comes in as nitrosdk_dep's `objects`, and ovl_ld, the linker script (link_depends). Missing, the
  # step could start before crt0.c.o (behind its pending PCH) or ovl_ld was made: "ld.lld: cannot open ...libinit.a.p/src_crt0.c.o"
  sdk_libs = subproject('libntr').get_variable('nitrosdk_libs') + nnsys_libs + [subproject('libntr').get_variable('libinit')]
  # an undefined DATA symbol is a table the game reads as zeros: in the HGSS build a new one fails the link step
  # (port/undefined_data_allow.txt names the known ones); the others (Black, Platinum) only list it (undefined.txt)
  data_args = ['--allow-data', repo / 'port/undefined_data_allow.txt'] + (build_hg ? ['--fail-on-data'] : [])
  stub_sources = custom_target('undefined_stubs',
    output: ['undefined_stubs.c', 'undefined.txt'],
    command: [python, repo / 'tools/port_undefined.py', meson.current_build_dir(), '--edge', 'bwport-nostubs',
              '--stubs', '@OUTPUT0@', '-o', '@OUTPUT1@'] + data_args + (dev_fast ? ['--cache'] : []),
    depend_files: [repo / 'port/undefined_data_allow.txt', repo / 'tools/port_undefined.py'],
    depends: [game, entry, vbridge, abridge, gbridge, gpu_lib, vcore_lib, acore_lib] + white_libs + host_libs + macho_ovl_libs + buildinfo + sdk_libs + [ovl_ld] + white_objs + b2w2_objs + pl_objs + hg_objs)[0]
endif

# The link is bwport-full, with the game's data as compiled, and bwport is it after tools/port_romfill.py: every byte of
# the game's data (and the SDK's) that the player's ROM holds zeroed, and where it is in the ROM written into the image's
# .bwfill (port/src/asset_load.c copies it back at start; docs/port-build.md "Game data from the ROM"). macOS and iOS:
# ld64's -map, the data in __DATA,__bwrom_* (port/machine/macos_rom_section.h), the step re-signs the binary (ad hoc).
# Only when every built game's ROM is there (a release package; the shared group must match every game's ROM): a build
# with no ROM links bwport as is, the C's own data compiled in and the assets.tsv rows filled at start. HeartGold and
# SoulSilver count as two games (two ROMs, one module): a fill without one of them would leave that game the shared group's
# zeros (port/src/asset_load.c refuses to start a game the image's fill has no row for)
rom_fill = fs.exists(b2_rom_dir / 'baserom_b_us.nds') and (not build_white or fs.exists(b2_rom_dir / 'baserom_w_us.nds')) and (
  not build_b2 or fs.exists(b2_rom_dir / 'baserom_b2_us.nds')) and (not build_w2 or fs.exists(b2_rom_dir / 'baserom_w2_us.nds')) and (
  not build_plat or fs.exists(plat_rom)) and (not build_hg or (fs.exists(hg_rom) and fs.exists(ss_rom))) and not dev_fast
map_arg = is_mac ? '-map,' : '-Map='
fill_link_args = ['-Wl,' + map_arg + meson.current_build_dir() / 'bwport-full.map']
if is_mac
  # what the compiler makes itself (local arrays' initialisers, switch tables) and the string literals stay out of the
  # pragma's sections: renamed to writable ones the step fills too (tools/port_romfill.py MACHO_SECTIONS)
  fill_link_args += ['-Wl,-rename_section,__TEXT,__const,__DATA,__bwconst',
                     '-Wl,-rename_section,__DATA_CONST,__const,__DATA,__bwconstp',
                     '-Wl,-rename_section,__TEXT,__cstring,__DATA,__bwcstring']
endif
bwport_full = executable(rom_fill ? 'bwport-full' : 'bwport', stub_sources, white_objs, b2w2_objs, pl_objs, hg_objs,
  name_suffix: exe_suffix,
  link_whole: [game, entry, vbridge, abridge, gbridge] + white_libs + host_libs + macho_ovl_libs + buildinfo,
  link_with: [gpu_lib, vcore_lib, acore_lib],
  link_args: ovl_link_args + (rom_fill ? fill_link_args : []),
  link_depends: ovl_ld,
  dependencies: exe_deps,
  kwargs: link_kw,
  c_args: ['-DBWPORT_NO_NITRO_FORCE'],   # (undefined_stubs.c declares its own types: port/pch/force_nitro.h)
  implicit_include_directories: false,
  native: true)
bwport = bwport_full
if rom_fill
  # the step needs numpy: the Mac's Homebrew Python has none, so a venv with it (python3 -m venv ~/bw-cache/venv &&
  # ~/bw-cache/venv/bin/pip install numpy; tools/release_check.sh takes it too)
  fill_python = python
  if run_command(python, '-c', 'import numpy', check: false).returncode() != 0
    fill_python = find_program(fs.expanduser('~/bw-cache/venv/bin/python3'), required: false)
    if not fill_python.found()
      error('tools/port_romfill.py needs numpy: python3 -m venv ~/bw-cache/venv && ~/bw-cache/venv/bin/pip install numpy')
    endif
  endif
  fill_games = ['--game', 'IRBO:b:' + b2_rom_dir / 'baserom_b_us.nds:config/b_us']   # (b2_rom_dir: orig/, Apple's ROM dir)
  if build_white
    fill_games += ['--game', 'IRAO:w:' + b2_rom_dir / 'baserom_w_us.nds:config/w_us']
  endif
  # Black 2 / White 2: their own C is not filled (it stays as compiled, b2.o), but the C every game runs (group s: the SDK's
  # libraries, lib/nnsys, the ARM7 tables) is filled only with the bytes every listed ROM holds, so a game left out would run
  # on zeroed shared data (OSi_ExtraHeapHandle's -1, the first crash of Black 2 on Linux)
  if build_b2
    fill_games += ['--game', 'IREO:b2:' + b2_rom_dir / 'baserom_b2_us.nds:third_party/pokebw2/config/b2_us']
  endif
  if build_w2
    fill_games += ['--game', 'IRDO:w2:' + b2_rom_dir / 'baserom_w2_us.nds:third_party/pokebw2/config/w2_us']
  endif
  # Platinum (group pl, pl.o: pret's C, its tables and strings like any game's) and the C every game runs (group s). Without
  # the game's own row the SDK's and lib/nnsys's initialised data are zero in Platinum (g2d_Renderer.c's currenVramTransferHandle_ 0,
  # not NNS_G2D_INVALID_CELL_TRANSFER_STATE_HANDLE: the opening's first sprite draw wrote through a NULL transfer state)
  if build_plat
    fill_games += ['--game', 'CPUE:pl:' + plat_rom + ':config/plat_us']   # (no delinks.txt there: no same-object preference)
  endif
  # HeartGold: the same (rom_fill: both its ROM and SoulSilver's are there)
  # SoulSilver: the same module (the version is a value the game reads), the same group: the shared C is filled with what both
  # ROMs have (port_romfill.py: every game of a group must match)
  if build_hg
    fill_games += ['--game', 'IPKE:hg:' + hg_rom + ':build/pokeheartgold']
    fill_games += ['--game', 'IPGE:hg:' + ss_rom + ':build/pokeheartgold']
  endif
  bwport = custom_target('bwport',
    input: bwport_full,
    output: build_target == 'win64' ? 'bwport.exe' : 'bwport',
    depend_files: [repo / 'tools/port_romfill.py', repo / 'tools/rom_modules.py'],
    command: [fill_python, repo / 'tools/port_romfill.py', '@INPUT@', '@OUTPUT@', '--map',
              meson.current_build_dir() / 'bwport-full.map', '--report', meson.current_build_dir() / 'romfill.txt'] + fill_games,
    build_by_default: true)
endif

# build/port/undefined.txt from this build dir's link
run_target('undefined',
  command: [python, repo / 'tools/port_undefined.py', meson.current_build_dir(), '--edge', 'bwport-nostubs',
            '--allow-data', repo / 'port/undefined_data_allow.txt'],
  depends: [game, entry, vbridge, abridge, gbridge, gpu_lib, vcore_lib, acore_lib])
