#!/usr/bin/env python3
"""What the event scripts of a Gen 5 game do, read from the user's ROM: a disassembler of the field script bytecode and
story queries over it, joined with tools/atlas.py (which actor, trigger, BG event or scene-table entry runs a script).

    python3 tools/story.py GAME who-sets work 0x4088 [=1]   # each script that sets the work (to 1), who runs it, and
                                                         # the flag/work checks on the way to the set
    python3 tools/story.py GAME who-sets flag 0x21b [=0]    # the scripts that set (=0: clear) the flag
    python3 tools/story.py GAME script ZONE ID              # one zone script disassembled (ID >= the global ranges:
                                                         # a global script; ZONE then only picks nothing)
    python3 tools/story.py GAME zone ZONE                   # each runnable thing in the zone: what it checks and sets
    python3 tools/story.py GAME check                       # decode every script; count the commands that don't decode
GAME: b w b2 w2. Numbers only: messages are their numbers, no game text is ever read.

The formats are the ones the decompiled code reads (src/ for Black and White, third_party/pokebw2 for Black 2):
- script files: archive 57 (Black) / 56 (Black 2), FieldScript_LoadData. A file starts with u32 offsets, each from the
  end of itself, one per script (zone script N is entry N - 1), ended by u16 0xFD13 (FieldScript_CreateVM).
- the commands: u16 id, then the arguments the handler reads in order. The handler table is
  src/ov010/scrcmd_vm.c's data_ov010_0217062c (pokebw2: src/ov012/scrcmd_vm.c's EVCMD_TABLE) and each argument's size
  comes from the handler's C (and the helpers it passes vm to): VM_Read8 1 byte, VM_Read16 2, VM_Read32 4,
  ScriptReadAny a u16 value or work id (>= 0x4000), ScriptReadVar a u16 work id. Jumps and calls are s32 offsets from
  the end of the command (VM_Jump(vm, vm->pc + offset)); a flag/work check is pushes + VMStackCmp + VMJumpIf 0xff
  (jump when the comparison is false) or WorkCmp* + VMJumpIf cond (VM_CMP_LUT: < == > <= >= !=).
- global script ids: script_sys.c's GLOBAL_SCRIPT_TABLE (start, end, file); a zone's own scripts are 1.. in the file
  its header's scriptDatId names.
- the scene table: the zone entities file's init scripts (counts + initScriptOffset): 6-byte entries u16 type, then a
  u16 script id (FieldScript_GetInitSCRID), or for type 1 an u32 offset from the entry's end to u16 (work, value,
  script) rows (FieldScript_GetSceneChangeSCRID); a type or work of 0 ends a list.
Caches the command table in build/story/; nothing here is committed game data.
"""
import json
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import atlas  # noqa: E402

GAMES = {"b": ("b_us", 57, "src", "ov010/scrcmd_vm.c", "data_ov010_0217062c"),
         "w": ("w_us", 57, "src", "ov010/scrcmd_vm.c", "data_ov010_0217062c"),
         "b2": ("b2_us", 56, "third_party/pokebw2/src", "ov012/scrcmd_vm.c", "EVCMD_TABLE"),
         "w2": ("w2_us", 56, "third_party/pokebw2/src", "ov012/scrcmd_vm.c", "EVCMD_TABLE")}
PRIM = {"VM_Read8": "1", "VM_Read16": "2", "VM_Read32": "4", "ScriptReadAny": "a", "ScriptReadVar": "v"}
SIZE = {"1": 1, "2": 2, "4": 4, "a": 2, "v": 2}
END = 0xFD13
OPS = ["<", "==", ">", "<=", ">=", "!="]
NEG = {0: 4, 1: 5, 2: 3, 3: 2, 4: 0, 5: 1}
STOP = {"VMHalt", "RTEndGlobal", "VMReturn", "VMJump"}
FUNC = re.compile(r"^[A-Za-z_][\w \t*]*?\b(\w+)\s*\(([^;{}()]*)\)\s*\{", re.M)


