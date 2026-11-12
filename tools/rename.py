#!/usr/bin/env python3
"""Renames symbols everywhere: config/b_us's symbols.txt and relocations, the Black names of the twins tables
(twins_b_us*.tsv's first column: not pokebw2's names), the C, the headers, docs/nonmatching.md. Whole words only. A
new name that is a name already is refused (two symbols of one name don't link), and every refusal says why on stdout
(t18a: one on stderr alone looked like a silent exit) and exits 1 with nothing changed. No clash:
  - a mention in a comment, or in docs/nonmatching.md (a note naming pokebw2's);
  - a prototype or an extern, in a header or a C file: the SDK's name of the function (os_init -> OS_Init, dgt.h's
    MATHi_SetOverlayTableMode); printed, the declarations must agree (ninja says so);
  - a definition in the C file that holds the old symbol (its address in the file's ranges): the C gives the object
    that name already (t35f's MIi_NDmaConfig, a static of mi_ndma.c);
  - a name the pairs rename too: a swap or a rotation, in one call (A B B A).
With --only, only those files change and only their own definitions clash: the names the C gives its tables (twin
tables at each other's addresses: btlv_mcss.c's swaps and rotations, t34i). Run ninja after it.
The files it changes go in build/renamed_files.txt: finish.py commits the C ones (the callers) with the file
it finishes, where it leaves out another C file the worktree changed (t18a).

    python3 tools/rename.py func_ov009_02169934 DWCi_FriendsCallback [OLD NEW ...] [--dry-run]
    python3 tools/rename.py data_ov094_02209fd0 data_ov094_02209fe0 data_ov094_02209fe0 data_ov094_02209fd0 --only src/ov094/btlv_mcss.c
"""
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from two_modules import real_of  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
# a string or character literal, a /* */ or a // comment: blanked (newlines kept), so offsets stay
CODE = re.compile(r'"(?:\x5c.|[^"\x5c\n])*"|' + r"'(?:\x5c.|[^'\x5c\n])*'|/\*.*?\*/|//[^\n]*", re.S)
C_SUFFIXES = (".c", ".h", ".s", ".inc", ".cpp")
NOT_A_FILE = r"(?!\.(?:c|h|o|s|cpp|inc|txt|tsv|md)\b)"  # os_init in delinks.txt's lib/nitro/src/os/os_init.c stays


class Refused(Exception):
    pass


def files(root):
    cfg = root / "config" / "b_us"
    return [*cfg.rglob("*.txt"), *sorted(cfg.glob("twins_b_us*.tsv")),
            *(p for top in ("src", "lib", "include") for p in sorted((root / top).rglob("*")) if p.suffix in C_SUFFIXES),
            root / "docs" / "nonmatching.md"]


def words(names):
    """A whole word of the names, not a file's name in a path (os_init in lib/nitro/src/os/os_init.c)."""
    return re.compile(r"(?<![\w/])(" + "|".join(map(re.escape, names)) + r")\b" + NOT_A_FILE)


# tools/prebuilt_map.py renames whole words in these: not the twins tables, whose pokebw2 column has func_ names too
FILES = [p for p in files(ROOT) if p.suffix != ".tsv"]


def bare(text):
    """The C text with its comments and literals blanked, offsets kept."""
    return CODE.sub(lambda m: re.sub(r"[^\n]", " ", m[0]), text)


def black_names(path, text):
    """The text a config file names symbols in: a twins table's first column only (the others are pokebw2's)."""
    if path.suffix == ".tsv":
        return "\n".join(line.split("\t", 1)[0] for line in text.split("\n"))
    return text


def defines(code, name):
    """Whether the blanked C text defines the name: a macro, a typedef, a function's body or a file-scope object (not
    an extern, not a prototype or its parameters, not a struct tag)."""
    if re.search(rf"^[ \t]*#[ \t]*define[ \t]+{name}\b", code, re.M):
        return True
    for m in re.finditer(rf"\b{name}\b", code):
        p, e = m.start(), m.end()
        before = code[:p]
        if before.count("{") != before.count("}"):
            continue  # in a body, a struct or an initializer
        s = before[max(before.rfind(";"), before.rfind("{"), before.rfind("}")) + 1:]
        s = "\n".join(line for line in s.split("\n") if not line.lstrip().startswith("#"))
        after = code[e:].lstrip()
        if s.count("(") > s.count(")"):
            continue  # a parameter, an argument
        if re.search(r"\btypedef\b", s):
            return True
        if after.startswith("("):
            depth, k = 0, e + code[e:].index("(")
            for k in range(k, len(code)):
                depth += {"(": 1, ")": -1}.get(code[k], 0)
                if depth == 0:
                    break
            if code[k + 1:].lstrip().startswith("{"):
                return True  # a function's body
            continue
        if re.search(r"\bextern\b", s) or re.search(r"\b(struct|union|enum)\s*$", s):
            continue
        if re.search(r"[\w*]\s*$", s) and after[:1] in (";", "=", "[", ","):
            return True
    return False


def own_files(root, name, texts):
    """The sources that hold the symbol: its address in a file's delinks ranges (symbols.txt), or a definition."""
    out = set()
    for path in (root / "config" / "b_us").rglob("symbols.txt"):
        m = re.search(rf"^{name} kind:\S+ addr:(0x[0-9a-fA-F]+)", path.read_text(errors="replace"), re.M)
        delinks = path.parent / "delinks.txt"
        if not m or not delinks.is_file():
            continue
        addr, src = int(m[1], 16), None
        for line in delinks.read_text(errors="replace").split("\n"):
            if line.strip() and not line[0].isspace():
                src = line.strip().rstrip(":")
                src = real_of(src) or src  # a two-module alias (card_rom@ltd.c): its file's C holds the symbol
            r = re.match(r"\s+\.\w+\s+start:(0x\w+) end:(0x\w+)", line)
            if r and src and int(r[1], 16) <= addr < int(r[2], 16):
                out.add(root / src)
    out.update(p for p, t in texts.items() if p.suffix in C_SUFFIXES and name in t and defines(bare(t), name))
    return out


