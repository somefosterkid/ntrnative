#!/usr/bin/env python3
"""Calls (and pointers) into an overlay whose DS address another overlay shares: which overlay's name the C uses.

On the DS, overlays that load into the same memory give their functions the same addresses, so a call from
overlay A to 0x021f3888 is the same instruction whether the decomp names it func_ov042_021f3888 or
func_ov045_021f3888: the ROM matches either way. The PC links every overlay into one exe, so the name picks the
code that runs: the wrong name runs the other overlay's function (a crash, or silently the wrong behaviour) when
the caller's context has the other overlay loaded (job ov1, after pz2's Icirrus lever crash, 69cc5c7bc).

For every relocation of a module (main, autoloads, overlays) whose target lies in overlay memory that two or more
overlays share, the tool finds the caller's function (or data symbol) in the module's symbols, its C file (the
module's delinks.txt), the body of that function in the C, and the overlay names in that body whose address is the
target. dsd's relocs.txt says which overlay the target is in: `module:overlay(N)` (one: the only one in range, or
pinned by hand) or `module:overlays(A,B)` (dsd could not tell). Verdicts:

    ok         the name is the one overlay relocs.txt pins (or the caller's own overlay)
    WRONG      relocs.txt pins overlay N, the C names another overlay's function at that address
    AMBIG      relocs.txt lists several; the C names one of them: the caller's context decides (see the report)
    unnamed    no name for the target in the body (a macro, an indirect table in another file): look by hand
    asm        the caller's file is assembly / not C (its name is checked in the .s if there is one)

    python3 tools/overlay_call_audit.py                  # b/w (b_us), only the entries that need a look
    python3 tools/overlay_call_audit.py --ver w_us --all
    python3 tools/overlay_call_audit.py --ver b2_us      # third_party/pokebw2
    python3 tools/overlay_call_audit.py --known          # AMBIG entries the KNOWN table explains, too

KNOWN below lists AMBIG sites checked by hand (caller, target address) -> why the name is right; they are not
printed again unless --known. Exit 1 when a WRONG or an unexplained AMBIG entry is left.
"""
import argparse
import re
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
VERS = {
    "b_us": (ROOT, ROOT / "config/b_us/arm9"),
    "w_us": (ROOT, ROOT / "config/w_us/arm9"),
    "b2_us": (ROOT / "third_party/pokebw2", ROOT / "third_party/pokebw2/config/b2_us/arm9"),
    "w2_us": (ROOT / "third_party/pokebw2", ROOT / "third_party/pokebw2/config/w2_us/arm9"),
}
SECTION = re.compile(r"^\s+(\.\w+)\s+start:0x([0-9a-fA-F]+)\s+end:0x([0-9a-fA-F]+)")
SYM = re.compile(r"^(\S+)\s+kind:(\w+)(?:\(([^)]*)\))?\s+addr:0x([0-9a-fA-F]+)")
RELOC = re.compile(r"from:0x([0-9a-fA-F]+)\s+kind:(\w+)\s+to:0x([0-9a-fA-F]+)(?:\s+add:\S+)?\s+module:(\w+)(?:\(([^)]*)\))?")
IDENT = re.compile(r"[A-Za-z_]\w*")

