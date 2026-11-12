#!/usr/bin/env python3
"""Tests of tools/inner_symbols.py on a small made-up tree: gamestats.c's client struct cut by
dsd's field symbols (to:<object> add:N, the symbols go), ov010's pointer to 0x021cf7dd that dsd couldn't tell between
ov021 and ov114 (inside the struct in ov114: ov021's), namein.c's proc table that other overlays load (the C names
data_ov194_: overlay(194), ov074's and ov093's made-up symbols go), a conflict, calls left alone, the file filter.

    python3 tools/inner_symbols_tests.py
"""
import os
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import inner_symbols as I  # noqa: E402

CFG = "config/b_us/arm9"
FILES = {
    "overlays/ov114/symbols.txt": """func_ov114_021c99a0 kind:function(arm,size=0x40) addr:0x021c99a0
data_ov114_021cf7ac kind:data(any) addr:0x021cf7ac
data_ov114_021cf7bc kind:data(any) addr:0x021cf7bc
data_ov114_021cf7dd kind:data(any) addr:0x021cf7dd ambiguous
data_ov114_021cf820 kind:data(any) addr:0x021cf820
data_ov114_021cf830 kind:data(any) addr:0x021cf830
data_ov114_021cf840 kind:data(any) addr:0x021cf840
data_ov114_021cf844 kind:data(any) addr:0x021cf844
data_ov114_021d1a75 kind:bss addr:0x021d1a75 ambiguous
""",
    "overlays/ov114/relocs.txt": """from:0x021c99c4 kind:load to:0x021cf7ac module:overlay(114)
from:0x021c99c8 kind:load to:0x021cf7bc module:overlay(114)
from:0x021c99cc kind:load to:0x021cf820 add:0x4 module:overlay(114)
""",
    "overlays/ov114/delinks.txt": """    .text       start:0x021c9800 end:0x021ca000 kind:code align:32
    .data       start:0x021cf700 end:0x021cf900 kind:data align:32
    .bss        start:0x021d1a00 end:0x021d1b00 kind:bss align:32

lib/dwc/src/gamestats/gamestats.c:
    complete
    .text       start:0x021c99a0 end:0x021c99e0
    .data       start:0x021cf7ac end:0x021cf84c

lib/dwc/src/nhttp/d_nhttp_common.c:
    complete
    .bss        start:0x021d1a00 end:0x021d1b00
""",
    "overlays/ov010/symbols.txt": """func_ov010_0215cd3c kind:function(thumb,size=0x44) addr:0x0215cd3c
func_ov010_02170000 kind:function(thumb,size=0x10) addr:0x02170000
func_ov010_02170f00 kind:function(thumb,size=0x40) addr:0x02170f00
""",
    "overlays/ov010/relocs.txt": """from:0x0215cd6c kind:thumb_call to:0x021f36c0 module:overlays(36,37)
from:0x02170ba0 kind:load to:0x021d1a75 module:overlays(21,93,114)
from:0x02170f10 kind:load to:0x021cf7dd module:overlays(21,114)
from:0x02170f14 kind:thumb_call to:0x021f36c0 module:overlays(21,74)
from:0x02170f18 kind:thumb_call to:0x021f36c0 module:overlays(21,74)
from:0x02170f1c kind:thumb_call to:0x021f36c0 module:overlays(21,74)
""",
    "overlays/ov010/delinks.txt": """    .text       start:0x0215c000 end:0x02171000 kind:code align:32

src/ov010/scrcmd_proc.c:
    .text       start:0x0215cd3c end:0x0215cd80

src/ov010/caller.c:
    .text       start:0x02170f00 end:0x02170f40
""",
    "overlays/ov036/symbols.txt": "func_ov036_021f36c0 kind:function(thumb,size=0x1ec) addr:0x021f36c0\n",
    "overlays/ov036/relocs.txt": "",
    "overlays/ov036/delinks.txt": "    .text       start:0x021f36c0 end:0x021f4000 kind:code align:32\n",
    "overlays/ov037/symbols.txt": "func_ov037_021f36c0 kind:function(thumb,size=0x26) addr:0x021f36c0\n",
    "overlays/ov037/relocs.txt": "",
    "overlays/ov037/delinks.txt": "    .text       start:0x021f36c0 end:0x021f4000 kind:code align:32\n",
    "overlays/ov021/symbols.txt": """func_ov021_021cf7dc kind:function(thumb,size=0x20) addr:0x021cf7dc
s0159_ActorAnimationPlay kind:function(thumb,size=0x28) addr:0x021d1a74
func_ov021_021f36c0 kind:function(thumb,size=0x20) addr:0x021f36c0
""",
    "overlays/ov021/relocs.txt": "",
    "overlays/ov021/delinks.txt": "    .text       start:0x02187f00 end:0x021f4000 kind:code align:32\n",
    "overlays/ov074/symbols.txt": """data_ov074_021f54f0 kind:data(any) addr:0x021f54f0
data_ov074_021f557c kind:data(any) addr:0x021f557c ambiguous
data_ov074_021f5580 kind:data(any) addr:0x021f5580 ambiguous
func_ov074_021f36c0 kind:function(thumb,size=0x20) addr:0x021f36c0
""",
    "overlays/ov074/relocs.txt": "",
    "overlays/ov074/delinks.txt": """    .data       start:0x021f5000 end:0x021f6000 kind:data align:32

src/ov074/table.c:
    complete
    .data       start:0x021f54f0 end:0x021f55f0
""",
    "overlays/ov093/symbols.txt": """func_ov093_021d1a74 kind:function(thumb,size=0x15c) addr:0x021d1a74
data_ov093_021f557c kind:bss addr:0x021f557c ambiguous
""",
    "overlays/ov093/relocs.txt": "",
    "overlays/ov093/delinks.txt": "    .bss        start:0x021f5000 end:0x021f6000 kind:bss align:32\n",
    "overlays/ov194/symbols.txt": """data_ov194_021f557c kind:data(any) addr:0x021f557c
data_ov194_021f5580 kind:data(any) addr:0x021f5580
""",
    "overlays/ov194/relocs.txt": "",
    "overlays/ov194/delinks.txt": "    .data       start:0x021f5000 end:0x021f6000 kind:data align:32\n",
    "overlays/ov088/symbols.txt": "GameStart_Init kind:function(thumb,size=0x30) addr:0x021bc200\n",
    "overlays/ov088/relocs.txt": """from:0x021bc21c kind:load to:0x021f557c module:overlays(74,93,194)
from:0x021bc220 kind:load to:0x021f5580 module:overlays(93,74,194)
""",
    "overlays/ov088/delinks.txt": """    .text       start:0x021bc000 end:0x021bd000 kind:code align:32

src/ov088/game_start.c:
    .text       start:0x021bc200 end:0x021bc230
""",
    "symbols.txt": """RTC_ConvertDateToDay kind:function(arm,size=0x94) addr:0x02088f58
data_020a86e8 kind:data(any) addr:0x020a86e8
data_020a86f4 kind:data(any) addr:0x020a86f4
""",
    "relocs.txt": "from:0x02088fe4 kind:load to:0x020a86f0 module:main\n",  # dsd's: sDayOfYear[month - 1]
    "delinks.txt": """    .text       start:0x02000000 end:0x020a0000 kind:code align:32
    .data       start:0x020a8000 end:0x020a9000 kind:data align:32

lib/nitro/src/os/os_table.c:
    complete
    .data       start:0x020a86e8 end:0x020a86f4

lib/nitro/src/rtc/convert.c:
    complete
    .text       start:0x02088f58 end:0x02088fec
    .data       start:0x020a86f4 end:0x020a8724
""",
}
XMAP = """# .arm9
  02088F58 00000094 .text   RTC_ConvertDateToDay\t(convert.o)
  020A86E8 0000000C .data   data_020a86e8\t(os_table.o)
  020A86F4 00000030 .data   sDayOfYear\t(convert.o)
# .ov114
  021CF7AC 000000A0 .data   data_ov114_021cf7ac\t(gamestats.o)
  021D1A00 00000100 .bss    sRootStorage\t(d_nhttp_common.o)
# .ov074
  021F54F0 00000100 .data   data_ov074_021f54f0\t(table.o)
"""
NULL = ("", 0, 0, 0, False, False)
NOSEC = ("", 0, 0, 0, 0)
ABS32, THM_CALL = 2, 10
OBJECTS = {  # read_object()'s tuples: (bytes, sections, symbols, relocations)
    "lib/dwc/src/gamestats/gamestats.c": (b"", [("", 0, 0, 0, 0), (".text", 1, 0, 0x40, 4), (".data", 1, 0, 0xa0, 4)],
                                          [NULL, ("func_ov114_021c99a0", 1, 0, 0x40, True, True),
                                           ("data_ov114_021cf7ac", 2, 0, 0xa0, False, False)],
                                          {1: [(0x24, 2, 2, 0), (0x28, 2, 2, 0x10), (0x2c, 2, 2, 0x78)]}),
    "src/ov074/table.c": (b"", [("", 0, 0, 0, 0), (".data", 1, 0, 0x100, 4)],
                          [NULL, ("data_ov074_021f54f0", 1, 0, 0x100, True, False)], {}),
    # the second load names ov074's symbol, inside ov074's table there: a conflict
    "src/ov088/game_start.c": (b"", [("", 0, 0, 0, 0), (".text", 1, 0, 0x30, 4)],
                               [NULL, ("GameStart_Init", 1, 0, 0x30, True, True),
                                ("data_ov194_021f557c", 0, 0, 0, True, False),
                                ("data_ov074_021f5580", 0, 0, 0, True, False)],
                               {1: [(0x1c, 2, 2, 0), (0x20, 2, 3, 0)]}),
    # sDayOfYear[month - 1]: the pool word is the static - 4, inside os_table.c's object
    "lib/nitro/src/rtc/convert.c": (b"", [NOSEC, (".data", 1, 0, 0x30, 4), (".text", 1, 0, 0x94, 4)],
                                    [NULL, ("sDayOfYear", 1, 0, 0x30, False, False),
                                     ("RTC_ConvertDateToDay", 2, 0, 0x94, True, True)], {2: [(0x8c, ABS32, 1, -4)]}),
    "lib/nitro/src/os/os_table.c": (b"", [NOSEC, (".data", 1, 0, 0xc, 4)],
                                    [NULL, ("data_020a86e8", 1, 0, 0xc, True, False)], {}),
    "lib/dwc/src/nhttp/d_nhttp_common.c": (b"", [NOSEC, (".bss", 8, 0, 0x100, 4)],
                                           [NULL, ("sRootStorage", 1, 0, 0x100, False, False)], {}),
    # GFL_OvlLoad(OVERLAY_37_ID); func_ov036_021f36c0(): the C named dsd's first overlay
    "src/ov010/scrcmd_proc.c": (b"", [NOSEC, (".text", 1, 0, 0x44, 4)],
                                [NULL, ("func_ov010_0215cd3c", 1, 0, 0x44, True, True),
                                 ("OVERLAY_37_ID", 0, 0, 0, True, False), ("func_ov036_021f36c0", 0, 0, 0, True, True)],
                                {1: [(0x40, ABS32, 2, 0), (0x30, THM_CALL, 3, 0)]}),
    # calls by name: ov074's (not dsd's first: taken), ov021's (dsd's first of overlays(21,74): no evidence)
    "src/ov010/caller.c": (b"", [NOSEC, (".text", 1, 0, 0x40, 4)],
                           [NULL, ("func_ov010_02170f00", 1, 0, 0x40, True, True),
                            ("func_ov074_021f36c0", 0, 0, 0, True, True), ("func_ov021_021f36c0", 0, 0, 0, True, True)],
                           {1: [(0x18, THM_CALL, 2, 0), (0x1c, THM_CALL, 3, 0)]}),
}