# ---- the command table, from the decompiled C ----

def norm(name):
    """A handler's name without its id or file prefix: s0023_FlagSet, Scrcmd_Vm_FlagSet -> FlagSet."""
    return re.sub(r"^(s[0-9A-F]{4}_|Scrcmd_[A-Za-z0-9]+?_)", "", name or "NULL")


def preprocess(text, white):
    """C text with the #if branches the game doesn't build dropped: GAME_VERSION tests are decided by the game, any
    other #if keeps its first branch (they only differ in how they match, not in what they read)."""
    out, stack, live = [], [], True   # stack: (live outside the #if, the open branch's condition)
    for line in text.split("\n"):
        s = line.strip()
        if s.startswith("#if"):
            c = True
            if "VERSION_WHITE" in s or "VERSION_BLACK" in s:
                c = ("VERSION_WHITE" in s) == white
                c = not c if "!=" in s else c
            stack.append((live, c))
            live = live and c
        elif s.startswith(("#else", "#elif")) and stack:
            outer, c = stack[-1]
            stack[-1] = (outer, False)
            live = outer and not c
        elif s.startswith("#endif") and stack:
            live = stack.pop()[0]
        elif live:
            out.append(line)
    return "\n".join(out)


def c_functions(texts):
    """{name: [(file, body)]} of the functions in C texts {file: text} that take a VM *."""
    funcs = {}
    for fname, text in texts.items():
        for m in FUNC.finditer(text):
            if not re.search(r"\bVM\s*\*", m[2]):
                continue
            end = text.find("\n}", m.end())
            funcs.setdefault(m[1], []).append((fname, text[m.end():end if end >= 0 else len(text)]))
    return funcs


def call_args(body, i):
    """The text between the parenthesis at body[i] and its match."""
    depth = 0
    for j in range(i, len(body)):
        depth += {"(": 1, ")": -1}.get(body[j], 0)
        if depth == 0:
            return body[i + 1:j]
    return body[i + 1:]


def layout(name, funcs, memo, fname=None, busy=()):
    """The argument kinds a handler reads, in order (PRIM's letters)."""
    key = (name, fname)
    if key in memo:
        return memo[key]
    cands = funcs.get(name, [])
    if not cands:
        return ""
    src_file, body = next(((f, b) for f, b in cands if f == fname), cands[0])
    out = []
    for m in re.finditer(r"\b(\w+)\s*\(|\bvm->pc\s*(?:\+\+|\+=\s*(\d+))", body):
        n = m[1]
        if n is None:   # the handler moves pc itself: one byte, or the count it adds
            out.append("1" * int(m[2] or 1))
        elif n in PRIM:
            out.append(PRIM[n])
        elif n in funcs and n != name and n not in busy and re.search(r"\bvm\b", call_args(body, m.end() - 1)):
            out.append(layout(n, funcs, memo, src_file, busy + (name,)))
    memo[key] = "".join(out)
    return memo[key]


def table_names(text, symbol):
    m = re.search(re.escape(symbol) + r"\[\]\s*=\s*\{(.*?)\n\};", text, re.S)
    return [None if e == "NULL" else e for e in re.findall(r"^\s*(\w+),", m[1], re.M)]


def load_cmds(game):
    """(names, layouts) of the game's script commands, cached in build/story/."""
    _, _, src, table_file, symbol = GAMES[game]
    root = ROOT / src
    cache = ROOT / "build" / "story" / ("cmds_%s.json" % game)
    stamp = max(p.stat().st_mtime for p in [Path(__file__), root / table_file])
    if cache.exists() and cache.stat().st_mtime > stamp:
        d = json.loads(cache.read_text())
        return d["names"], d["layouts"]
    names = table_names((root / table_file).read_text(errors="replace"), symbol)
    texts = {str(p): preprocess(p.read_text(errors="replace"), game in ("w", "w2")) for p in root.rglob("*.c")}
    texts[str(ROOT / "src/system/vm.c")] = (ROOT / "src/system/vm.c").read_text()
    funcs, memo = c_functions(texts), {}
    lays = [layout(n, funcs, memo) if n else "" for n in names]
    cache.parent.mkdir(parents=True, exist_ok=True)
    cache.write_text(json.dumps({"names": [norm(n) for n in names], "layouts": lays}))
    return [norm(n) for n in names], lays