# Checked by hand (job ov1, 2026-11-13; Black and White alike): (caller, the C's name for the target, or "-" for an
# unnamed one) -> why it is the right overlay. A twin = another overlay's function at the same DS address.
_FIELD_BACK = "field transition after the caller's proc closed and the field (ov021) came back"
KNOWN = {
    # the gym / gimmick script commands: who-uses gives the zone, the zone's gimmick is the named overlay
    ("func_ov021_021b2444", "func_ov040_021f37ec"): "zone 18's script; ov040 (Nacrene Gym); the twin is ov194's namein",
    ("func_ov021_021b23f0", "func_ov040_021f37a4"): "no script uses it; Nacrene Gym's sibling of func_ov021_021b2444",
    ("func_ov021_021b24f4", "func_ov042_021f3888"): "zone 7 (Striaton Gym)'s scripts; ov042; ov045's twin is Icirrus'",
    ("func_ov021_021b252c", "func_ov038_021f4184"): "zone 29's scripts; ov038 (Castelia Gym), ov039's twin is a static",
    ("func_ov021_021b2614", "func_ov043_021f3a28"): "zone 97's scripts; ov043 returns the event, ov046's twin is void(Field *)",
    ("func_ov021_021b2650", "func_ov044_021f3770"): "zone 98 (Driftveil lift); ov052's twin is void(Field *)",
    ("func_ov021_021b269c", "func_ov044_021f3970"): "zone 98 (Driftveil lift); the twins take other arguments",
    ("func_ov021_021b26ac", "func_ov045_021f384c"): "zone 114 (Icirrus Gym levers)",
    ("func_ov021_021b26e4", "func_ov045_021f3888"): "zone 114 (Icirrus Gym levers): was ov042's sp_poke, pz2 69cc5c7bc",
    ("func_ov021_021b273c", "func_ov046_021f3a88"): "zone 121, ov046's gimmick (its other commands call ov046): was ov038's "
                                                    "Castelia Gym function, job ov1",
    ("func_ov021_021b27f8", "func_ov046_021f3b18"): "zone 121's scripts; ov046; ov194's twin is namein",
    ("func_ov021_0218af90", "func_ov038_021f3a88"): "behind func_ov021_021be288(field, 6): gimmick 6 is ov038 (the "
                                                    "gimmick table data_ov021_021d9cbc)",
    ("func_ov021_021ca5c4", "func_ov052_021f388c"): "Battle Subway command 350: ov052 (field_gimmick_bsubway)",
    ("func_ov010_0216c2fc", "func_ov050_021f3788"): "the League lift event: ov050 (gimmick_league_lift)",
    # field code calling the field (ov021) or the overlay the field keeps loaded
    ("func_ov010_0215e1c0", "func_ov021_021b7740"): "a script command: the field is loaded; ov172's twin is the C-gear TV",
    ("func_ov010_021638ac", "func_ov021_021c8f7c"): "field actor tool: the field; ov212's twin is the Pokedex",
    ("func_ov010_02166734", "func_ov021_021c87a0"): "Royal Unova event: the field's pleasure_boat.c",
    ("func_ov021_02188758", "func_ov018_0217ae20"): "fieldmap update outside the union room: ov018 (intrude_field)",
    ("func_ov018_0217b214", "BeginForcePlaceNameDisp"): "intrusion on the field: ov021; relocs.txt pins ov016 (dwcutil's "
                                                       "Calloc) wrongly",
    ("func_ov021_021a1524", "func_ov034_021f0058"): "field_subscreen.c's ov034 (report) mode helpers; relocs.txt pins "
                                                    "ov030 wrongly",
    ("func_ov021_021a1554", "func_ov034_021f009c"): "field_subscreen.c's ov034 (report) mode helpers; relocs.txt pins "
                                                    "ov029 wrongly",
    ("func_ov010_02156fd0", "func_ov112_021f67e0"): "the musical stage: ov112 (musical_system); ov215 is the egg demo",
    # the caller names another overlay's id too, but calls the named one while that one is loaded
    ("func_ov000_021542c0", "CallFieldMapEntranceOutTransitionDefault"): _FIELD_BACK,
    ("func_ov000_021542c0", "CallFieldMapEntranceInTransition"): _FIELD_BACK,
    ("func_ov006_021544f0", "CallFieldMapEntranceInTransition"): _FIELD_BACK,
    ("func_ov010_0215fa54", "CallFieldMapEntranceOutTransitionDefault"): _FIELD_BACK,
    ("func_ov010_0215fa54", "CallFieldMapEntranceInTransition"): _FIELD_BACK,
    ("func_ov010_02166f9c", "CallFieldMapEntranceOutTransitionDefault"): _FIELD_BACK,
    ("func_ov010_02166f9c", "CallFieldMapEntranceInTransition"): _FIELD_BACK,
    ("func_ov012_02174744", "CallFieldMapEntranceOutTransitionDefault"): _FIELD_BACK,
    ("func_ov012_02174744", "CallFieldMapEntranceInTransition"): _FIELD_BACK,
    ("EventBattleCall_Callback", "CallFieldMapEntranceInTransition"): _FIELD_BACK,
    ("EventBattleCall_Callback", "EncEff_StartEvent"): "the encounter effect runs on the field before the battle's overlays",
    # unnamed: no pointer in the C, or a name symbols.txt doesn't have, at an address one overlay alone is pinned to
    ("data_ov010_021712cc", "-"): "event_field_proclink.c's table: the entry's proc functions (ov136's mail view)",
    ("data_ov023_02186b8c", "-"): "an asset table's word (0x021b0001), not a pointer",
    ("data_ov099_021c384c", "-"): "an asset table's word (0x021b0001), not a pointer",
    ("data_ov110_021c3614", "-"): "an asset table's word (0x021e0201), not a pointer",
    ("data_ov121_021df544", "-"): "an asset table's word, not a pointer",
    ("data_ov136_021c77f4", "-"): "an asset table's word (0x021f0001), not a pointer",
    ("data_ov137_021e1ba0", "-"): "an asset table's word (0x021b0001), not a pointer",
    ("func_ov175_021df734", "-"): "DWC's globals in ov009 (named otherwise in lib/dwc); ov021 is not loaded online",
    ("func_ov175_021df7fc", "-"): "DWC's globals in ov009 (named otherwise in lib/dwc); ov021 is not loaded online",
    ("func_ov175_021df968", "-"): "DWC's globals in ov009 (named otherwise in lib/dwc); ov021 is not loaded online",
    ("func_ov175_021e2290", "-"): "DWC's globals in ov009 (named otherwise in lib/dwc); ov021 is not loaded online",
    ("func_ov175_021e22e4", "-"): "DWC's globals in ov009 (named otherwise in lib/dwc); ov021 is not loaded online",
}
DSD_NAME = re.compile(r"(func|data)_ov\d+_[0-9a-f]{8}$")