class Tree(I.Tree):
    def read_object(self, src):
        return OBJECTS.get(src)


def tree():
    root = tempfile.mkdtemp(prefix="inner_symbols_")
    for path, text in FILES.items():
        os.makedirs(os.path.dirname(f"{root}/{CFG}/{path}"), exist_ok=True)
        open(f"{root}/{CFG}/{path}", "w").write(text)
    os.makedirs(f"{root}/build/b_us")
    open(f"{root}/build/b_us/arm9.o.xMAP", "w").write(XMAP)
    os.makedirs(f"{root}/src/ov114")
    # a name after a string's // is a use; one in a comment is none
    open(f"{root}/src/ov114/other.c", "w").write('const char *s = "http://x"; extern int data_ov114_021cf830;\n'
                                                 "// data_ov114_021cf840\n/* data_ov114_021cf844 */\n")
    return Tree(root)


def changes(acts, kind):
    return [text for k, text, _, _ in acts if k == kind]


def test_inner_symbols_become_the_object_plus_offset():
    acts = tree().plan()
    inner = changes(acts, "inner")
    assert any("0x021c99c8: kind:load to:0x021cf7bc module:overlay(114) -> kind:load to:0x021cf7ac add:0x10 "
               "module:overlay(114)" in t for t in inner), inner
    assert any("0x021c99cc: kind:load to:0x021cf820 add:0x4 module:overlay(114) -> kind:load to:0x021cf7ac "
               "add:0x78 module:overlay(114)" in t for t in inner), inner
    assert not any("0x021c99c4" in t for t in inner), inner  # the object's start: nothing to do
    gone = changes(acts, "symbol")
    assert any(t.startswith("ov114 data_ov114_021cf7bc ") for t in gone), gone
    assert any(t.startswith("ov114 data_ov114_021cf820 ") for t in gone), gone
    assert not any("data_ov114_021cf7ac" in t.split(" goes")[0] for t in gone), gone
    # a C file still names it: kept
    assert any("data_ov114_021cf830" in t and "src/ov114/other.c names it" in t for t in changes(acts, "keep"))


