# Contributing to NTR Native

Thank you for helping. A few rules keep the project buildable and legal.

## Bring your own ROM

No ROM, game asset or anything extracted from one is in this repository, and none may be committed. You need your
own dumps in `orig/` (the file names and SHA-1s are in [README.md](README.md)). `orig/`, `build/` and `extract/` are
ignored by git, as are saves and signing keys.

- Game data never goes into C files. Arrays of graphics, sound or text bytes stay out: the build writes
  `build/asset/<name>.inc` from your ROM (see `config/b_us/assets.tsv`, `tools/asset.py`).
- `python3 tools/rom_leak_check.py` finds ROM bytes in tracked files (with your ROMs present; `--static` works without
  them). Run it on what you change before you open a pull request. CI runs the static check.
- Do not use leaked Nintendo or Game Freak material, and do not copy code from repositories that have no licence.
  Code from MIT, CC0 or GPL-compatible projects is welcome with credit in [THIRD_PARTY.md](THIRD_PARTY.md).

## The ROM must stay byte-identical

The decompilation rebuilds both ROMs from the same `src/`. A change that touches the DS build must keep both hashes:

```sh
python3 configure.py
ninja            # checks the SHA-1 of build/pokeblack_us.nds (and build/pokewhite_us.nds when White's dump is in orig/)
```

Black must give `26ad0b9967aa279c4a266ee69f52b9b2332399a5`, White `bc696a0dfb448c7b3a8a206f0f8214411a039208`.
A function you decompile is accepted when its object matches the original bytes (`tools/compiler_probe.py` tells
which functions match and shows a diff for the others). Write natural C: readable names and types, no copied
constants where a named value exists.

## Port changes

The native port (`port/`) builds with meson on top of the ROM build: see [docs/port-build.md](docs/port-build.md).
It must not change the DS ROMs. Renderer and sound changes are checked against the reference outputs described in
[docs/vcore.md](docs/vcore.md), [docs/gpu.md](docs/gpu.md) and [docs/acore.md](docs/acore.md).

## Tests

Tools with a `tools/*_tests.py` file run with `python3 tools/<name>_tests.py`. The ones that need no ROM also run in
CI (`.github/workflows/ci.yml`). Add a test file in the same style when you add a tool.

## Commits and pull requests

Plain commit messages that say what changed and why. Keep one topic per pull request. Do not include ROM data, build
output or saves.