class Module:
    def __init__(self, name, ovid, base):
        self.name, self.id, self.base = name, ovid, base
        self.sections = []          # (name, start, end) of the module
        self.files = []             # (start, end, path)
        self.syms = []              # (addr, name, kind, size)
        self.relocs = []            # (from, kind, to, module, args)
        self.lo = self.hi = 0

    def load(self):
        cur = None
        for line in (self.base / "delinks.txt").read_text().splitlines():
            if line.strip().endswith(":") and not line.startswith(" "):
                cur = line.strip()[:-1]
                continue
            m = SECTION.match(line)
            if not m:
                continue
            s, e = int(m.group(2), 16), int(m.group(3), 16)
            if cur is None:
                self.sections.append((m.group(1), s, e))
            elif e > s:
                self.files.append((s, e, cur))
        if self.sections:
            self.lo = min(s for _, s, _ in self.sections)
            self.hi = max(e for _, _, e in self.sections)
        for line in (self.base / "symbols.txt").read_text().splitlines():
            m = SYM.match(line)
            if not m or m.group(2) == "label":
                continue
            size = None
            if m.group(3):
                sm = re.search(r"size=0x([0-9a-fA-F]+)", m.group(3))
                size = int(sm.group(1), 16) if sm else None
            self.syms.append((int(m.group(4), 16), m.group(1), m.group(2), size))
        self.syms.sort()
        rel = self.base / "relocs.txt"
        for line in (rel.read_text().splitlines() if rel.exists() else []):
            m = RELOC.search(line)
            if m:
                args = tuple(int(x) for x in m.group(5).split(",")) if m.group(5) else ()
                self.relocs.append((int(m.group(1), 16), m.group(2), int(m.group(3), 16), m.group(4), args))

    def file_at(self, a):
        for s, e, p in self.files:
            if s <= a < e:
                return p
        return None

    def sym_at(self, a):
        best = None
        for i, (sa, name, kind, size) in enumerate(self.syms):
            if sa > a:
                break
            end = sa + size if size else (self.syms[i + 1][0] if i + 1 < len(self.syms) else sa + 1)
            if sa <= a < end or (size is None and sa <= a):
                best = name
        return best


