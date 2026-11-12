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
endif

nitrosdk_dep = dependency('NitroSDK', fallback: ['libntr', 'nitrosdk_dep'], native: true)
# Our NitroSystem (lib/nnsys, docs/nnsys-port.md; libntrsystem's place until 2026-11-07). Its headers here (the umbrella
# <nnsys.h> port_prefix.h includes is port/include_nnsys/nnsys.h), the library below the game's flags
nitrosystem_dep = declare_dependency(compile_args: ['-I' + repo / 'port/include_nnsys', '-I' + repo / 'lib/nnsys/include'])

sources = run_command(python, repo / 'tools/port_sources.py', check: true).stdout().strip().split('\n')

rom_build = get_option('rom_build_dir') != '' ? get_option('rom_build_dir') : repo / 'build'

# The second game, Pokemon White (docs/launcher.md "More than one game" and both "Step 3 as built", docs/white.md). The PC
# (Linux, Windows, Android) builds it when the ROM build made White's asset .inc files (<rom build>/w_us/asset: ninja with
# orig/baserom_w_us.nds) and links it renamed (the w_ header below); macOS and iOS when White's ROM is beside Black's
# (tools/mac_rom.sh's dir: tools/rom_modules.py writes its assets) and link it as one relocatable object (white.o below).
white_dir = rom_build / 'w_us'

# The game's include dirs in configure.py's order (INCLUDE_DIRS), with libntr's and libntrsystem's (from the
# dependencies) where lib/nitro's, lib/twl's and lib/nnsys's were. port/include first: the SDK header names pokebw2
# uses and libntr lacks (nitro/fx.h ...) and port_prefix.h, which every file includes first (as libntr's
# include/pch/nitro_pch.h has its users do). MSL after the host's libc: libntr's thread.h includes SDL2, which
# includes the host's headers, and MSL's stdio.h, stdarg.h ... can't stand in for them.
game_includes = ['-I' + repo / 'port/include', '-I' + repo / 'include']
foreach lib : ['dpw', 'dsprot', 'dwc', 'dwcutil', 'nitrowifi', 'poke_net', 'spl']
  game_includes += '-I' + repo / 'lib' / lib / 'include'
endforeach
# asset/<name>.inc: the ROM build's values (tools/asset.py, or tools/rom_modules.py on Apple), as every other table of the C.
# Off Apple the shipped image holds none of them: tools/port_romfill.py zeroes the game's data after the link and
# port/src/asset_load.c copies it from the player's ROM at start (the bwport target below; docs/port-build.md, "Game data
# from the ROM"). asset_loader: rt1's per-table placeholders (tools/port_assets.py), off since the whole-image step.
asset_loader = false
asset_inc_dir = meson.current_build_dir() / 'asset_ph'
black_asset_inc = '-I' + (asset_loader ? asset_inc_dir : rom_build)
white_asset_inc = '-I' + (asset_loader ? asset_inc_dir / 'w_us' : rom_build / 'w_us')
game_includes += [black_asset_inc]
# With no dsd extraction (macOS, iOS: dsd is a Linux x86_64 program) the port writes them itself, from the base ROM
# (tools/mac_rom.sh's dir) through tools/rom_modules.py; each run rewrites only the files whose text changed
asset_deps = []
asset_tool = repo / 'tools/port_assets.py'
asset_tsvs = [repo / 'config/b_us/assets.tsv', repo / 'config/w_us/assets.tsv']
# each version's rows for asset_load.c (names, DS ranges, layouts: no ROM bytes; every platform links the loader)
asset_rows_c = custom_target('asset_rows',
  output: 'asset_rows.c',
  depend_files: [asset_tool] + asset_tsvs,
  command: [python, asset_tool, 'rows', '@OUTPUT@'])
