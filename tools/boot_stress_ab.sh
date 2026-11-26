#!/bin/bash
# tools/boot_stress_ab.sh BASE [GAMES] [-- boot_stress.sh options]
#   Before / after for a change that is in the working tree on top of BASE (a commit): builds the Linux exe as the tree is (AFTER), then
#   again with every tracked file that differs from BASE put back to BASE's version (BEFORE), restores the tree, and runs
#   tools/boot_stress.sh on both exes for each game (GAMES: comma list, default black,platinum,heartgold), each into build/boot_stress/
#   <variant>-<game>/. Prints the two summary lines per game. Slow (BOOTS x games x 2): run it on a slot (docs/port-worker.md, Offload).
#   Example: bash tools/boot_stress_ab.sh d1969b581 black,platinum -- -n 500 -j 12 -l 30
cd "$(dirname "$0")/.." || exit 2
BASE=${1:?usage: boot_stress_ab.sh BASE [GAMES] [-- boot_stress options]}; shift
GAMES=black,platinum,heartgold
if [ $# -gt 0 ] && [ "$1" != -- ]; then GAMES=$1; shift; fi
[ "${1:-}" = -- ] && shift
FILES=$(git diff --name-only "$BASE" -- . ':!tools' ':!docs')
[ -n "$FILES" ] || { echo "boot_stress_ab: nothing differs from $BASE"; exit 2; }
build() {   # build the exe and keep it as build/port/$1/bwport
    python3 -c "import sys; sys.path.insert(0, 'tools'); sys.argv = ['x']; import port_run; e = port_run.build(); print('build $1:', 'ok' if e is None else e[-1500:]); sys.exit(0 if e is None else 1)" || exit 1
    mkdir -p "build/port/$1"; cp build/port/linux/bwport "build/port/$1/bwport"
}
build after
for f in $FILES; do git show "$BASE:$f" > "$f" 2> /dev/null || rm -f "$f"; done
build before
git checkout HEAD -- $FILES
git status --short | grep -v '^??' | head -5
rc=0
for g in ${GAMES//,/ }; do
    for v in before after; do
        BWPORT_BUILD=build/port/$v bash tools/boot_stress.sh -g "$g" -b "$v" -o "build/boot_stress/$v-$g" "$@" | grep -v '^boot_stress.*load before' ; [ "${PIPESTATUS[0]}" = 0 ] || rc=1
    done
done
rm -rf build/boot_stress/*/w[0-9]* 2> /dev/null   # the per-worker run dirs are ~1 GB each: results.tsv, fail/ stay
exit $rc
