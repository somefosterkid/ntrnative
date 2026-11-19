#!/usr/bin/env python3
"""The Gen 4 reader of tools/atlas.py (Platinum: pt; HeartGold / SoulSilver: hg, ss): the same atlas JSON as Gen 5's, read
from the user's ROM, so `atlas.py pt zone N`, `atlas.py pt path A B` and route.py's G:zone expansion work unchanged.
Zone numbers are the map header ids (pret: Platinum's enum MapHeaderID, include/constants/fieldmap.h; HGSS's MAP_*,
include/constants/maps.h).

The formats are the ones pret's loaders read (third_party/pokeplatinum; pokeheartgold for HGSS):
- map headers: the const sMapHeaders table in the ARM9 (src/map_header.c; HGSS src/data/map_headers.h), 0x18 bytes each.
  No pointer leads to it, so header_table finds it: the longest run of 0x18-byte records whose matrix, script, init script
  and events ids are all in their archives' ranges. Platinum: matrix 0x02, scripts 0x04, init scripts 0x06, text 0x08,
  encounters 0x0e, events 0x10, place name (mapLabelTextID) the low byte of 0x12, mapType the low 7 bits of 0x16.
  HGSS: encounters 0x00 (u8), matrix 0x04, scripts 0x06, script header 0x08, text 0x0a, events 0x10, mapsec the low
  byte of 0x12, mapType bits 8-11 of the u32 at 0x14.
- map matrices (src/map_matrix.c MapMatrixData_Load): u8 width, height, has-header-ids, has-altitudes, name length, the
  name, [u16 header id per chunk], [u8 altitude per chunk], u16 land data id per chunk (0xFFFF: none). Without the
  header ids every chunk is the zone that loads the matrix. Chunks are 32 x 32 tiles; event coordinates are matrix-wide
  tiles (outdoors: the world's), tile (x, z) in chunk (x // 32, z // 32).
- zone events (src/map_header_data.c; HGSS src/map_events.c): u32 count + records for BG events (0x14: script, type,
  x, z, y, facing), object events (0x20: localID, graphics, movement, trainerType, hiddenFlag, script, dir, data[3],
  rangeX, rangeZ, x, z, fx32 y), warps (0x0c: x, z, dest header, dest warp, y) and coord triggers (0x10: script, x, z,
  width, length, y, value, var).
- land data's terrain attributes (src/terrain_attributes.c): 32 x 32 u16 at 0x10 (HGSS 0x14) of the chunk's file, bit 15
  the collision bit (chunk_walk: water is not excluded, unlike Gen 5's).
Kinds: an object is a trainer when its trainerType is not 0, its sight data[0] (as Gen 5's param0: pret's events.json
gives the Route 202 trainers data [5]); items by script number as Gen 5 (an object 7000-7999 an item ball, a BG event
8000-8999 a hidden item). Warps carry no direction or transition (Gen 4 derives them from the tile behavior): w = h = 1.
Not read (TODO): HGSS's runtime matrix swaps (map_matrix.c: Lake of Rage water, Safari Zone areas), heights (BDHC).
For tools/story.py (J3): each zone's "script" (zone scripts) and "init" (init scripts / HGSS script header) are member
ids of the scr_seq archive (GEN4[game]["scripts"]); "text" a member of the message archive.
"""
import struct

from rom_leak_check import narc_members
from rom_modules import arm9

NONE16 = 0xFFFF
# archive paths in the ROM's file system (HGSS names its archives a/X/Y/Z: include/filesystem_files_def.h's NARC_* ids)
GEN4 = {
    "pt": {"rom": "plat", "matrix": "fielddata/mapmatrix/map_matrix.narc", "events": "fielddata/eventdata/zone_event.narc",
           "scripts": "fielddata/script/scr_seq.narc", "land": "fielddata/land_data/land_data.narc", "attr": 0x10,
           "hdr": "pt"},
    "hg": {"rom": "hg", "matrix": "a/0/4/1", "events": "a/0/3/2", "scripts": "a/0/1/2", "land": "a/0/6/5", "attr": 0x14,
           "hdr": "hg"},
}
GEN4["ss"] = dict(GEN4["hg"], rom="ss")
# MapHeader fields: (matrix, scripts, init, text, events) u16 offsets
HDR = {"pt": (2, 4, 6, 8, 0x10), "hg": (4, 6, 8, 0xA, 0x10)}


def parse_header4(b, kind, zone):
    mo, so, io, to, eo = HDR[kind]
    g = lambda o: struct.unpack_from("<H", b, o)[0]   # noqa: E731
    if kind == "pt":
        enc, typ = g(0xE), g(0x16) & 0x7F
    else:
        enc, typ = b[0], struct.unpack_from("<I", b, 0x14)[0] >> 8 & 0xF
    return {"zone": zone, "matrix": g(mo), "script": g(so), "init": g(io), "text": g(to), "enc": enc,
            "entities": g(eo), "parent": NONE16, "name": g(0x12) & 0xFF, "type": typ, "area": b[0] if kind == "pt" else b[1]}


