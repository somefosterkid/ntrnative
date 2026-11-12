#!/usr/bin/env python3
"""Tests of tools/rename.py on a small made-up tree: os_init -> OS_Init though an SDK header declares OS_Init and a
comment names it (t35h); a definition in the file that holds the old symbol (t35f's MIi_NDmaConfig); swaps and
rotations, in the tree and in one C file only (btlv_mcss.c's twin tables, t34i); the twins tables' Black names (not
pokebw2's); paths stay; the refusals.

    python3 tools/rename_tests.py
"""
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rename as R  # noqa: E402

FILES = {
    "config/b_us/arm9/symbols.txt": """os_init kind:function(arm,size=0xc) addr:0x02086368
func_02086374 kind:function(arm,size=0x10) addr:0x02086374
Taken_Name kind:function(arm,size=0x4) addr:0x02000000
data_0209d000 kind:data(any) addr:0x0209d000
data_0209d010 kind:data(any) addr:0x0209d010
data_0209d020 kind:data(any) addr:0x0209d020
data_02732efc kind:bss addr:0x02732efc
""",
    "config/b_us/arm9/delinks.txt": """    .text       start:0x02000000 end:0x020a0000 kind:code align:32

lib/nitro/src/os/os_init.c:
    .text       start:0x02086368 end:0x02086390

src/tables.c:
    .data       start:0x0209d000 end:0x0209d030

lib/twl/src/mi/mi_ndma.c:
    .bss        start:0x02732efc end:0x02732f3c
""",
    "config/b_us/arm9/relocs.txt": "from:0x02000010 kind:arm_call to:0x02086368 module:main\n",
    "config/b_us/twins_b_us.tsv": "black_symbol\tmodule\taddress\tsize\tbw2_source\tbw2_function\tcompilers\n"
                                  "os_init\tmain\t0x02086368\t12\tlib/nitro/src/os/os_init.c\tOS_Init\tdsi/1.1p1\n"
                                  "func_02086374\tmain\t0x02086374\t16\tsrc/x.c\tos_init\tdsi/1.1p1\n",
    "lib/nitro/sdk/nitro/os/common/init.h": "void OS_Init(void);\n",
    "lib/nitro/include/nitro/os.h": "// os_init is TwlSDK's OS_Init; OSi_InitCommon is func_02086374\nvoid os_init(void);\n",
    "lib/nitro/src/os/os_init.c": '#include "os_init.h"\nvoid os_init(void) {\n    func_02086374();\n}\n',
    "src/system/machine_use.c": 'void f(void) { os_init(); }\nconst char *s = "Other_Defined";\n',
    "src/other.c": "int Other_Defined = 1;\nstatic void Other_Static(void) {\n}\n",
    "src/tables.c": "const int data_0209d000[4] = {1};\nconst int data_0209d010[4] = {2};\nconst int data_0209d020[4] = {3};\n",
    "lib/twl/src/mi/mi_ndma.c": "static NDmaConfig MIi_NDmaConfig;\n",
    "config/b_us/arm9/ltd_autoload_0/symbols.txt": "data_02732da0 kind:bss addr:0x02732da0\n",
    "config/b_us/arm9/ltd_autoload_0/delinks.txt": "lib/nitro/src/card/card_rom@ltd.c:\n"
                                                  "    .bss        start:0x02732da0 end:0x02732e34\n",
    "lib/nitro/src/card/card_rom.c": "u32 cardi_rom_base;\nu8 *CARDiHashBufferAddress = NULL;\n",
    "docs/nonmatching.md": "| `os_init` | `0x02086368` | `lib/nitro/src/os/os_init.c` | 3 lines | x |\n",
}


def tree():
    root = Path(tempfile.mkdtemp(prefix="rename_"))
    for path, text in FILES.items():
        (root / path).parent.mkdir(parents=True, exist_ok=True)
        (root / path).write_text(text)
    return root


def read(root, path):
    return (root / path).read_text()


def refused(pairs, root, only=None):
    before = {p: read(root, p) for p in FILES}
    try:
        R.run(pairs, root, only)
    except R.Refused as e:
        assert {p: read(root, p) for p in FILES} == before, "a refusal changed files"
        return str(e)
    raise AssertionError(f"not refused: {pairs}")


def test_an_sdk_header_declares_the_new_name():
    root = tree()
    changed, notes = R.run({"os_init": "OS_Init"}, root)
    assert any("OS_Init is declared already, in lib/nitro/sdk/nitro/os/common/init.h" in n for n in notes), notes
    assert "OS_Init kind:function" in read(root, "config/b_us/arm9/symbols.txt")
    assert "lib/nitro/src/os/os_init.c:" in read(root, "config/b_us/arm9/delinks.txt")  # a path stays
    assert "config/b_us/arm9/delinks.txt" not in changed, changed
    assert '#include "os_init.h"' in read(root, "lib/nitro/src/os/os_init.c")
    assert "void OS_Init(void) {" in read(root, "lib/nitro/src/os/os_init.c")
    assert "OS_Init();" in read(root, "src/system/machine_use.c")
    twins = read(root, "config/b_us/twins_b_us.tsv")
    assert "\nOS_Init\tmain\t0x02086368" in twins, twins
    assert "src/x.c\tos_init\t" in twins, twins  # pokebw2's name of another function: not Black's, stays
    assert "| `OS_Init` |" in read(root, "docs/nonmatching.md")


