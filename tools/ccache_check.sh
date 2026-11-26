#!/bin/bash
# Standing ccache check (ccx2 11-26, from ccx1's proof): does the same commit, built in a SECOND fresh build dir, come out of the cache?
# Two fresh dirs, a private cache (CCACHE_DIR under ~/.cache/bwport/ccache_check) and CCACHE_REMOTE_STORAGE empty, so the only thing measured is
# the objects of dir 1 serving dir 2 (the .pch sums of port/machine/ccache-nopch). Expect ~100% (ccx1 11-26: desktop/laptop 100%, BD 96%);
# before the sums: 8.6%. Cost: dir 1 is a cold build of bwport (~3.5 min on 8-16 threads), dir 2 about 35 s.
#   bash tools/ccache_check.sh [--min PCT] [-- extra meson setup args]       run from the repo root; exit 1 when below --min (default 90)
# Last line: `ccache_check: second-dir hits 100.0% (1553/1553) PASS (>= 90)`. Both build dirs and the private cache are removed at the end (--keep keeps them).
MIN=90; KEEP=; while [ $# -gt 0 ]; do case $1 in --min) MIN=$2; shift 2;; --keep) KEEP=1; shift;; --) shift; break;; *) echo "usage: $0 [--min PCT] [--keep] [-- meson args]" >&2; exit 2;; esac; done
cd "$(dirname "$0")/.." || exit 2
[ $# -gt 0 ] || set -- -Dgames=black   # one game: ~1550 objects, the launcher's cases all in it
command -v ccache > /dev/null || { echo "ccache_check: no ccache"; exit 2; }
CONF=$(ccache --show-config | sed -n 's/^(\(\/[^)]*\)).*/\1/p' | head -1)   # the machine's own settings file (the first setting that names one), read before CCACHE_DIR points at the private cache
W=${CCACHE_CHECK_DIR:-$HOME/.cache/bwport/ccache_check}; mkdir -p "$W"
export CCACHE_DIR=$W/cache CCACHE_REMOTE_STORAGE=
rm -rf "$CCACHE_DIR" build/port/ccchka build/port/ccchkb; mkdir -p "$CCACHE_DIR"
# the machine's ccache settings (sloppiness, ...) without its size limit and its remote storage
for c in "$CONF" /etc/ccache.conf; do [ -f "$c" ] && { grep -v '^remote_storage\|^max_size' "$c" > "$CCACHE_DIR/ccache.conf"; break; }; done
cleanup() { [ -n "$KEEP" ] || rm -rf "$CCACHE_DIR" build/port/ccchka build/port/ccchkb; }
trap cleanup EXIT
for d in a b; do
  nice -n 10 meson setup build/port/ccchk$d port --native-file port/machine/linux.ini --native-file port/machine/linux-ccache.ini -Ddev_fast=true "$@" > "$W/setup_$d.log" 2>&1 ||
    { echo "ccache_check: meson setup failed:"; tail -3 "$W/setup_$d.log"; exit 2; }
done
hits=0; total=0
for d in a b; do
  ccache -z > /dev/null
  s=$(date +%s)
  nice -n 10 ninja -C build/port/ccchk$d -j"${NTR_SLOT_JOBS:-$(nproc)}" bwport > "$W/ninja_$d.log" 2>&1 || { echo "ccache_check: build $d failed:"; tail -3 "$W/ninja_$d.log"; exit 2; }
  st=$(ccache --print-stats); g() { echo "$st" | awk -v k="$1" '$1==k {print $2}'; }
  hits=$(( $(g direct_cache_hit) + $(g preprocessed_cache_hit) )); total=$(( hits + $(g cache_miss) ))
  echo "ccache_check: dir $d: $hits/$total hits, $(( $(date +%s) - s )) s"
done
pct=$(awk -v h=$hits -v t=$total 'BEGIN { printf "%.1f", t ? 100 * h / t : 0 }')
if awk -v p=$pct -v m=$MIN -v t=$total 'BEGIN { exit !(t > 0 && p >= m) }'; then v=PASS; rc=0; else v=FAIL; rc=1; fi
echo "ccache_check: second-dir hits $pct% ($hits/$total) $v (>= $MIN)"
exit $rc
