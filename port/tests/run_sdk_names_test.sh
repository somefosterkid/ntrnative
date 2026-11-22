#!/bin/sh
# Builds port/tests/sdk_names_test.c with port/src/sdk_mi.c and port/src/sdk_names.c on libntr (linux, SDK 5 twl_hyb, the
# PC build's settings) and runs it on libntr's simulator. libntr is the tree's own, port/subprojects/libntr (a plain
# directory, docs/libntr/fold.md: the sdk-gaps change is part of it), linked into the test's scratch project; NitroSystem
# is our lib/nnsys (its headers: port/include_nnsys, lib/nnsys/include; the test needs no library of it).
#   port/tests/run_sdk_names_test.sh [OUTDIR]
# Needs a display (WSLg's :0, or run it under xvfb-run -a).
set -e
PORT=$(cd "$(dirname "$0")/.." && pwd)
REPO=$(cd "$PORT/.." && pwd)
OUT=${1:-${TMPDIR:-/tmp}/sdk_names_test}
mkdir -p "$OUT/subprojects"
ln -sfn "$PORT/subprojects/libntr" "$OUT/subprojects/libntr"
cat > "$OUT/meson.build" <<MESON
project('sdk_names_test', 'c', 'cpp', default_options: {'default_library': 'static'})
add_global_arguments('-U_FORTIFY_SOURCE', language: ['c', 'cpp'], native: true)
nitrosdk_dep = dependency('NitroSDK', fallback: ['libntr', 'nitrosdk_dep'], native: true)
nitrosystem_dep = declare_dependency(compile_args: ['-I$REPO/port/include_nnsys', '-I$REPO/lib/nnsys/include'])
executable('sdk_names_test', '$PORT/tests/sdk_names_test.c', '$PORT/src/sdk_mi.c', '$PORT/src/sdk_names.c',
           '$PORT/src/sdk_dgt.c', '$PORT/src/sdk_twl.c',
           include_directories: include_directories('$PORT/include'),
           c_args: ['-Werror=implicit-function-declaration'],
           dependencies: [nitrosdk_dep, nitrosystem_dep], native: true)
MESON
# the parent owns libntr's yielding options (it reads build_target)
cat > "$OUT/meson.options" <<OPTS
option('build_target', type: 'combo', choices: ['arm', 'win64', 'linux', 'nx', 'prospero'], value: 'linux')
option('sdk_version_major', type: 'integer', value: 5)
option('hardware_type', type: 'combo', choices: ['ntr', 'twl_hyb', 'twl_ltd'], value: 'twl_hyb')
OPTS
# libntr's gen_git_info.py runs git in the consumer's source dir
[ -d "$OUT/.git" ] || (cd "$OUT" && git init -q && git add meson.build && git -c user.name=t -c user.email=t@t commit -qm t)
[ -d "$OUT/build" ] || meson setup "$OUT/build" "$OUT" -Dbuild_target=linux -Dsdk_version_major=5 -Dhardware_type=twl_hyb     >"$OUT/setup.log" || { tail -20 "$OUT/setup.log"; exit 1; }
ninja -C "$OUT/build" sdk_names_test >"$OUT/build.log" || { grep -B2 -A10 "error" "$OUT/build.log" | head -60; exit 1; }
mkdir -p "$OUT/run"
cd "$OUT/run" && SDL_AUDIODRIVER=dummy timeout -s KILL 60 "$OUT/build/sdk_names_test"