def run(pairs, root=ROOT, only=None, dry=False):
    """Renames the pairs {old: new} in the tree (in only's files alone, if given; dry: writes nothing). Returns
    ([changed files], [notes]); raises Refused with nothing changed."""
    nonmatching = root / "docs" / "nonmatching.md"
    scope = [root / f for f in only] if only else [p for p in files(root) if p.is_file()]
    lost = [str(p.relative_to(root)) for p in scope if not p.is_file()]
    if lost:
        raise Refused(f"no such file: {', '.join(lost)}")
    texts = {p: p.read_text(errors="surrogateescape") for p in scope}
    where = ", ".join(only) if only else "config/b_us, src, lib, include, docs/nonmatching.md"
    news = list(pairs.values())
    twice = sorted({n for n in news if news.count(n) > 1})
    if twice:
        raise Refused(f"two names would become {twice[0]}: " + ", ".join(o for o, n in pairs.items() if n == twice[0]))
    notes = []
    for old, new in pairs.items():
        if not re.fullmatch(r"[A-Za-z_]\w*", new):
            raise Refused(f"not a name: {new}")
        if old == new:
            raise Refused(f"{old} would keep its name")
        if not any(old in t and words([old]).search(t) for t in texts.values()):
            raise Refused(f"{old} isn't anywhere ({where})")
        if new in pairs:
            continue  # a swap or a rotation: the pairs rename that one too
        own = own_files(root, old, texts) if not only else set()
        symbol, defined, same, declared = [], [], [], []
        for p, t in texts.items():
            if new not in t or p == nonmatching:
                continue
            rel = str(p.relative_to(root))
            if p.suffix in C_SUFFIXES:
                code = bare(t)
                if not words([new]).search(code):
                    continue  # in comments and strings only
                (declared if not defines(code, new) else same if p in own else defined).append(rel)
            elif words([new]).search(black_names(p, t)):
                symbol.append(rel)
        what = (f"a Black symbol's name already, in {listed(symbol)}" if symbol else
                f"defined already, in {listed(defined)}" if defined else None)
        if what:
            raise Refused(f"{new} is {what}: two symbols of one name don't link. To swap or rotate names, give all the "
                          f"pairs in one call ({old} {new} {new} {old}); to change which names the C gives its own "
                          f"tables, --only its file; else pick another name for {old}, or, if {new} is the same "
                          "function (a copy, or C written under that name by hand), remove or rename that one first")
        if same:
            notes.append(f"{new} is defined already in {listed(same)}, which holds {old}: the same object")
        if declared:
            notes.append(f"{new} is declared already, in {listed(declared)}: its declarations must agree with "
                         f"{old}'s (ninja)")
    word = words(pairs)
    sub = lambda s: word.sub(lambda m: pairs[m[1]], s)  # noqa: E731
    changed = []
    for p, t in texts.items():
        if not any(old in t for old in pairs):
            continue
        if p.suffix == ".tsv":  # the Black names only
            new_t = "\n".join("\t".join([sub(f[0])] + f[1:]) for f in (line.split("\t", 1) for line in t.split("\n")))
        else:
            new_t = sub(t)
        if new_t != t:
            if not dry:
                p.write_text(new_t, errors="surrogateescape")
            changed.append(str(p.relative_to(root)))
    return changed, notes


def listed(paths):
    return ", ".join(paths[:5]) + (f" and {len(paths) - 5} more files" if len(paths) > 5 else "")


def refuse(why):
    print(f"NOT RENAMED (nothing changed): {why}", flush=True)
    sys.exit(1)


def main():
    args, only = sys.argv[1:], None
    dry = "--dry-run" in args
    args = [a for a in args if a != "--dry-run"]
    if "--only" in args:
        k = args.index("--only")
        args, only = args[:k], args[k + 1:]
        if not only:
            refuse("--only needs files")
    if not args or len(args) % 2:
        refuse("give pairs of names, OLD NEW ..." + chr(10) + __doc__)
    pairs = dict(zip(args[::2], args[1::2]))
    if len(pairs) != len(args) // 2:
        refuse("a name is given twice as an old name")
    try:
        changed, notes = run(pairs, ROOT, only, dry)
    except Refused as e:
        refuse(str(e))
    if dry:
        print("\n".join(f"note: {n}" for n in notes + [f"would change {c}" for c in changed]))
        return
    log = ROOT / "build" / "renamed_files.txt"  # the files it changed: finish.py commits the C ones with the file (t18a)
    log.parent.mkdir(exist_ok=True)
    with open(log, "a") as f:
        f.writelines(c + chr(10) for c in changed)
    for note in notes:
        print(f"note: {note}")
    c_files = [c for c in changed if c.endswith(".c")]
    print(f"{len(pairs)} renamed in {len(changed)} files: run ninja" +
          (f". The C files changed, which finish.py commits with yours: {', '.join(c_files[:8])}" +
           (f" and {len(c_files) - 8} more" if len(c_files) > 8 else "") if c_files else ""))


if __name__ == "__main__":
    main()
