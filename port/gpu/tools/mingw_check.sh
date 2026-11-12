#!/bin/bash
# G1: libgpu's GL-facing sources compile for win64 (clang --target=x86_64-w64-mingw32, the port's compiler), with and
# without the SDL context; no EGL there. Prints "ok FILE" per file, the compiler's complaints otherwise; exit 1 on any.
cd "$(dirname "$0")/../../.." || exit 1
M=${MINGW:-/root/pc/t40u/mingw}
rc=0
for f in port/gpu/src/gpu_gl.c port/gpu/src/gpu_egl.c port/gpu/src/gpu_sdl.c port/gpu/src/g00_frame.c port/gpu/src/g02_spans.c port/gpu/tools/gpu_info.c; do
  for d in "" "-DGPU_HAVE_SDL"; do
    if clang --target=x86_64-w64-mingw32 -std=c11 -Wall -Wno-unused-function $d -c -o /dev/null -I$M/include -I$M/include/SDL2 -I$M/shim \
        -Iport/gpu/include -Iport/gpu/src -Iport/vcore/include -Iport/vcore/src $f; then echo "ok $f $d"; else rc=1; fi
  done
done
exit $rc
