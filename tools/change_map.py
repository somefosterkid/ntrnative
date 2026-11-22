#!/usr/bin/env python3
"""change_map.py: ONE map from a changed path to the kind of change it is, shared by the merge gate's selector
(tools/gate_select.py) and the merge round's selector (round_select.py, outside the repo; it loads this file from --repo).
Before this, the two kept their own copies and disagreed (a port/machine/android-*.ini change was 'shared port code' to the gate:
switch, touch, a 572 s build for a file no Linux build reads).
Kinds (a path can be in more than one set; the selectors decide what each kind needs):
  PLAT_ONLY       Platinum only: no Gen 5 check, no HG/SS check looks at it
  HG_ONLY         HeartGold/SoulSilver only (the decomp, its patches, headers, host files, generators)
  OTHER_PLATFORM  Android/iOS/Metal host files and their machine files: in no Linux build
  WIN_ONLY        Windows only: the exe (win64_*, machine/win64*.ini)
  RENDER_ONLY     port/gpu/: pictures only
Nothing here is a test list: what a kind runs is each selector's business (gate: cores/apk; round: legs, chains, exe)."""
import re

PLAT_ONLY = re.compile(
    r'^(port/game/(route_gen4|plat_online)\.c|port/include_plat/|port/include_gen4/'
    r'|port/subprojects/pokeplatinum[^/]*\.patch|third_party/pokeplatinum|config/plat_us/|lib/nnsys/src/.*_gen4\.c'
    r'|tools/(pokeplat_[a-z_]+|plat_symbols|atlas_gen4)(_tests)?\.py|tools/routes/pt\.txt|tools/plays/platinum/)')
HG_ONLY = re.compile(
    r'^(third_party/pokeheartgold/|port/include_hgss/|port/hgss_[a-z_]+\.(txt|tsv)|port/subprojects/pokeheartgold[^/]*\.patch'
    r'|port/game/hg_[a-z_]+\.c|tools/(?![a-z_]*_tests\.py)(pokehg_[a-z_]+|hg_assets)\.py)')
OTHER_PLATFORM = re.compile(r'^port/src/(android_|ios_|apple_|metal_|imgui_impl_metal)|^port/src/[^/]*\.mm?$|^port/machine/android-')
WIN_ONLY = re.compile(r'^port/src/win64_|^port/machine/win64[^/]*\.ini$')
RENDER_ONLY = re.compile(r'^port/gpu/')
# the names a selector copies (round_select.py's module-level fallback is replaced by these when the repo has this file)
NAMES = ('PLAT_ONLY', 'HG_ONLY', 'OTHER_PLATFORM', 'WIN_ONLY', 'RENDER_ONLY')
