#!/usr/bin/env python3
"""What a save holds, for route writers (numbers only: species and item NUMBERS, no names from the ROM):

    python3 tools/route.py GAME state SAVE          # zone + tile, badges, money, party (and moves), bag, story values
    python3 tools/route.py GAME diff SAVE_A SAVE_B  # what changed from A to B (works, flags, party, bag, money, badges, place)

SAVE is a name in the saves library (`w-route1`) or a path. The save's blocks are read at the offsets of tools/story.py's
SAVE_BLOCK / SAVE_BAG (the game's own structs: bag.h's BagSave, poke_party.h's PokeParty, trainer_game_info.h).
A Pokemon is stored as on the DS: the four 0x20 byte blocks (0x08..0x87) are shuffled by pid and encrypted with the
checksum as the seed, the party part (0x88..0xDB: level, HP) with the pid.

Gen 4 (Platinum: `route.py pt state SAVE`): the same Pokemon cipher and field layout, a party Pokemon is 0xEC bytes (party part
0x88..0xEB). The save is two 0x40000 slots (primary at 0, backup at 0x40000), each with a general block (the first save table
entries of pokeplatinum's src/savedata/save_table.c, each padded to 4 bytes + 4 bytes of its own checksum: SavePageInfo_Init) ending
in a 0x14 byte footer (savedata.h SaveBlockFooter: counters, size, signature 0x20060623, block id, CRC). The offsets in GEN4 were
summed from the table's sizeof()s (probed on the port's own objects) and agree with the known Platinum layout (party 0xA0, bag 0x630,
vars 0xDAC, footer 0xCF18); tools/route_tests.py builds a fixture save from them.
"""
import itertools
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import story  # noqa: E402

ORDERS = list(itertools.permutations(range(4)))   # the block order of shuffle value 0..23 (ABCD, ABDC ... DCBA)
HM_ITEMS = range(420, 426)                         # HM01-06 (the TM/HM pocket's item numbers)
SHOW = 10                                          # most changes of one kind a diff line lists
GEN4_SIGNATURE = 0x20060623                        # SECTOR_SIGNATURE: SaveBlockFooter.signature
# per Gen 4 game: the slot size, the general block's size (its footer is the last 0x14 bytes), then offsets in the block:
# player Location (mapHeaderID, warpId, x, z, face: ints), TrainerInfo (money u32, badge mask u8), party (s32 capacity, s32 count,
# 0xEC byte Pokemon), bag (BagItem {u16 item, u16 count}: pockets name, offset, slots), vars (u16 x nvars from id 0x4000), flags
GEN4 = {"pt": {"slot": 0x40000, "block": 0xCF2C, "loc": 0x1280, "money": 0x7C, "badges": 0x82, "party": 0x98, "pkm": 0xEC,
               "bag": 0x630, "vars": 0xDAC, "nvars": 288, "nflags": 2912,
               "pockets": (("items", 0, 165), ("key", 660, 50), ("tmhm", 860, 100), ("mail", 1260, 12), ("medicine", 1308, 40),
                           ("berries", 1468, 64), ("balls", 1724, 15), ("battle", 1784, 30))}}
HM_ITEMS4 = range(420, 428)                        # HM01-08 in Gen 4 (item numbers)


def _crypt(buf, start, end, seed):
    """the DS Pokemon cipher: each u16 of buf[start:end] xor the top half of the next LCG state"""
    for i in range(start, end, 2):
        seed = (seed * 0x41C64E6D + 0x6073) & 0xFFFFFFFF
        struct.pack_into("<H", buf, i, struct.unpack_from("<H", buf, i)[0] ^ (seed >> 16))


def decode_pkm(raw):
    """(species, level, hp, max hp, held item, ((move, PP) x4)) of a stored party Pokemon (Gen 5 PartyPkm, 0xDC bytes; Gen 4: 0xEC)"""
    buf = bytearray(raw)
    pid, _, checksum = struct.unpack_from("<IHH", buf, 0)
    _crypt(buf, 8, 0x88, checksum)
    _crypt(buf, 0x88, len(buf), pid)
    order = ORDERS[(pid >> 13 & 31) % 24]
    a, b = (8 + 0x20 * order.index(i) for i in (0, 1))   # where blocks A (species, item) and B (moves, PP) were shuffled to
    species, item = struct.unpack_from("<2H", buf, a)
    level, _, hp, maxhp = struct.unpack_from("<BBHH", buf, 0x8C)
    moves = tuple(zip(struct.unpack_from("<4H", buf, b), struct.unpack_from("<4B", buf, b + 8)))
    return species, level, hp, maxhp, item, moves


