#!/bin/bash
# tools/pokeplat_fetch.sh: Pokemon Platinum's C for the port survey (docs/platinum.md), from pret's pokeplatinum
# (github.com/pret/pokeplatinum: NO LICENSE file, so nothing of it is ever committed to bw-port), at the pinned commit
# below, into build/pokeplatinum/ (git-ignored).
#
# pokeplatinum's res/ holds ~24,800 files extracted from the ROM. Its C includes ~1,470 headers generated from res/ at
# build time (text bank IDs, map locals, NARC member enums (.naix), metang enums in generated/, and a few compiled-in
# tables: species egg moves/tutor sets/icon palettes, item maps, sprite templates, field/maps/headers.h,
# fonts/arrow_cursor.4bpp.h). So this checkout is a full one (blob:none, then checked out: ~260 MB) and the
# headers are made by pret's own meson setup (its cross toolchain metroskrew + meson, both fetched by its Makefile into
# build/pokeplatinum/tools and .../subprojects) and `ninja <the header targets>` only: no ROM is built. A later job can
# trim this to a sparse checkout of the header inputs (as tools/pokebw2_fetch.sh does).
#
# Then the native fixes (port/subprojects/pokeplat-*.patch, in PATCHES' order). Run again, it does nothing while the
# commit and the patches are the same (.bwport-stamp). POKEPLAT_URL overrides the URL (a local clone works too).
# metroskrew gets its own TMPDIR (two builds sharing /tmp/metroskrew race).
#
#   tools/pokeplat_fetch.sh            # prints the checkout's path
set -e
ROOT=$(cd "$(dirname "$0")/.." && pwd)
URL=${POKEPLAT_URL:-https://github.com/pret/pokeplatinum.git}
COMMIT=1776eba4c24a137b484d4135e79555f9063aa90c
DEST=$ROOT/build/pokeplatinum
PKG=$ROOT/port/subprojects
PATCHES=(
)

stamp=$(cd "$PKG" && { echo "$COMMIT"; for p in "${PATCHES[@]}"; do echo "$p"; cat "$p"; done; cat "$ROOT/tools/pokeplat_fixups.py"; } | sha1sum | cut -c1-40)
if [ -f "$DEST/.bwport-stamp" ] && [ "$(cat "$DEST/.bwport-stamp")" = "$stamp" ]; then
    echo "$DEST"; exit 0
fi
log=$ROOT/build/pokeplat_fetch.log
mkdir -p "$ROOT/build"
{
if [ ! -d "$DEST/.git" ]; then
    git clone -q --filter=blob:none --no-checkout "$URL" "$DEST"
fi
cd "$DEST"
git cat-file -e "$COMMIT^{commit}" 2>/dev/null || git fetch -q origin "$COMMIT"
git checkout -q -f "$COMMIT"
git clean -qfdx -e build/ -e tools/metroskrew/ -e subprojects/ -e .bwport-stamp -e .tmp/ -e platinum.us/main.lsf
for p in "${PATCHES[@]}"; do git apply "$PKG/$p"; done
python3 "$ROOT/tools/pokeplat_fixups.py" "$DEST"

# The generated headers, by pret's own build (configure only + the header targets).
export TMPDIR=$DEST/.tmp
mkdir -p "$TMPDIR"
rm -f build/lsfgen.stamp   # (lsfgen writes platinum.us/main.lsf, which git clean drops)
make configure </dev/null
targets=$(ninja -C build -t targets all | sed -n 's/^\([^:]*\.\(h\|naix\)\): .*/\1/p' | grep -v '^subprojects/\|\.p/' | sort -u)
ninja -C build -j "${BW_JOBS:-8}" $targets </dev/null
} >"$log" 2>&1 || { echo "pokeplat_fetch: failed, see $log" >&2; tail -20 "$log" >&2; exit 1; }
echo "$stamp" >"$DEST/.bwport-stamp"
echo "$DEST"