def test_ambiguous_load_inside_our_object_in_one_overlay_is_the_others():
    acts = tree().plan()
    over = changes(acts, "overlay")
    assert any("ov010 0x02170f10: kind:load to:0x021cf7dd module:overlays(21,114) -> kind:load to:0x021cf7dd "
               "module:overlay(21): inside ov114's data_ov114_021cf7ac" in t for t in over), over
    assert any(t.startswith("ov114 data_ov114_021cf7dd ") for t in changes(acts, "symbol"))
    assert not any("0x02170f14" in t for t in over), over  # a call: whichever overlay is loaded


def test_ambiguous_load_the_c_names_takes_its_overlay():
    acts = tree().plan()
    over = changes(acts, "overlay")
    assert any("ov088 0x021bc21c: kind:load to:0x021f557c module:overlays(74,93,194) -> kind:load to:0x021f557c "
               "module:overlay(194): src/ov088/game_start.c names data_ov194_021f557c" in t for t in over), over
    gone = changes(acts, "symbol")
    for name in ("data_ov074_021f557c", "data_ov093_021f557c"):
        assert any(t.startswith(f"{name} ", 6) for t in gone), (name, gone)
    assert not any("data_ov194_021f557c" in t.split(" goes")[0] for t in gone), gone
    # the C names ov074's symbol, but in ov074 the address is inside table.c's table: a conflict, nothing changed
    conflict = changes(acts, "conflict")
    assert any("0x021bc220" in t and "inside data_ov074_021f54f0 (src/ov074/table.c)" in t for t in conflict), conflict
    assert not any("0x021bc220" in t for t in over), over
    assert not any("data_ov074_021f5580" in t.split(" goes")[0] for t in gone), gone