# ---- the bytecode ----

def script_offsets(f):
    """The file offsets of a script file's scripts."""
    offs, p = [], 0
    while p + 4 <= len(f) and struct.unpack_from("<H", f, p)[0] != END:
        offs.append(p + 4 + struct.unpack_from("<i", f, p)[0])
        p += 4
    return offs


def decode(f, off, cmds):
    """The command at off: (off, name, [(kind, value)], end), or None when it isn't one."""
    names, lays = cmds
    if off < 0 or off + 2 > len(f):
        return None
    op = struct.unpack_from("<H", f, off)[0]
    if op >= len(names) or names[op] == "NULL":
        return None
    p, args = off + 2, []
    for k in lays[op]:
        n = SIZE[k]
        if p + n > len(f):
            return None
        v = struct.unpack_from({1: "<B", 2: "<H", 4: "<i"}[n], f, p)[0]
        args.append((k, v))
        p += n
    return off, names[op], tuple(args), p


def target(ins):
    off, name, args, end = ins
    if name in ("VMJump", "VMCall", "VMJumpIf", "VMCallIf"):
        return end + args[-1][1]
    return None


def reach(f, entry, cmds):
    """{offset: command} of everything a script can run (calls followed), and the offsets that didn't decode."""
    code, bad, todo = {}, [], [entry]
    while todo:
        p = todo.pop()
        while p not in code:
            ins = decode(f, p, cmds)
            if ins is None:
                bad.append(p)
                break
            code[p] = ins
            t = target(ins)
            if t is not None:
                todo.append(t)
            if ins[1] in STOP:
                break
            p = ins[3]
    return code, bad


# ---- symbolic run: the conditions on each path ----

def operand(k, v, env):
    if k == "a" and v < 0x4000 or k in "124":
        return ("c", v)
    return env.get(v, ("work", v) if v < 0x8000 else ("var", v))


def show(e):
    t = e[0]
    if t == "c":
        return str(e[1])
    if t in ("work", "flag", "var"):
        return "%s 0x%x" % (t, e[1])
    if t == "ret":
        return e[1] + "()"
    if t == "cmp":
        l, op, r = e[1:]
        if l[0] == "flag" and r[0] == "c" and op in (1, 5) and r[1] in (0, 1):
            return "%s %s" % (show(l), "set" if (r[1] == 1) == (op == 1) else "clear")
        return "%s %s %s" % (show(l), OPS[op], show(r))
    return "?"


def neg(e):
    return ("cmp", e[1], NEG[e[2]], e[3]) if e[0] == "cmp" else ("cmp", e, 1, ("c", 0))


def truth(e):
    """True/False when a condition is decided by values the path set itself, else None."""
    if e[0] == "cmp" and e[1][0] == "c" and e[3][0] == "c":
        a, b = e[1][1], e[3][1]
        return [a < b, a == b, a > b, a <= b, a >= b, a != b][e[2]]
    if e[0] == "c":
        return e[1] == 1
    return None


def as_cond(e):
    return e if e[0] == "cmp" else ("cmp", e, 1, ("c", 1))