asset_files = []
if asset_loader
  # the placeholders (the rows' shapes, every number 1) in asset_ph/asset and asset_ph/w_us/asset
  asset_deps += declare_dependency(sources: custom_target('asset_placeholders',
    output: 'asset_placeholders.stamp',
    depend_files: [asset_tool, repo / 'tools/asset_table.py'] + asset_tsvs,
    command: [python, asset_tool, 'incs', rom_build, asset_inc_dir, '@OUTPUT@']))
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
endif
build_white = not is_mac and get_option('white').allowed() and fs.is_dir(white_dir / 'asset')
build_b2 = false
build_w2 = false
b2_rom_dir = repo / 'orig'   # Black 2's and White 2's ROMs (their asset rows: tools/rom_modules.py below)
if get_option('rom_build_dir') == '' and not fs.exists(repo / 'extract/b_us/config.yaml')
  rom = run_command(repo / 'tools/mac_rom.sh', check: true).stdout().strip()
  # White beside Black in the one binary (docs/launcher.md, "More than one game"), when its ROM is there for its assets:
  # macOS and iOS for now (ld64's relocatable link below; the PC's link needs the same with GNU ld and lld)
  build_white = is_mac and get_option('white').allowed() and fs.exists(fs.parent(rom) / 'baserom_w_us.nds')
  # Black 2 from pokebw2's C (the maintainers' notes), when its ROM is there: macOS and iOS for now, as White
  build_b2 = is_mac and fs.exists(fs.parent(rom) / 'baserom_b2_us.nds')
  build_w2 = is_mac and fs.exists(fs.parent(rom) / 'baserom_w2_us.nds')   # White 2: the same C, WHITE2
  b2_rom_dir = fs.parent(rom)
  asset_deps += declare_dependency(sources: custom_target('rom_assets',
    output: 'rom_assets.stamp',
    command: [python, repo / 'tools/rom_modules.py', '--assets', fs.parent(rom), '--stamp', '@OUTPUT@'],
    build_always_stale: true))
endif

if not is_mac
  # Black 2 and White 2 on Linux, Windows and Android (GNU ld, lld, PE: tools/port_relink.py), when their ROMs are in orig/
  # (the sha1s are checked at run time: port/src/launcher_core.c); no ROM, no module
  build_b2 = fs.exists(repo / 'orig/baserom_b2_us.nds')
  build_w2 = fs.exists(repo / 'orig/baserom_w2_us.nds')
endif

if get_option('white').enabled() and not build_white
  error('white: needs ' + white_dir / 'asset' + ' (ninja with orig/baserom_w_us.nds; Apple: White\'s ROM beside Black\'s)')
endif

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
  if is_mac   # (the PC's configure.py writes it)
    # White's config (config/w_us, not committed: Black's and the delta) for its overlays' layout below
    run_command(python, repo / 'tools/version_config.py', 'w_us', check: true)
  endif
endif
if build_b2
  game_args += '-DBWPORT_B2'   # port/src: Black 2's table (port/include/port_game.h), its sha1 in launcher_core.c
