#!/usr/bin/env python3
"""DS offsets and sizes of the game's structs, computed by the DS's own compiler (mwccarm, the game's flags).

The lockstep tester's melonDS side reads game variables from DS memory by address (tools/lockstep/vars.txt): a symbol
from config/b_us/arm9/**/symbols.txt and a chain of offsets. The offsets are the DS layout's (4-byte pointers, enums
4 bytes, u64 4-aligned), not the PC build's, so this compiles the C expressions with mwccarm into a table and reads
it back from the object file.

    python3 tools/lockstep/ds_offsets.py 'OFF(Field, player)' 'sizeof(FieldActor)' 'OFF(FieldActor, gPos.x)'
    python3 tools/lockstep/ds_offsets.py --tu src/system/game_data.c 'OFF(GameData, mmSys)'

OFF(TYPE, MEMBER) is offsetof (MEMBER may be a path: a.b[2].c). Each expression is evaluated in a translation unit that
includes the --header files (default: the headers below, enough for the structs in vars.txt) or the --tu C file
itself, for the structs a .c file defines (GameData in game_data.c, GameEvent in game_event.c). Run from the repo root
after `ninja` has run once (tools/wibo, the compilers). Plain python3, no packages.
"""
import argparse
import os
import struct
import subprocess
import sys
import tempfile

DEFAULT_HEADERS = [
    "types.h",
    "nitro/os.h",
    "system/gfl_use.h",
    "system/game_system.h",
    "system/vm.h",
    "field/field_internal.h",
    "field/field_player_core.h",
    "field/field_actor_internal.h",
    "field/player_state.h",
    "field/field_script.h",
    "field/field_script_supervisor.h",
    "save/event_work.h",
    "pml/poke_party.h",
    "battle/btl_main.h",
]

INCLUDES = ["include", "lib/dpw/include", "lib/dsprot/include", "lib/dwc/include", "lib/msl/include",
            "lib/nitro/include", "lib/nitrowifi/include", "lib/nnsys/include", "lib/poke_net/include",
            "lib/spl/include", "lib/twl/include", "build"]

# The game's flags (src/, as configure.py gives them), less -requireprotos
FLAGS = ["-O4,p", "-proc", "arm946e", "-thumb", "-interworking", "-enum", "int", "-char", "signed", "-fp", "soft",
         "-lang=c99", "-Cpp_exceptions", "off", "-gccext,on", "-gccinc", "-inline", "on,noauto", "-ipa", "file",
         "-sym", "on", "-nolink", "-msgstyle", "gcc"]

TABLE = "lockstep_ds_offsets"


def read_table(obj, count):
    data = open(obj, "rb").read()
    if data[:4] != b"\x7fELF" or data[4] != 1 or data[5] != 1:
        sys.exit("ds_offsets: %s is not a 32-bit little-endian ELF" % obj)
    shoff, = struct.unpack_from("<I", data, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", data, 0x2e)
    secs = [struct.unpack_from("<IIIIIIIIII", data, shoff + i * shentsize) for i in range(shnum)]
    for sec in secs:
        if sec[1] != 2:  # SHT_SYMTAB
            continue
        strtab = secs[sec[6]]
        for off in range(sec[4], sec[4] + sec[5], 16):
            name, value, size, info, other, shndx = struct.unpack_from("<IIIBBH", data, off)
            end = data.index(b"\0", strtab[4] + name)
            if data[strtab[4] + name:end].decode() != TABLE:
                continue
            base = secs[shndx][4] + value
            return list(struct.unpack_from("<%dI" % count, data, base))
    sys.exit("ds_offsets: no %s in %s" % (TABLE, obj))


def compute(exprs, headers=None, tu=None, root="."):
    lines = []
    if tu is not None:
        lines.append('#include "%s"' % tu)
    else:
        for h in headers or DEFAULT_HEADERS:
            lines.append('#include "%s"' % h)
    lines.append("#define OFF(T, m) ((unsigned long)&(((T *)0)->m))")
    lines.append("const unsigned long %s[] = {" % TABLE)
    for e in exprs:
        lines.append("    (unsigned long)(%s)," % e)
    lines.append("};")
    with tempfile.TemporaryDirectory() as tmp:
        src = os.path.join(tmp, "offs.c")
        obj = os.path.join(tmp, "offs.o")
        with open(src, "w") as f:
            f.write("\n".join(lines) + "\n")
        cmd = ["tools/wibo", "tools/mwccarm/dsi/1.1p1/mwccarm.exe"] + FLAGS
        for i in INCLUDES + ([] if tu is None else ["."]):
            cmd += ["-i", i]
        cmd += ["-o", obj, src]
        r = subprocess.run(cmd, cwd=root, capture_output=True, text=True)
        if r.returncode != 0 or not os.path.exists(obj):
            sys.exit("ds_offsets: mwccarm failed:\n" + r.stdout + r.stderr)
        return read_table(obj, len(exprs))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--header", action="append", help="a header to include (repeatable; default: vars.txt's set)")
    ap.add_argument("--tu", help="include this C file instead (for the structs it defines)")
    ap.add_argument("exprs", nargs="+", help="C constant expressions: OFF(T, m), sizeof(T)")
    a = ap.parse_args()
    for e, v in zip(a.exprs, compute(a.exprs, a.header, a.tu)):
        print("0x%-6x %s" % (v, e))


if __name__ == "__main__":
    main()