def explore(code, entry, want, cap=20000):
    """[(command, conditions)] for each path reaching a command want() picks; and whether the cap cut paths."""
    hits, steps = [], 0
    todo = [(entry, (), (), None, (), (), frozenset())]
    while todo:
        pc, calls, stack, reg, env, conds, seen = todo.pop()
        env = dict(env)
        while True:
            steps += 1
            if steps > cap:
                return hits, True
            ins = code.get(pc)
            if ins is None:
                break
            off, name, args, end = ins
            if want(ins):
                hits.append((ins, conds, env))
            vals = [operand(k, v, env) for k, v in args]
            nxt = end
            if name == "VMStackPushConst" or name == "VMStackPush":
                stack += (vals[0],)
            elif name == "VMStackPushFlag":
                stack += (env.get(("f", args[0][1]), ("flag", args[0][1])),)
            elif name == "VMStackCmp":
                r, l = (stack[-1:] or (("?",),))[0], (stack[-2:-1] or (("?",),))[0]
                stack = stack[:-2] + (("cmp", l, args[0][1], r) if args[0][1] < 6 else ("?",),)
            elif name in ("VMStackPop", "VMStackDiscard", "VMStackAdd", "VMStackSub", "VMStackMul", "VMStackDiv"):
                top, stack = (stack[-1:] or (("?",),))[0], stack[:-1]
                if name == "VMStackPop":
                    env[args[0][1]] = top
                elif name != "VMStackDiscard":
                    stack = stack[:-1] + (("?",),)
            elif name in ("WorkCmpConst", "WorkCmpWork"):
                reg = (vals[0], vals[1])
            elif name.startswith("VMRegCmp"):
                reg = None
            elif name in ("WorkSetConst", "WorkSet"):
                env[args[0][1]] = vals[1]
            elif name in ("WorkAdd", "WorkSub", "WorkMul", "WorkDiv", "WorkMod", "WorkAnd", "WorkOr"):
                env[args[0][1]] = ("?",)
            elif name in ("FlagSet", "FlagReset"):
                env[("f", args[0][1])] = ("c", int(name == "FlagSet"))
            elif name == "FlagGet":
                env[args[1][1]] = env.get(("f", args[0][1]), ("flag", args[0][1]))
            elif name in ("VMJumpIf", "VMCallIf"):
                cond = args[0][1]
                if cond == 0xFF and name == "VMJumpIf":
                    e, stack = as_cond((stack[-1:] or (("?",),))[0]), stack[:-1]
                    taken_c, fall_c = neg(e), e
                elif reg is not None and cond < 6:
                    taken_c = ("cmp", reg[0], cond, reg[1])
                    fall_c = neg(taken_c)
                else:
                    taken_c = fall_c = None
                t = end + args[1][1]
                branches = []
                for c, dest in ((taken_c, t), (fall_c, end)):
                    known = truth(c) if c is not None else None
                    if known is False:
                        continue
                    branches.append((dest, conds + ((c,) if c is not None and known is None and "?" not in show(c)
                                                    else ())))
                if name == "VMCallIf":
                    branches = [(b[0], b[1], 1 if b[0] == t else 0) for b in branches]
                else:
                    branches = [(b[0], b[1], 0) for b in branches]
                if not branches:
                    break
                for dest, cs, is_call in branches[1:]:
                    if (dest, calls) in seen:
                        continue
                    todo.append((dest, calls + ((end,) if is_call else ()), stack, reg, tuple(env.items()), cs,
                                 seen | {(dest, calls)}))
                dest, conds, is_call = branches[0]
                if (dest, calls) in seen:
                    break
                seen = seen | {(dest, calls)}
                if is_call:
                    calls += (end,)
                nxt = dest
            elif name == "VMCall":
                calls += (end,)
                nxt = end + args[0][1]
            elif name == "VMJump":
                nxt = end + args[0][1]
                if (nxt, calls) in seen:
                    break
                seen = seen | {(nxt, calls)}
            elif name == "VMReturn":
                if not calls:
                    break
                nxt, calls = calls[-1], calls[:-1]
            elif name in ("VMHalt", "RTEndGlobal"):
                break
            else:
                for k, v in args:
                    if k == "v":
                        env[v] = ("ret", name)
            pc = nxt
    return hits, False


def merge(condsets):
    """Paths' condition lists, without repeats and with pairs that differ only in one condition's sense joined."""
    sets = {frozenset(cs) for cs in condsets}
    changed = True
    while changed:
        changed = False
        for a in sorted(sets, key=len, reverse=True):
            for c in a:
                b = (a - {c}) | {neg(c)}
                if b != a and b in sets:
                    sets -= {a, b}
                    sets.add(a - {c})
                    changed = True
                    break
            if changed:
                break
    sets = {a for a in sets if not any(b < a for b in sets)}   # a path's conditions already cover a longer one's
    return sorted((sorted(show(c) for c in s) for s in sets), key=len)


