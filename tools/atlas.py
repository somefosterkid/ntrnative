#!/usr/bin/env python3
"""The world atlas of a Gen 5 game, read from the user's ROM: every zone's header, map matrix chunks and event data, and
the connection graph (warps, and the edges where a map matrix walks on into the neighbour chunk's zone).

    python3 tools/atlas.py GAME build            # build/atlas/GAME.json from orig/baserom_GAME_us.nds (GAME: b w b2 w2)
    python3 tools/atlas.py GAME zone N           # one zone: header numbers, chunks, warps, actors, triggers, BG events
    python3 tools/atlas.py GAME path A B         # the shortest zone chain A -> B by warps and matrix edges, tiles used
    python3 tools/atlas.py GAME find npc S       # actors with script S (trainer [S], item [S]: trainers / items; flag F:
                                                 # actors, BG events and triggers that test flag or work F)
The other commands build the JSON first when it is missing or older than the ROM or this file. Numbers only: no game
text (a zone's name is its place-name id), and the JSON is build output, never committed.

The formats are the ones the decompiled loaders read (src/ for Black, third_party/pokebw2 for Black 2):
- the ROM's file system (GBATEK "DS Cartridge NitroROM"): the FNT names the archives, a/X/Y/Z for archive id XYZ
  (src/system/gfl_use.c's table); each archive a NARC.
- zone headers: archive 12, file 0, one 0x30-byte ZoneData per zone (include/field/zone.h; ZoneData_GetHeader reads
  zoneId * 0x30): matrixId at 0x04, scriptDatId 0x06, textDatId 0x0a, encId 0x14 (13 bits in Black 2), entitiesId 0x16,
  parentZone 0x18, placeNameId the low 10 bits of 0x1a.
- map matrices: archive 9, file matrixId (ProcessMapMatrix, src/ov010/map_matrix.c): u16 format, u16, u16 width,
  u16 height, u32 chunk ids[w*h], and with format 1 a u32 zone id per chunk (format 0: every chunk is the zone that loads
  it). A chunk is 32 x 32 tiles; tile (x, z) is in chunk (x // 32, z // 32).
- zone entities: archive 125 (Black, White) / 126 (Black 2, White 2), file entitiesId (LoadZoneEntities,
  src/ov010/eventdata_system.c): u32 init script offset (from the counts), u8 counts of BG events, actors, warps,
  triggers, then the ZoneBGEntity (0x14), ZoneNPC (0x24), ZoneWarp (0x14) and ZoneTrigger (0x16) records.
Kinds from the code: an actor is a trainer when its evType is 1 or one of fldmmdl.c's func_ov010_0216e434 types, and its
sight range is its param0 (event_trainer_eye.c, GetActorUserParam(actor, 0)). Items go by script number (Gen 5's script
ranges): an actor with a script 7000-7999 is an item ball, a BG event with 8000-8999 a hidden item.
Matrix edges keep only the sides with a tile walkable on both sides (chunk_walk: collision and water flags, no heights,
ledges or side walls). Not read (TODO): a per-tile walk grid (the live dump, route.py --map, has it), init scripts,
map_replace's story patches of the matrices, rail zones' positions beyond their raw rail numbers.
"""
import json
import os
import struct
import sys
from collections import deque
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from rom_leak_check import narc_members  # noqa: E402

GAMES = {"b": 125, "w": 125, "b2": 126, "w2": 126}   # the zone entities' archive per game
ARC_ZONEDATA, ARC_MATRIX, ARC_TERRAIN = 12, 9, 8
TRAINER_TYPES = {1, 4, 5, 6, 7, 8, 11, 12, 13}
NONE16 = 0xFFFF


def fnt_file_id(rom, path):
    """The file id of a path in the ROM's file name table."""
    fnt = struct.unpack_from("<I", rom, 0x40)[0]
    d = 0
    for i, part in enumerate(path.split("/")):
        sub, fid = struct.unpack_from("<IH", rom, fnt + 8 * d)
        p = fnt + sub
        while True:
            n = rom[p]
            if n == 0:
                raise KeyError(path)
            is_dir, ln = n & 0x80, n & 0x7F
            name = rom[p + 1:p + 1 + ln].decode("ascii")
            p += 1 + ln
            if is_dir:
                sub_id = struct.unpack_from("<H", rom, p)[0] & 0xFFF
                p += 2
                if name == part and i < len(path.split("/")) - 1:
                    d = sub_id
                    break
            else:
                if name == part and i == len(path.split("/")) - 1:
                    return fid
                fid += 1


def rom_file(rom, fid):
    fat = struct.unpack_from("<I", rom, 0x48)[0]
    start, stop = struct.unpack_from("<II", rom, fat + 8 * fid)
    return rom[start:stop]


