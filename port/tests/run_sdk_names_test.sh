#!/bin/sh
# Builds port/tests/sdk_names_test.c with port/src/sdk_mi.c and port/src/sdk_names.c on libntr and libntrsystem (linux,
# SDK 5 twl_hyb, the PC build's settings) and runs it on libntr's simulator. libntr comes in as the PC build's will: a
# wrap-git of the pinned commit with port/subprojects/packagefiles/libntr-sdk-gaps.patch as its diff_files.
#   port/tests/run_sdk_names_test.sh LIBNTR_GIT LIBNTRSYSTEM_SRC [OUTDIR]
# LIBNTR_GIT: a clone (or URL) holding commit f6b3a69. Needs a display (WSLg's :0, or run it under xvfb-run -a).
set -e
NTR_GIT=$1 NNS=$2
PORT=$(cd "$(dirname "$0")/.." && pwd)
OUT=${3:-${TMPDIR:-/tmp}/sdk_names_test}
mkdir -p "$OUT/subprojects/packagefiles"
cp "$PORT/subprojects/packagefiles/libntr-sdk-gaps.patch" "$OUT/subprojects/packagefiles/"
cat > "$OUT/subprojects/libntr.wrap" <<WRAP
[wrap-git]
url = $NTR_GIT
revision = f6b3a690c72b1484fda86c47021dbda2b54ffb69
depth = 1
diff_files = libntr-sdk-gaps.patch
WRAP
ln -sfn "$NNS" "$OUT/subprojects/libntrsystem"
cat > "$OUT/meson.build" <<MESON
project('sdk_names_test', 'c', 'cpp', default_options: {'default_library': 'static'})
add_global_arguments('-U_FORTIFY_SOURCE', language: ['c', 'cpp'], native: true)
nitrosdk_dep = dependency('NitroSDK', fallback: ['libntr', 'nitrosdk_dep'], native: true)
nitrosystem_dep = dependency('NitroSystem', fallback: ['libntrsystem', 'nitrosystem_dep'], native: true)
executable('sdk_names_test', '$PORT/tests/sdk_names_test.c', '$PORT/src/sdk_mi.c', '$PORT/src/sdk_names.c',
           '$PORT/src/sdk_dgt.c', '$PORT/src/sdk_twl.c',
           include_directories: include_directories('$PORT/include'),
           c_args: ['-Werror=implicit-function-declaration'],
           dependencies: [nitrosdk_dep, nitrosystem_dep], native: true)
MESON
# the parent owns libntr's and libntrsystem's yielding options (both read build_target)
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