# ---- the ROM ----

class Game:
    def __init__(self, game):
        self.game = game
        ver, arc_id, _, _, _ = GAMES[game]
        rom = (ROOT / "orig" / ("baserom_%s_us.nds" % game)).read_bytes()
        self.files = atlas.arc(rom, arc_id)
        self.ents = atlas.arc(rom, atlas.GAMES[game])
        self.atlas = atlas.load(game)
        self.cmds = load_cmds(game)
        self.globals = global_table(game)
        self.zone_of_file = {}
        for z in self.atlas["zones"]:
            self.zone_of_file.setdefault(z["script"], []).append(z["zone"])

    def resolve(self, zone, sid):
        """(file, index) of script sid as zone runs it."""
        for start, end, fid in self.globals:
            if sid >= start:
                return (fid, sid - start) if sid <= end else None
        if sid >= 1 and zone is not None:
            return self.atlas["zones"][zone]["script"], sid - 1
        return None

    def ids(self, fid, idx):
        """[(zone or None, script id)] that run entry idx of file fid."""
        for start, end, g in self.globals:
            if g == fid:
                return [(None, start + idx)]
        return [(z, idx + 1) for z in self.zone_of_file.get(fid, [])]

    def scene(self, zone):
        """[(type, script, work, value)] of a zone's init scripts (work None: not a scene-change row)."""
        b = self.ents[self.atlas["zones"][zone]["entities"]]
        p = 4 + struct.unpack_from("<I", b, 0)[0]
        out = []
        while p + 6 <= len(b):
            typ = struct.unpack_from("<H", b, p)[0]
            if typ == 0:
                break
            if typ == 1:
                q = p + 6 + struct.unpack_from("<I", b, p + 2)[0]
                while q + 6 <= len(b) and struct.unpack_from("<H", b, q)[0]:
                    w, v, s = struct.unpack_from("<3H", b, q)
                    out.append((typ, s, w, v))
                    q += 6
            else:
                out.append((typ, struct.unpack_from("<H", b, p + 2)[0], None, None))
            p += 6
        return out

    def runners(self, zone, sid):
        """Who in the zone (None: any zone) runs script sid, as short labels."""
        out = []
        for z in ([self.atlas["zones"][zone]] if zone is not None else self.atlas["zones"]):
            here = "" if zone is not None else "zone %d " % z["zone"]
            for a in z["npcs"]:
                if a["scr"] == sid:
                    out.append("%sactor uid %d (%s)%s" % (here, a["uid"], atlas.tile(a),
                                                          " spawn flag 0x%x" % a["flag"] if a["flag"] else ""))
            for t in z["triggers"]:
                if t["scr"] == sid:
                    out.append("%strigger %d (%s) when work 0x%x == %d" % (here, t["i"], atlas.tile(t), t["work"],
                                                                         t["value"]))
            for i, g in enumerate(z["bg"]):
                if g["scr"] == sid:
                    out.append("%sbg %d (%s)" % (here, i, atlas.tile(g)))
            if zone is not None:
                for typ, s, w, v in self.scene(z["zone"]):
                    if s == sid:
                        out.append("scene table (type 1) when work 0x%x == %d" % (w, v) if w is not None
                                   else "init script type %d" % typ)
        return out

    def scripts(self):
        """Every (file, index, offset) entry of the script files a zone or the global table names (the other files
        of the archive are not scripts)."""
        used = set(self.zone_of_file) | {fid for _, _, fid in self.globals}
        for fid, f in enumerate(self.files):
            if fid not in used:
                continue
            for i, off in enumerate(script_offsets(f)):
                yield fid, i, off


