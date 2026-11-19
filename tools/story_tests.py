#!/usr/bin/env python3
"""Tests of tools/story.py: synthetic bytecode and C, and (with the ROM) the decoder over every Black script."""
import re
import struct
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import story  # noqa: E402

# a made-up table: the names story.py gives meaning to, at made-up ids
NAMES = ["VMHalt", "VMStackPush", "VMStackPushConst", "VMStackCmp", "VMJumpIf", "WorkSetConst", "FlagSet",
         "VMStackPushFlag"]
LAYS = ["", "a", "2", "2", "14", "v2", "a", "a"]
CMDS = (NAMES, LAYS)


def op(name, *vals):
    out = struct.pack("<H", NAMES.index(name))
    for k, v in zip(LAYS[NAMES.index(name)], vals):
        out += struct.pack({"1": "<B", "2": "<H", "a": "<H", "v": "<H", "4": "<i"}[k], v)
    return out


def script(body):
    return struct.pack("<iH", 2, story.END) + body   # one entry, 2 bytes on from its end: past the terminator


class Tests(unittest.TestCase):
    def setUp(self):
        story.NAMES4.clear()   # a Gen 4 Game fills it; the Gen 5 expectations are numbers

    def test_layout_from_c(self):
        c = ("static u16 helper(VM *vm, FieldScriptEnv *env) {\n    return VM_Read16(vm);\n}\n"
             "BOOL s0001_Cmd(VM *vm, FieldScriptEnv *env) {\n    u16 a = ScriptReadAny(vm, env);\n"
             "#if GAME_VERSION == VERSION_WHITE\n    VM_Read32(vm);\n#else\n    VM_Read8(vm);\n#endif\n"
             "    u8 f = *vm->pc;\n    vm->pc++;\n    helper(vm, env);\n    return FALSE;\n}\n")
        for white, want in ((False, "a112"), (True, "a412")):
            funcs = story.c_functions({"x.c": story.preprocess(c, white)})
            self.assertEqual(story.layout("s0001_Cmd", funcs, {}), want)

    def test_layout_follows_native_callback_and_ends_at_the_function_end(self):
        # a wait command: its callback reads one var in each branch (so one var), after the handler's own argument;
        # a table closing in column 0 inside a handler doesn't end it
        c = ("static BOOL wait(VM *vm, FieldScriptEnv *env) {\n    if (x) {\n        *ScriptReadVar(vm, env) = 1;\n"
             "    } else {\n        *ScriptReadVar(vm, env) = 0;\n    }\n    return TRUE;\n}\n"
             "BOOL s0002_Cmd(VM *vm, FieldScriptEnv *env) {\n    static const u16 t[] = {\n1,\n};\n"
             "    u16 a = ScriptReadAny(vm, env);\n    VM_SetNativeCallback(vm, (VMCommand)wait);\n    return TRUE;\n}\n")
        funcs = story.c_functions({"x.c": c})
        self.assertEqual(story.layout("s0002_Cmd", funcs, {}), "av")

    def test_table_without_terminator_ends_where_scripts_begin(self):
        f = struct.pack("<ii", 4, 6) + b"\0\0\0\0\0\0\0\0\0\0"   # two entries; the first script is at 8
        self.assertEqual(story.script_offsets(f), [8, 14])
        self.assertEqual(story.script_offsets(script(op("VMHalt"))), [6])

    def test_shop_swap_between_start_and_end(self):
        names = ["VMHalt", "JoinAvenueStoreStart", "JoinAvenueStoreEnd", "StartTalk"]
        f = b"".join(struct.pack("<H", i) + b"\0\0" * n for i, n in ((1, 0), (3, 2), (2, 0), (3, 0), (0, 0)))
        shop = ["", "", "", "22"]
        code, bad = story.reach(f, 0, (names, ["", "", "", ""], shop))
        self.assertEqual(bad, [])
        self.assertEqual([i[1] for _, i in sorted(code.items())],
                         ["JoinAvenueStoreStart", "StartTalk", "JoinAvenueStoreEnd", "StartTalk", "VMHalt"])

    def test_who_sets_with_condition(self):
        # if work 0x4088 == 1 (else jump to the end): flag 0x21b set, work 0x4088 = 2
        tail = op("FlagSet", 0x21B) + op("WorkSetConst", 0x4088, 2) + op("VMHalt")
        body = op("VMStackPush", 0x4088) + op("VMStackPushConst", 1) + op("VMStackCmp", 1) + \
            op("VMJumpIf", 0xFF, len(tail) - 2) + tail
        f = script(body)
        entry = story.script_offsets(f)[0]
        self.assertEqual(entry, 6)
        code, bad = story.reach(f, entry, CMDS)
        self.assertEqual(bad, [])
        hits, cut = story.explore(code, entry, story.setter("work", 0x4088, 2))
        self.assertFalse(cut)
        self.assertEqual([story.merge([c]) for _, c, _ in hits], [[["work 0x4088 == 1"]]])
        hits, _ = story.explore(code, entry, story.setter("flag", 0x21B, None))
        self.assertEqual(len(hits), 1)

    def test_cap_cuts_to_a_pointer_to_all(self):
        lines = ["l%d" % i for i in range(20)]
        self.assertEqual(story.cap(lines, True), lines)
        out = story.cap(lines, False)
        self.assertEqual((len(out), out[-1]), (story.BRIEF + 1, "... 8 more lines: --all"))
        self.assertEqual(story.cap(lines[:5], False), lines[:5])

    def test_merge_joins_both_senses(self):
        a = ("cmp", ("flag", 1), 1, ("c", 1))
        b = ("cmp", ("work", 0x4000), 1, ("c", 3))
        self.assertEqual(story.merge([(a, b), (story.neg(a), b)]), [["work 0x4000 == 3"]])

    def rom_decodes(self, game, least):
        if not (story.ROOT / ("orig/baserom_%s_us.nds" % game)).exists():
            self.skipTest("no ROM")
        g = story.Game(game)
        n = bad = 0
        for fid, idx, off in g.scripts():
            code, b = story.reach(g.files[fid], off, g.cmds_for(fid))
            n, bad = n + len(code), bad + len(b)
        self.assertGreater(n, least)
        self.assertEqual(bad, 0)

    def test_version_checks_pick_the_dead_twin(self):
        ver = ("cmp", ("ret", "GameGetVersion"), 1, ("c", 23))   # GameGetVersion() == 23
        other = ("cmp", ("work", 0x4000), 1, ("c", 3))
        self.assertIsNone(story.path_version((other,), 22))   # no version check on the path
        self.assertIs(story.path_version((ver, other), 23), True)
        self.assertIs(story.path_version((ver,), 22), False)
        self.assertIs(story.path_version((story.neg(ver),), 22), True)   # != 23 is true in 22
        door = lambda i, to, x: {"i": i, "to": to, "to_warp": 0, "x": x, "z": 7}   # noqa: E731
        warps = [door(0, 531, 5), door(1, 537, 5), door(2, 9, 6)]
        # warp 1 moved off its door by this version's script: its twin 0 on the tile stays, 1 is dead; 2 has no twin
        self.assertEqual(story.dead_among({1}, warps), [1])
        self.assertEqual(story.dead_among({2}, warps), [])
        self.assertEqual(story.dead_among({0, 1}, warps), [])   # both moved: neither is the one left standing
        self.assertEqual(story.dead_among(set(), warps), [])

    def test_rom_black2_and_white2_take_their_own_interiors(self):
        for game, dead, live in (("b2", 537, 531), ("w2", 531, 537)):
            if not (story.ROOT / ("orig/baserom_%s_us.nds" % game)).exists():
                self.skipTest("no ROM")
            import atlas
            data = atlas.load(game)
            # Reversal Mountain's outside (zone 461): a warp into each version's interior on the same door tile
            warps = {w["to"]: w for w in data["zones"][461]["warps"] if w["to"] in (dead, live)}
            self.assertTrue(warps[dead].get("dead") and not warps[live].get("dead"), game)
            self.assertEqual([b for _, _, b in atlas.path(data, 461, 412)][:1], [live])

    def test_warper_picks_map_changes_to_the_zone(self):
        ins = (0, "MapChangeCore", [("2", 488), ("2", 15)], 12)
        self.assertTrue(story.warper(488)(ins))
        self.assertTrue(story.warper(None)(ins))
        self.assertFalse(story.warper(52)(ins))
        self.assertFalse(story.warper(None)((0, "WorkSetConst", [("v", 488), ("2", 1)], 8)))
        self.assertEqual(story.what_warp(ins), "MapChangeCore to zone 488")

    def test_rom_black2_scripted_doors(self):
        if not (story.ROOT / "orig/baserom_b2_us.nds").exists():
            self.skipTest("no ROM")
        import atlas
        data = atlas.load("b2")
        # Castelia (zone 39): actor uid 0's script 2 changes map to zone 52; Castelia Gym's ride script 3 (zone 29) to
        # Burgh's room 488 is run by the gym's own code, so it is no edge, and who-warps says nothing runs it
        self.assertIn(("G:npc:0", 52), [(e["tok"], e["to"]) for e in data["scripted"] if e["from"] == 39])
        self.assertFalse([e for e in data["scripted"] if e["to"] == 488])
        out = story.who_warps(story.Game("b2", data), 488, True)
        brief = story.who_warps(story.Game("b2", data), 488)
        self.assertEqual(len(brief), 2)
        self.assertIn("run by nothing in the atlas", brief[0])
        self.assertTrue(out[0].startswith("zone 29 script 3 @"), out[0])
        self.assertIn("nothing in the atlas", out[1])
        self.assertEqual(atlas.path(data, 39, 52)[0][1].split()[:2], ["script", "G:npc:0:"])

    def fake_save(self, works, flags, zone, copy_dates=(5, 6)):
        """A made-up Black save: both copies with footers; copy 1 newer by its date, holding works/flags."""
        end, copy2, base, nw, nf = story.SAVE["b"]
        d = bytearray(0x80000)
        for i, copy in enumerate((0, copy2)):
            struct.pack_into("<2I", d, copy + end - 12, end, story.SAVE_MAGIC)
            struct.pack_into("<I", d, copy + story.SAVE_DATE, 24 | copy_dates[i] << 7)   # year 24, month 5 or 6
            if i == 1:
                for w, v in works.items():
                    struct.pack_into("<H", d, copy + base + 2 * (w - 0x4000), v)
                for f in flags:
                    d[copy + base + 2 * nw + f // 8] |= 1 << (f % 8)
                struct.pack_into("<2H3i", d, copy + story.SAVE_PLACE, zone, 0, 15 << 16 | 0x8000, 0, 10 << 16 | 0x8000)
        return bytes(d)

    def test_save_state_newer_copy(self):
        st = story.State("b", self.fake_save({0x40ad: 1}, [0x31c, 0x10], 38))
        self.assertEqual((st.zone, st.tile), (38, (15, 10)))
        self.assertEqual(st.value(("work", 0x40ad)), 1)
        self.assertEqual(st.value(("flag", 0x31c)), 1)
        self.assertEqual(st.value(("flag", 0x4001)), 0)            # a temp flag: clear when a zone starts
        self.assertEqual(st.value(("flag", 0x10), here=False), 0)  # a map-local flag, in another zone
        self.assertIsNone(st.value(("var", 0x8004)))
        self.assertTrue(st.holds(("cmp", ("work", 0x40ad), 1, ("c", 1))))
        self.assertFalse(st.holds(("cmp", ("flag", 0x31c), 1, ("c", 0))))
        self.assertIsNone(st.holds(("cmp", ("ret", "ItemCheckAmount"), 1, ("c", 1))))
        old = story.State("b", self.fake_save({0x40ad: 1}, [], 38, copy_dates=(7, 6)))   # copy 0 newer: all clear
        self.assertEqual(old.value(("work", 0x40ad)), 0)
        with self.assertRaises(ValueError):
            story.State("b", bytes(0x80000))

    def test_story_change(self):
        st = story.State("b", self.fake_save({0x40ad: 1}, [0x31c], 38))
        ch = story.story_change
        self.assertEqual(ch((0, "WorkSetConst", [("v", 0x40ad), ("2", 2)], 6), st, 28), "work 0x40ad = 2")
        self.assertIsNone(ch((0, "WorkSetConst", [("v", 0x40ad), ("2", 1)], 6), st, 28))   # already 1
        self.assertEqual(ch((0, "FlagReset", [("a", 0x31c)], 4), st, 28), "flag 0x31c clear")
        self.assertIsNone(ch((0, "FlagSet", [("a", 0x31c)], 4), st, 28))
        self.assertIsNone(ch((0, "FlagSet", [("a", 0xAAF)], 4), st, 28))                  # a daily flag
        self.assertIsNone(ch((0, "WorkSetConst", [("v", 0x4003), ("2", 1)], 6), st, 28))   # map-local, elsewhere
        self.assertEqual(ch((0, "WorkSetConst", [("v", 0x4003), ("2", 1)], 6), st, 38), "work 0x4003 = 1")
        self.assertEqual(ch((0, "MapChangeCore", [("2", 52), ("2", 0)], 8), st, 39), "warp to zone 52")
        self.assertIsNone(ch((0, "MapChangeCore", [("2", 39), ("2", 0)], 8), st, 39))

    def rom_next(self, game, save, first):
        import route
        if not (story.ROOT / ("orig/baserom_%s_us.nds" % game)).exists() or \
                not (route.SAVES / (save + ".bin")).exists():
            self.skipTest("no ROM or save")
        out = story.next_steps(story.Game(game), story.load_state(game, save))
        self.assertTrue(out[2].strip().startswith(first), out[2])

    def test_rom_next_black_castelia(self):
        # b-cas3 -> b-cas4 (route b c-burgh) sets exactly what Castelia's street trigger T0 sets
        self.rom_next("b", "b-cas3", "1*zone 28 G:trig:0 (script 19): work 0x40ad = 2, flag 0x281 set")

    def test_rom_next_black2_sewers(self):
        # b2-c7 -> b2-c8 (route b2 c-plaza) sets work 0x40b2 3 -> 4: Thumb Pier's (zone 40) trigger
        self.rom_next("b2", "b2-c7", "1*zone 40 G:trig:0 (script 6): work 0x40b2 = 4")

    def test_cmd_name_by_id_or_name(self):
        self.assertEqual(story.cmd_name("6", NAMES), "FlagSet")
        self.assertEqual(story.cmd_name("0x6", NAMES), "FlagSet")
        self.assertEqual(story.cmd_name("flagset", NAMES), "FlagSet")
        self.assertIsNone(story.cmd_name("99", NAMES))
        self.assertIsNone(story.cmd_name("nothing", NAMES))
        self.assertIsNone(story.cmd_name("1", ["VMHalt", "NULL"]))
        self.assertIsNone(story.cmd_name("null", ["VMHalt", "NULL"]))

    def test_zone_names_from_the_constants_header(self):
        self.assertEqual(story.zone_names("b")[6], "STRIATON_CITY")
        self.assertEqual(story.zone_names("w")[6], "STRIATON_CITY")
        if (story.ROOT / "third_party/pokebw2/generated/constants/zones.h").exists():
            self.assertEqual(story.zone_names("b2")[0], "BLACK_CITY")
            self.assertGreater(len(story.zone_names("w2")), 500)

    def test_rom_who_uses(self):
        for rom in ("b", "b2", "w2"):
            if not (story.ROOT / ("orig/baserom_%s_us.nds" % rom)).exists():
                self.skipTest("no ROM")
        # Black: the game-clear command (0x156) is run unconditionally by zone 145 script 1, and by zone 278's 11, 15, 16
        out = story.who_uses(story.Game("b"), "0x156", True)
        self.assertEqual([l.split(":")[0] for l in out],
                         ["zone 145 script 1", "zone 278 script 11", "zone 278 script 15", "zone 278 script 16"])
        # Black 2 and White 2: by name, the zone's constant name is there (w2 once crashed in the plugin lookup)
        for rom in ("b2", "w2"):
            out = story.who_uses(story.Game(rom), "mapchangewarp", True)
            self.assertTrue(out[0].startswith("zone 0 (BLACK_CITY) script 3: "), out[0])
            self.assertIn("MapChangeWarp 479 7 14 0", out[0])
            self.assertEqual(story.who_uses(story.Game(rom), "0x9999"), ["no script runs command 0x9999"])
        # a plugin's command (ids from 1000) is looked up in the plugin the script runs under
        self.assertTrue(story.who_uses(story.Game("b2"), "1000", True))

    def test_gen4_flag_check(self):
        # Gen 4: CheckFlag (FlagCheck) makes the flag the comparison result; goto_if_unset is GoToIf < (cond 0)
        code = {0: (0, "FlagCheck", (("2", 0x10),), 4), 4: (4, "VMJumpIf", (("1", 0), ("4", 6)), 9),
                9: (9, "FlagSet", (("2", 0x20),), 13), 13: (13, "VMHalt", (), 15),
                15: (15, "FlagSet", (("2", 0x30),), 19), 19: (19, "VMHalt", (), 21)}
        for flag, cond in ((0x30, "flag 0x10 clear"), (0x20, "flag 0x10 set")):
            hits, _ = story.explore(code, 0, story.setter("flag", flag, None))
            self.assertEqual([story.merge([h[1]]) for h in hits], [[[cond]]])
        self.assertEqual(story.what_set(code[15]), "flag 0x30 set")

    def test_gen4_state_and_names(self):
        import route_tests
        d = route_tests.gen4_save(5, 3000, 0b101, [], [], [], (0x4001, 7), [0x0A, 2911], 71, (200, 313))
        st = story.State("pt", d)   # the newer slot's place, works (all 288 of them) and flags; map-local flags are 1..0x40
        self.assertEqual((st.zone, st.tile, st.works[0x4001], st.works[0x4000], len(st.works)), (71, (200, 313), 7, 0, 288))
        self.assertEqual([st.value(("flag", n)) for n in (0x0A, 0x0B, 2911)], [1, 0, 1])
        self.assertEqual(st.local_flags, range(1, 0x41))
        with self.assertRaises(ValueError):   # HGSS has no reader yet
            story.State("hg", d)
        saved, load = dict(story.NAMES4), story.load_names4
        story.load_names4 = lambda game: None
        story.NAMES4.update({0x4001: "VAR_SOMETHING", 0x0A: "FLAG_SOMETHING"})
        try:
            self.assertEqual(story.show(("cmp", ("work", 0x4001), 1, ("c", 7))), "work 0x4001 VAR_SOMETHING == 7")
            self.assertEqual(story.what_set((0, "FlagSet", (("2", 0x0A),), 4)), "flag 0xa FLAG_SOMETHING set")
            out = story.state_lines("pt", st)   # flag 2911 is a daily flag: left out
            self.assertIn("tile 200,313; 1 works nonzero, 1 flags set (1 named)", out[0])
            self.assertEqual(out[1:], ["work 0x4001 VAR_SOMETHING = 7", "flag 0xa FLAG_SOMETHING"])
            self.assertEqual(story.works_text(st, [0x4000, 0x4001]), "VAR_SOMETHING=7")
        finally:
            story.load_names4 = load
            story.NAMES4.clear()
            story.NAMES4.update(saved)

    def test_script_entries_are_the_scripts_in_order(self):
        text = "    ScriptEntry A_b\n    ScriptEntry C\n    ScriptEntryEnd\n\nA_b:\n    End\n"
        self.assertEqual(story.script_entries(text), ["A_b", "C"])

    def test_rom_platinum_next_from_a_save(self):
        import route
        if not (story.ROOT / "orig/baserom_plat_us.nds").exists() or not (route.SAVES / "pt-barry.bin").exists() or \
                not (story.ROOT / "third_party/pokeplatinum/res/field/maps").exists():
            self.skipTest("no Platinum ROM, pokeplatinum or pt-barry save")
        out = "\n".join(story.next_steps(story.Game("pt"), story.load_state("pt", "pt-barry")))
        self.assertIn("pt save: zone 413 tile 3,5", out)
        # a trigger: the zone constant, its tile, the script number and its label, a changed flag by name
        self.assertRegex(out, r"zone \d+ \(\w+\) G:trig:\d+ \(\d+,\d+\) \(script \d+ \w+\): .*flag 0x[0-9a-f]+ FLAG_\w+")

    def test_rom_platinum(self):
        if not (story.ROOT / "orig/baserom_plat_us.nds").exists() or \
                not (story.ROOT / "third_party/pokeplatinum/src/scrcmd.c").exists():
            self.skipTest("no Platinum ROM or pokeplatinum")
        g = story.Game("pt")
        head = story.check(g)[0]   # pt: 219763 commands decoded, 11 spots that don't decode
        n, bad = map(int, re.findall(r"\d+", head)[:2])
        self.assertGreater(n, 200000)
        self.assertLess(bad, 30)
        # the new game starts in the player's room (zone 415): its scene table row runs script 3 while work 0x40f9 is 0,
        # and script 3 sets it to 1
        self.assertIn((1, 3, 0x40f9, 0), g.scene(415))
        self.assertTrue(story.who_sets(g, "work", 0x40f9, 1)[0].startswith("zone 415 script 3 @"))
        st = story.load_state("pt", "new")
        self.assertEqual(st.zone, 415)
        self.assertRegex("\n".join(story.next_steps(g, st)),
                         r"zone 415 \(TWINLEAF_TOWN_PLAYER_HOUSE_2F\) scene 1 \(script 3 \w+\): work 0x40f9 VAR_\w+ = 1")
        # Twinleaf Town (zone 411): trigger 1 runs script 2 while work 0x40e6 is 0, and it sets the work to 1
        out = story.show_zone_scripts(g, 411)
        self.assertRegex(out[2], r"^script 2 \w+: trigger 1 \(105,876\) when work 0x40e6 == 0")
        self.assertRegex(out[2], r"work 0x40e6 VAR_\w+ = 1")
        self.assertEqual(story.zone_names("pt")[415], "TWINLEAF_TOWN_PLAYER_HOUSE_2F")

    def test_rom_black_decodes(self):
        self.rom_decodes("b", 200000)

    def test_rom_black2_decodes(self):   # with the script plugins' commands (ids from 1000)
        self.rom_decodes("b2", 300000)


if __name__ == "__main__":
    unittest.main()