def test_only_the_cases_of_the_files_asked_for():
    acts = tree().plan({"src/ov088/game_start.c"})
    assert changes(acts, "overlay") and all("ov088" in t for t in changes(acts, "overlay")), acts
    assert not changes(acts, "inner"), acts
    assert not any("ov114" in t for t in changes(acts, "symbol")), acts


def test_apply_writes_the_lines_and_drops_the_symbols():
    t = tree()
    written = t.apply(t.plan())
    relocs = open(f"{t.root}/{CFG}/overlays/ov114/relocs.txt").read()
    assert "from:0x021c99c8 kind:load to:0x021cf7ac add:0x10 module:overlay(114)\n" in relocs, relocs
    assert "from:0x021c99c4 kind:load to:0x021cf7ac module:overlay(114)\n" in relocs, relocs
    syms = open(f"{t.root}/{CFG}/overlays/ov114/symbols.txt").read()
    assert "data_ov114_021cf7bc" not in syms and "data_ov114_021cf7ac kind" in syms, syms
    assert "data_ov114_021cf830" in syms, syms
    assert "module:overlay(21)\n" in open(f"{t.root}/{CFG}/overlays/ov010/relocs.txt").read()
    assert f"{t.root}/{CFG}/overlays/ov021/relocs.txt" not in written  # untouched
    again = Tree(t.root).plan()  # applied: nothing left to do but the conflict and what is kept
    assert not [a for a in again if a[0] in ("inner", "overlay", "symbol")], again