def global_table(game):
    """[(start, end, file)] of the global script ranges, highest first (the order the game searches them)."""
    ver = GAMES[game][0]
    if game in ("b", "w"):
        import asset
        row = next(r.split("\t") for r in (ROOT / "config" / ver / "assets.tsv").read_text().splitlines()
                   if r.startswith("script_sys_GLOBAL_SCRIPT_TABLE\t"))
        data = asset.rom_bytes(ver, row[1], int(row[2], 16), int(row[3], 16))
        return [struct.unpack_from("<3H", data, i) for i in range(0, len(data), 10)]
    pb = ROOT / "third_party/pokebw2"
    defs = dict(re.findall(r"#define (SCRIPTS_GLOBAL_\d+) (\d+)",
                           (pb / "generated/constants/field_scripts.h").read_text()))
    text = (pb / "src/ov012/script_sys.c").read_text()
    body = re.search(r"GLOBAL_SCRIPT_TABLE\[\d+\] = \{(.*?)\n\};", text, re.S)[1]
    return [(int(a), int(b), int(defs.get(c, c))) for a, b, c in re.findall(r"\{\s*(\d+),\s*(\d+),\s*(\w+),", body)]


# ---- the queries ----

def fmt(ins):
    off, name, args, end = ins
    t = target(ins)
    parts = []
    for i, (k, v) in enumerate(args):
        if t is not None and i == len(args) - 1:
            parts.append("-> 0x%04x" % t)
        elif k == "v" or (k == "a" and v >= 0x4000):
            parts.append("0x%x" % v)
        elif name in ("FlagSet", "FlagReset", "FlagGet", "VMStackPushFlag") and i == 0:
            parts.append("0x%x" % v)
        elif name in ("VMJumpIf", "VMCallIf") and i == 0:
            parts.append("pop" if v == 0xFF else OPS[v] if v < 6 else str(v))
        else:
            parts.append(str(v))
    return "0x%04x  %s %s" % (off, name, " ".join(parts))


def setter(kind, num, value):
    """want() for the commands that set a flag or work (to value)."""
    def want(ins):
        off, name, args, end = ins
        if kind == "flag":
            return name in ("FlagSet", "FlagReset") and args[0] == ("a", num) and \
                (value is None and name == "FlagSet" or value is not None and (name == "FlagSet") == bool(value))
        if not args or args[0] != ("v", num):
            return False
        if name in ("WorkSetConst", "WorkSet"):
            return value is None or args[1][1] == value and (name == "WorkSetConst" or args[1][1] < 0x4000)
        return name in ("WorkAdd", "WorkSub") and value is None
    return want


def what_set(ins):
    off, name, args, end = ins
    if name in ("FlagSet", "FlagReset"):
        return "flag 0x%x %s" % (args[0][1], "set" if name == "FlagSet" else "clear")
    op = {"WorkSetConst": "=", "WorkSet": "=", "WorkAdd": "+=", "WorkSub": "-="}[name]
    v = args[1][1]
    return "work 0x%x %s %s" % (args[0][1], op, "work 0x%x" % v if name != "WorkSetConst" and v >= 0x4000 else v)


def who_sets(g, kind, num, value):
    want = setter(kind, num, value)
    out = []
    for fid, idx, off in g.scripts():
        f = g.files[fid]
        code, _ = reach(f, off, g.cmds)
        if not any(want(i) for i in code.values()):
            continue
        hits, cut = explore(code, off, want)
        by_ins = {}
        for ins, conds, _ in hits:
            by_ins.setdefault(ins, []).append(conds)
        ids = g.ids(fid, idx)
        where = ", ".join("zone %d script %d" % (z, s) if z is not None else "global script %d" % s for z, s in ids) \
            or "file %d script %d" % (fid, idx + 1)
        runs = [r for z, s in ids for r in g.runners(z, s)]
        for ins in sorted(by_ins or {i: [] for i in code.values() if want(i)}):
            out.append("%s @0x%04x: %s" % (where, ins[0], what_set(ins)))
            out.append("  run by: " + ("; ".join(runs[:6]) or "nothing in the atlas (called by another script?)"))
            for cs in merge(by_ins.get(ins, []))[:4]:
                out.append("  if: " + (", ".join(cs) or "always"))
            if cut:
                out.append("  (paths cut at the cap)")
    return out or ["nothing sets %s 0x%x%s" % (kind, num, "" if value is None else " to %d" % value)]


