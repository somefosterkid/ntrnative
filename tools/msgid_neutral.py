#!/usr/bin/env python3
"""Rename message-ID names that carry game text to neutral names made of the bank and the message index only.

A message-ID header holds lines like

    #define Title_Text_WouldLikeSaveLast 0

where the words after `_Text_` are the first words of the message: game text, which must not sit in the repository.
This tool renames every such ID to `<Prefix>_Text_<NNN>` (NNN: the message index in decimal, at least 3 digits), so
`Title_Text_000`. The prefix (the bank's label) stays. A name that is defined with different indices (an #ifdef BLACK2
branch, or two headers with one prefix) cannot take its name from one index and becomes `<Prefix>_Text_Var<k>`
(k counted per prefix in the order of the sorted names). A name whose define has no number (an alias such as
`#define X_Text_VersionName X_Text_Shiro`) keeps its own name; the names inside its value are renamed.

All uses are rewritten too, in the text files under --dirs. The tool is deterministic and idempotent: a name whose part
after `_Text_` is already NNN or VarK is left alone, so a second run changes nothing.

    python3 tools/msgid_neutral.py                    # this tree: headers include/text, uses in src include ...
    python3 tools/msgid_neutral.py --check            # change nothing; exit 1 when something would change
    python3 tools/msgid_neutral.py --root DIR --headers build/include/generated/text \\
        --dirs src lib include build/include/generated   # another tree (pokebw2)

--all-ids (pret's Platinum banks, run by tools/pokeplat_gen.py): every numeric #define in the headers is a message ID,
also one without `_Text_` (`BoxText_PickOne`, `GenericNames_PlayerMale_3`, `Text_Gts_X`): it becomes
`<Head>_Text_NNN`, Head = the name's first `_` part (after a leading `Text_`). Kept as they are (KEEP_RE): ALL_CAPS
constants (entry counts, met-location names), pret's numbered `pl_msg_NNNNNNNN_NNNNN` and `species_name_<species>`
(the species constants' own names, used all over the C as SPECIES_*). Uses are then matched as whole identifiers.
"""
import argparse
import os
import re
import sys
from pathlib import Path

NAME_RE = re.compile(r'(?<![A-Za-z0-9])([A-Za-z0-9]+)_Text_([A-Za-z0-9_]+)(?![A-Za-z0-9_])')
DEFINE_RE = re.compile(r'^\s*#\s*define\s+([A-Za-z0-9]+_Text_[A-Za-z0-9_]+)\s+(0[xX][0-9a-fA-F]+|[0-9]+)\b')
ANY_DEFINE_RE = re.compile(r'^\s*#\s*define\s+([A-Za-z_][A-Za-z0-9_]*)\s+(0[xX][0-9a-fA-F]+|[0-9]+)\b')
KEEP_RE = re.compile(r'^(?:[A-Z0-9_]+|pl_msg_\d+_\d+|species_name_\w+)$')
IDENT_RE = re.compile(r'(?<![A-Za-z0-9_])[A-Za-z_][A-Za-z0-9_]*')
NEUTRAL_RE = re.compile(r'^(\d+|Var\d+)$')
BRANCH_RE = re.compile(r'^\s*#\s*(if|ifdef|ifndef|elif|else|endif)\b')
SKIP_DIRS = {'.git', 'build', 'orig', 'extract', '__pycache__', 'node_modules'}
SELF = {'msgid_neutral.py', 'msgid_neutral_tests.py'}


def is_neutral(name):
    m = NAME_RE.fullmatch(name)
    return bool(m) and bool(NEUTRAL_RE.match(m.group(2)))


def header_files(root, headers):
    out = []
    for h in headers:
        base = Path(root) / h
        if base.is_file():
            out.append(base)
        elif base.is_dir():
            out.extend(sorted(p for p in base.rglob('*.h')))
    return sorted(set(out))


def read(p):
    try:
        return p.read_bytes().decode('utf-8')
    except (UnicodeDecodeError, OSError):
        return None


def split_name(name):
    """(prefix, the part to neutralise): `P_Text_S` -> (P, S); with --all-ids `Head_Rest` -> (Head, Rest)."""
    if '_Text_' in name:
        return tuple(name.split('_Text_', 1))
    parts = name.split('_')
    if parts[0] == 'Text' and len(parts) > 2:
        parts = parts[1:]
    return parts[0], '_'.join(parts[1:])


