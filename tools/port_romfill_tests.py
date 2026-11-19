#!/usr/bin/env python3
"""Tests for tools/port_romfill.py's cache and object lookup, and for the coverage of the game data the build fills from the
ROMs (every HeartGold/SoulSilver blank has a row; the fill gate waits for every filled game's ROM), no ROM needed:
python3 tools/port_romfill_tests.py (prints "ok" per test, exits 1 on a failure)"""
import os
import random
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, HERE)
import port_romfill as R  # noqa: E402

OPEN = set()   # the rev1 review's items not fixed yet (none: item 2 fixed by lcfix1): their tests are expected to fail


def expect(item, ok, what):
    """ok must hold, unless the review item is still open: then it is expected not to (and holding fails: remove it from
    OPEN)."""
    if item in OPEN:
        print(f"{'XPASS' if ok else 'xfail'} (rev1 item {item}{': fixed, remove it from OPEN' if ok else ''}) {what}")
        assert not ok, what
    else:
        assert ok, what


class Stub:
    """A corpus as far as obj_file and Matcher.match's cache see it."""

    def __init__(self, files):
        self.files = {f: [(0, 1)] for f in files}
        self.memo, self.used, self.misses = {}, {}, 0
        self._mangled = None

    mangled_files = R.Corpus.mangled_files


def old_obj_file(obj, corpus):
    """The loop obj_file was before the dict: every file's mangled name against the object's name."""
    base = R.re.split(r"[/(]", obj.replace("\\", "/").rstrip(")"))[-1]
    best = None
    for f in corpus.files:
        m = R.mangled(f)
        if base.endswith(m) or base.endswith(m + "bj"):
            if best is None or len(f) > len(best):
                best = f
    return best


def test_obj_file_is_the_old_loop():
    files = ["src/a.c", "lib/nitro/src/a.c", "a.c", "src/ov1/x_y.c", "src/ov1/x/y.c", "lib/twl/x/y.c", "src/main/main_0207e3d0.c"]
    c = Stub(files)
    objs = ["libgame.a.p/_root_src_a.c.o", "libgame.a.p/_root_lib_nitro_src_a.c.o", "x/a.c.o", "libgame.a(src_a.c.obj)",
            "libgame.a.p/aa.c.o", "w/src_ov1_x_y.c.o", "q/lib_twl_x_y.c.obj", "libgame.a.p/h_src_main_main_0207e3d0.c.o", "nothing.o", ""]
    rnd = random.Random(7)
    for _ in range(200):   # names built from the pieces, with and without the "bj" ending
        parts = [R.mangled(rnd.choice(files)) for _ in range(rnd.randint(0, 2))] + [rnd.choice(["_", "x", "src_"])]
        objs.append("d/" + "".join(parts) + rnd.choice(["", "bj"]))
    for o in objs:
        assert R.obj_file(o, c) == old_obj_file(o, c), o
    assert R.obj_file("libgame.a.p/_root_lib_nitro_src_a.c.o", c) == "lib/nitro/src/a.c"   # the longest wins


class Counting(R.Matcher):
    def search(self, piece):
        self.searches = getattr(self, "searches", 0) + 1
        self.cursor = (self.cursor or 0) + len(piece)   # what a match does: moves the cursor
        return [(0, self.cursor - len(piece), len(piece))]


def test_match_is_cached_by_piece_cursor_and_ranges():
    c = Stub(["a.c"])
    m = Counting(c)
    m.ranges = [(0, 100)]
    first = m.match(b"abcdefgh")
    assert (m.searches, m.cursor) == (1, 8)
    c.memo, c.used, c.misses = dict(c.used), {}, 0   # a later run: it loads what the run before kept
    m2 = Counting(c)   # same cursor (None), ranges and piece
    m2.ranges = [(0, 100)]
    assert m2.match(b"abcdefgh") == first and m2.cursor == 8 and not hasattr(m2, "searches")
    m3 = Counting(c)
    m3.ranges = [(0, 100)]
    m3.cursor = 8   # another cursor: another answer
    m3.match(b"abcdefgh")
    assert m3.searches == 1 and m3.cursor == 16
    m4 = Counting(c)
    m4.ranges = [(0, 101)]   # another object's ranges
    m4.match(b"abcdefgh")
    assert m4.searches == 1
    assert len(c.used) == 3 and c.misses == 2