def test_names_in_comments_are_no_uses():
    acts = tree().plan()
    gone, kept = changes(acts, "symbol"), changes(acts, "keep")
    for name in ("data_ov114_021cf840", "data_ov114_021cf844"):  # only a comment of other.c names them
        assert any(t.startswith(f"ov114 {name} ") for t in gone), (name, gone)
    assert any("data_ov114_021cf830" in t and "other.c names it" in t for t in kept), kept  # after "http://"
    assert I.uncommented('a /* b */ c // d\ne "x // y" f\n') == 'a   c  \ne "x // y" f\n'


def test_a_table_minus_one_stays_the_table():
    # dsd's to:0x020a86f0 is inside os_table.c's object, but the C names sDayOfYear - 4 (rtc/convert.c, t34i)
    inner = changes(tree().plan(), "inner")
    assert any("main 0x02088fe4: kind:load to:0x020a86f0 module:main -> kind:load to:0x020a86f4 add:-0x4 "
               "module:main: the C names sDayOfYear-0x4" in t for t in inner), inner
    assert not any("data_020a86e8" in t for t in inner), inner
    t = tree()
    t.apply(t.plan())
    assert not [a for a in Tree(t.root).plan() if a[0] == "inner" and "0x02088fe4" in a[1]]


def test_ambiguous_calls():
    acts = tree().plan()
    over = changes(acts, "overlay")
    # the function loads OVERLAY_37_ID: ov037's, though the C names ov036's (dsd's first), which it should rename
    assert any("ov010 0x0215cd6c: kind:thumb_call to:0x021f36c0 module:overlays(36,37) -> kind:thumb_call "
               "to:0x021f36c0 module:overlay(37): src/ov010/scrcmd_proc.c: its function loads OVERLAY_37_ID" in t
               for t in over), over
    assert any("scrcmd_proc.c names func_ov036_021f36c0 (ov036) at ov010 0x0215cd6c, but ov037's "
               "func_ov037_021f36c0 is the one" in t for t in changes(acts, "rename")), changes(acts, "rename")
    # the C names a function of one overlay that isn't dsd's first: that one
    assert any("0x02170f18: kind:thumb_call to:0x021f36c0 module:overlays(21,74) -> kind:thumb_call to:0x021f36c0 "
               "module:overlay(74): src/ov010/caller.c names func_ov074_021f36c0" in t for t in over), over
    # dsd's first overlay's name, or no C: no evidence, left
    assert not any("0x02170f1c" in t or "0x02170f14" in t for t in over), over
    # an ARM function where the call is Thumb's (bl, no blx): not that overlay
    t = tree()
    t.syms["ov074"][0x021f36c0] = [(3, "func_ov074_021f36c0", "function", "(arm,size=0x20)")]
    acts = t.plan()
    assert not any("0x02170f18" in x for x in changes(acts, "overlay")), acts
    assert any("0x02170f18" in x and "but ov074 has no thumb function at 0x21f36c0" in x
               for x in changes(acts, "conflict")), changes(acts, "conflict")
    assert I.dsd_default("func_ov036_021f36c0", ["ov036", "ov037"])
    assert not I.dsd_default("func_ov037_021f36c0", ["ov036", "ov037"])
    assert not I.dsd_default("GymNacrene_Init", ["ov036", "ov037"])