def test_a_name_in_comments_only_is_free():
    root = tree()
    changed, notes = R.run({"func_02086374": "OSi_InitCommon"}, root)
    assert not notes, notes
    assert "OSi_InitCommon();" in read(root, "lib/nitro/src/os/os_init.c")


def test_a_definition_in_the_file_that_holds_the_symbol():
    root = tree()
    changed, notes = R.run({"data_02732efc": "MIi_NDmaConfig"}, root)
    assert changed == ["config/b_us/arm9/symbols.txt"], changed
    assert any("lib/twl/src/mi/mi_ndma.c, which holds data_02732efc: the same object" in n for n in notes), notes
    why = refused({"data_0209d000": "Other_Defined"}, root)  # another file's object
    assert "Other_Defined is defined already, in src/other.c" in why, why
    assert "defined already, in src/other.c" in refused({"data_0209d000": "Other_Static"}, root)


def test_a_definition_in_the_file_of_a_two_module_alias():
    # card_rom@ltd.c in ltd_autoload_0's delinks is card_rom.c's block there (tools/two_modules.py): its C holds it
    root = tree()
    changed, notes = R.run({"data_02732da0": "CARDiHashBufferAddress"}, root)
    assert changed == ["config/b_us/arm9/ltd_autoload_0/symbols.txt"], changed
    assert any("lib/nitro/src/card/card_rom.c, which holds data_02732da0: the same object" in n for n in notes), notes


def test_swaps_and_rotations():
    root = tree()
    R.run({"os_init": "func_02086374", "func_02086374": "os_init"}, root)
    syms = read(root, "config/b_us/arm9/symbols.txt")
    assert "func_02086374 kind:function(arm,size=0xc) addr:0x02086368" in syms, syms
    assert "os_init kind:function(arm,size=0x10) addr:0x02086374" in syms, syms
    # which names the C gives its tables, in its file only: symbols.txt keeps its names
    root = tree()
    rot = {"data_0209d000": "data_0209d010", "data_0209d010": "data_0209d020", "data_0209d020": "data_0209d000"}
    changed, _ = R.run(rot, root, ["src/tables.c"])
    assert changed == ["src/tables.c"], changed
    assert read(root, "src/tables.c") == ("const int data_0209d010[4] = {1};\nconst int data_0209d020[4] = {2};\n"
                                          "const int data_0209d000[4] = {3};\n"), read(root, "src/tables.c")
    assert read(root, "config/b_us/arm9/symbols.txt") == FILES["config/b_us/arm9/symbols.txt"]
    why = refused({"data_0209d000": "data_0209d010"}, root, ["src/tables.c"])  # half a swap
    assert "defined already, in src/tables.c" in why and "give all the pairs in one call" in why, why


def test_refusals():
    root = tree()
    why = refused({"os_init": "Taken_Name"}, root)
    assert "Taken_Name is a Black symbol's name already, in config/b_us/arm9/symbols.txt" in why, why
    assert "isn't anywhere" in refused({"func_09999999": "Fine"}, root)
    assert "two names would become Same" in refused({"os_init": "Same", "func_02086374": "Same"}, root)
    assert "not a name" in refused({"os_init": "9lives"}, root)
    assert "no such file" in refused({"os_init": "OS_Init"}, root, ["src/none.c"])
    changed, _ = R.run({"os_init": "OS_Init"}, root, dry=True)  # dry: said, nothing written
    assert "config/b_us/arm9/symbols.txt" in changed
    assert read(root, "config/b_us/arm9/symbols.txt") == FILES["config/b_us/arm9/symbols.txt"]


def test_defines():
    d = lambda text: R.defines(R.bare(text), "Name")  # noqa: E731
    assert d("void Name(int a) {\n}\n") and d("static u8 Name[4] = {0};\n") and d("NDmaConfig Name;\n")
    assert d("#define Name(x) (x)\n") and d("typedef int Name;\n") and d("static const u32 *Name;\n")
    assert not d("void Name(int a);\n") and not d("extern u8 Name[];\n") and not d("struct Name {\n    int x;\n};\n")
    assert not d("void f(int Name);\n") and not d("void f(void) {\n    int Name;\n}\n")
    assert not d("const void *tbl[] = { Name };\nint *p = &Name;\n") and not d("// void Name(void) {}\n")
    assert not d('const char *s = "int Name;";\n') and not d("void f(void) { Name(); }\n")


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
