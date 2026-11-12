#!/usr/bin/env python3
"""port/acore/meson.build's source list: every *.c in the directory given, one absolute path a line, sorted."""
import sys
from pathlib import Path

for p in sorted(Path(sys.argv[1]).glob("*.c")):
    print(p.resolve())