def arc(rom, arc_id):
    """An archive's members: a/X/Y/Z for archive id XYZ."""
    return narc_members(rom_file(rom, fnt_file_id(rom, "a/" + "/".join("%03d" % arc_id))))


def parse_header(b, zone):
    (map_type, _, area, matrix, script, _, text, _, _, _, _, enc, ent, parent, name) = struct.unpack_from(
        "<BBHHHHHHHHHHHHH", b, 0)
    return {"zone": zone, "matrix": matrix, "script": script, "text": text, "enc": enc & 0x1FFF, "entities": ent,
            "parent": parent, "name": name & 0x3FF, "type": map_type, "area": area}


def parse_entities(b):
    """(bg, npcs, warps, triggers) of a zone entities file, and whether its sizes add up to its init script offset."""
    init_off = struct.unpack_from("<I", b, 0)[0]
    n_bg, n_npc, n_warp, n_trig = b[4:8]
    p = 8
    bg, npcs, warps, trigs = [], [], [], []
    for _ in range(n_bg):
        scr, cond, dirn, rail = struct.unpack_from("<4H", b, p)
        e = {"scr": scr, "flag": cond, "dir": dirn}
        if rail:
            e["rail"] = list(struct.unpack_from("<HHh", b, p + 8))
        else:
            e["x"], e["z"], e["y"] = struct.unpack_from("<3i", b, p + 8)   # tiles; y in 1/16 tiles
        if 8000 <= scr < 9000:
            e["item"] = 1
        bg.append(e)
        p += 0x14
    for _ in range(n_npc):
        (uid, model, move, evt, flag, scr, dirn, p0, p1, p2, aw, ah, rail) = struct.unpack_from("<12HI", b, p)
        e = {"uid": uid, "model": model, "move": move, "ev": evt, "flag": flag, "scr": scr, "dir": dirn,
             "params": [p0, p1, p2], "area": [aw, ah]}
        if rail:
            e["rail"] = list(struct.unpack_from("<HHh", b, p + 28))
        else:
            e["x"], e["z"], e["y"] = struct.unpack_from("<HHi", b, p + 28)
        if evt in TRAINER_TYPES:
            e["trainer"] = 1
            e["sight"] = p0
        if 7000 <= scr < 8000:
            e["item"] = 1
        npcs.append(e)
        p += 0x24
    for i in range(n_warp):
        dz, dw, dirn, trans, rail = struct.unpack_from("<HHBBH", b, p)
        e = {"i": i, "to": dz, "to_warp": dw, "dir": dirn, "trans": trans}
        if rail:
            e["rail"] = list(struct.unpack_from("<HHh", b, p + 8))
        else:
            x, y, z, e["w"], e["h"] = struct.unpack_from("<hhhHH", b, p + 8)   # 1/16 tiles, the size in tiles
            e["x"], e["y"], e["z"] = x >> 4, y, z >> 4
        warps.append(e)
        p += 0x14
    for i in range(n_trig):
        scr, val, work, typ, rail = struct.unpack_from("<5H", b, p)
        e = {"i": i, "scr": scr, "work": work, "value": val, "type": typ}
        if rail:
            e["rail"] = list(struct.unpack_from("<HHhHH", b, p + 10))
        else:
            e["x"], e["z"], e["w"], e["h"], e["y"] = struct.unpack_from("<HHHHh", b, p + 10)
        trigs.append(e)
        p += 0x16
    return bg, npcs, warps, trigs, p == 4 + init_off


def parse_matrix(b):
    fmt, _, w, h = struct.unpack_from("<4H", b, 0)
    chunks = list(struct.unpack_from("<%dI" % (w * h), b, 8))
    zones = list(struct.unpack_from("<%dI" % (w * h), b, 8 + 4 * w * h)) if fmt == 1 else None
    return {"format": fmt, "w": w, "h": h, "chunks": chunks, "zones": zones}


