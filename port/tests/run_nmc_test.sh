#!/bin/sh
# Builds and runs port/tests/nmc_test.c on the host (linux x86_64): libntrsystem's multi-cell bank loader and expanded
# heap (a build with port/subprojects/packagefiles/libntrsystem-boot.patch applied) on real NMCR files.
#   port/tests/run_nmc_test.sh LIBNTR_SRC LIBNTR_BUILD LIBNTRSYSTEM_SRC LIBNTRSYSTEM_BUILD NMCR_FILE...
# The PC build's: port/subprojects/libntr build/port/linux/subprojects/libntr port/subprojects/libntrsystem
# build/port/linux/subprojects/libntrsystem. NMCR files: the multi-cell banks in the Pokemon sprites' NARC (a/0/0/4).
set -e
NTR=$1 B=$2 NNS=$3 NB=$4
shift 4
PORT=$(cd "$(dirname "$0")/.." && pwd)
OUT=${TMPDIR:-/tmp}/nmc_test.$$
mkdir -p "$OUT"
GEN=$(dirname "$(find "$B" -name fx_const.h -path '*nitro/fx/*' | head -1)")/../..
DEFS="-DSDK_CW_FORCE_EXPORT_SUPPORT -DSDK_TS -DSDK_4M -DSDK_FINALROM -DNNS_FINALROM -DSDK_VERSION_MAJOR=5UL -DSDK_TWL
      -DSDK_TWLHYB -DSDK_PORT -DSDK_X86 -DSDK_CODE_X86 -DSDK_BUILD_LINUX -DSDK_64BIT -D_NITRO"
CFLAGS="-std=gnu99 -g -O1 -w -U_FORTIFY_SOURCE -Werror=implicit-function-declaration $DEFS -I$NTR/include -I$GEN -I$NNS/include"
gcc $CFLAGS -c "$PORT/tests/nmc_test.c" -o "$OUT/nmc_test.o"
# the loader and the heap only: their archives' other members' references stay unresolved
gcc -no-pie -o "$OUT/nmc_test" "$OUT/nmc_test.o" -Wl,--start-group "$NB/libraries/g2d/libnnsg2d.a" \
    "$NB/libraries/fnd/libnnsfnd.a" "$B/libraries/os/libos.a" -Wl,--end-group -Wl,--unresolved-symbols=ignore-all
"$OUT/nmc_test" "$@"
rm -r "$OUT"
