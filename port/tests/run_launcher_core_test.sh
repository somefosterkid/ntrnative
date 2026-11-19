#!/bin/sh
# The launcher core's unit tests (port/tests/launcher_core_test.c): ROM identification, save checks per game family, the
# playable names and edition lines, on synthetic files. Builds the test with the launcher core (port/src/launcher_core.c)
# alone three times, every game in the table, Black and White, and Black alone, and runs each; prints one line a case,
# then PASS or FAIL per build; exits 1 when one failed.
set -e
here=$(cd "$(dirname "$0")" && pwd)
out=${TMPDIR:-/tmp}/launcher_core_test.$$
status=0
for flags in "-DBWPORT_WHITE -DBWPORT_B2 -DBWPORT_W2 -DBWPORT_PLAT -DBWPORT_HG" "-DBWPORT_WHITE" ""; do
    echo "== launcher_core_test ${flags:-(Black alone)}"
    # shellcheck disable=SC2086
    cc -std=gnu11 -O1 -w $flags -I"$here/../src" -o "$out" "$here/launcher_core_test.c" "$here/../src/launcher_core.c"
    "$out" 2>/dev/null || status=1
done
rm -f "$out"
exit $status
