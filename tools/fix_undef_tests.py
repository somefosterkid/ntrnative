#!/usr/bin/env python3
"""Tests of tools/fix_undef.py: of the overlays at a shared address, the one the caller's overlay is laid out on
when no relocation of its module tells (wifi_p2pmatch.c's TouchBar call is ov067's function, not ov098's at the same
address, which our code defines too); the module's own relocations still come first (ov207 reaches ov206, not
ov093 below it).

    python3 tools/fix_undef_tests.py
"""
import collections
import contextlib
import io
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import fix_undef as fu  # noqa: E402

P2P = "src/ov099/wifi_p2pmatch.c"


def pick(y, src, loads=None):
    real = fu.co_loaded
    if loads is not None:
        fu.co_loaded = lambda s: collections.Counter(loads)
    try:
        return fu.pick_overlay(y, re.match(r"(data|func)_ov(\d+)_([0-9a-f]{8})$", y), src, "func")
    finally:
        fu.co_loaded = real


def test_ancestors_follow_the_layout():
    under = fu.ancestors(99)
    assert under[:2] == [67, 13], under  # ov099 starts where ov067 ends, ov067 where ov013 does
    assert 98 not in under
    assert fu.ancestors(10 ** 6) == []


def test_shared_address_takes_the_overlay_the_caller_is_laid_out_on():
    # no module:overlay(N) relocation of ov099 tells: before, the pick went to the name our code defines, and both
    # ov067's touchbar.c and ov098's pdc_main.c define one at 0x021b6578 ("SHARED address ... by use, check")
    for y in ("func_ov098_021b6578", "func_ov067_021b6578"):
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            assert pick(y, P2P, loads={}) == "func_ov067_021b6578", y
        assert "laid out on" in out.getvalue() and "SHARED" not in out.getvalue(), out.getvalue()


def test_the_module_s_relocations_come_first():
    # ov207 is laid out on ov094 on ov093, but its relocations reach ov206 four times at ov206-only addresses
    src = next(l[:-1] for l in (ROOT / "config/b_us/arm9/overlays/ov207/delinks.txt").read_text().splitlines()
               if l.endswith(".c:"))
    assert 93 in fu.ancestors(207)
    assert pick("func_ov093_021ba534", src) == "func_ov206_021ba534"


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
