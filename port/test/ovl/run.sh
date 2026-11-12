# The overlay model's test (port/src/ovl.c, tools/port_overlays.py) outside the game: three small objects named as
# meson names ov001's and ov021's files, linked with the script for linux (clang PIE and non-PIE, gcc) and win64
# (mingw gcc). Each exe dirties the overlays' data and checks that FS_LoadOverlay resets theirs and nothing else.
#   sh port/test/ovl/run.sh     # from the repo root; then run build/port/test-ovl/win64/test.exe on Windows
set -e
T=port/test/ovl
O=build/port/test-ovl
rm -rf $O && mkdir -p $O
python3 tools/port_overlays.py --target linux -o $O/ovl_linux.ld
python3 tools/port_overlays.py --target win64 -o $O/ovl_win64.ld
build() {  # compiler, dir, script, flags...
    cc=$1; d=$O/$2/bwport.p; ld=$PWD/$3; shift 3
    mkdir -p $d
    p=_root_bw-port_
    $cc -g -fcommon "$@" -c $T/ova.c -o $d/${p}src_ov001_event_gtsnego.c.o
    $cc -g -fcommon "$@" -c $T/ovb.c -o $d/${p}src_ov021_field_map_chunk_wb.c.o
    $cc -g -fcommon "$@" -c $T/ovc.c -o $d/${p}src_ov021_field_map_chunk_gc.c.o
    $cc -g -fcommon "$@" -I$T/include -Iport/include -DBWPORT_OVL_SCRIPT -c port/src/ovl.c -o $d/${p}port_src_ovl.c.o
    $cc -g -fcommon "$@" -I$T/include -Iport/include -c $T/main.c -o $d/${p}main.c.o
    (cd $d/.. && $cc "$@" -o test$EXE bwport.p/*.o -Wl,-T,$ld)
}
EXE= build clang linux-pie $O/ovl_linux.ld -fPIE
EXE= build clang linux-nopie $O/ovl_linux.ld -fno-pie -no-pie
EXE= build gcc linux-gcc $O/ovl_linux.ld
EXE=.exe build x86_64-w64-mingw32-gcc-posix win64 $O/ovl_win64.ld
for t in linux-pie linux-nopie linux-gcc; do echo "$t: $($O/$t/test)"; done
