#!/usr/bin/env python3
"""Tests for tools/veneers.py's delinks.txt rewrite (code_range()).

    python3 tools/veneers_tests.py      # "ok" per test, exit 1 on a failure
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import veneers as V  # noqa: E402

NL = chr(10)
TEXT = NL.join(["    .text       start:0x02000000 end:0x02100000 kind:code align:32", "",
                "src/a.c:", "    .text       start:0x02000100 end:0x02000200", "",
                "lib/twl/src/x_twl.c:", "    complete", "    .ltdmain    start:0x02700000 end:0x02700100",
                "    .bss        start:0x02701000 end:0x02701010", "", "src/b.c:",
                "    .text       start:0x02000200 end:0x02000300", ""])


def test_code_range_moves_a_text_end_and_start():
    t = V.code_range(TEXT, "src/a.c", 0x02000100, end=(0x02000200, 0x02000210))
    t = V.code_range(t, "src/b.c", 0x02000200, start=0x02000210)
    assert "src/a.c:" + NL + "    .text       start:0x02000100 end:0x02000210" in t, t
    assert "src/b.c:" + NL + "    .text       start:0x02000210 end:0x02000300" in t, t


def test_code_range_moves_an_ltdmain_end():
    # t36c: the rewrite matched .text only: an LTD file's range past its veneers was left as it was
    t = V.code_range(TEXT, "lib/twl/src/x_twl.c", 0x02700000, end=(0x02700100, 0x02700108))
    assert "    .ltdmain    start:0x02700000 end:0x02700108" in t, t
    assert "    .bss        start:0x02701000 end:0x02701010" in t, t


def main():
    failed = 0
    for name, fn in sorted(globals().items()):
        if name.startswith("test_") and callable(fn):
            try:
                fn()
                print(f"ok {name}")
            except Exception as e:  # noqa: BLE001
                failed += 1
                print(f"FAIL {name}: {type(e).__name__}: {e}")
    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