def collect(files, all_ids=False):
    """name -> set of integer values (numeric defines only)."""
    values = {}
    for p in files:
        txt = read(p)
        if txt is None:
            continue
        for line in txt.split('\n'):
            m = (ANY_DEFINE_RE if all_ids else DEFINE_RE).match(line)
            if m and not (all_ids and (KEEP_RE.match(m.group(1)) or '_' not in m.group(1))):
                values.setdefault(m.group(1), set()).add(int(m.group(2), 0))
    return values


def build_map(values):
    """name -> neutral name. Raises ValueError when two names would collide with different values."""
    mapping = {}
    var_n = {}
    for name in sorted(values):
        pre, suf = split_name(name)
        if NEUTRAL_RE.match(suf):
            continue
        if len(values[name]) == 1:
            mapping[name] = '%s_Text_%03d' % (pre, next(iter(values[name])))
    for name in sorted(values):
        pre, suf = split_name(name)
        if NEUTRAL_RE.match(suf) or len(values[name]) == 1:
            continue
        k = var_n.get(pre, 0)
        while '%s_Text_Var%d' % (pre, k) in values:
            k += 1
        var_n[pre] = k + 1
        mapping[name] = '%s_Text_Var%d' % (pre, k)
    # a neutral name must stand for one value only (a VarK name stands for several, by design)
    seen = {}
    for name, vs in values.items():
        seen.setdefault(mapping.get(name, name), set()).update(vs)
    for n, vs in seen.items():
        if len(vs) > 1 and '_Text_Var' not in n:
            raise ValueError('neutral name %s would stand for the values %s' % (n, sorted(vs)))
    return mapping


def sub_names(text, mapping, all_ids=False):
    return (IDENT_RE if all_ids else NAME_RE).sub(lambda m: mapping.get(m.group(0), m.group(0)), text)


def rewrite_header(txt, mapping, all_ids=False):
    """The header's lines renamed; a numeric define whose neutral name and value were just defined in the same
    #if branch is dropped."""
    out = []
    seen = set()
    for line in txt.split('\n'):
        if BRANCH_RE.match(line):
            seen = set()
        m = (ANY_DEFINE_RE if all_ids else DEFINE_RE).match(line)
        if m and m.group(1) in mapping:
            key = (mapping[m.group(1)], int(m.group(2), 0))
            if key in seen:
                continue
            seen.add(key)
        out.append(sub_names(line, mapping, all_ids))
    return '\n'.join(out)


def walk_files(root, dirs):
    root = Path(root)
    for d in dirs:
        base = root / d
        if base.is_file():
            yield base
            continue
        for dp, dn, fn in os.walk(base):
            dn[:] = sorted(x for x in dn if x not in SKIP_DIRS)
            for f in sorted(fn):
                yield Path(dp) / f
    for f in sorted(root.glob('*.md')):
        yield f


def run(root, headers, dirs, check=False, out=sys.stdout, all_ids=False):
    root = Path(root)
    hfiles = header_files(root, headers)
    values = collect(hfiles, all_ids)
    mapping = build_map(values)
    hset = set(hfiles)
    changed = []
    seen = set()
    for p in list(hfiles) + list(walk_files(root, dirs)):
        if p in seen or p.name in SELF:
            continue
        seen.add(p)
        txt = read(p)
        if txt is None or not (all_ids or '_Text_' in txt):
            continue
        new = rewrite_header(txt, mapping, all_ids) if p in hset else sub_names(txt, mapping, all_ids)
        if new != txt:
            changed.append(p)
            if not check:
                p.write_bytes(new.encode('utf-8'))
    print('ids %d, renamed %d (of which VarK %d), files %s %d' % (
        len(values), len(mapping), sum(1 for v in mapping.values() if '_Text_Var' in v),
        'to change' if check else 'changed', len(changed)), file=out)
    return changed


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--root', default=str(Path(__file__).resolve().parent.parent))
    ap.add_argument('--headers', nargs='+', default=['include/text'])
    ap.add_argument('--dirs', nargs='+', default=['src', 'include', 'lib', 'port', 'tools', 'docs', 'config'])
    ap.add_argument('--check', action='store_true')
    ap.add_argument('--all-ids', action='store_true', help='every numeric define in the headers is an ID (pret)')
    a = ap.parse_args(argv)
    changed = run(a.root, a.headers, a.dirs, a.check, all_ids=a.all_ids)
    return 1 if (a.check and changed) else 0


if __name__ == '__main__':
    sys.exit(main())
