#!/usr/bin/env python3
"""Runs every tools/*_tests.py from the repo root, a few at a time, and says in one line whether the suite is green.

    python3 tools/run_tests.py [-j N] [--timeout SECONDS] [NAME ...]

A file passes when it exits 0. A test that needs what the public repo does not have (the ROMs, the files extracted
from them, the ~/ref checkouts) skips itself and prints a line starting with "skip", which is counted but is no
failure. NAME words (any part of a file name) pick the files; none picks all. Per file: its exit code and seconds, then
the tail of a failing file's output and one summary line; the exit code is 1 when any file failed or timed out.
Jobs default to BW_JOBS or half the CPUs; a file is stopped after --timeout seconds (600).
"""
import argparse
import os
import subprocess
import sys
import time
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def test_files(tools, names=()):
    """The *_tests.py files of the directory, sorted, those with a NAME in the file name when names are given; not this
    tool itself (run_tests.py ends in _tests.py too, and running it runs the suite again, forever)."""
    return [p for p in sorted(Path(tools).glob("*_tests.py"))
            if p.name != Path(__file__).name and (not names or any(n in p.name for n in names))]


def run_one(path, root, timeout):
    """(file name, exit code (None on a timeout), seconds, output) of one test file run from the repo root."""
    start = time.time()
    try:
        done = subprocess.run([sys.executable, str(path)], cwd=root, capture_output=True, text=True, errors="replace",
                              timeout=timeout)
        rc, out = done.returncode, done.stdout + done.stderr
    except subprocess.TimeoutExpired as e:
        out = e.stdout or ""
        rc, out = None, out.decode(errors="replace") if isinstance(out, bytes) else out
    return path.name, rc, time.time() - start, out


def run_all(files, root=ROOT, jobs=4, timeout=600, report=print):
    """The results [(name, rc, seconds, output)] of the files, each reported as it finishes."""
    results = []
    with ThreadPoolExecutor(max_workers=max(1, jobs)) as pool:
        for res in pool.map(lambda f: run_one(f, root, timeout), files):
            name, rc, secs, out = res
            report(f"{'ok  ' if rc == 0 else 'FAIL'} {name} rc={'timeout' if rc is None else rc} {secs:.0f}s")
            results.append(res)
    return results


def summary(results, secs):
    """(the summary line, the failing file names) of run_all's results."""
    bad = [name for name, rc, _, _ in results if rc != 0]
    skips = sum(1 for _, _, _, out in results for line in out.splitlines() if line.startswith("skip"))
    line = f"run_tests: {len(results)} files, {len(results) - len(bad)} ok, {len(bad)} failed, {skips} skipped tests, {secs:.0f}s"
    return line + (": " + " ".join(bad) if bad else ""), bad


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("names", nargs="*", help="parts of the test file names to run (default: all)")
    ap.add_argument("-j", type=int, default=int(os.environ.get("BW_JOBS") or (os.cpu_count() or 2) // 2))
    ap.add_argument("--timeout", type=int, default=600, help="seconds a file may take")
    args = ap.parse_args(argv)
    files = test_files(ROOT / "tools", args.names)
    if not files:
        print("run_tests: no test file matches", " ".join(args.names))
        return 1
    start = time.time()
    results = run_all(files, ROOT, args.j, args.timeout, report=lambda s: print(s, flush=True))
    for name, rc, _, out in results:
        if rc != 0:
            print(f"--- {name}: last lines of its output")
            print("\n".join(out.splitlines()[-8:]))
    line, bad = summary(results, time.time() - start)
    print(line)
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
