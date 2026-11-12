#!/usr/bin/env python3
"""tools/pokeplat_fixups.py: the mechanical rewrites of pret pokeplatinum's C for the PC build, applied by
tools/pokeplat_fetch.sh on its checkout (build/pokeplatinum).

pret's tree has no licence, so the port commits no patch with its lines in it (a diff's context and removed lines
are pret's code): the rewrites are rules here (a pattern and its replacement, nothing of pret's text), and the port's
own headers (port/include_plat, port/include_gen4) supply the rest. Each rule says why. Idempotent.

  python3 tools/pokeplat_fixups.py ROOT     # prints the files it changed, by rule
"""
import os, re, sys

RULES = [
    # lib/nnsys's NNS_G2D_VRAM_TYPE is an untagged typedef (Black's NitroSystem); 071126 code spells the tag
    ('vram-enum-tag', re.compile(r'\benum\s+NNS_G2D_VRAM_TYPE\b'), 'NNS_G2D_VRAM_TYPE'),
]


def main():
    root = sys.argv[1]
    counts = {}
    for top in ('include', 'src', 'lib'):
        for dp, dn, fn in os.walk(os.path.join(root, top)):
            for f in fn:
                if not f.endswith(('.c', '.h')):
                    continue
                p = os.path.join(dp, f)
                s = open(p, encoding='utf-8', errors='surrogateescape').read()
                t = s
                for name, pat, rep in RULES:
                    t, n = pat.subn(rep, t)
                    if n:
                        counts[name] = counts.get(name, 0) + 1
                if t != s:
                    open(p, 'w', encoding='utf-8', errors='surrogateescape').write(t)
    for k, v in counts.items():
        print('%s: %d files' % (k, v))


if __name__ == '__main__':
    main()
