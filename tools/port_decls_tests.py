#!/usr/bin/env python3
"""Tests for tools/port_decls.py's placing of the C files' own prototypes and of names the headers beside C files
declare (prep's queue of 2026-10-24: card.h's CARDAccessLevel, NATify.h's NatNegPeer):

    python3 tools/port_decls_tests.py    # prints "ok" per test, exits 1 on a failure
"""
import os
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import port_decls as pd  # noqa: E402

U32 = ("include/types.h", pd.Item("typedef unsigned long u32;"))


def plan_with(proto, header="lib/dwc/include/dwc/NATify.h", ours=None):
    plan = pd.Planner({}, dict(ours or {"u32": U32}))
    item = pd.Item(proto)
    plan.protos[header].append((item, True))
    plan.placed[item.names[0]] = header
    return plan, item


def tree(files):
    root = tempfile.mkdtemp()
    for path, text in files.items():
        os.makedirs(os.path.dirname(f"{root}/{path}"), exist_ok=True)
        Path(f"{root}/{path}").write_text(text)
    return Path(root)


def test_a_c_files_prototype_naming_its_own_type_stays_out_of_the_header():
    root = tree({"lib/dwc/src/natneg/natneg.c": "typedef struct NatNegPeer { int x; } NatNegPeer;\n"
                 "u32 func_ov009_02198e80(NatNegPeer *peer) {\n    return 0;\n}\n"})
    plan, _ = plan_with("gsi_bool DetermineNatType(NAT *nat);".replace("DetermineNatType", "func_ov009_02198e80"))
    changed = pd.adopt_local_declarations(plan, root)
    assert plan.protos["lib/dwc/include/dwc/NATify.h"] == [], plan.protos
    assert "func_ov009_02198e80" not in plan.placed
    assert any("NatNegPeer" in c for c in changed), changed


def test_a_c_files_prototype_with_the_headers_types_is_adopted_and_includes_them():
    root = tree({"lib/nitro/src/card/card_common.c": "CARDAccessLevel CARDi_GetAccessLevel(void) {\n    return 0;\n}\n"})
    ours = {"u32": U32, "CARDAccessLevel": ("lib/nitro/include/nitro/card_types.h",
                                            pd.Item("typedef u32 CARDAccessLevel;"))}
    plan, item = plan_with("u32 CARDi_GetAccessLevel(void);", "lib/nitro/include/nitro/card.h", ours)
    pd.adopt_local_declarations(plan, root)
    assert item.text == "CARDAccessLevel CARDi_GetAccessLevel(void);", item.text
    assert "lib/nitro/include/nitro/card_types.h" in plan.includes["lib/nitro/include/nitro/card.h"], plan.includes


def test_a_type_of_a_header_beside_the_c_files_is_not_copied_into_a_public_one():
    plan = pd.Planner({}, {"u32": U32})
    plan.beside = {"CARDAccessLevel": "lib/nitro/src/card/card_internal.h"}
    plan.place(pd.Item("typedef u32 CARDAccessLevel;"), "lib/nitro/include/nitro/card.h")
    assert not plan.types["lib/nitro/include/nitro/card.h"], plan.types
    assert any("card_internal.h" in why for _, why in plan.skipped), plan.skipped


def test_private_names_reads_the_headers_beside_the_c_files():
    root = tree({"lib/nitro/src/card/card_internal.h": "typedef u32 CARDAccessLevel;\nCARDAccessLevel "
                 "CARDi_GetAccessLevel(void);\n", "include/nitro/x.h": "void PublicOne(void);\n"})
    names = pd.private_names(str(root))
    assert names.get("CARDi_GetAccessLevel") == "lib/nitro/src/card/card_internal.h", names
    assert "CARDAccessLevel" in names and "PublicOne" not in names, names