def header_table(image, kind, n_matrix, n_events, n_scripts):
    """(offset, count) of the longest run of 0x18-byte records in image whose ids fit their archives."""
    mo, so, io, _, eo = HDR[kind]
    best = (0, 0)
    for phase in range(0, 0x18, 2):
        n = (len(image) - phase) // 0x18
        run = 0
        for i, r in enumerate(struct.iter_unpack("<12H", image[phase:phase + n * 0x18])):
            if r[mo // 2] < n_matrix and r[eo // 2] < n_events and r[so // 2] < n_scripts and r[io // 2] < n_scripts:
                run += 1
                if run > best[1]:
                    best = (phase + (i + 1 - run) * 0x18, run)
            else:
                run = 0
    return best


def parse_matrix4(b):
    """the Gen 5 parse_matrix shape: chunks (0xFFFFFFFF: none), zones per chunk (None: all the loading zone's)."""
    w, h, has_ids, has_alt, nlen = b[:5]
    p = 5 + nlen
    zones = None
    if has_ids:
        zones = list(struct.unpack_from("<%dH" % (w * h), b, p))
        p += 2 * w * h
    if has_alt:
        p += w * h
    chunks = [0xFFFFFFFF if c == NONE16 else c for c in struct.unpack_from("<%dH" % (w * h), b, p)]
    return {"format": int(bool(has_ids)), "w": w, "h": h, "chunks": chunks, "zones": zones}


def parse_events4(b):
    """(bg, npcs, warps, triggers, sizes add up) of a zone events file."""
    p = 0
    out = []
    for size in (0x14, 0x20, 0x0C, 0x10):
        n = struct.unpack_from("<I", b, p)[0]
        out.append([b[p + 4 + size * i:p + 4 + size * (i + 1)] for i in range(n)])
        p += 4 + size * n
    bg, npcs, warps, trigs = [], [], [], []
    for r in out[0]:
        scr, typ, x, z, y, dirn = struct.unpack_from("<HH3iH", r)
        e = {"scr": scr, "type": typ, "flag": NONE16, "dir": dirn, "x": x, "z": z, "y": y}
        if 8000 <= scr < 9000:
            e["item"] = 1
        bg.append(e)
    for r in out[1]:
        (uid, model, move, ttype, flag, scr, dirn, p0, p1, p2, rx, rz, x, z, y) = struct.unpack_from("<6Hh3Hhh2Hi", r)
        e = {"uid": uid, "model": model, "move": move, "ev": ttype, "flag": flag, "scr": scr, "dir": dirn,
             "params": [p0, p1, p2], "area": [rx, rz], "x": x, "z": z, "y": y}
        if ttype:
            e["trainer"] = 1
            e["sight"] = p0
        if 7000 <= scr < 8000:
            e["item"] = 1
        npcs.append(e)
    for i, r in enumerate(out[2]):
        x, z, to, tw, y = struct.unpack_from("<4HI", r)
        warps.append({"i": i, "to": to, "to_warp": tw, "x": x, "z": z, "y": y, "w": 1, "h": 1})
    for i, r in enumerate(out[3]):
        scr, x, z, w, h, y, val, var = struct.unpack_from("<H2h5H", r)
        trigs.append({"i": i, "scr": scr, "work": var, "value": val, "x": x, "z": z, "w": w, "h": h, "y": y})
    return bg, npcs, warps, trigs, p == len(b) or not any(b[p:])


def chunk_walk4(b, attr):
    """the 32 x 32 tiles of a land data file, True where the collision bit is clear (None: not a land data file)."""
    if len(b) < attr + 0x800:
        return None
    return [not t >> 15 for t in struct.unpack_from("<1024H", b, attr)]


def build4(rom, game, arc_path, matrix_edges):
    """the atlas of a Gen 4 ROM. arc_path(rom, path): an archive's members; matrix_edges: atlas.matrix_edges."""
    g = GEN4[game]
    raw = [arc_path(rom, g[k]) for k in ("matrix", "events", "scripts", "land")]
    mats, ents, scripts, land = raw
    image = arm9(rom)
    off, n = header_table(image, g["hdr"], len(mats), len(ents), len(scripts))
    zones, bad, matrices = [], 0, {}
    for z in range(n):
        h = parse_header4(image[off + 0x18 * z:off + 0x18 * (z + 1)], g["hdr"], z)
        bg, npcs, warps, trigs, ok = parse_events4(ents[h["entities"]])
        bad += not ok
        key = h["matrix"]
        if key not in matrices:
            matrices[key] = parse_matrix4(mats[key])
        m = matrices[key]
        chunks = [[i % m["w"], i // m["w"]] for i, c in enumerate(m["chunks"])
                  if c != 0xFFFFFFFF and (m["zones"] is None or m["zones"][i] == z)]
        h.update(chunks=chunks, bg=bg, npcs=npcs, warps=warps, triggers=trigs)
        zones.append(h)
    walks = {}

    def walk(cid):
        if cid not in walks:
            walks[cid] = chunk_walk4(land[cid], g["attr"]) if cid < len(land) else None
        return walks[cid]
    edges = matrix_edges(list(matrices.values()), len(zones), walk)
    return {"zones": zones, "edges": edges, "matrices": len(mats), "entity_size_mismatch": bad,
            "header_table": [off, n]}
