#!/usr/bin/env python3
"""Tests of tools/compiler_probe.py: a call of a function the file defines that Black lacks is not a match
(field_buildmodel.c called a second copy of a function and matched); a static table is judged by where it lands, not
by its name (the C's statics often carry another table's address in their name), so a read of the wrong table is no
match (gaps.md 13m: md5.c); a call of a name that nothing in the link defines is no match (bsubway_scr.c).

    python3 tools/compiler_probe_tests.py
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import compiler_probe as cp  # noqa: E402

MD5 = ROOT / "lib/nitrowifi/src/ssl/md5.c"


def test_same_place_of_a_function_black_lacks():
    known = next(n for n in ("CPSi_md5_calc", "MI_CpuCopy8") if cp.symbol_address(n) is not None)
    assert cp.same_place("NotBlacks_Copy", known)  # an extern unknown to the symbols: by undefined()
    assert not cp.same_place("NotBlacks_Copy", known, {"NotBlacks_Copy"})  # defined in the file: another function
    assert cp.same_place(known, known, {known})


def test_a_call_of_a_name_nothing_defines_is_no_match():
    # bsubway_scr.c called Black 2's func_ov033_0217c11c, which neither Black's symbols nor any object of the link
    # define, and the judge said 32/32: a call's bytes are relocated, so only its symbol tells
    assert cp.undefined("func_ov033_0217c11c") and cp.undefined("func_ov033_0217c11c+0x4")
    assert not cp.undefined("MI_CpuFill8") and not cp.undefined("OVERLAY_1_ID")  # Black's; the linker script's
    assert not cp.undefined("func_ov033_0217c11c", {"func_ov033_0217c11c"})  # the compiled object defines it
    names = cp.link_names()
    other = next(n for n, os_ in names.items() if cp.symbol_address(n) is None and os_ != ("lcf",))
    assert not cp.undefined(other)  # another object of the link defines it, under a name Black's symbols lack
    assert cp.undefined(other, own=set(names[other]))  # only the file's own (older) objects do
    text = MD5.read_text()
    s = re.search(r"^void CPSi_md5_init[(].*?^[}]", text, re.M | re.S)
    body = s[0].replace("MI_CpuFill8(", "NotBlacks_Fill8(")
    assert body != s[0]
    copy = MD5.with_name(".md5_undefined_call.c")
    copy.write_text(text[:s.start()] + "void NotBlacks_Fill8(void *dest, u8 data, u32 size);" + chr(10) + body
                    + text[s.end():])
    try:
        results, diffs, failed = cp.probe_source(copy, functions={"CPSi_md5_init", "CPSi_md5_calc"}, original=MD5,
                                                 cwd=ROOT)
    finally:
        copy.unlink()
    assert not failed, failed
    assert list(results["CPSi_md5_calc"].values()) == ["MATCH"], results
    assert list(results["CPSi_md5_init"].values()) == ["1 symbols"], results
    assert any("NotBlacks_Fill8 is in no symbols.txt" in l for l in diffs), diffs


def test_static_table_named_apart_from_its_place_matches():
    # poke_tool.c is complete (it links byte for byte); its table is named data_0209e298 and lands at Black's
    # stat_nature_adjust (0x0209e2a4): a static table isn't judged by its name
    pt = ROOT / "src/system/poke_tool.c"
    results, _, failed = cp.probe_source(pt, functions={"isStatAdjustedForNature"}, original=pt, cwd=ROOT)
    assert not failed, failed
    assert list(results["isStatAdjustedForNature"].values()) == ["MATCH"], results


def test_wrong_static_table_does_not_match():
    text = MD5.read_text()
    assert "data_ov009_021a1de8" in text and "data_ov009_021a1da8" in text
    # CPSi_md5_result reads the padding table; the other u8[64] table compiles to the same bytes
    s = re.search(r"^void CPSi_md5_result[(].*?^[}]", text, re.M | re.S)
    copy = MD5.with_name(".md5_wrong_table.c")
    body = s[0].replace("data_ov009_021a1de8", "data_ov009_021a1da8")
    assert body != s[0]
    copy.write_text(text[:s.start()] + body + text[s.end():])
    try:
        results, diffs, failed = cp.probe_source(copy, functions={"CPSi_md5_result", "CPSi_md5_calc"}, original=MD5,
                                                 cwd=ROOT)
    finally:
        copy.unlink()
    assert not failed, failed
    assert list(results["CPSi_md5_calc"].values()) == ["MATCH"], results
    assert list(results["CPSi_md5_result"].values()) == ["1 symbols"], results
    assert any("data_ov009_021a1da8 (0x21a1da8)" in l and "data_ov009_021a1de8 (0x21a1de8)" in l for l in diffs), diffs


def test_static_misses_falls_back_to_the_bytes_where_the_layout_is_not_blacks():
    # md5.c's .data placed 0x200 off: no read lands on Black's, so the bytes decide: the padding table read for the
    # padding table passes, the other table read for it doesn't; with no .data range nothing is placed: the bytes too.
    # A static of Black's name passes whatever its bytes (the file's data to fix, not the read)
    obj = ROOT / "build/b_us/lib/nitrowifi/src/ssl/md5.o"
    if not obj.exists():
        return
    statics = {}
    cp.object_functions(obj, statics)
    originals = cp.delinked_functions(cp.delinked_object(MD5))
    sizes = {n: len(v[0]) for n, v in originals.items()}
    real = cp.source_ranges
    try:
        cp.source_ranges = lambda source: ("ov009", {".data": (0x021a1da8 + 0x200, 0x021a1f28 + 0x200)})
        assert cp.static_misses(obj, MD5, statics, originals, sizes) == {}
        moved = {n: {o: (sec, 0 if label == "data_ov009_021a1de8" else off, label) for o, (sec, off, label) in f.items()}
                 for n, f in statics.items()}
        assert cp.static_misses(obj, MD5, moved, originals, sizes) == {}
        wrong = {n: {o: (sec, 0, "data_ov009_021a1da8") if label == "data_ov009_021a1de8" else (sec, off, label)
                     for o, (sec, off, label) in f.items()} for n, f in statics.items()}
        assert list(cp.static_misses(obj, MD5, wrong, originals, sizes)) == ["CPSi_md5_result"]
        cp.source_ranges = lambda source: ("ov009", {})
        assert list(cp.static_misses(obj, MD5, wrong, originals, sizes)) == ["CPSi_md5_result"]
    finally:
        cp.source_ranges = real


RESET_ITCM = ROOT / "lib/nitro/src/os/os_reset_itcm.c"


def test_word_into_a_thumb_function_matches_and_its_data_reads_as_data():
    # os_reset_itcm.c: OSi_VerifyHmac's pool word 0x01ff8368 holds the address of the key inside its own Thumb code
    # (+0x80). MWCC writes it OSi_VerifyHmac+0x7f (the link adds the Thumb bit), dsd +0x80: the judge said "1 symbols"
    # of a function that links byte for byte, and its diff read the original's key words as Thumb code (t35m)
    results, diffs, failed = cp.probe_source(RESET_ITCM, functions={"OSi_VerifyHmac"}, original=RESET_ITCM, cwd=ROOT)
    assert not failed, failed
    assert list(results["OSi_VerifyHmac"].values()) == ["MATCH"], (results, diffs)
    theirs = cp.object_functions(cp.delinked_object(RESET_ITCM))["OSi_VerifyHmac"]
    assert theirs[1].get(0xc0) == "OSi_VerifyHmac+0x80", theirs[1]
    marks = cp.inner_data("OSi_VerifyHmac", theirs[1], theirs[2], len(theirs[0]), True)
    assert (0x80, "$d") in marks and (0x80, "$d") not in theirs[2], marks
    listing = {a: text for a, _, text, _ in cp.disassemble(theirs[0], 0x01ff82a8, True, theirs[1], marks)}
    assert listing["01ff8328"] == ".word 0xdec00621" and listing["01ff8368"] == ".word OSi_VerifyHmac+0x80", listing
    assert listing["01ff8324"].startswith("pop") and listing["01ff8326"] == "mov r8, r8"  # the code before it
    # a version that differs elsewhere: the key is no hunk of its diff (the same words both sides)
    copy = RESET_ITCM.with_name(".os_reset_itcm_probe.c")
    text = RESET_ITCM.read_text()
    assert text.count("    mov r0, #0x1" + chr(10) + "@done:") == 1
    copy.write_text(text.replace("    mov r0, #0x1" + chr(10) + "@done:", "    mov r0, #0x2" + chr(10) + "@done:"))
    try:
        results, diffs, failed = cp.probe_source(copy, functions={"OSi_VerifyHmac"}, original=RESET_ITCM, cwd=ROOT)
    finally:
        copy.unlink()
    assert not failed, failed
    assert list(results["OSi_VerifyHmac"].values()) == ["1 bytes"], results
    assert not any("0xdec00621" in d or "lsls r1, r4, #0x18" in d for d in diffs), diffs
    assert any("movs r0, #2" in d for d in diffs), diffs


def main():
    tests = [v for k, v in sorted(globals().items()) if k.startswith("test_")]
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
