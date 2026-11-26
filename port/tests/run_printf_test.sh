#!/bin/sh
# Builds and runs port/tests/printf_test.c on the host: libntr's os_printf.c integer conversions against the DS's semantics.
#   port/tests/run_printf_test.sh [LIBNTR_BUILD]     (default build/port/linux/subprojects/libntr; needs its generated headers)
set -e
PORT=$(cd "$(dirname "$0")/.." && pwd)
REPO=$(cd "$PORT/.." && pwd)
NTR=$PORT/subprojects/libntr
B=${1:-$REPO/build/port/linux/subprojects/libntr}
OUT=${TMPDIR:-/tmp}/printf_test.$$
mkdir -p "$OUT"
GEN=$(dirname "$(find "$B" -name fx_const.h -path '*nitro/fx/*' | head -1)")/../..
DEFS="-DSDK_CW_FORCE_EXPORT_SUPPORT -DSDK_TS -DSDK_4M -DSDK_FINALROM -DNNS_FINALROM -DSDK_VERSION_MAJOR=5UL -DSDK_TWL
      -DSDK_TWLHYB -DSDK_PORT -DSDK_X86 -DSDK_CODE_X86 -DSDK_BUILD_LINUX -DSDK_64BIT -D_NITRO"
CFLAGS="-std=gnu99 -g -O1 -w -U_FORTIFY_SOURCE -fwrapv -fno-strict-aliasing $DEFS -I$NTR/include -I$GEN ${PRINTF_TEST_CFLAGS:-}"
cc $CFLAGS -c "$PORT/tests/printf_test.c" -o "$OUT/t.o"
# OS_SNPrintf goes through STD_TVSNPrintf (std_sprintf.c); os_printf.c's OS_VSNPrintfEx has the same integer code
cc $CFLAGS -c "$NTR/libraries/os/src/os_printf.c" -o "$OUT/p.o"
cc $CFLAGS -c "$NTR/libraries/std/src/std_sprintf.c" -o "$OUT/s.o"
cc -no-pie -o "$OUT/t" "$OUT/t.o" "$OUT/p.o" "$OUT/s.o" -Wl,--unresolved-symbols=ignore-all
rc=0
"$OUT/t" || rc=$?
rm -r "$OUT"
exit $rc
