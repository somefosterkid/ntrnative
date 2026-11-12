# Shared by pics_ab.sh, vcore_ab.sh and fast_ab.sh (sourced, not run). Picture A/B checks of port/gpu and port/vcore.
#   BWPORT_CAPTURES   colon list of capture dirs (.vcs files, searched recursively); the tools also take them as arguments
#   BWPORT_SCRATCH    work dir (default build/pics-ab under the repo, which is git-ignored)
#   BWPORT_APIS       gpu_render --api list: default "gl metal" on macOS, "gl es" elsewhere (the Linux list is untested)
#   BWPORT_JOBS       ninja -j (default 3, run under nice -n 10: other work runs beside)
#   BWPORT_NOCMAKE    a dir with a stub `cmake` put first on PATH for meson (the Mac sandbox: meson's `cmake --version` probe
#                     hangs); default $REPO/build/.nocmake when that exists; unset elsewhere
# Captures hold data extracted from the ROM: never commit them.

AB_REPO=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
AB_WORK=${BWPORT_SCRATCH:-$AB_REPO/build/pics-ab}
mkdir -p "$AB_WORK"
AB_WORK=$(cd "$AB_WORK" && pwd)
if [ -n "${BWPORT_NOCMAKE:-}" ]; then
  PATH=$BWPORT_NOCMAKE:$PATH
elif [ -d "$AB_REPO/build/.nocmake" ]; then
  PATH=$AB_REPO/build/.nocmake:$PATH
fi
export PATH
if [ "$(uname)" = Darwin ]; then
  AB_APIS=${BWPORT_APIS:-gl metal}
  export SDL_MAC_BACKGROUND_APP=1
else
  AB_APIS=${BWPORT_APIS:-gl es}
fi

# md5 of a file, empty when it is missing
ab_md5() {
  if command -v md5 > /dev/null; then md5 -q "$1" 2> /dev/null; else md5sum "$1" 2> /dev/null | cut -d' ' -f1; fi
}

# ab_captures [DIR...]: the capture list into $AB_WORK/captures.txt (the dirs, else $BWPORT_CAPTURES); exit 2 when there is none
ab_captures() {
  local dirs=("$@") d
  if [ ${#dirs[@]} -eq 0 ] && [ -n "${BWPORT_CAPTURES:-}" ]; then
    IFS=: read -ra dirs <<< "$BWPORT_CAPTURES"
  fi
  : > "$AB_WORK/captures.txt"
  for d in "${dirs[@]}"; do
    find "$d" -name '*.vcs' -type f | sort >> "$AB_WORK/captures.txt"
  done
  if [ ! -s "$AB_WORK/captures.txt" ]; then
    echo "no .vcs captures: give capture dirs or set BWPORT_CAPTURES (a colon list)" >&2
    exit 2
  fi
}

# ab_build SRC_ROOT PROJECT(gpu|vcore) BUILDDIR: meson setup (once) and ninja of port/PROJECT of that tree
ab_build() {
  local src=$1 proj=$2 bd=$3
  if [ ! -d "$bd" ]; then
    meson setup "$bd" "$src/port/$proj" > "$bd.setup.log" 2>&1 || { echo "$proj setup failed"; tail -3 "$bd.setup.log"; exit 1; }
  fi
  nice -n 10 ninja -C "$bd" -j"${BWPORT_JOBS:-3}" > "$bd.build.log" 2>&1 || { echo "$proj build failed ($bd)"; tail -3 "$bd.build.log"; exit 1; }
}

# ab_exe BUILDDIR NAME: the built tool's path
ab_exe() {
  find "$1" -name "$2" -type f | head -1
}

# ab_abs DIR: absolute path of an existing dir
ab_abs() {
  (cd "$1" && pwd)
}