endif
if build_w2
  game_args += '-DBWPORT_W2'   # port/src: White 2's table and sha1
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
              'override_options': ['c_std=gnu99'], 'implicit_include_directories': false, 'native': true}
  nnsys_libs += static_library('nnsys',
    run_command(python, repo / 'tools/port_sources.py', '--nnsys', check: true).stdout().strip().split('\n'),
    c_args: game_args + machine_args + ['-include', repo / 'port/include_nnsys/nnsys_lib.h'], kwargs: nnsys_kw)
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
  c_args: game_args + machine_args + ['-DBWPORT_ENTRY'],
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
  endif
  macho_ovl_libs += static_library('game_ovl', ovl_members,
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
game_gen = asset_loader ? [asset_gen.process(game_assets, preserve_path_from: repo)] : []
if entry_c in asset_files
  error('src/system/main.c includes an asset: port/meson.build compiles it apart (entry), without the rewrite')
endif
game = static_library('game', game_plain, game_gen,
  c_args: game_args + machine_args,
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
    asset_loader ? [asset_gen.process(white_assets, preserve_path_from: repo)] : [],
    c_args: white_args + machine_args,
    dependencies: [nitrosdk_dep.partial_dependency(compile_args: true, includes: true, sources: true),
                   nitrosystem_dep.partial_dependency(compile_args: true, includes: true)],
    override_options: ['c_std=gnu99'],
    implicit_include_directories: false,
    native: true)
  white_libs += static_library('entry_w', entry_c, rename_h,
    c_args: white_args + machine_args + ['-DBWPORT_ENTRY'],
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
  foreach s : game_sources
    if s == ovl_gen_dir / 'ovltab.c'
      w_sources += w_ovl_gen_dir / 'ovltab.c'
    elif not (s.startswith(repo / 'port/src/') or s in shared_game)
      w_sources += s
    endif
  endforeach
  w_args = ['-DWHITE']
  foreach a : game_args
    w_args += a == '-I' + rom_build ? '-I' + rom_build / 'w_us' : a   # asset/<name>.inc from White's ROM
  endforeach
  w_kw = {'dependencies': [nitrosdk_dep.partial_dependency(compile_args: true, includes: true, sources: true),
                           nitrosystem_dep.partial_dependency(compile_args: true, includes: true)] + asset_deps,
          'override_options': ['c_std=gnu99'], 'implicit_include_directories': false, 'native': true}
  w_libs = [static_library('w_game', w_sources, c_args: w_args + machine_args, kwargs: w_kw),
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
  b2_kw = {'dependencies': [nitrosdk_dep.partial_dependency(compile_args: true, includes: true, sources: true),
                            nitrosystem_dep.partial_dependency(compile_args: true, includes: true)],
           'override_options': ['c_std=gnu99'], 'implicit_include_directories': false, 'native': true}
  b2_ld = []
  foreach a : meson.get_compiler('c', native: true).cmd_array() + get_option('c_args')
    b2_ld += a == 'ccache' ? [] : [a]
  endforeach
  # (PE: GNU objcopy and nm: llvm-objcopy has no --keep-global-symbol for COFF)
  b2_objcopy = disabler()
  b2_nm = disabler()
  if not is_mac
    if build_target == 'win64'
      b2_objcopy = find_program('x86_64-w64-mingw32-objcopy', native: true)
      b2_nm = find_program('x86_64-w64-mingw32-nm', native: true)
    else
      b2_objcopy = find_program('llvm-objcopy', 'objcopy', native: true)
      b2_nm = find_program('llvm-nm', 'nm', native: true)
    endif
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
    # the tables the C includes (asset/<name>.inc, config/<version>/assets.tsv) from the player's ROM into build/<version>
    b2_assets = custom_target(tag + '_assets',
      output: tag + '_assets.stamp',
      command: [python, repo / 'tools/rom_modules.py', '--assets', b2_rom_dir, '--version', g[2], '--stamp', '@OUTPUT@'],
      build_always_stale: true)
    kw = b2_kw + {'dependencies': b2_kw['dependencies'] + declare_dependency(sources: b2_assets)}
    args = ['-D' + g[1]] + b2_args + ['-I' + repo / 'build' / g[2]] + b2_late
    # the writable data in sections of its own (the machine's range, the overlays'): ld64's by pragma, the others' by
    # port/machine/gnu_*_section.h and the linker script (tools/port_overlays.py)
    b2_mach = is_mac ? machine_args : ['-fno-common', '-include', meson.current_source_dir() / 'machine/gnu_machine_section.h']
    b2_ovlh = meson.current_source_dir() / (is_mac ? 'machine/macos_ovl_section.h' : 'machine/gnu_ovl_section.h')
    libs = [static_library(tag + '_game', main_files, c_args: args + b2_mach, kwargs: kw),
            static_library(tag + '_entry', b2_entry_c, c_args: args + b2_mach + ['-DBWPORT_ENTRY'], kwargs: kw),
            static_library(tag + '_game_ovl', ovl_members,
              c_args: args + ['-fno-common', '-include', b2_ovlh], kwargs: kw)]
    if is_mac
      b2w2_objs += custom_target(tag + '_o',
        output: tag + '.o',
        input: libs,
        command: b2_ld + ['-r', '-nostdlib', '-Wl,-exported_symbol,_' + tag + '_bwgame', '-o', '@OUTPUT@', '-Wl,-all_load', '@INPUT@'])
    else
      b2w2_objs += custom_target(tag + '_o',
        output: tag + '.o',
        input: libs,
        command: [python, repo / 'tools/port_relink.py', '--cc', ' '.join(b2_ld), '--objcopy', b2_objcopy.full_path(),
                  '--keep', tag + '_bwgame', '-o', '@OUTPUT@'] + (build_target == 'win64' ? ['--nm', b2_nm.full_path(), '--tag', tag] : []) + ['@INPUT@'])
    endif
  endforeach
endif

# The overlays' writable data gathered into ranges port/src/ovl.c resets at each FS_LoadOverlay, with the DS's overlay
# table (tools/port_overlays.py: an INSERT script for GNU ld, ELF and PE)
ovl_ld = custom_target('ovl_ld',
  output: 'ovl.ld',
  depend_files: repo / 'tools/port_overlays.py',   # (the machine's ranges are in it too)
  command: [python, repo / 'tools/port_overlays.py', '--target', build_target, '-o', '@OUTPUT@'])
ovl_link_args = is_mac ? ['-Wl,-order_file,' + meson.current_source_dir() / 'machine/macos.order'] : ['-Wl,-T,' + ovl_ld.full_path()]
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
  ovl_link_args += ['-shared', '-Wl,-soname,libmain.so']
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

# The link is C++'s (not objcpp: port/src/metal_present.mm). On win64 meson picks it itself: link_language: 'cpp' makes it
# add -lstdc++, which finds libstdc++.dll.a before the static libstdc++ and the link fails on duplicate symbols
link_kw = build_target == 'win64' ? {} : {'link_language': 'cpp'}
bwport_nostubs = executable('bwport-nostubs', white_objs, b2w2_objs,
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
  sdk_libs = subproject('libntr').get_variable('nitrosdk_libs') + nnsys_libs
  stub_sources = custom_target('undefined_stubs',
    output: ['undefined_stubs.c', 'undefined.txt'],
    command: [python, repo / 'tools/port_undefined.py', meson.current_build_dir(), '--edge', 'bwport-nostubs',
              '--stubs', '@OUTPUT0@', '-o', '@OUTPUT1@'],
    depends: [game, entry, vbridge, abridge, gbridge, gpu_lib, vcore_lib, acore_lib] + white_libs + host_libs + macho_ovl_libs + buildinfo + sdk_libs + white_objs + b2w2_objs)[0]
endif

# Off Apple the link is bwport-full, with the game's data as compiled, and bwport is it after tools/port_romfill.py:
# every byte of the game's data (and the SDK's) that the player's ROM holds zeroed, and where it is in the ROM written
# into the image's .bwfill (port/src/asset_load.c copies it back at start; docs/port-build.md "Game data from the ROM")
rom_fill = not is_mac
bwport_full = executable(rom_fill ? 'bwport-full' : 'bwport', stub_sources, white_objs, b2w2_objs,
  name_suffix: exe_suffix,
  link_whole: [game, entry, vbridge, abridge, gbridge] + white_libs + host_libs + macho_ovl_libs + buildinfo,
  link_with: [gpu_lib, vcore_lib, acore_lib],
  link_args: ovl_link_args + (rom_fill ? ['-Wl,-Map=' + meson.current_build_dir() / 'bwport-full.map'] : []),
  link_depends: ovl_ld,
  dependencies: exe_deps,
  kwargs: link_kw,
  implicit_include_directories: false,
  native: true)
bwport = bwport_full
if rom_fill
  fill_games = ['--game', 'IRBO:b:' + repo / 'orig/baserom_b_us.nds:config/b_us']
  if build_white
    fill_games += ['--game', 'IRAO:w:' + repo / 'orig/baserom_w_us.nds:config/w_us']
  endif
  # Black 2 / White 2: their own C is not filled (it stays as compiled, b2.o), but the C every game runs (group s: the SDK's
  # libraries, lib/nnsys, the ARM7 tables) is filled only with the bytes every listed ROM holds, so a game left out would run
  # on zeroed shared data (OSi_ExtraHeapHandle's -1, the first crash of Black 2 on Linux)
  if build_b2
    fill_games += ['--game', 'IREO:b2:' + repo / 'orig/baserom_b2_us.nds:third_party/pokebw2/config/b2_us']
  endif
  if build_w2
    fill_games += ['--game', 'IRDO:w2:' + repo / 'orig/baserom_w2_us.nds:third_party/pokebw2/config/w2_us']
  endif
  bwport = custom_target('bwport',
    input: bwport_full,
    output: build_target == 'win64' ? 'bwport.exe' : 'bwport',
    depend_files: [repo / 'tools/port_romfill.py', repo / 'tools/rom_modules.py'],
    command: [python, repo / 'tools/port_romfill.py', '@INPUT@', '@OUTPUT@', '--map',
              meson.current_build_dir() / 'bwport-full.map', '--report', meson.current_build_dir() / 'romfill.txt'] + fill_games,
    build_by_default: true)
endif

# build/port/undefined.txt from this build dir's link
run_target('undefined',
  command: [python, repo / 'tools/port_undefined.py', meson.current_build_dir(), '--edge', 'bwport-nostubs'],
  depends: [game, entry, vbridge, abridge, gbridge, gpu_lib, vcore_lib, acore_lib])