def load_modules(base):
    mods = [Module("main", None, base)]
    if not (base / "config.yaml").exists():          # pokebw2: no config.yaml, no relocs.txt; the overlay dirs
        for d in sorted((base / "overlays").glob("ov[0-9]*")):
            mods.append(Module(d.name, int(d.name[2:]), d))
        for d in ("itcm", "dtcm", "ltd_autoload_0", "autoload_2", "autoload_3"):
            if (base / d / "delinks.txt").exists():
                mods.append(Module(d, None, base / d))
        for mod in mods:
            mod.load()
        return mods
    text = (base / "config.yaml").read_text()
    for m in re.finditer(r"- id: (\d+)\n(?:\s+\w+:.*\n)*?\s+delinks: (overlays/ov\d+)/delinks\.txt", text):
        mods.append(Module("ov%03d" % int(m.group(1)), int(m.group(1)), base / m.group(2)))
    for m in re.finditer(r"name: (\w+)\n(?:\s+\w+:.*\n)*?\s+delinks: (\w+)/delinks\.txt", text):
        if not m.group(1).startswith("ov"):
            mods.append(Module(m.group(1), None, base / m.group(2)))
    for mod in mods:
        mod.load()
    return mods


_src_cache = {}


def strip_comments(t):
    t = re.sub(r"/\*.*?\*/", lambda m: re.sub(r"[^\n]", " ", m.group(0)), t, flags=re.S)
    return re.sub(r"//[^\n]*", "", t)


def source(path):
    if path not in _src_cache:
        _src_cache[path] = strip_comments(path.read_text(errors="replace")) if path.exists() else None
    return _src_cache[path]


def body_of(text, name):
    """The C text of NAME's definition (a function body, or a data initializer), or None."""
    for m in re.finditer(r"\b%s\b" % re.escape(name), text):
        i = m.end()
        # a definition: `name(...) {` or `name[...] = {` / `name = ` before the next `;`
        j = i
        depth = 0
        while j < len(text):
            c = text[j]
            if c == "(":
                depth += 1
            elif c == ")":
                depth -= 1
            elif depth == 0 and c in ";{":
                break
            j += 1
        if j >= len(text):
            return None
        if text[j] == ";" and "=" not in text[i:j]:
            continue
        if text[j] == ";":                    # `x = func;` style initializer
            return text[i:j]
        # brace-match
        k, depth = j, 0
        while k < len(text):
            if text[k] == "{":
                depth += 1
            elif text[k] == "}":
                depth -= 1
                if depth == 0:
                    return text[i:k + 1]
            k += 1
        return text[i:]
    return None


_mod_tokens = {}


def module_tokens(mod, srcroot):
    if mod.name not in _mod_tokens:
        toks = set()
        for p in {p for _, _, p in mod.files}:
            t = source(srcroot / p)
            if t:
                toks |= set(IDENT.findall(t))
        _mod_tokens[mod.name] = toks
    return _mod_tokens[mod.name]