def ported_tree(file_includes_tables=True):
    """A repo and a pokebw2 checkout in miniature: btl_tables.c defines IsArceus, which btl_server_flow.h declares
    here and btl_tables.h in pokebw2; OtherFn is a named function of Black's that no header here declares."""
    head = '// From pokebw2 (GPL-3.0, x), src/ov169/btl_tables.c, ported by tools/port_file.py.\n'
    body = "BOOL IsArceus(u16 species) {\n    return species == 493;\n}\n"
    guard = "#ifndef {0}\n#define {0}\n#include \"types.h\"\n{1}#endif\n"
    root = tree({"include/types.h": "typedef int BOOL;\ntypedef unsigned short u16;\n",
                 "include/battle/btl_tables.h": guard.format("BTL_TABLES_H", ""), "include/gfl/overlay.h": "",
                 "include/battle/btl_server_flow.h": guard.format("BTL_SERVER_FLOW_H", "BOOL IsArceus(u16 species);\n"),
                 "config/b_us/arm9/symbols.txt": "IsArceus kind:function(arm,size=0x10) addr:0x02000000\n"
                                                 "OtherFn kind:function(arm,size=0x10) addr:0x02000010\n",
                 "build/twins.tsv": "module\taddress\tbw2_function\nmain\t0x02000000\tIsArceus\n",
                 "src/x/btl_tables.c": head + ('#include "battle/btl_tables.h"\n' if file_includes_tables else
                                               '#include "types.h"\n') + "\n" + body})
    bw2 = tree({"config/names.txt": "", "src/ov169/btl_tables.c": '#include "battle/btl_tables.h"\n\n' + body,
                "include/battle/btl_tables.h": guard.format("B_H", "BOOL IsArceus(u16 species);\n"),
                "include/other.h": guard.format("O_H", "void OtherFn(void);\n")})
    return root, bw2


def run_main(root, bw2, *args):
    saved = pd.ROOT, pd.twin_evidence, pd.black_constants.read_black, pd.clang_ast, sys.argv, os.getcwd()
    pd.ROOT, pd.twin_evidence, pd.clang_ast = root, lambda *a: None, lambda *a: None
    pd.black_constants.read_black = lambda *a: {}
    sys.argv = ["port_decls.py", "--bw2", str(bw2), "--twins", str(root / "build/twins.tsv"), *args]
    os.chdir(root)
    try:
        pd.main()
    finally:
        pd.ROOT, pd.twin_evidence, pd.black_constants.read_black, pd.clang_ast, sys.argv, _ = saved
        os.chdir(saved[-1])


def test_file_declares_only_what_that_file_uses():
    root, bw2 = ported_tree()
    run_main(root, bw2, "--apply", "--file", "src/x/btl_tables.c")
    assert not (root / "include/other.h").exists(), "--file ran the whole-tree pass"
    assert "IsArceus" in (root / "include/battle/btl_tables.h").read_text()


def test_a_file_that_reaches_no_header_of_its_function_includes_the_one_that_declares_it():
    root, bw2 = ported_tree(file_includes_tables=False)
    run_main(root, bw2, "--apply", "--file", "src/x/btl_tables.c", "--file-only")
    assert '#include "battle/btl_server_flow.h"' in (root / "src/x/btl_tables.c").read_text()
    assert "IsArceus" not in (root / "include/battle/btl_tables.h").read_text()


def test_a_prototype_that_disagrees_stays_where_it_is():
    root, bw2 = ported_tree()
    flow = root / "include/battle/btl_server_flow.h"
    flow.write_text(flow.read_text().replace("BOOL IsArceus(u16", "BOOL IsArceus(int"))
    run_main(root, bw2, "--apply", "--file", "src/x/btl_tables.c")
    assert "IsArceus" not in (root / "include/battle/btl_tables.h").read_text()
    assert '#include "battle/btl_server_flow.h"' in (root / "src/x/btl_tables.c").read_text()


def test_the_whole_tree_pass_is_a_run_without_file():
    root, bw2 = ported_tree()
    run_main(root, bw2, "--apply")
    assert "OtherFn" in (root / "include/other.h").read_text()


if __name__ == "__main__":
    failed = 0
    for name, fn in sorted(globals().items()):
        if name.startswith("test_") and callable(fn):
            try:
                fn()
                print("ok", name)
            except Exception as e:  # noqa: BLE001
                failed += 1
                print("FAIL", name, repr(e))
    sys.exit(1 if failed else 0)