def test_platinum_is_a_group_of_its_own():
    """pl.o (Platinum's one relocatable object) is group pl, its .rodata is the linker script's .bwrom_pl, and the SDK stays s."""
    import port_overlays
    assert R.GAMES["pl"] == ("CPUE", ["pl.o"])
    assert R.group_of("pl.o") == "pl" and R.group_of("/x/y/pl.o") == "pl"
    assert R.group_of("b2.o") == "b2" and R.group_of("libgame.a(libgame.a.p/src_ov001_x.c.o)") == "b"
    assert R.group_of("subprojects/libntr/libraries/snd/libsnd.a(libsnd.a.p/src_snd_util.c.o)") == "s"
    assert R.GROUP_SECTIONS[".bwrom_pl"] == "pl"
    for target in ("linux", "win64"):
        text = "\n".join(port_overlays.romdata_script(target))
        assert ".bwrom_pl" in text and "*pl.o(" in text


HG_TREE = os.path.join(ROOT, "third_party", "pokeheartgold")
REGISTER = re.compile(r'PORT_AssetAdd\("(IPKE|IPGE)",\s*"([^"]+)"')


def hg_registrations(tree):
    """{code: {blank name}} of the PORT_AssetAdd calls in the .c files of the tree's src/ and port/ (tools/hg_assets.py's
    constructors and port/asmdata's)."""
    regs = {"IPKE": set(), "IPGE": set()}
    for d, dirs, files in (w for top in ("src", "port") for w in os.walk(os.path.join(tree, top))):
        for f in files:
            if f.endswith(".c"):
                with open(os.path.join(d, f), encoding="utf-8", errors="replace") as fh:
                    text = fh.read()
                if "PORT_AssetAdd(" in text:
                    for code, name in REGISTER.findall(text):
                        regs[code].add(name)
    return regs


def tsv_rows(path):
    """{name: kind} of a rows file (tools/port_assets.py's format: name, module, start, end, kind)."""
    with open(path, encoding="utf-8") as fh:
        rows = [line.rstrip("\n").split("\t") for line in fh if line.strip()]
    assert rows[0][0] == "name", path
    return {r[0]: r[4] for r in rows[1:]}


def test_every_hgss_blank_has_a_row():
    """A blank without its game's row ends the game at start (asset_load.c: "not in IPKE's rows"); one blank is one object,
    so both games' rows give it the same layout."""
    if not os.path.exists(os.path.join(HG_TREE, "port", "assets_hg.tsv")):
        print("skip test_every_hgss_blank_has_a_row: no third_party/pokeheartgold checkout")
        return
    regs = hg_registrations(HG_TREE)
    hg, ss = (tsv_rows(os.path.join(HG_TREE, "port", f"assets_{v}.tsv")) for v in ("hg", "ss"))
    assert len(regs["IPKE"]) > 1000 and regs["IPKE"] == regs["IPGE"], (len(regs["IPKE"]), len(regs["IPGE"]))
    for code, rows in (("IPKE", hg), ("IPGE", ss)):
        missing = sorted(regs[code] - set(rows))
        assert not missing, f"{code}: {len(missing)} blanks without a row, e.g. {missing[:3]}"
    differ = sorted(n for n in regs["IPKE"] if hg[n] != ss[n])
    assert not differ, f"{len(differ)} blanks with another layout in each game's rows, e.g. {differ[:3]}"


def test_the_fill_gate_waits_for_every_filled_rom():
    """port/meson.build fills the image only when every game it fills has its ROM there (the shared group takes only
    bytes every listed ROM holds; a game left out runs on zeroed shared data): each ROM a `fill_games += ['--game', ...]`
    names is in the rom_fill condition."""
    with open(os.path.join(ROOT, "port", "meson.build"), encoding="utf-8") as fh:
        text = fh.read()
    m = re.search(r"^rom_fill = (.*?)\n(?=\S)", text, re.S | re.M)
    assert m, "no rom_fill condition in port/meson.build"
    cond = m.group(1)
    # the ROM is a variable (`'CPUE:pl:' + plat_rom + ':config/plat_us'`) or a path in the same string
    # (`'IRBO:b:' + b2_rom_dir / 'baserom_b_us.nds:config/b_us'`)
    roms = [(code, var or f"{base} / '{name}'") for code, var, base, name in
            re.findall(r"fill_games \+?= \['--game', '(\w{4}):\w+:' \+ (?:(\w+) \+ ':|(\w+) / '([^':]+):)", text)]
    assert len(roms) >= 7, roms
    for code, rom in roms:
        expect(2 if code in ("IPKE", "IPGE") else 0, f"fs.exists({rom})" in cond,
               f"the rom_fill condition waits for {code}'s ROM ({rom})")


if __name__ == "__main__":
    for name, fn in sorted(globals().items()):
        if name.startswith("test_"):
            fn()
            print("ok", name)