def overlay_macros(srcroot):
    """`#define OVERLAY_BOX2 OVERLAY_ID(170)` and the like: macro name -> overlay id."""
    out = {}
    pat = re.compile(r"#define\s+(\w+)\s+(?:\(\s*)?(?:FS_OVERLAY_ID\(|OVERLAY_ID\()?\s*(?:OVERLAY_)?(\d+)(?:_ID)?\s*\)")
    for d in ("include", "src"):
        for p in list((srcroot / d).rglob("*.h")) + list((srcroot / d).rglob("*.c")):
            t = p.read_text(errors="replace")
            if "OVERLAY" not in t:
                continue
            for m in pat.finditer(t):
                if "OVERLAY" in m.group(0)[len("#define"):].split(None, 1)[1]:
                    out[m.group(1)] = int(m.group(2))
            # a one-line loader: `void func_0200ba58(void) { GFL_OvlLoad(OVERLAY_ID(188)); }` (battle_rec.c)
            for m in re.finditer(r"\bvoid\s+(\w+)\s*\(\s*void\s*\)\s*\{\s*GFL_OvlLoad\(\s*OVERLAY_ID\(\s*(\d+)\s*\)\s*\);\s*\}", t):
                out[m.group(1)] = int(m.group(2))
    return out


def loaded_ids(text, macros):
    ids = {int(x) for x in re.findall(r"OVERLAY_ID\(\s*(\d+)\s*\)", text)}
    ids |= {int(x) for x in re.findall(r"\bOVERLAY_(\d+)_ID\b", text)}
    ids |= {int(x) for x in re.findall(r"FS_OVERLAY_ID\(\s*ov(\d+)\s*\)", text)}
    for tok in set(IDENT.findall(text)):
        if tok in macros:
            ids.add(macros[tok])
    return ids


def functions_of(text):
    """(name, body) of each function definition in C text (comments stripped), top level only."""
    out = []
    for m in re.finditer(r"^[A-Za-z_][^;{}()=]*?\b([A-Za-z_]\w*)\s*\(([^;{}]*?)\)\s*\{", text, re.M):
        k, depth = m.end() - 1, 0
        while k < len(text):
            if text[k] == "{":
                depth += 1
            elif text[k] == "}":
                depth -= 1
                if depth == 0:
                    break
            k += 1
        out.append((m.group(1), text[m.end() - 1:k + 1]))
    return out


def audit_names(ver, show_all=False, show_known=False):
    """No relocs.txt (pokebw2): every name a C function uses that is another overlay's symbol at an address where a
    third overlay has a symbol too (the twin dsd could have named instead)."""
    srcroot, base = VERS[ver]
    mods = load_modules(base)
    ovs = {m.id: m for m in mods if m.id is not None}
    at = defaultdict(dict)                    # address -> {overlay: name}
    sym = {}                                  # name -> (overlay, address)
    seen = defaultdict(int)
    for m in mods:
        for a, n, k, s in m.syms:
            seen[n] += 1
    for m in ovs.values():
        for a, n, k, s in m.syms:
            at[a][m.id] = n
            # functions, and data under dsd's own names: pokebw2's symbols.txt has data called `data`, `pad`
            if seen[n] == 1 and (k == "function" or re.match(r"data_ov\d+_", n)):
                sym[n] = (m.id, a)
    macros = overlay_macros(srcroot)

    def overlaps(a, b):
        return a.lo < b.hi and b.lo < a.hi
    rows = []
    deps = defaultdict(lambda: defaultdict(int))
    units = []
    for mod in mods:
        for p in sorted({p for _, _, p in mod.files}):
            text = source(srcroot / p)
            if not text:
                continue
            for fname, body in functions_of(text):
                uses = []
                for tok in set(IDENT.findall(body)):
                    if tok not in sym or tok == fname:
                        continue
                    b, a = sym[tok]
                    if b == mod.id:
                        continue
                    twins = {o for o in at[a] if o not in (b, mod.id)}
                    if not twins:
                        deps[mod.name][b] += 1
                    uses.append((tok, b, a, twins))
                if uses:
                    units.append((mod, p, fname, body, uses))
    for mod, p, fname, body, uses in units:
        loads = None
        for tok, b, a, twins in uses:
            if not twins:
                continue
            if loads is None:
                loads = loaded_ids(body, macros)
            live = sorted(c for c in twins if mod.id is None or not overlaps(ovs[c], mod))
            if mod.id is not None and overlaps(ovs[b], mod):
                verdict = "ok:load" if b in loads else "CLASH"
            elif b in loads:
                verdict = "ok:load"
            elif not live:
                verdict = "ok:excl"
            elif set(live) & loads:
                verdict = "LOADS-OTHER"
            elif deps[mod.name][b] and not any(deps[mod.name][c] for c in live):
                verdict = "ok:dep"
            elif not DSD_NAME.match(tok):
                verdict = "ok:named"      # a name a person gave after reading the code (pokebw2), not dsd's default
            else:
                verdict = "AMBIG"
            rows.append((mod.name, fname, a, tok, b, live, verdict, p, KNOWN.get((fname, tok))))
    bad = 0
    for modn, fname, a, tok, b, live, verdict, p, known in rows:
        if verdict in ("CLASH", "LOADS-OTHER", "AMBIG") and not known:
            bad += 1
        if not show_all and (verdict.startswith("ok") or (known and not show_known)):
            continue
        print("%-5s %-6s %-36s to %08x %s (ov%03d) twins:%s  %s%s  (%s)" % (
            ver, modn, fname, a, tok, b, "/".join(map(str, live)), verdict, "  [%s]" % known if known else "", p))
    counts = defaultdict(int)
    for r in rows:
        counts[r[6]] += 1
    print("%s: %d names of another overlay's symbol at a shared address: %s; %d need a look" % (
        ver, len(rows), ", ".join("%s %d" % kv for kv in sorted(counts.items())), bad), file=sys.stderr)
    return bad


