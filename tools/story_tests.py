#!/usr/bin/env python3
"""Tests of tools/story.py: synthetic bytecode and C, and (with the ROM) the decoder over every Black script."""
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
    def test_layout_from_c(self):
        c = ("static u16 helper(VM *vm, FieldScriptEnv *env) {\n    return VM_Read16(vm);\n}\n"
             "BOOL s0001_Cmd(VM *vm, FieldScriptEnv *env) {\n    u16 a = ScriptReadAny(vm, env);\n"
             "#if GAME_VERSION == VERSION_WHITE\n    VM_Read32(vm);\n#else\n    VM_Read8(vm);\n#endif\n"
             "    u8 f = *vm->pc;\n    vm->pc++;\n    helper(vm, env);\n    return FALSE;\n}\n")
        for white, want in ((False, "a112"), (True, "a412")):
            funcs = story.c_functions({"x.c": story.preprocess(c, white)})
            self.assertEqual(story.layout("s0001_Cmd", funcs, {}), want)

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

    def test_merge_joins_both_senses(self):
        a = ("cmp", ("flag", 1), 1, ("c", 1))
        b = ("cmp", ("work", 0x4000), 1, ("c", 3))
        self.assertEqual(story.merge([(a, b), (story.neg(a), b)]), [["work 0x4000 == 3"]])

    @unittest.skipUnless((story.ROOT / "orig/baserom_b_us.nds").exists(), "no Black ROM")
    def test_rom_black_decodes(self):
        g = story.Game("b")
        n = bad = 0
        for fid, idx, off in g.scripts():
            code, b = story.reach(g.files[fid], off, g.cmds)
            n, bad = n + len(code), bad + len(b)
        self.assertGreater(n, 200000)
        self.assertLess(bad, n // 100)


if __name__ == "__main__":
    unittest.main()
