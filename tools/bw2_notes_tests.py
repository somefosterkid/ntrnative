#!/usr/bin/env python3
"""Tests for tools/bw2_notes.py: pokebw2's notes on the twin's function in the brief (s755, s768 read them by hand).

    python3 tools/bw2_notes_tests.py      # "ok" per test, exit 1 on a failure
"""
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import bw2_notes as N  # noqa: E402

NONMATCHING = """# Nonmatching functions

| File | Function | Address | Difference |
| --- | --- | --- | --- |
| `src/x/a.c` | `Foo_Draw` | `0x0200` / same | 0x30 against 0x34: the original keeps `r4`. Tried more. |
| `src/x/b.c` | `Other` | `0x0300` / same | Mentions `Foo_Draw` in passing. |
"""
MATCHING = """# How MWCC compiles

## Loops

- A rule stated first. Another sentence that is long enough to push the paragraph past the cap of a note, and it goes
  on with more words about registers and slots and the order of loads and stores and what MWCC does with them all.
  `a.c`'s `Foo_Draw` matches so, with its buffer typed `u32 *`. The sentence after it. A last sentence to cut.
- Nothing about it.

## Switches

- `Bar` only.
"""


def with_docs(fn):
    with tempfile.TemporaryDirectory() as d:
        Path(d, "nonmatching-functions.md").write_text(NONMATCHING)
        Path(d, "matching.md").write_text(MATCHING)
        old = N.DOCS, N.CAP
        N.DOCS, N.CAP = Path(d), 250
        N._BLOCKS.clear()
        try:
            fn()
        finally:
            N.DOCS, N.CAP = old
            N._BLOCKS.clear()


def test_rows_first_then_paragraphs_cut_to_the_name():
    def check():
        found = N.notes(["func_ov1_02000000", "Foo_Draw"])
        assert [(d, line) for d, line, _, _ in found] == [("nonmatching-functions.md", 5),
                                                          ("nonmatching-functions.md", 6), ("matching.md", 5)], found
        assert found[0][3] == "`src/x/a.c`: 0x30 against 0x34: the original keeps `r4`. Tried more.", found[0]
        para = found[2][3]
        assert found[2][2] == "Loops" and "`Foo_Draw` matches so" in para and "The sentence after it." in para
        assert len(para) <= N.CAP + 10 and "A last sentence to cut." not in para, para
        block = N.brief_block(["func_ov1_02000000", "Foo_Draw"])
        assert block.startswith("pokebw2's notes naming `func_ov1_02000000`, `Foo_Draw`") and \
            "- matching.md:5 (Loops): " in block, block
        assert N.brief_block(["Nowhere", None]) == "" and N.notes([None]) == []
    with_docs(check)


def test_a_name_inside_another_is_no_mention():
    def check():
        assert N.notes(["Foo"]) == [] and N.notes(["Draw"]) == []
    with_docs(check)


def main():
    bad = 0
    for name, fn in sorted(globals().items()):
        if name.startswith("test_"):
            try:
                fn()
                print("ok", name)
            except Exception as e:  # noqa: BLE001
                bad += 1
                print("FAIL", name, type(e).__name__, e)
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
