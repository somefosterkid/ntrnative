#!/bin/sh
# Builds and runs port/tests/sdk_test.c on the host (linux x86_64): port/src/sdk_mi.c and sdk_dgt.c against a libntr build with
# port/subprojects/packagefiles/libntr-sdk-gaps.patch applied.
#   port/tests/run_sdk_test.sh LIBNTR_SRC LIBNTR_BUILD LIBNTRSYSTEM_SRC [ROM]
# LIBNTR_BUILD: a build of LIBNTR_SRC with -Dbuild_target=linux -Dsdk_version_major=5 -Dhardware_type=twl_hyb
# (the PC build's settings). The fx_const.h it generates is taken from LIBNTR_BUILD/gen.
set -e
NTR=$1 B=$2 NNS=$3 ROM=$4
PORT=$(cd "$(dirname "$0")/.." && pwd)
OUT=${TMPDIR:-/tmp}/sdk_test.$$
mkdir -p "$OUT"
GEN=$(dirname "$(find "$B" -name fx_const.h -path '*nitro/fx/*' | head -1)")/../..
DEFS="-DSDK_CW_FORCE_EXPORT_SUPPORT -DSDK_TS -DSDK_4M -DSDK_FINALROM -DNNS_FINALROM -DSDK_VERSION_MAJOR=5UL -DSDK_TWL
      -DSDK_TWLHYB -DSDK_PORT -DSDK_X86 -DSDK_CODE_X86 -DSDK_BUILD_LINUX -DSDK_64BIT -D_NITRO"
CFLAGS="-std=gnu99 -g -O1 -w -U_FORTIFY_SOURCE -Werror=implicit-function-declaration $DEFS -I$PORT/include -I$NTR/include -I$GEN -I$NNS/include"
gcc $CFLAGS -c "$PORT/src/sdk_mi.c" -o "$OUT/sdk_mi.o"
gcc $CFLAGS -c "$PORT/src/sdk_dgt.c" -o "$OUT/sdk_dgt.o"
gcc $CFLAGS -c "$PORT/tests/sdk_test.c" -o "$OUT/sdk_test.o"
# the pure functions only: their archives' other members' references (the simulator's registers ...) stay unresolved
gcc -no-pie -o "$OUT/sdk_test" "$OUT/sdk_test.o" "$OUT/sdk_mi.o" "$OUT/sdk_dgt.o" "$B/libraries/fx/libfx.a" "$B/libraries/os/libos.a" \
    "$B/libraries/math/libmath.a" "$B/libraries/mi/libmi.a" "$B/libraries/dgt/libdgt.a" -Wl,--unresolved-symbols=ignore-all
"$OUT/sdk_test" $ROM
rm -r "$OUT"