def test_unresolved_ambiguous_load_leaves_the_overlays_it_is_inside_of():
    # t35q: ov021 and ov093 both have a Thumb function at 0x021d1a74; in ov114 the address is inside our static
    acts = tree().plan()
    assert any("ov010 0x02170ba0: kind:load to:0x021d1a75 module:overlays(21,93,114) -> kind:load to:0x021d1a75 "
               "module:overlays(21,93): not ov114 (inside sRootStorage)" in t for t in changes(acts, "overlay")), acts
    assert any(t.startswith("ov114 data_ov114_021d1a75 ") for t in changes(acts, "symbol")), changes(acts, "symbol")


def test_a_complete_file_the_link_map_lacks_is_placed_at_its_ranges():
    class Stale(Tree):  # the map is older than gamestats.o (t35n's fx_sincos.c: "nothing to change")
        def xmap(self):
            out = super().xmap()
            del out["ov114", "gamestats.o"]
            return out

    t = tree()
    stale = Stale(t.root)
    acts = stale.plan()
    assert stale.unmapped == ["lib/dwc/src/gamestats/gamestats.c"], stale.unmapped
    assert sorted(changes(acts, "inner")) == sorted(changes(t.plan(), "inner"))

    class Moved(Tree):  # the map has gamestats.o at its old ranges, 8 bytes later (t37e's un_select.c)
        def xmap(self):
            out = super().xmap()
            out["ov114", "gamestats.o"] = {n: a + 8 for n, a in out["ov114", "gamestats.o"].items()}
            return out

    assert sorted(changes(Moved(t.root).plan(), "inner")) != sorted(changes(t.plan(), "inner"))
    moved = Moved(t.root, ["lib/dwc/src/gamestats/gamestats.c"])  # fit_data.py: --placed, its new ranges
    assert sorted(changes(moved.plan(), "inner")) == sorted(changes(t.plan(), "inner"))
    assert moved.unmapped == [], moved.unmapped

    class Unbuilt(Tree):  # a file asked for without its object: said so
        def read_object(self, src):
            return None if src == "src/ov088/game_start.c" else super().read_object(src)

    unbuilt = Unbuilt(t.root)
    unbuilt.plan({"src/ov088/game_start.c"})
    assert unbuilt.missing == ["src/ov088/game_start.c"], unbuilt.missing


def test_module_fields():
    for spec, mods in (("overlay(21)", ["ov021"]), ("overlays(18,19,176)", ["ov018", "ov019", "ov176"]),
                       ("main", ["main"]), ("itcm", ["itcm"]), ("ltd_autoload(0)", ["ltd_autoload_0"])):
        assert I.mods_of(spec) == mods, (spec, I.mods_of(spec))
        assert I.spec_of(mods) == spec, (mods, I.spec_of(mods))


def main():
    tests = [f for n, f in sorted(globals().items()) if n.startswith("test_")]
    bad = 0
    for t in tests:
        try:
            t()
            print(f"ok   {t.__name__}")
        except Exception as e:  # noqa: BLE001
            bad += 1
            print(f"FAIL {t.__name__}: {type(e).__name__}: {e}")
    print(f"{len(tests) - bad} of {len(tests)} passed")
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
