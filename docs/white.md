# White

`ninja` builds Pokémon White (USA/EUR, NDSi Enhanced, sha1 bc696a0dfb448c7b3a8a206f0f8214411a039208) from the same
source as Black when its ROM is in orig/ as `baserom_w_us.nds`: build/pokewhite_us.nds. Without it only Black is built.

## How its config is kept

White is Black with a few functions and tables changed. Every module but main and the title screen's overlay (ov088)
has the same size, and all but about a hundred of Black's 45,000 functions and data symbols are in White with the same
bytes at other addresses (main is 32 bytes longer, so the overlays load 32 bytes higher). So White's config is not a
second copy of Black's 35,000 symbols to keep in step by hand, as pokebw2 keeps Black 2's and White 2's:

- config/w_us/delta.txt (committed) holds only what differs: each module's shifts (from this Black address on, White's
  is Black's plus so much), White's sizes and relocations of the functions and data that differ, White's module hashes,
  and the files complete in Black that don't match White yet (`incomplete <file>`: White links the code delinked from
  its ROM there).
- tools/version_config.py makes config/w_us/arm9/ and config/w_us/assets.tsv (not committed) from config/b_us and the
  delta. configure.py runs it, so a change to Black's config (a rename, a new file, a split) is White's at the next
  `ninja`. It fails when the delta no longer fits (a size of a function that isn't there, a file that's gone).
- tools/version_map.py writes the delta from both extracted ROMs: it finds each Black symbol in White (its bytes with
  the relocation sites masked), reads every relocation at its new place for the shift of its target, and takes what
  differs from dsd's own analysis of White (`dsd init`). It keeps the old delta's `incomplete` lines (a file made to
  match White stays complete; --fresh starts over).

The build defines WHITE for White (`-d WHITE`, as pokebw2 defines BLACK2 or WHITE2) and BLACK for Black;
include/constants/version.h makes GAME_VERSION VERSION_WHITE or VERSION_BLACK of it. The port compiles Black with neither
defined and, when White's ROM was there for its assets, White beside it with WHITE, in the same binary: on Linux, Windows
and Android every global renamed w_ by a header, on macOS and iOS as one relocatable object (docs/launcher.md, "Step 3 as
built", both sections). Code that differs tests the version one way: `#if GAME_VERSION == VERSION_WHITE`, a macro in the file or a
header when the same choice comes back. Black's assets are written to build/asset (the port reads them there), White's
to build/w_us/asset, each from its own ROM.

## Making a file match White

A file in the delta's `incomplete` lines is complete in Black and differs in White. Its worker judges against White
with `BW_VERSION=w_us` before the tools (tools/compiler_probe.py compiles with `-d WHITE` and compares with White's
bytes and relocations):

    BW_VERSION=w_us python3 tools/step.py src/ov010/trcard_sys.c

Black must still match: judge without BW_VERSION too. When all of the file matches White, remove its `incomplete`
line from config/w_us/delta.txt and run ninja: both ROMs must still be their sha1s.

build/w_us_todo.tsv (`python3 tools/version_map.py w_us --report build/w_us_todo.tsv`) lists every function and data
symbol that differs, with its file and how much.