def audit(ver, show_all=False, show_known=False):
    srcroot, base = VERS[ver]
    if not (base / "config.yaml").exists():
        return audit_names(ver, show_all, show_known)
    mods = load_modules(base)
    ovs = {m.id: m for m in mods if m.id is not None}
    # address -> {overlay id: [names]} for every overlay symbol
    names_at = defaultdict(lambda: defaultdict(list))
    name_ov = {}
    for m in ovs.values():
        for a, n, k, s in m.syms:
            names_at[a][m.id].append(n)
            name_ov[n] = m.id
    # deps[A][B]: A's relocations relocs.txt gives to overlay B alone (the only one there, or pinned)
    deps = defaultdict(lambda: defaultdict(int))
    for mod in mods:
        for frm, kind, to, tmod, args in mod.relocs:
            if tmod == "overlay" and len(args) == 1:
                deps[mod.name][args[0]] += 1
    ovl_macros = overlay_macros(srcroot)

    def overlaps(a, b):
        return a.lo < b.hi and b.lo < a.hi

    rows = []
    for mod in mods:
        for frm, kind, to, tmod, args in mod.relocs:
            if tmod not in ("overlay", "overlays"):
                continue
            region = [o.id for o in ovs.values() if o.lo <= to < o.hi and o.id != mod.id]
            if len(region) < 2 and tmod == "overlay":
                continue
            if mod.id is not None and mod.lo <= to < mod.hi:
                continue                      # the caller's own memory: its own overlay is meant
            cands = set(args)
            caller = mod.sym_at(frm)
            f = mod.file_at(frm)
            named = []
            verdict = None
            if f is None:
                verdict = "asm"
            else:
                p = srcroot / f
                text = source(p)
                if text is None:
                    verdict = "asm"
                else:
                    body = body_of(text, caller) if caller else None
                    want = dict(names_at.get(to, {}))
                    for oid, ns in names_at.get(to & ~1, {}).items():
                        want.setdefault(oid, ns)
                    # the caller's body; a static helper the compiler inlined: the whole file
                    for scope in ([body] if body is not None else []) + [text]:
                        toks = set(IDENT.findall(scope))
                        named = [(oid, n) for oid, ns in want.items() for n in ns if n in toks]
                        if named:
                            break
                    is_data = caller is not None and any(n == caller and k == "data" for _, n, k, _ in mod.syms)
                    if named and (scope is text or is_data):
                        # the whole file (a renamed data table, an inlined helper): it holds the names of
                        # several sites; keep the ones relocs.txt allows when there are any
                        fit = [x for x in named if x[0] in cands]
                        named = fit or named
            if verdict is None and not named:
                # delinks' file ranges and the C's files can differ (a function moved): every C file of the module
                toks = module_tokens(mod, srcroot)
                named = [(oid, n) for oid, ns in want.items() for n in ns if n in toks]
                fit = [x for x in named if x[0] in cands]
                named = fit or named
                if not named and body is not None and '#include "asset/' in body:
                    verdict = "raw"           # an asset table's word that looks like an address: no pointer in C
            loads = set()
            if verdict is None and named:
                loads = loaded_ids(body if body is not None else "", ovl_macros)
            if verdict is None:
                nov = {o for o, _ in named}
                if not named:
                    verdict = "unnamed"
                elif nov <= loads:
                    verdict = "ok:load"       # the caller names the overlay's id (loads it, starts its proc)
                elif loads & (cands | set(region)):
                    verdict = "LOADS-OTHER"   # the caller loads another overlay that is there
                elif mod.id is not None and any(overlaps(ovs[o], mod) for o in nov):
                    verdict = "CLASH"         # the named overlay shares the caller's memory: never loaded with it
                elif not nov <= cands or len(cands) > 1:
                    verdict = "WRONG" if not nov <= cands else "AMBIG"
                    if len(nov) == 1:
                        b = next(iter(nov))
                        others = (cands | set(region)) - {b} if verdict == "WRONG" else cands - {b}
                        live = [c for c in others if mod.id is None or not overlaps(ovs[c], mod)]
                        if not live:
                            verdict = "ok:excl"   # every other overlay there shares the caller's memory
                        elif deps[mod.name][b] and not any(deps[mod.name][c] for c in live):
                            verdict = "ok:dep"    # the caller uses the named overlay elsewhere, never the others
                else:
                    verdict = "ok"
            known = None
            for n in ([x for _, x in named] or ["-"]):
                known = known or KNOWN.get((caller, n))
            rows.append((mod.name, caller, frm, kind, to, sorted(cands), sorted(region), named, verdict, f, known))
    bad = 0
    for r in rows:
        modn, caller, frm, kind, to, cands, region, named, verdict, f, known = r
        if verdict in ("WRONG", "CLASH", "LOADS-OTHER", "AMBIG", "unnamed") and not known:
            bad += 1
        if not show_all and verdict in ("ok", "asm", "ok:excl", "ok:dep", "ok:load", "raw"):
            continue
        if known and not show_known and not show_all:
            continue
        nm = ",".join("%s" % n for o, n in sorted(named)) or "-"
        print("%-5s %-6s %-36s from %08x %-10s to %08x relocs:%s region:%s names:%s%s" % (
            ver, modn, caller or "?", frm, kind, to, "/".join(map(str, cands)), "/".join(map(str, region)), nm,
            "" if verdict == "ok" else "  " + verdict) + ("  [%s]" % known if known else "") + ("  (%s)" % f if verdict != "ok" else ""))
    counts = defaultdict(int)
    for r in rows:
        counts[r[8]] += 1
    print("%s: %d relocations into shared overlay memory: %s; %d need a look" % (
        ver, len(rows), ", ".join("%s %d" % kv for kv in sorted(counts.items())), bad), file=sys.stderr)
    return bad


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--ver", action="append", choices=sorted(VERS))
    ap.add_argument("--all", action="store_true", help="every entry, ok ones too")
    ap.add_argument("--known", action="store_true", help="AMBIG entries the KNOWN table explains, too")
    a = ap.parse_args()
    bad = 0
    for v in a.ver or ["b_us"]:
        bad += audit(v, a.all, a.known)
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