def build(rom, ent_arc):
    hdr = arc(rom, ARC_ZONEDATA)[0]
    matrices = [parse_matrix(m) for m in arc(rom, ARC_MATRIX)]
    ents = arc(rom, ent_arc)
    zones, bad = [], 0
    for z in range(len(hdr) // 0x30):
        h = parse_header(hdr[z * 0x30:z * 0x30 + 0x30], z)
        bg, npcs, warps, trigs, ok = parse_entities(ents[h["entities"]])
        bad += not ok
        m = matrices[h["matrix"]] if h["matrix"] < len(matrices) else None
        chunks = []
        if m is not None:
            for i, c in enumerate(m["chunks"]):
                if c != 0xFFFFFFFF and (m["zones"] is None or m["zones"][i] == z):
                    chunks.append([i % m["w"], i // m["w"]])
        h.update(chunks=chunks, bg=bg, npcs=npcs, warps=warps, triggers=trigs)
        zones.append(h)
    terrain, walks = arc(rom, ARC_TERRAIN), {}

    def walk(cid):
        if cid not in walks:
            walks[cid] = chunk_walk(terrain[cid]) if cid < len(terrain) else None
        return walks[cid]
    edges = {}
    for m in matrices:
        if m["zones"] is None:
            continue
        w, hgt, zs = m["w"], m["h"], m["zones"]
        for cz in range(hgt):
            for cx in range(w):
                a = zs[cz * w + cx]
                for dx, dz in ((1, 0), (0, 1)):
                    nx, nz = cx + dx, cz + dz
                    if nx >= w or nz >= hgt:
                        continue
                    b = zs[nz * w + nx]
                    if a == b or a >= len(zones) or b >= len(zones) or \
                            0xFFFFFFFF in (m["chunks"][cz * w + cx], m["chunks"][nz * w + nx]):
                        continue
                    # the tiles along the side walkable on both sides of it (unknown terrain: all of them, unchecked)
                    ta, tb = walk(m["chunks"][cz * w + cx]), walk(m["chunks"][nz * w + nx])
                    if ta is None or tb is None:
                        open_ = list(range(32))
                    else:
                        open_ = [i for i in range(32) if
                                 ((ta[i * 32 + 31] and tb[i * 32]) if dx else (ta[31 * 32 + i] and tb[i]))]
                    if not open_:
                        continue
                    lo, hi = open_[0], open_[-1]
                    # the line crossed: x (or z) of the last tile in a, the walkable range along the side
                    if dx:
                        seg = {"x": nx * 32 - 1, "z": [cz * 32 + lo, cz * 32 + hi], "dir": "right", "open": len(open_)}
                        back = dict(seg, x=nx * 32, dir="left")
                    else:
                        seg = {"z": nz * 32 - 1, "x": [cx * 32 + lo, cx * 32 + hi], "dir": "down", "open": len(open_)}
                        back = dict(seg, z=nz * 32, dir="up")
                    if ta is None or tb is None:
                        seg["unchecked"] = back["unchecked"] = 1
                    edges.setdefault("%d>%d" % (a, b), []).append(seg)
                    edges.setdefault("%d>%d" % (b, a), []).append(back)
    return {"zones": zones, "edges": edges, "matrices": len(matrices), "entity_size_mismatch": bad}


def chunk_walk(b):
    """32 * 32 walkable flags (row-major, z then x) of a chunk file's first terrain layer, or None for a kind without
    one read here. WB (0x4257) and GC (0x4347) chunks: the terrain offset at 8, then u16 width, u16 height and an 8-byte
    MapHeightTile per tile whose u32 at 4 is the tile type (31 bits); a tile blocks when map_attr.c's MapTile_IsValid
    fails (class 0xff) or its flags (the high half) have bit 0 (collision) or bit 1 (the fill's water) set."""
    if len(b) < 12 or struct.unpack_from("<H", b, 0)[0] not in (0x4257, 0x4347):
        return None
    off = struct.unpack_from("<I", b, 8)[0]
    w, h = struct.unpack_from("<HH", b, off)
    if (w, h) != (32, 32):
        return None
    out = []
    for i in range(32 * 32):
        t = struct.unpack_from("<I", b, off + 4 + 8 * i + 4)[0] & 0x7FFFFFFF
        out.append(t & 0xFFFF != 0xFF and not (t >> 16) & 3)
    return out


def atlas_path(game):
    return ROOT / "build" / "atlas" / ("%s.json" % game)


def load(game, rebuild=False):
    rom_path = ROOT / "orig" / ("baserom_%s_us.nds" % game)
    out = atlas_path(game)
    if not rebuild and out.exists() and out.stat().st_mtime > max(rom_path.stat().st_mtime, Path(__file__).stat().st_mtime):
        return json.loads(out.read_text())
    data = build(rom_path.read_bytes(), GAMES[game])
    data["game"] = game
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps(data, separators=(",", ":")))
    return data


def tile(e):
    return "rail%s" % e["rail"] if "rail" in e else "%d,%d" % (e["x"], e["z"])


def neighbours(data, z):
    """(next zone, how) for each warp and matrix edge out of zone z."""
    for w in data["zones"][z]["warps"]:
        if w["to"] < len(data["zones"]):
            yield w["to"], "warp %d at %s -> warp %d" % (w["i"], tile(w), w["to_warp"])
    for key, segs in data["edges"].items():
        a, b = map(int, key.split(">"))
        if a == z:
            s = segs[len(segs) // 2]
            line = "x=%d z=%d-%d" % (s["x"], *s["z"]) if "x" in s and isinstance(s["x"], int) else \
                "z=%d x=%d-%d" % (s["z"], *s["x"])
            yield b, "edge %s %s (%d open tiles%s)" % (s["dir"], line, sum(x["open"] for x in segs),
                                                     ", terrain unchecked" if "unchecked" in s else "")


def path(data, a, b):
    prev = {a: None}
    q = deque([a])
    while q:
        z = q.popleft()
        if z == b:
            break
        for n, how in neighbours(data, z):
            if n not in prev:
                prev[n] = (z, how)
                q.append(n)
    if b not in prev:
        return None
    steps = []
    while prev[b] is not None:
        z, how = prev[b]
        steps.append((z, how, b))
        b = z
    return steps[::-1]


def show_zone(data, n):
    z = data["zones"][n]
    out = ["zone %d: matrix %d entities %d script %d text %d enc %d parent %d name %d type %d" % (
        n, z["matrix"], z["entities"], z["script"], z["text"], z["enc"], z["parent"], z["name"], z["type"])]
    if z["chunks"]:
        xs, zs = [c[0] for c in z["chunks"]], [c[1] for c in z["chunks"]]
        out.append("  chunks %d: x %d-%d z %d-%d (tiles x %d-%d z %d-%d)" % (
            len(z["chunks"]), min(xs), max(xs), min(zs), max(zs), min(xs) * 32, max(xs) * 32 + 31, min(zs) * 32,
            max(zs) * 32 + 31))
    for w in z["warps"]:
        out.append("  W%d %s -> zone %d warp %d (dir %d trans %d)" % (w["i"], tile(w), w["to"], w["to_warp"], w["dir"],
                                                                     w["trans"]))
    for e in z["npcs"]:
        kind = " trainer sight %d" % e["sight"] if "trainer" in e else " item" if "item" in e else ""
        out.append("  uid %d %s model %d move %d ev %d scr %d flag %d dir %d%s" % (
            e["uid"], tile(e), e["model"], e["move"], e["ev"], e["scr"], e["flag"], e["dir"], kind))
    for t in z["triggers"]:
        out.append("  T%d %s %dx%d scr %d work %d == %d type %d" % (
            t["i"], tile(t), t.get("w", 0), t.get("h", 0), t["scr"], t["work"], t["value"], t["type"]))
    for i, e in enumerate(z["bg"]):
        out.append("  b%d %s scr %d flag %d dir %d%s" % (i, tile(e), e["scr"], e["flag"], e["dir"],
                                                        " hidden item" if "item" in e else ""))
    for n2, how in neighbours(data, n):
        if how.startswith("edge"):
            out.append("  -> zone %d %s" % (n2, how))
    return "\n".join(out)


def find(data, what, x):
    out = []
    for z in data["zones"]:
        for e in z["npcs"]:
            if (what == "npc" and e["scr"] == x) or (what in ("trainer", "item") and what in e and x in (None, e["scr"])) \
                    or (what == "flag" and e["flag"] == x):
                out.append("zone %d uid %d %s scr %d flag %d%s" % (z["zone"], e["uid"], tile(e), e["scr"], e["flag"],
                                                                   " sight %d" % e["sight"] if "sight" in e else ""))
        for i, e in enumerate(z["bg"]):
            if (what == "item" and "item" in e and x in (None, e["scr"])) or (what == "flag" and e["flag"] == x):
                out.append("zone %d b%d %s scr %d flag %d" % (z["zone"], i, tile(e), e["scr"], e["flag"]))
        for t in z["triggers"]:
            if what == "flag" and t["work"] == x:
                out.append("zone %d T%d %s scr %d work %d == %d" % (z["zone"], t["i"], tile(t), t["scr"], t["work"],
                                                                    t["value"]))
    return out


def main(argv):
    if len(argv) < 2 or argv[0] not in GAMES:
        sys.exit(__doc__)
    game, cmd, args = argv[0], argv[1], [int(a, 0) for a in argv[2:] if a.lstrip("-").isdigit() or a.startswith("0x")]
    data = load(game, rebuild=cmd == "build")
    if cmd == "build":
        print("%s: %d zones, %d matrices, %d edges, %d entity files whose sizes don't add up -> %s" % (
            game, len(data["zones"]), data["matrices"], len(data["edges"]), data["entity_size_mismatch"],
            atlas_path(game).relative_to(ROOT)))
    elif cmd == "zone":
        print(show_zone(data, args[0]))
    elif cmd == "path":
        steps = path(data, args[0], args[1])
        if steps is None:
            sys.exit("no path from zone %d to zone %d" % (args[0], args[1]))
        for a, how, b in steps:
            print("%d -> %d: %s" % (a, b, how))
    elif cmd == "find":
        res = find(data, argv[2], args[0] if args else None)
        print("\n".join(res[:40]) + ("\n... %d in all" % len(res) if len(res) > 40 else ""))
    else:
        sys.exit(__doc__)


if __name__ == "__main__":
    main(sys.argv[1:])
