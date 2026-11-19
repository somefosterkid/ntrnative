#!/usr/bin/env python3
"""The port's C unit tests (port/tests/run_*_test.sh that need nothing but a C compiler) from tools/run_tests.py:
python3 tools/port_unit_tests.py (prints "ok" per test, the expected failures of open review items, exits 1 on a failure)

- run_launcher_core_test.sh: ROM identification, save checks per game family, the playable names and edition lines
- run_launcher_plat_save_test.sh: Platinum's save check
- run_asset_load_test.sh: the game data fill (assets.tsv rows and the image's .bwfill copies) from synthetic ROMs
"""
import os
import shutil
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SCRIPTS = ("run_launcher_core_test.sh", "run_launcher_plat_save_test.sh", "run_asset_load_test.sh")


def run(script):
    """(exit code, standard output) of a port/tests script (its stderr: the core's own messages)."""
    done = subprocess.run(["sh", os.path.join(ROOT, "port", "tests", script)], cwd=ROOT, capture_output=True, text=True,
                          errors="replace", timeout=300)
    return done.returncode, done.stdout


def check(script):
    rc, out = run(script)
    lines = out.splitlines()
    for line in lines:
        if line.startswith(("xfail", "XPASS", "FAIL")):
            print("  " + line[:200])
    assert rc == 0 and lines and lines[-1] == "PASS", f"{script}: rc={rc}, last lines:\n" + "\n".join(lines[-6:])
    print("ok", script, f"({sum(1 for x in lines if x.startswith('ok'))} checks)")


def main():
    if os.name != "posix" or shutil.which("cc") is None:
        print("skip port_unit_tests: needs a POSIX host with cc")
        return 0
    bad = 0
    for script in SCRIPTS:
        try:
            check(script)
        except AssertionError as e:
            print("FAIL", e)
            bad += 1
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
