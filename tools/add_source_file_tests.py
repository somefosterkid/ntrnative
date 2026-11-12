#!/usr/bin/env python3
"""Tests for tools/add_source_file.py: a new file's entry goes where its address puts it (insert_entry()).

    python3 tools/add_source_file_tests.py      # "ok" per test, exit 1 on a failure
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import add_source_file as A  # noqa: E402

LINES = ["    .text       start:0x02000000 end:0x02100000 kind:code align:32",
         "    .rodata     start:0x02100000 end:0x02110000 kind:rodata align:4", "",
         "src/a.c:", "    complete", "    .text       start:0x02000100 end:0x02000200",
         "    .rodata     start:0x02100000 end:0x02100010", "",
         "src/data.c:", "    .rodata     start:0x02100010 end:0x02100040", "",
         "src/c.c:", "    .text       start:0x02000300 end:0x02000400",
         "    .rodata     start:0x02100040 end:0x02100050", ""]


def headers(lines):
    return [l for l in lines if l and not l[0].isspace()]


def test_a_new_file_goes_in_address_order():
    # t36k moved the new files' entries from the end of delinks.txt into order by hand
    entry = ["src/b.c:", "    .text       start:0x02000200 end:0x02000300"]
    out = A.insert_entry(LINES, entry)
    assert headers(out) == ["src/a.c:", "src/b.c:", "src/data.c:", "src/c.c:"], headers(out)  # a.c's .text next
    i = out.index("src/b.c:")
    assert out[i - 1] == "" and out[i + 2] == "", out[i - 1:i + 3]


def test_a_data_file_goes_by_its_section():
    entry = ["src/d2.c:", "    .rodata     start:0x02100008 end:0x02100010"]
    assert headers(A.insert_entry(LINES, entry)) == ["src/a.c:", "src/d2.c:", "src/data.c:", "src/c.c:"]


def test_the_last_file_goes_at_the_end():
    entry = ["src/z.c:", "    complete", "    .text       start:0x02000400 end:0x02000500"]
    out = A.insert_entry(LINES, entry)
    while out[-1] == "":
        out = out[:-1]
    assert headers(out)[-1] == "src/z.c:" and out[-3:] == entry and out[-4] == "", out


def test_real_delinks_a_file_goes_after_its_neighbour():
    # main's delinks.txt isn't in address order as a whole: a file goes right after the one of the nearest address
    lines = (A.ROOT / "config/b_us/arm9/delinks.txt").read_text().splitlines()
    out = A.insert_entry(lines, ["src/system/new.c:", "    .text       start:0x02023d00 end:0x02023d10"])
    i = out.index("src/system/new.c:")
    assert out[i - 1] == "" and out[i - 4] == "src/system/wipe_wnd.c:", out[i - 6:i + 2]
    assert out[i + 2] == "" and out[i + 3] == "src/system/brightness.c:", out[i:i + 4]


def test_the_first_file_goes_before_its_neighbour():
    entry = ["src/0.c:", "    .text       start:0x02000000 end:0x02000100"]
    assert headers(A.insert_entry(LINES, entry)) == ["src/0.c:", "src/a.c:", "src/data.c:", "src/c.c:"]


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
