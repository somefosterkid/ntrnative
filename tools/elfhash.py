#!/usr/bin/env python3
"""elfhash.py: a hash of the code and data an ELF executable runs, not of how it was built (tb4 11-20, arch1 B4).

Two builds of one source differ in the debug sections (the build directory's path is in .debug_str/.debug_line_str when
one object came from ccache with another slot's path) and so in the GNU build id (a hash of them). A leg's result depends
on none of that. The hash covers the ELF header's identity bytes and every section except .debug*, .note.gnu.build-id and
.gnu_debuglink: name, type, flags, address, and the bytes (NOBITS: the size). The section HEADER TABLE's file offsets are
not in it (they shift with the debug sections' sizes).

  elfhash.py EXE [--sections]     prints the sha256 (--sections: also one line per section: name size hash)
"""
import hashlib
import struct
import sys

SKIP = (".debug", ".note.gnu.build-id", ".gnu_debuglink", ".comment")


def sections(data):
    """[(name, type, flags, addr, offset, size)] of a 64-bit little-endian ELF"""
    if data[:4] != b"\x7fELF" or data[4] != 2 or data[5] != 1:
        raise ValueError("not a 64-bit little-endian ELF")
    shoff, = struct.unpack_from("<Q", data, 0x28)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", data, 0x3A)
    raw = [struct.unpack_from("<IIQQQQIIQQ", data, shoff + i * shentsize) for i in range(shnum)]
    stroff = raw[shstrndx][4]
    out = []
    for name, typ, flags, addr, off, size, *_ in raw:
        end = data.index(b"\0", stroff + name)
        out.append((data[stroff + name:end].decode(), typ, flags, addr, off, size))
    return out


def elf_hash(data, listing=None):
    h = hashlib.sha256()
    h.update(data[:16])
    for name, typ, flags, addr, off, size in sections(data):
        if name.startswith(SKIP):
            continue
        body = b"" if typ == 8 else data[off:off + size]   # 8 = SHT_NOBITS
        h.update(f"{name}\0{typ}\0{flags}\0{addr}\0{size}\0".encode())
        h.update(body)
        if listing is not None:
            listing.append(f"{name} {size} {hashlib.sha256(body).hexdigest()[:12]}")
    return h.hexdigest()


def main(argv):
    if not argv or argv[0].startswith("-"):
        print(__doc__)
        return 2
    with open(argv[0], "rb") as f:
        data = f.read()
    ls = [] if "--sections" in argv else None
    print(elf_hash(data, ls))
    for l in ls or []:
        print(l)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
