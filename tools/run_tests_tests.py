#!/usr/bin/env python3
"""Tests of tools/run_tests.py on a directory of small fake test files: exit codes, a timeout, the skip count, the
summary line, and that the files run from the repo root they are given.

    python3 tools/run_tests_tests.py
"""
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import run_tests as RT  # noqa: E402


def check(results, what, ok):
    results.append(ok)
    print(("ok  " if ok else "FAIL") + " " + what)


def main():
    results = []
    with tempfile.TemporaryDirectory() as d:
        root = Path(d)
        files = []
        for name, code in (("a_tests.py", "print('ok t')"),
                           ("b_tests.py", "import sys; print('FAIL t'); sys.exit(1)"),
                           ("c_tests.py", "print('skip t - no data'); print('skip u - no data')"),
                           ("d_tests.py", "import time; time.sleep(30)"),
                           ("e_tests.py", "import os; print(os.getcwd())")):
            (root / name).write_text(code)
            files.append(root / name)
        (root / "run_tests.py").write_text("raise SystemExit(1)")
        check(results, "the runner is not one of the files it runs", RT.test_files(root) == files
              and RT.test_files(root, ["b_", "e_"]) == [root / "b_tests.py", root / "e_tests.py"])
        seen = []
        got = RT.run_all(files, root, jobs=3, timeout=2, report=seen.append)
        by = {name: (rc, out) for name, rc, _, out in got}
        check(results, "results come in file order, each reported once", [r[0] for r in got] == [f.name for f in files]
              and len(seen) == 5)
        check(results, "exit codes: 0, 1, 0, a timeout (None), 0", [by[n][0] for n in sorted(by)] == [0, 1, 0, None, 0])
        check(results, "a file runs in the root it is given", by["e_tests.py"][1].strip() == str(root.resolve()))
        check(results, "a failure shows in its line", any(s.startswith("FAIL b_tests.py rc=1") for s in seen)
              and any(s.startswith("FAIL d_tests.py rc=timeout") for s in seen))
        line, bad = RT.summary(got, 3.0)
        check(results, "the summary counts files, failures and skipped tests",
              line.startswith("run_tests: 5 files, 3 ok, 2 failed, 2 skipped tests") and bad == ["b_tests.py", "d_tests.py"])
    print(f"{sum(results)} of {len(results)} passed")
    sys.exit(0 if all(results) else 1)


if __name__ == "__main__":
    main()
