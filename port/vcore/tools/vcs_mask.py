#!/usr/bin/env python3
"""A .vcs frame with only some layers and no effects: isolates one piece's work on a real frame, for both port/vcore
and the oracle (docs/vcore.md, "Done-check").

    python3 port/vcore/tools/vcs_mask.py IN.vcs OUT.vcs [--only LAYERS] [--plain]
    python3 port/vcore/tools/vcs_mask.py IN_DIR OUT_DIR [--only LAYERS] [--plain] [--feature F]

--only LAYERS   keep these layers on both engines, turn the others off in DISPCNT: a comma list of bg0 bg1 bg2 bg3
                obj 3d (3d = engine A's BG0 when it is the 3D layer; bg0 = a 2D BG0)
--plain         no windows, colour effects, mosaic, master brightness or capture; display mode 1 when it was 2/3
--feature F     DIR mode: only the frames whose features (vcs_features.py) include F
The other chunks (VRAM, palettes, OAM, the 3D stream, LSHT) are copied as they are; INFO gets a mask= line.
"""
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import vcs_features  # noqa: E402


def mask(src, dst, only, plain):
    d = bytearray(Path(src).read_bytes())
    if d[:4] != b"VCST":
        sys.exit(f"{src}: not a .vcs file")
    at = 8
    out = bytearray(d[:8])
    while at + 8 <= len(d):
        tag = bytes(d[at:at + 4])
        n = struct.unpack_from("<I", d, at + 4)[0]
        body = bytearray(d[at + 8:at + 8 + n])
        at = (at + 8 + n + 3) & ~3
        if tag in (b"IO9 ", b"IO9B"):
            eng_a = tag == b"IO9 "
            dc = struct.unpack_from("<I", body, 0)[0]
            if only is not None:
                keep = 0
                is3d = eng_a and dc & 8
                for i, name in enumerate(("bg0", "bg1", "bg2", "bg3", "obj")):
                    want = name in only
                    if i == 0 and is3d:
                        want = "3d" in only
                    if want:
                        keep |= 1 << (8 + i)
                dc &= ~(0x1F << 8) | keep
            if plain:
                dc &= ~(7 << 13)  # windows
                if (dc >> 16) & 3 in (2, 3):
                    dc = (dc & ~(3 << 16)) | 1 << 16
                struct.pack_into("<H", body, 0x50, 0)  # BLDCNT
                struct.pack_into("<H", body, 0x4C, 0)  # MOSAIC
                struct.pack_into("<H", body, 0x6C, 0)  # MASTER_BRIGHT
                if eng_a:
                    struct.pack_into("<I", body, 0x64, 0)  # DISPCAPCNT
            struct.pack_into("<I", body, 0, dc)
        elif tag == b"INFO":
            body += f"mask=only {','.join(sorted(only)) if only else 'all'} plain {int(plain)}\n".encode()
        out += tag + struct.pack("<I", len(body)) + body + b"\0" * (-len(body) % 4)
    Path(dst).write_bytes(out)


def main():
    a = sys.argv[1:]
    if len(a) < 2:
        sys.exit(__doc__)
    src, dst, rest = Path(a[0]), Path(a[1]), a[2:]
    only, plain, feature = None, False, None
    i = 0
    while i < len(rest):
        if rest[i] == "--only":
            only = set(rest[i + 1].split(","))
            i += 2
        elif rest[i] == "--plain":
            plain = True
            i += 1
        elif rest[i] == "--feature":
            feature = rest[i + 1]
            i += 2
        else:
            sys.exit(__doc__)
    if src.is_dir():
        dst.mkdir(parents=True, exist_ok=True)
        n = 0
        for f in sorted(src.glob("*.vcs")):
            if feature and feature not in vcs_features.features(f):
                continue
            mask(f, dst / f.name, only, plain)
            n += 1
        print(f"vcs_mask: {n} frames into {dst}")
    else:
        mask(src, dst, only, plain)


if __name__ == "__main__":
    main()
