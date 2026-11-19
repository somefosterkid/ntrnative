#!/usr/bin/env python3
"""tools/hg_vendor.py: third_party/pokeheartgold, the HeartGold/SoulSilver C the port builds (docs/hgss.md, "Vendored
tree"), as plain files: the paths the port's build reads (KEEP) from a decompilation clone at a commit, and the headers
pret's generators write from its files/ (gen/: message IDs and NARC member names, no data). Everything else of the
decomp stays out: files/ (the game's assets and text), tools/, graphics, sound, the asm with functions (port/nonmatching
has their C), docs. PIN says the commit.

    python3 tools/hg_vendor.py SRC [--rev COMMIT]   # SRC: a clone of the decomp (default rev: its HEAD)

tools/pokehg_gen.py --src SRC --rev COMMIT runs the generators in build/pokeheartgold-vendor (once per commit), then
third_party/pokeheartgold is emptied and filled. Commit it with the port's patch updates; tools/pokehg_gen.py applies the
patches to its copy in build/pokeheartgold as before. The data's ROM bytes become blanks plus rows (tools/hg_assets.py:
needs both ROMs in orig/ and the HG port build's compile commands). Then run tools/rom_leak_check.py and
tools/rom_text_check.py over it.
"""
import argparse, io, os, re, shutil, subprocess, sys, tarfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import pokehg_gen  # noqa: E402

ROOT = pokehg_gen.ROOT
DEST = os.path.join(ROOT, pokehg_gen.SUB)
WORK = os.path.join(ROOT, 'build/pokeheartgold-vendor')
# the decomp's paths the port's build reads (port/meson.build, tools/pokehg_cc.py, tools/pokehg_asm.py): the C and its
# headers, libspl's C (tools/port_overlays.py HG_LIBS), main.lsf.in (the object list), the data-only .s
# (tools/pokehg_asm.py) and their includes, the script
# headers under files/ the C includes, the port's C for the asm functions and the typed asm data
KEEP = re.compile(r'^(main\.lsf\.in|include/.*\.h|src/.*\.[ch]|asm/.*\.inc|lib/.*\.h|lib/spl/src/[^/]*\.c'
                  r'|files/fielddata/script/scr_seq/[^/]*\.h|port/(nonmatching|asmdata)/.*)$')
TOPS = ['main.lsf.in', 'include', 'src', 'asm', 'lib', 'files/fielddata/script/scr_seq', 'port/nonmatching',
        'port/asmdata']
FUNC = re.compile(r'\b(thumb_func_start|arm_func_start|glabel|non_word_aligned_thumb_func_start)\b')
GEN_COMMENT = re.compile(rb'(?m)^[ \t]*//.*\n')   # the message text msgenc writes above each ID


def keep(path, data, linked):
    """Whether the vendored tree gets this file of the decomp (data: its bytes; linked: main.lsf.in's objects)."""
    if KEEP.match(path):
        return True
    # asm: only the data-only .s main.lsf.in links (tools/pokehg_asm.py's files(); the ones with code have C)
    return path in linked and path.endswith('.s') and not FUNC.search(data.decode('latin-1'))


def members(src, rev):
    """{path: bytes} of the kept files of src at rev."""
    tar = subprocess.check_output(['git', '-C', src, 'archive', '--format=tar', rev, '--'] + TOPS)
    with tarfile.open(fileobj=io.BytesIO(tar)) as t:
        files = {m.name: t.extractfile(m).read() for m in t if m.isfile()}
    linked = set(re.findall(r'Object\s+(\S+\.s)\b', files['main.lsf.in'].decode()))
    return {p: d for p, d in files.items() if keep(p, d, linked)}


def uncomment(lines):
    """lines (a file's, keepends) with the comments in them emptied: a `//` comment cut, a block comment's text dropped
    (its markers kept). For the lines tools/rom_text_check.py finds game text in: the decomp's comments quote messages."""
    out, block = [], False
    for ln in lines:
        nl = '\n' if ln.endswith('\n') else ''
        s, res, i = ln.rstrip('\n'), '', 0
        while i < len(s):
            if block:
                j = s.find('*/', i)
                if j < 0:
                    i = len(s)
                else:
                    res, block, i = res + '*/', False, j + 2
            elif s.startswith('/*', i):
                res, block, i = res + '/*', True, i + 2
            elif s.startswith('//', i):
                i = len(s)
            elif s[i] == '"':   # a string: copied whole
                j = i + 1
                while j < len(s) and s[j] != '"':
                    j += 2 if s[j] == '\\' else 1
                res, i = res + s[i:j + 1], j + 1
            else:
                res, i = res + s[i], i + 1
        if not block and res.strip() in ('', '*', '/**/'):
            res = res.rstrip() if res.strip() else ''
        out.append(res.rstrip() + nl)
    return out