class Save:
    """a save's trainer state: place, badges, money, party [(species, level, hp, max hp, item)], bag {pocket: {item: n}}"""
    def __init__(self, game, data):
        self.hms = HM_ITEMS
        if game in GEN4:
            return self._gen4(game, data)
        self.st = story.State(game, data)
        blk, c = story.SAVE_BLOCK[game], self.st.copy
        self.zone, self.tile = self.st.zone, self.st.tile
        self.money, self.badge_bits = struct.unpack_from("<2I", data, c + blk["info"])
        self.badges = bin(self.badge_bits).count("1")
        count = struct.unpack_from("<i", data, c + blk["party"] + 4)[0]
        self.party = [decode_pkm(data[o:o + story.SAVE_PKM]) for o in
                      (c + blk["party"] + 8 + i * story.SAVE_PKM for i in range(max(0, min(count, 6))))]
        self.bag = {}
        for name, off, slots in story.SAVE_BAG:
            items = {}
            for i in range(slots):
                item, n = struct.unpack_from("<2H", data, c + blk["bag"] + off + 4 * i)
                if item and n:
                    items[item] = items.get(item, 0) + n
            self.bag[name] = items
        w = self.st.works
        self.works = {k: v for k, v in w.items() if v}
        self.flags = {i for i in range(self.st.nflags) if self.st.flags[i // 8] >> (i % 8) & 1}

    def _gen4(self, game, data):
        """the newer valid slot's general block (footer: size, signature, block id 0; larger save counter, then block counter)"""
        g = GEN4[game]
        best = None
        for slot in (0, g["slot"]):
            foot = slot + g["block"] - 0x14
            if len(data) < foot + 0x14:
                continue
            *counters, size, sig = struct.unpack_from("<4I", data, foot)
            bid = data[foot + 0x10]
            if size == g["block"] and sig == GEN4_SIGNATURE and bid == 0 and (best is None or counters > best[0]):
                best = (counters, slot)
        if best is None:
            raise ValueError("not a %s save" % game)
        c = best[1]
        self.hms = HM_ITEMS4
        self.st = None
        zone, _, x, z = struct.unpack_from("<4i", data, c + g["loc"])
        self.zone, self.tile = zone, (x, z)   # the player's Location: map header id, the matrix's tile x,z
        self.money = struct.unpack_from("<I", data, c + g["money"])[0]
        self.badge_bits = data[c + g["badges"]]
        self.badges = bin(self.badge_bits).count("1")
        count = struct.unpack_from("<i", data, c + g["party"] + 4)[0]
        self.party = [decode_pkm(data[o:o + g["pkm"]]) for o in (c + g["party"] + 8 + i * g["pkm"] for i in range(max(0, min(count, 6))))]
        self.bag = {}
        for name, off, slots in g["pockets"]:
            items = {}
            for i in range(slots):
                item, n = struct.unpack_from("<2H", data, c + g["bag"] + off + 4 * i)
                if item and n:
                    items[item] = items.get(item, 0) + n
            self.bag[name] = items
        nv = g["nvars"]
        self.works = {0x4000 + i: v for i, v in enumerate(struct.unpack_from("<%dH" % nv, data, c + g["vars"])) if v}
        fl = data[c + g["vars"] + 2 * nv: c + g["vars"] + 2 * nv + g["nflags"] // 8]
        self.flags = {i for i in range(g["nflags"]) if fl[i // 8] >> (i % 8) & 1}


def load(game, name):
    p = Path(name)
    if "/" not in name and not name.endswith(".bin"):
        import route
        p = route.SAVES / ("%s.bin" % name)
    return Save(game, p.read_bytes())


def pkm_text(p):
    return "#%d L%d %d/%d%s" % (p[0], p[1], p[2], p[3], " item %d" % p[4] if p[4] else "")


def bag_text(items, cap=14):
    out = ["%dx%d" % (i, n) for i, n in sorted(items.items())]
    return " ".join(out[:cap]) + (" +%d" % (len(out) - cap) if len(out) > cap else "")


def state_lines(game, name, s):
    zx, zz = s.tile
    out = ["%s %s: zone %d tile %d,%d | badges %d (bits 0x%02x) | money %d" % (game, name, s.zone, zx, zz, s.badges, s.badge_bits, s.money)]
    out.append("party %d: %s" % (len(s.party), ", ".join(pkm_text(p) for p in s.party) or "-"))
    for p in s.party:   # the battle pilot fights with the move that hurts most (docs/port-worker.md, IN A BATTLE)
        out.append("  #%d moves (move:PP): %s" % (p[0], " ".join("%d:%d" % m for m in p[5] if m[0]) or "-"))
    out.append("bag slots: " + " ".join("%s %d" % (k, len(v)) for k, v in s.bag.items()))
    for pocket in ("items", "medicine", "berries", "balls", "battle"):
        if s.bag.get(pocket):
            out.append("  %s (item x count): %s" % (pocket, bag_text(s.bag[pocket])))
    key = s.bag["key"]
    if key:
        out.append("  key items: %s" % " ".join(str(i) for i in sorted(key)))
    hms = sorted(i for i in s.bag["tmhm"] if i in s.hms)
    tms = sorted(i for i in s.bag["tmhm"] if i not in s.hms)
    out.append("  HMs held: %s | TMs held: %d" % (" ".join(map(str, hms)) or "none", len(tms)))
    out.append("story: %d works nonzero, %d flags set (diff shows what changed)" % (len(s.works), len(s.flags)))
    return out


def _set_diff(a, b, what, fmt):
    """a line for the keys that came, went or changed between dicts/sets a and b"""
    ka, kb = set(a), set(b)
    parts = ["+" + fmt(k, b[k] if isinstance(b, dict) else None, None) for k in sorted(kb - ka)]
    parts += ["-" + fmt(k, None, a[k] if isinstance(a, dict) else None) for k in sorted(ka - kb)]
    if isinstance(a, dict):
        parts += [fmt(k, b[k], a[k]) for k in sorted(ka & kb) if a[k] != b[k]]
    if not parts:
        return None
    return "%s (%d): %s%s" % (what, len(parts), "; ".join(parts[:SHOW]), "; +%d more" % (len(parts) - SHOW) if len(parts) > SHOW else "")


def diff_lines(game, na, sa, nb, sb):
    out = ["%s %s -> %s" % (game, na, nb)]
    if (sa.zone, sa.tile) != (sb.zone, sb.tile):
        out.append("place: zone %d %d,%d -> zone %d %d,%d" % ((sa.zone,) + sa.tile + (sb.zone,) + sb.tile))
    if sa.money != sb.money:
        out.append("money: %d -> %d (%+d)" % (sa.money, sb.money, sb.money - sa.money))
    if sa.badge_bits != sb.badge_bits:
        out.append("badges: %d -> %d (bits 0x%02x -> 0x%02x)" % (sa.badges, sb.badges, sa.badge_bits, sb.badge_bits))
    if sa.party != sb.party:
        out.append("party: %d: %s -> %d: %s" % (len(sa.party), ", ".join(map(pkm_text, sa.party)) or "-",
                                                 len(sb.party), ", ".join(map(pkm_text, sb.party)) or "-"))
    for pocket in sa.bag:
        line = _set_diff(sa.bag[pocket], sb.bag[pocket], "bag " + pocket,
                         lambda k, n, was: "%d x%s" % (k, n if was is None else "%d (was %d)" % (n, was)) if n is not None
                         else "%d (was x%d)" % (k, was))
        if line:
            out.append(line)
    line = _set_diff(sa.works, sb.works, "works", lambda k, n, was: "0x%04x = %d" % (k, n) if was is None and n is not None
                     else "0x%04x -> 0" % k if n is None else "0x%04x = %d (was %d)" % (k, n, was))
    line and out.append(line)
    fa, fb = sa.flags, sb.flags
    if fa != fb:
        def flags(sign, s):
            return "%s%d: %s%s" % (sign, len(s), " ".join("0x%04x" % f for f in sorted(s)[:SHOW]), " ..." if len(s) > SHOW else "") if s else ""
        out.append("flags " + " | ".join(x for x in (flags("+", fb - fa), flags("-", fa - fb)) if x))
    if len(out) == 1:
        out.append("no difference in place, money, badges, party, bag, works or flags")
    return out


def main(game, argv):
    """route.py's dispatch: argv = ["state", SAVE] or ["diff", A, B]; returns the exit code"""
    try:
        if argv[0] == "state" and len(argv) == 2:
            lines = state_lines(game, argv[1], load(game, argv[1]))
        elif argv[0] == "diff" and len(argv) == 3:
            lines = diff_lines(game, argv[1], load(game, argv[1]), argv[2], load(game, argv[2]))
        else:
            print("usage: route.py GAME state SAVE | route.py GAME diff SAVE_A SAVE_B")
            return 2
    except (OSError, ValueError, struct.error) as e:
        print("route.py %s %s: %s" % (game, argv[0], e))
        return 1
    print("\n".join(lines))
    return 0


if __name__ == "__main__":
    if len(sys.argv) < 3:
        sys.exit("usage: route_state.py GAME state SAVE | GAME diff A B")
    sys.exit(main(sys.argv[1], sys.argv[2:]))