def show_script(g, zone, sid):
    r = g.resolve(zone, sid)
    if r is None or r[0] >= len(g.files):
        return ["no script %d in zone %d" % (sid, zone)]
    f = g.files[r[0]]
    offs = script_offsets(f)
    if r[1] >= len(offs):
        return ["zone %d has %d scripts" % (zone, len(offs))]
    code, bad = reach(f, offs[r[1]], g.cmds)
    out = ["file %d script %d: %d commands" % (r[0], r[1] + 1, len(code))]
    out += [fmt(code[o]) for o in sorted(code)]
    out += ["0x%04x  ?? does not decode" % b for b in bad]
    return out


def summary(code):
    checks, sets = set(), set()
    for ins in code.values():
        off, name, args, end = ins
        if name in ("VMStackPushFlag", "FlagGet"):
            checks.add("flag 0x%x" % args[0][1])
        elif name in ("VMStackPush", "WorkCmpConst", "WorkCmpWork") and 0x4000 <= args[0][1] < 0x8000:
            checks.add("work 0x%x" % args[0][1])
        elif name in ("FlagSet", "FlagReset") or (name in ("WorkSetConst", "WorkSet", "WorkAdd", "WorkSub") and
                                                  0x4000 <= args[0][1] < 0x8000):
            sets.add(what_set(ins))
    return sorted(checks), sorted(sets)


def show_zone_scripts(g, zone):
    z = g.atlas["zones"][zone]
    f = g.files[z["script"]]
    out = ["zone %d: script file %d, %d scripts" % (zone, z["script"], len(script_offsets(f)))]
    for i, off in enumerate(script_offsets(f)):
        code, bad = reach(f, off, g.cmds)
        checks, sets = summary(code)
        runs = g.runners(zone, i + 1)
        out.append("script %d: %s" % (i + 1, "; ".join(runs) or "-"))
        if checks:
            out.append("  checks " + ", ".join(checks))
        if sets:
            out.append("  sets " + ", ".join(sets))
        if bad:
            out.append("  %d commands don't decode" % len(bad))
    return out


def check(g):
    n = bad = 0
    ops = {}
    for fid, idx, off in g.scripts():
        f = g.files[fid]
        code, b = reach(f, off, g.cmds)
        n += len(code)
        bad += len(b)
        for p in b:
            # the command before an undecodable spot is the likely wrong one
            prev = [i for i in code.values() if i[3] == p]
            for i in prev:
                ops[i[1]] = ops.get(i[1], 0) + 1
    out = ["%s: %d commands decoded, %d spots that don't decode" % (g.game, n, bad)]
    out += ["  before a bad spot: %s x%d" % kv for kv in sorted(ops.items(), key=lambda kv: -kv[1])[:15]]
    return out


def main(argv):
    if len(argv) < 2 or argv[0] not in GAMES:
        sys.exit(__doc__)
    game, cmd = argv[0], argv[1]
    nums = [int(a, 0) for a in argv[2:] if not a.startswith("=") and a not in ("work", "flag")]
    g = Game(game)
    if cmd == "who-sets" and len(argv) >= 4 and argv[2] in ("work", "flag"):
        value = next((int(a[1:], 0) for a in argv[4:] if a.startswith("=")), None)
        out = who_sets(g, argv[2], nums[0], value)
    elif cmd == "script" and len(nums) == 2:
        out = show_script(g, nums[0], nums[1])
    elif cmd == "zone" and nums:
        out = show_zone_scripts(g, nums[0])
    elif cmd == "check":
        out = check(g)
    else:
        sys.exit(__doc__)
    print("\n".join(out))


if __name__ == "__main__":
    main(sys.argv[1:])
