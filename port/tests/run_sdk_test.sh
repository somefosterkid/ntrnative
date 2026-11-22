#!/bin/sh
# Builds and runs port/tests/sdk_test.c on the host (linux x86_64): port/src/sdk_mi.c and sdk_dgt.c against a libntr build
# (the tree's own libntr, port/subprojects/libntr: docs/libntr/fold.md; NitroSystem's headers are our lib/nnsys's).
#   port/tests/run_sdk_test.sh LIBNTR_BUILD [ROM]
# LIBNTR_BUILD: a build of port/subprojects/libntr with -Dbuild_target=linux -Dsdk_version_major=5 -Dhardware_type=twl_hyb
# (the PC build's settings: build/port/linux/subprojects/libntr is one). The fx_const.h it generates is taken from LIBNTR_BUILD/gen.
set -e
B=$1 ROM=$2
PORT=$(cd "$(dirname "$0")/.." && pwd)
NTR=$PORT/subprojects/libntr
NNS=$PORT/../lib/nnsys
OUT=${TMPDIR:-/tmp}/sdk_test.$$
mkdir -p "$OUT"
GEN=$(dirname "$(find "$B" -name fx_const.h -path '*nitro/fx/*' | head -1)")/../..
DEFS="-DSDK_CW_FORCE_EXPORT_SUPPORT -DSDK_TS -DSDK_4M -DSDK_FINALROM -DNNS_FINALROM -DSDK_VERSION_MAJOR=5UL -DSDK_TWL
      -DSDK_TWLHYB -DSDK_PORT -DSDK_X86 -DSDK_CODE_X86 -DSDK_BUILD_LINUX -DSDK_64BIT -D_NITRO"
CFLAGS="-std=gnu99 -g -O1 -w -U_FORTIFY_SOURCE -Werror=implicit-function-declaration $DEFS -I$PORT/include -I$NTR/include -I$GEN -I$PORT/include_nnsys -I$NNS/include"
gcc $CFLAGS -c "$PORT/src/sdk_mi.c" -o "$OUT/sdk_mi.o"
gcc $CFLAGS -c "$PORT/src/sdk_dgt.c" -o "$OUT/sdk_dgt.o"
gcc $CFLAGS -c "$PORT/tests/sdk_test.c" -o "$OUT/sdk_test.o"
# the pure functions only: their archives' other members' references (the simulator's registers ...) stay unresolved
gcc -no-pie -o "$OUT/sdk_test" "$OUT/sdk_test.o" "$OUT/sdk_mi.o" "$OUT/sdk_dgt.o" "$B/libraries/fx/libfx.a" "$B/libraries/os/libos.a" \
    "$B/libraries/math/libmath.a" "$B/libraries/mi/libmi.a" "$B/libraries/dgt/libdgt.a" -Wl,--unresolved-symbols=ignore-all
"$OUT/sdk_test" $ROM
rm -r "$OUT"
