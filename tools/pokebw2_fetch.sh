#!/bin/bash
# tools/pokebw2_fetch.sh: Black 2's and White 2's C for the port (the maintainers' notes), from pokebw2
# (github.com/fuddlesworth/pokebw2, GPL-3, the same license as ours: a 100% matched decompilation of Pokemon Black 2 and
# White 2 by its contributors), at the pinned commit below, into build/pokebw2/ (git-ignored, never committed;
# meson refuses files from inside its subprojects dir).
#
# pokebw2 commits its game's assets (res/: about 140k PNG, WAV, model files, all made from the ROM): the port reads the
# game's files from the user's ROM at run time, so the checkout is a partial (blob:none), sparse one of only what the
# C build needs: src lib include generated, the scripts that write the generated headers (tools/data, tools/text), their
# inputs (res/text's message lists for the message ID headers and the data lists their \from lines count), and
# config/<version>/arm9 (the overlays' layout: tools/port_overlays.py --root). About 200 MB with its .git, not the full
# clone's 1.3 GB.
#
# Then the native fixes (port/subprojects/pokebw2-*.patch, in PATCHES' order: each says what it fixes and why; not in
# packagefiles/, whose every change makes tools/port_wraps.py reset libntr's checkout) and the generated headers into
# build/pokebw2/build/include/generated, as pokebw2's configure.py writes them. Run again, it does nothing while the
# commit and the patches are the same (.bwport-stamp); a changed patch builds the patched tree in a scratch index and
# moves the checkout to it, rewriting only the files that differ (the headers only when the commit changes).
# POKEBW2_URL overrides the URL (a local clone works too).
#
#   tools/pokebw2_fetch.sh            # prints the checkout's path
#   tools/pokebw2_fetch.sh --vendor   # and copies it into third_party/pokebw2 (the updater: docs/black2-white2.md)
#
# The port builds from third_party/pokebw2, the vendored copy of this checkout (docs/black2-white2.md, "The vendored
# tree"); this script is only its updater. --vendor writes the file set the port's build reads: src, include, the
# eight libraries' headers (and lib/poke_net's and lib/spl's sources), the generated headers the C includes (text/
# message IDs with tools/msgid_neutral.py's neutral names, constants/) in generated/, and each version's
# config/<version>/arm9 delinks.txt and symbols.txt (tools/port_overlays.py, tools/asset.py, the ROM fill). Then
# third_party/pokebw2's own commits since the last update (the asset moves) are applied again by hand: git diff.
set -e
ROOT=$(cd "$(dirname "$0")/.." && pwd)
URL=${POKEBW2_URL:-https://github.com/fuddlesworth/pokebw2.git}
COMMIT=921123bed5426a59de3e79a6912936be2ed7cd70
DEST=$ROOT/build/pokebw2
PKG=$ROOT/port/subprojects
PATCHES=(
    pokebw2-evalorder.patch
    pokebw2-workareas.patch
    pokebw2-sdk.patch
    pokebw2-pointers.patch
    pokebw2-dsprot.patch
    pokebw2-pointers2.patch
    pokebw2-hwaddr.patch
    pokebw2-pointers3.patch
    pokebw2-skipmenu.patch
    pokebw2-vmjump.patch
    pokebw2-eventsizes.patch
    pokebw2-allocsizes.patch
)
SPARSE='/src/
/lib/
/include/
/generated/
/tools/
/config/b2_us/arm9/
/config/w2_us/arm9/
/res/text/
/res/pokemon/**/*.json
/res/moves/**/*.json
/res/items/**/*.json
/res/trainers/**/*.json
/res/abilities/**/*.json
/res/types/**/*.json
/res/field/trades/**/*.json
/res/field/places/**/*.json
/res/natures/**/*.json'

stamp=$(cd "$PKG" && { echo "$COMMIT"; for p in "${PATCHES[@]}"; do echo "$p"; cat "$p"; done; } | shasum | cut -c1-40)
if [ -f "$DEST/.bwport-stamp" ] && [ "$(cat "$DEST/.bwport-stamp")" = "$stamp" ] && [ "$1" != --vendor ]; then
    echo "$DEST"
    exit 0
fi
if [ ! -d "$DEST/.git" ]; then
    rm -rf "$DEST"
    git clone --quiet --filter=blob:none --no-checkout "$URL" "$DEST" >&2
fi
git -C "$DEST" sparse-checkout set --no-cone $SPARSE >&2
git -C "$DEST" cat-file -e "$COMMIT^{commit}" 2>/dev/null || git -C "$DEST" fetch --quiet --filter=blob:none origin "$COMMIT" >&2
if [ "$(git -C "$DEST" rev-parse -q --verify HEAD)" != "$COMMIT" ]; then
    git -C "$DEST" -c advice.detachedHead=false checkout --quiet --force "$COMMIT" >&2
fi

# The patched tree, built in a scratch index with the working copy untouched: the commit, then each patch
IDX=$DEST/.git/bwport-index
rm -f "$IDX"
GIT_INDEX_FILE=$IDX git -C "$DEST" read-tree "$COMMIT"
for p in "${PATCHES[@]}"; do
    GIT_INDEX_FILE=$IDX git -C "$DEST" apply --cached "$PKG/$p" >&2
done
tree=$(GIT_INDEX_FILE=$IDX git -C "$DEST" write-tree)
rm -f "$IDX"
# The working copy moves to it rewriting only the files whose content changes (their mtimes are what ninja sees, so a
# patch edit recompiles only the files it touches): the index becomes the tree and the files whose entry changed are
# written; a file that still differs from it (a local edit) is written again.
git -C "$DEST" read-tree --reset -u "$tree" >&2
git -C "$DEST" diff --name-only -z | (cd "$DEST" && xargs -0 git checkout-index -f --) >&2
git -C "$DEST" clean --quiet -fdx -e /build/ -e /.bwport-stamp -e /.bwport-gen >&2

# The generated headers (pokebw2's configure.py: gen_constants, text_ids, pms_words) come from the commit's files,
# which no patch changes: written again only when the commit changes (.bwport-gen)
GEN=$DEST/build/include/generated
if [ "$(cat "$DEST/.bwport-gen" 2> /dev/null)" != "$COMMIT" ]; then
    mkdir -p "$GEN/constants"
    cd "$DEST"
    for f in generated/*.txt; do
        python3 tools/data/gen_constants.py "$f" "$GEN/constants/$(basename "${f%.txt}").h" >&2
    done
    for f in res/text/system/*.txt res/text/script/*.txt; do
        d=$(basename "$(dirname "$f")")
        mkdir -p "$GEN/text/$d"
        python3 tools/text/text_ids.py "$f" "$GEN/text/$d/$(basename "${f%.txt}").h" >&2
    done
    python3 tools/text/pms_words.py "$GEN/constants/pms_words.h" >&2
    echo "$COMMIT" > "$DEST/.bwport-gen"
fi
echo "$stamp" > "$DEST/.bwport-stamp"
if [ "$1" = --vendor ]; then
    V=$ROOT/third_party/pokebw2
    rm -rf "$V/src" "$V/include" "$V/lib" "$V/generated" "$V/config"
    mkdir -p "$V/generated"
    cp -r "$DEST/src" "$DEST/include" "$V/"
    for l in dpw dsprot dwc dwcutil irc nitrowifi poke_net spl; do
        mkdir -p "$V/lib/$l"
        cp -r "$DEST/lib/$l/include" "$V/lib/$l/"
    done
    cp -r "$DEST/lib/poke_net/src" "$V/lib/poke_net/"
    cp -r "$DEST/lib/spl/src" "$V/lib/spl/"
    # only the generated headers some file includes (the message-ID headers hold every message's first words)
    (cd "$DEST" && grep -rhoE '#\s*include\s*[<"](text|constants)/[^>"]+' src include lib) | sed -E 's/.*[<"]//' | sort -u |
        while read -r h; do
            [ -f "$GEN/$h" ] || continue
            mkdir -p "$V/generated/$(dirname "$h")"
            cp "$GEN/$h" "$V/generated/$h"
        done
    (cd "$DEST" && find config/b2_us/arm9 config/w2_us/arm9 -name delinks.txt -o -name symbols.txt) | while read -r f; do
        mkdir -p "$V/$(dirname "$f")"
        cp "$DEST/$f" "$V/$f"
    done
    git -C "$DEST" show "$COMMIT:LICENSE" > "$V/LICENSE"
    python3 "$ROOT/tools/msgid_neutral.py" --root "$V" --headers generated/text --dirs src include lib generated >&2
fi
echo "$DEST"