def untext(files):
    """Each file's comment lines that hold game text (rom_text_check.py's, over every ROM it reads that is here: the
    public gate's check) emptied; the findings left (code, not comments) as (path, first line, last line, words, id)."""
    import rom_text_check as T
    idx = set()
    for r in T.ROMS:
        if os.path.exists(T.find_rom(os.path.join(ROOT, r))):
            T.rom_index(T.find_rom(os.path.join(ROOT, r)), idx)
    allow, left = T.load_allow(), []
    for path in sorted(files):
        text = files[path].decode('utf-8', 'replace')
        found = T.scan_text(text, idx, allow)
        if not found:
            continue
        lines = text.splitlines(True)
        # a block comment that starts above the run: its earlier lines say whether the run's lines are inside it
        for first, last, n, sid in found:
            a = first - 1
            while a > 0 and '/*' not in lines[a] and lines[a].lstrip().startswith('*'):
                a -= 1
            lines[a:last] = lines[a:first - 1] + uncomment(lines[a:last])[first - 1 - a:]
        files[path] = ''.join(lines).encode()
        left += [(path,) + f for f in T.scan_text(''.join(lines), idx, allow)]
    return left


def strip_root(data, roots):
    """data without the checkout's path, as the path (nitroarc's comment) and as the identifier nitroarc makes of it in the
    include guard (every other character an underscore): the file must not name the worktree it was made in"""
    for r in roots:
        data = data.replace(r.encode() + b'/', b'')
        data = data.replace(re.sub(rb'[^A-Za-z0-9]', b'_', r.encode() + b'/'), b'')
    return data


def vendor(src, rev, dest=DEST, work=WORK):
    commit = subprocess.check_output(['git', '-C', src, 'rev-parse', rev + '^{commit}'], text=True).strip()
    pokehg_gen.tree(src=src, rev=commit, work=work)
    files = members(src, commit)
    gen = os.path.join(work, 'gen')
    for dp, _, fs in os.walk(gen):
        for f in fs:
            data = open(os.path.join(dp, f), 'rb').read()
            # (nitroarc's .naix name the path they were made in: relative to the checkout)
            data = strip_root(data, {os.path.realpath(ROOT), os.path.abspath(ROOT)})
            files['gen/' + os.path.relpath(os.path.join(dp, f), gen)] = GEN_COMMENT.sub(b'', data)
    # the ROM bytes in the data (asm labels, C tables): blanks of their layout plus rows, filled from the ROM at start
    import hg_assets
    rows, problems = hg_assets.apply(files, work, os.path.join(ROOT, pokehg_gen.G4.hgss_build()), hg_assets.load_roms())
    hg_assets.write_rows(files, rows)
    for p in problems:
        print('hg_vendor: ROM bytes left (tools/hg_assets.py): ' + p)
    before = dict(files)
    left = untext(files)
    changed = [p for p in files if files[p] != before[p]]
    print('hg_vendor: game text cut from the comments of %d files (%d lines)' % (len(changed), sum(
        sum(a != b for a, b in zip(before[p].splitlines(), files[p].splitlines())) for p in changed)))
    shutil.rmtree(dest, ignore_errors=True)
    for path, data in files.items():
        p = os.path.join(dest, path)
        os.makedirs(os.path.dirname(p), exist_ok=True)
        open(p, 'wb').write(data)
    open(os.path.join(dest, 'PIN'), 'w').write(commit + '\n')
    for f in left:
        print('hg_vendor: still game text (code, not a comment: tools/rom_text_allow.txt): %s:%d-%d: %d words [%s]' % f)
    return commit, len(files)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    ap.add_argument('src', help='a clone of the decompilation')
    ap.add_argument('--rev', default='HEAD')
    o = ap.parse_args()
    commit, n = vendor(os.path.abspath(o.src), o.rev)
    print(f'hg_vendor: {pokehg_gen.SUB} = {commit[:9]}: {n} files')


if __name__ == '__main__':
    main()
