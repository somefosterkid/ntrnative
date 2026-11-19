#!/usr/bin/env python3
"""What the event scripts of a Gen 5 game do, read from the user's ROM: a disassembler of the field script bytecode and
story queries over it, joined with tools/atlas.py (which actor, trigger, BG event or scene-table entry runs a script).

    python3 tools/story.py GAME who-sets work 0x4088 [=1]   # each script that sets the work (to 1), who runs it, and
                                                         # the flag/work checks on the way to the set
    python3 tools/story.py GAME who-sets flag 0x21b [=0]    # the scripts that set (=0: clear) the flag
    python3 tools/story.py GAME script ZONE ID              # one zone script disassembled (ID >= the global ranges:
                                                         # a global script; ZONE then only picks nothing)
    python3 tools/story.py GAME zone ZONE                   # each runnable thing in the zone: what it checks and sets
    python3 tools/story.py GAME who-warps ZONE              # the scripts that change map to ZONE (MapChange* commands),
                                                         # who runs them and the checks on the way: the scripted doors
    python3 tools/story.py GAME who-uses CMD                # the scripts that run command CMD (a name, any case, or an id):
                                                         # zone (and its constant name), script id, the command's arguments
    python3 tools/story.py GAME check                       # decode every script; count the commands that don't decode
    python3 tools/story.py GAME state SAVE                  # a save's story values: zone, the nonzero works, the flags set
                                                         # (named ones; Gen 4: pret's names)
    python3 tools/story.py GAME next SAVE [--zone Z] [--n 15]
        # from a save's story values (SAVE.bin in route.py's saves dir, or a path): what can run now and change a value
        # other scripts check (or warp), nearest first by atlas path length from the save's zone (or Z); then a few
        # one condition away
GAME: b w b2 w2 pt hg ss. Numbers only: messages are their numbers, no game text is ever read.
Gen 4 (pt: Platinum, hg/ss: HeartGold/SoulSilver; zones are tools/atlas_gen4.py's map header ids): the same queries on pret's
command tables (pokeplatinum include/data/scripts/scrcmd.h, pokeheartgold src/data/fieldmap/script_cmd_table.h; the HGSS
tree is the hgss-port submodule third_party/pokeheartgold, or BW_HGSS_SRC). Its control and story commands show as the Gen 5
names (ALIAS4: GoToIf -> VMJumpIf, SetFlag -> FlagSet, CompareVarToValue -> WorkCmpConst ...; CheckFlag -> FlagCheck: the
flag is the comparison result). The scene table is the zone's init script file (its "init", FieldSystem_RunInitScript:
u8 type + u32, type 1 an offset to u16 (var, value, script) rows); the global ranges are script_manager.c's
(SCRIPT_RANGE_TABLE / sScriptBankMapping). A Gen 4 save's values come from tools/route_state.py's reader (Platinum; HGSS keeps
its vars and flags in another general block and has none yet: State raises ValueError); `next new` is a new game, all values 0
in the start zone. Platinum output carries pret's names, which are identifiers and numbers, never text: the zone constant, a
zone script's label (res/field/maps/ZONE/scripts.s: its ScriptEntry order is the script numbers), flag and var names
(generated/vars_flags.txt), and the tile of each actor, trigger and BG event.
Every query is capped (12 lines, one a script; script: 60 commands) with "N more: --all"; --all gives the full listing
(who-sets / who-warps: the runners and every condition set a line each).

The formats are the ones the decompiled code reads (src/ for Black and White, third_party/pokebw2 for Black 2):
- script files: archive 57 (Black) / 56 (Black 2), FieldScript_LoadData. A file starts with u32 offsets, each from the
  end of itself, one per script (zone script N is entry N - 1), ended by u16 0xFD13 (FieldScript_CreateVM).
- the commands: u16 id (Black 2/White 2: ids from 1000 are the script plugin's commands, SCRIPT_PLUGIN_TABLE: a zone's
  plugin by its zones, a global script's by the zones that run it), then the arguments the handler reads in order. The handler table is
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
- scripted doors: a MapChange* command (MAPCHANGE: the first argument is the zone, scrcmd_mapchange.c) moves the
  player. scripted_edges() makes them atlas edges (tools/atlas.py keeps them in its JSON: `atlas.py path` and route.py's
  G:zone walk them), one per actor / trigger / BG event that runs the script, from that runner's zone: the token that
  starts it (G:npc:UID, G:trig:N, G:bg:N) and the script's conditions. A script nothing in the atlas runs (a gym's
  ride: gym_insect.c starts Castelia Gym's scripts itself) or a scene table row is listed by who-warps only.
Caches the command table in build/story/; nothing here is committed game data.
"""
import json
import re
import struct
import sys
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import atlas  # noqa: E402
import atlas_gen4  # noqa: E402

GAMES = {"b": ("b_us", 57, "src", "ov010/scrcmd_vm.c", "data_ov010_0217062c"),
         "w": ("w_us", 57, "src", "ov010/scrcmd_vm.c", "data_ov010_0217062c"),
         "b2": ("b2_us", 56, "third_party/pokebw2/src", "ov012/scrcmd_vm.c", "EVCMD_TABLE"),
         "w2": ("w2_us", 56, "third_party/pokebw2/src", "ov012/scrcmd_vm.c", "EVCMD_TABLE"),
         "pt": ("pt", None, None, None, None), "hg": ("hg", None, None, None, None), "ss": ("ss", None, None, None, None)}
PRIM = {"VM_Read8": "1", "VM_Read16": "2", "VM_Read32": "4", "ScriptReadAny": "a", "ScriptReadVar": "v"}
# how a family's handlers read their arguments: (primitives, the handler's parameter, its name, the wait call)
FAM5 = (PRIM, r"\bVM\s*\*", "vm", "VM_SetNativeCallback")
FAM4 = {"pt": ({"ScriptContext_ReadByte": "1", "ScriptContext_ReadHalfWord": "2", "ScriptContext_ReadWord": "4",
                "ScriptContext_GetVar": "a", "ScriptContext_GetVarPointer": "v"},
               r"\bScriptContext\s*\*", "ctx", "ScriptContext_Pause"),
        "hg": ({"ScriptReadByte": "1", "ScriptReadHalfword": "2", "ScriptReadWord": "4", "ScriptGetVar": "a",
                "ScriptGetVarPointer": "v"}, r"\bScriptContext\s*\*", "ctx", "SetupNativeScript")}
# Gen 4 sources: (tree, command table file, its rows (a regex, or the array's name), the handlers' file, the start zone)
GEN4_SRC = {"pt": ("third_party/pokeplatinum", "include/data/scripts/scrcmd.h", r"ScriptCommand\(\w+,\s*(\w+)\)",
                   "src/scrcmd.c", "TWINLEAF_TOWN_PLAYER_HOUSE_2F"),
            "hg": ("third_party/pokeheartgold", "src/data/fieldmap/script_cmd_table.h", "gScriptCmdTable",
                   "src/scrcmd_c.c", "NEW_BARK_PLAYER_HOUSE_2F")}
# pret's names of the Gen 4 commands story.py models -> the Gen 5 names the queries know (CheckFlag: FlagCheck, its own)
ALIAS4 = {"End": "VMHalt", "GoTo": "VMJump", "Call": "VMCall", "Return": "VMReturn", "GoToIf": "VMJumpIf",
          "CallIf": "VMCallIf", "SetFlag": "FlagSet", "ClearFlag": "FlagReset", "CheckFlag": "FlagCheck",
          "CompareVarToValue": "WorkCmpConst", "CompareVarToVar": "WorkCmpWork", "SetVarFromValue": "WorkSetConst",
          "SetVar": "WorkSetConst", "SetVarFromVar": "WorkSet", "CopyVar": "WorkSet", "SetOrCopyVar": "WorkSet",
          "AddVar": "WorkAdd", "SubVar": "WorkSub"}
REG4 = set()   # Gen 4 commands that write the comparison result themselves (load_cmds4): a later GoToIf tests that
NAMES4 = {}    # Gen 4 (Platinum): {flag or var number: pret's name}, filled by Game() (load_names4); flags are below 0x4000
SIZE = {"1": 1, "2": 2, "4": 4, "a": 2, "v": 2}
END = 0xFD13
PLUGIN_BASE = 1000   # the id of a script plugin's first command
OPS = ["<", "==", ">", "<=", ">=", "!="]
NEG = {0: 4, 1: 5, 2: 3, 3: 2, 4: 0, 5: 1}
STOP = {"VMHalt", "RTEndGlobal", "VMReturn", "VMJump"}
# the map changes a script makes (scrcmd_mapchange.c: the first argument, a u16, is the zone)
MAPCHANGE = {"MapChangeFake", "MapChangeWarpPad", "MapChangeWarpRail", "MapChangeQuicksand", "MapChangeWarp",
             "MapChangeCore", "MapChangeRail", "MapChangeFlyWarp", "Warp"}   # Warp: Gen 4's (zone, unused, x, z, dir)
FUNC_END = re.compile(r"\n\}(?!;)")
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


def c_functions(texts, param=FAM5[1]):
    """{name: [(file, body)]} of the functions in C texts {file: text} that take a VM * (param: the family's)."""
    funcs = {}
    for fname, text in texts.items():
        for m in FUNC.finditer(text):
            if not re.search(param, m[2]):
                continue
            end = FUNC_END.search(text, m.end())   # not "};": a table in the body can close in column 0
            funcs.setdefault(m[1], []).append((fname, text[m.end():end.start() if end else len(text)]))
    return funcs


def call_args(body, i):
    """The text between the parenthesis at body[i] and its match."""
    depth = 0
    for j in range(i, len(body)):
        depth += {"(": 1, ")": -1}.get(body[j], 0)
        if depth == 0:
            return body[i + 1:j]
    return body[i + 1:]


def layout(name, funcs, memo, fname=None, busy=(), fam=FAM5):
    """The argument kinds a handler reads, in order (PRIM's letters)."""
    prim, _, vm, pause = fam
    key = (name, fname)
    if key in memo:
        return memo[key]
    cands = funcs.get(name, [])
    if not cands:
        return ""
    here = str(Path(fname).parent) if fname else None
    src_file, body = (next((c for c in cands if c[0] == fname), None)
                      or next((c for c in cands if str(Path(c[0]).parent) == here), cands[0]))
    out = []
    for m in re.finditer(r"\b(\w+)\s*\(|\b%s->(?:pc|scriptPtr|script_ptr)\s*(?:\+\+|\+=\s*(\d+))" % vm, body):
        n = m[1]
        if n is None:   # the handler moves pc itself: one byte, or the count it adds
            out.append("1" * int(m[2] or 1))
        elif n in prim:
            out.append(prim[n])
        elif n == pause:   # a wait command: what its callback reads comes after its own arguments
            cb = re.findall(r"\w+", call_args(body, m.end() - 1))[-1]
            if cb in funcs and cb != name and cb not in busy:
                lay = layout(cb, funcs, memo, src_file, busy + (name,), fam)
                # a callback reads in the branch that ends the wait, so branches read the same thing again
                unit = next((u for u in range(1, len(lay) + 1) if len(lay) % u == 0 and lay[:u] * (len(lay) // u) == lay), 0)
                out.append(lay[:unit])
        elif n in funcs and n != name and n not in busy and re.search(r"\b%s\b" % vm, call_args(body, m.end() - 1)):
            out.append(layout(n, funcs, memo, src_file, busy + (name,), fam))
    memo[key] = "".join(out)
    return memo[key]


# The Join Avenue plugin's table points at overlay 59's commands; JoinAvenueStoreStart loads overlay 60 in its place (and
# JoinAvenueStoreEnd puts 59 back), so between them these commands are the shop's: {table handler: the one that runs}
SHOP_SWAP = {"Scrcmd_Resort_StartTalk": "Scrcmd_ResortShop_Open"}


def plugin_tables(root, texts, funcs, memo):
    """[(zones, names, layouts, shop)] per script plugin number (0: none), from script_plugin.c's SCRIPT_PLUGIN_TABLE:
    the plugin's command table (its ids start at 1000, VM_Create's extraCommandStart), the zones it serves, and the
    layouts the commands have between JoinAvenueStoreStart and End ({slot as text: layout}, SHOP_SWAP)."""
    path = root / "ov012/script_plugin.c"
    if not path.exists():
        return []
    text = texts[str(path)]
    zones = {m[1]: [int(z) for z in re.findall(r"\d+", m[2])] for m in re.finditer(r"u16 (\w+)\[\] = \{([^}]*)\}", text)}
    out = []
    for cmds, zs in re.findall(r"^\s*\{\s*(\w+),\s*(\w+),", re.search(r"SCRIPT_PLUGIN_TABLE\[\] = \{(.*?)\n\};", text, re.S)[1],
                               re.M):
        names, lays = [], []
        for fname, t in texts.items():
            m = re.search(r"\b%s\[\]\s*=\s*\{(.*?)\};" % re.escape(cmds), t, re.S)
            if m:
                names = [None if e == "NULL" else e for e in re.findall(r"\w+", re.sub(r"\(\w+\)0x\w+", "", m[1]))]
                lays = [layout(n, funcs, memo, fname) if n else "" for n in names]
                break
        shop = {str(i): layout(SHOP_SWAP[n], funcs, memo) for i, n in enumerate(names) if n in SHOP_SWAP}
        out.append((zones.get(zs, []), [norm(n) for n in names], lays, shop))
    return out


def table_names(text, symbol):
    m = re.search(re.escape(symbol) + r"\[\]\s*=\s*\{(.*?)\n\};", text, re.S)
    return [None if e == "NULL" else e for e in re.findall(r"^\s*(\w+),", m[1], re.M)]


def src4(game):
    """(kind 'pt'/'hg', the game's pret source tree)."""
    kind = "pt" if game == "pt" else "hg"
    root = ROOT / GEN4_SRC[kind][0]
    if kind == "hg" and not (root / "src").exists() and "BW_HGSS_SRC" in __import__("os").environ:
        root = Path(__import__("os").environ["BW_HGSS_SRC"])
    return kind, root


def norm4(name):
    n = re.sub(r"^ScrCmd_(?=[A-Za-z])", "", name or "NULL")
    return ALIAS4.get(n, n)


def load_cmds4(game):
    """load_cmds for Gen 4: pret's command table, the layouts from its handlers (FAM4), no plugins; REG4 updated."""
    kind, root = src4(game)
    src, table_file, rows, handlers, _ = GEN4_SRC[kind]
    cache = ROOT / "build" / "story" / ("cmds_%s.json" % kind)
    if cache.exists() and (not (root / table_file).exists() or
                           cache.stat().st_mtime > max(Path(__file__).stat().st_mtime, (root / table_file).stat().st_mtime)):
        d = json.loads(cache.read_text())
    else:
        text = (root / table_file).read_text(errors="replace")
        raw = re.findall(rows, text) if "(" in rows else table_names(text, rows)
        texts = {str(p): preprocess(p.read_text(errors="replace"), False) for p in (root / "src").rglob("*.c")}
        funcs, memo = c_functions(texts, FAM4[kind][1]), {}
        lays = [layout(n, funcs, memo, str(root / handlers), fam=FAM4[kind]) if n else "" for n in raw]
        reg = sorted({norm4(n) for n in raw if n in funcs and re.search(r"comparisonResult\s*=[^=]", funcs[n][0][1])}
                     - set(ALIAS4.values()))
        d = {"names": [norm4(n) for n in raw], "layouts": lays, "plugins": [], "reg": reg}
        cache.parent.mkdir(parents=True, exist_ok=True)
        cache.write_text(json.dumps(d))
    REG4.update(d["reg"])
    return d["names"], d["layouts"], d["plugins"]


def load_cmds(game):
    """(names, layouts, plugins) of the game's script commands (plugins: plugin_tables'), cached in build/story/."""
    if game in atlas_gen4.GEN4:
        return load_cmds4(game)
    _, _, src, table_file, symbol = GAMES[game]
    root = ROOT / src
    cache = ROOT / "build" / "story" / ("cmds_%s.json" % game)
    stamp = max(p.stat().st_mtime for p in [Path(__file__), root / table_file])
    if cache.exists() and cache.stat().st_mtime > stamp:
        d = json.loads(cache.read_text())
        return d["names"], d["layouts"], d["plugins"]
    names = table_names((root / table_file).read_text(errors="replace"), symbol)
    texts = {str(p): preprocess(p.read_text(errors="replace"), game in ("w", "w2")) for p in root.rglob("*.c")}
    texts[str(ROOT / "src/system/vm.c")] = (ROOT / "src/system/vm.c").read_text()
    funcs, memo = c_functions(texts), {}
    lays = [layout(n, funcs, memo, str(root / table_file)) if n else "" for n in names]
    plugins = plugin_tables(root, texts, funcs, memo)
    cache.parent.mkdir(parents=True, exist_ok=True)
    cache.write_text(json.dumps({"names": [norm(n) for n in names], "layouts": lays, "plugins": plugins}))
    return [norm(n) for n in names], lays, plugins


# ---- the bytecode ----

def script_offsets(f):
    """The file offsets of a script file's scripts. The table ends at the 0xFD13 terminator, or (some files have none)
    where the scripts begin."""
    offs, p = [], 0
    while p + 4 <= len(f) and p < min(offs, default=len(f)) and struct.unpack_from("<H", f, p)[0] != END:
        offs.append(p + 4 + struct.unpack_from("<i", f, p)[0])
        p += 4
    return offs


def decode(f, off, cmds, shop=False):
    """The command at off: (off, name, [(kind, value)], end), or None when it isn't one. cmds is (names, layouts) and
    maybe the layouts of a shop swap (shop: use those)."""
    names, lays = cmds[0], cmds[2 if shop and len(cmds) > 2 else 1]
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
    code, bad, todo = {}, [], [(entry, False)]
    while todo:
        p, shop = todo.pop()
        while p not in code:
            ins = decode(f, p, cmds, shop)
            if ins is None:
                bad.append(p)
                break
            code[p] = ins
            shop = {"JoinAvenueStoreStart": True, "JoinAvenueStoreEnd": False}.get(ins[1], shop)
            t = target(ins)
            if t is not None:
                todo.append((t, shop))
            if ins[1] in STOP:
                break
            p = ins[3]
    return code, bad


# ---- symbolic run: the conditions on each path ----

def operand(k, v, env):
    if k == "a" and v < 0x4000 or k in "124":
        return ("c", v)
    return env.get(v, ("work", v) if v < 0x8000 else ("var", v))


def nm(n):
    """A flag or work number as '0x4086', with Gen 4's names loaded '0x4086 VAR_NAME'."""
    return "0x%x%s" % (n, " " + NAMES4[n] if n in NAMES4 else "")


def show(e):
    t = e[0]
    if t == "c":
        return str(e[1])
    if t in ("work", "flag", "var"):
        return "%s %s" % (t, nm(e[1]))
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
            elif name == "FlagCheck":   # Gen 4: the result is the flag (0: <, 1: ==), as if compared with 1
                reg = (env.get(("f", args[0][1]), ("flag", args[0][1])), ("c", 1), "flag")
            elif name in REG4:
                reg = None
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
                    if len(reg) > 2:   # a flag: < 1 is clear (!= 1), >= 1 set (== 1)
                        cond = {0: 5, 4: 1}.get(cond, cond)
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
    def __init__(self, game, atlas_data=None):
        self.game = game
        ver, arc_id, _, _, _ = GAMES[game]
        if game in atlas_gen4.GEN4:   # the scr_seq archive holds the scripts and the init script files (scene())
            g4 = atlas_gen4.GEN4[game]
            self.files = atlas.arc_path((ROOT / "orig" / ("baserom_%s_us.nds" % g4["rom"])).read_bytes(), g4["scripts"])
            self.ents = None
        else:
            rom = (ROOT / "orig" / ("baserom_%s_us.nds" % game)).read_bytes()
            self.files = atlas.arc(rom, arc_id)
            self.ents = atlas.arc(rom, atlas.GAMES[game])
        self.atlas = atlas_data or atlas.load(game)
        self.znames, self.slabels = zone_names(game) if game in atlas_gen4.GEN4 else {}, {}
        load_names4(game)   # (empties NAMES4 for a Gen 5 game)
        names, lays, self.plugins = load_cmds(game)
        self.cmds = (names, lays)
        self.file_cmds, self.plugin_refs = {}, None
        self.globals = global_table(game)
        self.zone_of_file = {}
        for z in self.atlas["zones"]:
            self.zone_of_file.setdefault(z["script"], []).append(z["zone"])

    def plugin_of(self, fid):
        """The script plugin number a script file runs under (0: none). A zone's file: the plugin of its zones; a
        global file (it runs in whatever zone calls it): the commonest plugin among the zones whose actors, triggers
        and scene tables run it; one nobody runs directly: whichever plugin's commands decode it best."""
        def of(z):
            return next((i for i, (zs, _, _, _) in enumerate(self.plugins) if i and z in zs), 0)
        if not self.plugins:
            return 0
        if self.zone_of_file.get(fid):
            return max(map(of, self.zone_of_file[fid]))
        if self.plugin_refs is None:
            self.plugin_refs = {}
            glob = {f for _, _, f in self.globals}
            for zd in self.atlas["zones"]:
                z = zd["zone"]
                ids = {a["scr"] for k in ("npcs", "triggers", "bg") for a in zd[k]} | {s for _, s, _, _ in self.scene(z)}
                for r in filter(None, (self.resolve(z, sid) for sid in ids)):
                    if r[0] in glob:
                        self.plugin_refs.setdefault(r[0], Counter())[of(z)] += 1
        if fid in self.plugin_refs:
            return self.plugin_refs[fid].most_common(1)[0][0]
        best = []
        for no in range(len(self.plugins)):
            cmds = self.table_with(no)
            best.append((sum(len(reach(self.files[fid], off, cmds)[1]) for off in script_offsets(self.files[fid])), no))
        return min(best)[1]

    def table_with(self, no):
        """The base commands (names, layouts) with plugin no's from id PLUGIN_BASE on; for a plugin with a shop
        swap a third list: the layouts between JoinAvenueStoreStart and End."""
        names, lays = self.cmds
        if not no:
            return names, lays
        _, pnames, plays, swap = self.plugins[no]
        pad = [""] * (PLUGIN_BASE - len(names))
        out = (names + ["NULL"] * len(pad) + pnames, lays + pad + plays)
        if swap:
            out += (lays + pad + [swap.get(str(i), l) for i, l in enumerate(plays)],)
        return out

    def cmds_for(self, fid):
        """The commands (names, layouts) a script file runs: the base table, then (from id PLUGIN_BASE) the script
        plugin it runs under, when it has one."""
        if fid not in self.file_cmds:
            self.file_cmds[fid] = self.table_with(self.plugin_of(fid))
        return self.file_cmds[fid]

    def resolve(self, zone, sid):
        """(file, index) of script sid as zone runs it."""
        for start, end, fid in self.globals:
            if sid >= start:
                return (fid, sid - start) if sid <= end else None
        if sid >= 1 and zone is not None:
            return self.atlas["zones"][zone]["script"], sid - 1
        return None

    def label(self, zone, sid):
        """Gen 4 (Platinum): pret's label of the zone's own script sid ('' for a global script, another game, or a zone
        whose scripts.s has not as many ScriptEntry lines as its binary file has scripts)."""
        r = self.resolve(zone, sid) if self.game == "pt" else None
        fid = self.atlas["zones"][zone]["script"] if r else None
        if r is None or r[0] != fid:
            return ""
        if zone not in self.slabels:
            path = src4(self.game)[1] / "res/field/maps" / self.znames.get(zone, "-").lower() / "scripts.s"
            ents = script_entries(path.read_text(errors="replace")) if path.exists() else []
            self.slabels[zone] = ents if len(ents) == len(script_offsets(self.files[fid])) else []
        return self.slabels[zone][r[1]] if r[1] < len(self.slabels[zone]) else ""

    def ids(self, fid, idx):
        """[(zone or None, script id)] that run entry idx of file fid."""
        for start, end, g in self.globals:
            if g == fid:
                return [(None, start + idx)]
        return [(z, idx + 1) for z in self.zone_of_file.get(fid, [])]

    def scene(self, zone):
        """[(type, script, work, value)] of a zone's init scripts (work None: not a scene-change row)."""
        if self.ents is None:   # Gen 4: entries u8 type + u32 (the script id; type 1: an offset from the entry's end)
            init = self.atlas["zones"][zone]["init"]
            b, p, out = self.files[init] if init < len(self.files) else b"", 0, []
            while p + 5 <= len(b) and b[p]:
                typ, val = b[p], struct.unpack_from("<I", b, p + 1)[0]
                if typ != 1:
                    out.append((typ, val & 0xFFFF, None, None))
                elif val:
                    q = p + 5 + val
                    while q + 6 <= len(b) and struct.unpack_from("<H", b, q)[0]:
                        w, v, s = struct.unpack_from("<3H", b, q)
                        out.append((typ, s, w, v))
                        q += 6
                p += 5
            return out
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

    def runs(self, zone, sid):
        """[(zone, token, label)] of who in the zone (None: any zone) runs script sid: an actor, trigger or BG event
        with the pilot's token that starts it, a scene table row with token None."""
        out = []
        for z in ([self.atlas["zones"][zone]] if zone is not None else self.atlas["zones"]):
            here = "" if zone is not None else "zone %d " % z["zone"]
            for a in z["npcs"]:
                if a["scr"] == sid:
                    out.append((z["zone"], "G:npc:%d" % a["uid"], "%sactor uid %d (%s)%s" % (
                        here, a["uid"], atlas.tile(a), " spawn flag 0x%x" % a["flag"] if a["flag"] else "")))
            for t in z["triggers"]:
                if t["scr"] == sid:
                    out.append((z["zone"], "G:trig:%d" % t["i"], "%strigger %d (%s) when work 0x%x == %d" % (
                        here, t["i"], atlas.tile(t), t["work"], t["value"])))
            for i, g in enumerate(z["bg"]):
                if g["scr"] == sid:
                    out.append((z["zone"], "G:bg:%d" % i, "%sbg %d (%s)" % (here, i, atlas.tile(g))))
            if zone is not None:
                for typ, s, w, v in self.scene(z["zone"]):
                    if s == sid:
                        out.append((z["zone"], None, "scene table (type 1) when work 0x%x == %d" % (w, v)
                                    if w is not None else "init script type %d" % typ))
        return out

    def runners(self, zone, sid):
        """Who in the zone (None: any zone) runs script sid, as short labels."""
        return [label for _, _, label in self.runs(zone, sid)]

    def scripts(self):
        """Every (file, index, offset) entry of the script files a zone or the global table names (the other files
        of the archive are not scripts)."""
        used = set(self.zone_of_file) | {fid for _, _, fid in self.globals}
        for fid, f in enumerate(self.files):
            if fid not in used:
                continue
            for i, off in enumerate(script_offsets(f)):
                yield fid, i, off


def script_entries(text):
    """The labels of a pret scripts.s's ScriptEntry lines, in order: the script numbers (1 on) of the zone's file."""
    return re.findall(r"^\s*ScriptEntry\s+(\w+)", text, re.M)


def load_names4(game):
    """NAMES4 for the Gen 4 game: Platinum's from generated/vars_flags.txt (tools/route_story_names.py reads it); HeartGold
    and SoulSilver have none yet (the tree is not a registered submodule)."""
    NAMES4.clear()
    if game == "pt" and (ROOT / GEN4_SRC["pt"][0] / "generated/vars_flags.txt").exists():
        import route_story_names
        NAMES4.update(route_story_names.read_pt())


def global_table(game):
    """[(start, end, file)] of the global script ranges, highest first (the order the game searches them)."""
    if game in atlas_gen4.GEN4:   # Gen 4: a range ends where the next higher one starts
        kind, root = src4(game)
        text = (root / "src/script_manager.c").read_text()
        if kind == "pt":
            defs = dict(re.findall(r"#define (SCRIPT_ID_OFFSET_\w+)\s+(\d+)", (root / "include/script_manager.h").read_text()))
            order = (root / "res/field/scripts.order").read_text().split()
            rows = [(int(defs[a]), order.index(f)) for a, f in re.findall(r"Entry\((\w+),\s*(\w+),", text)]
        else:
            defs = dict(re.findall(r"#define (_std_\w+)\s+(\d+)", (root / "include/constants/std_script.h").read_text()))
            rows = [(int(defs.get(a, a)), int(f)) for a, f in re.findall(r"\{\s*(\w+),\s*scr_seq_(\d+)_bin,", text)]
        rows.sort(key=lambda r: -r[0])
        return [(s, (rows[i - 1][0] - 1) if i else 0xFFFF, f) for i, (s, f) in enumerate(rows)]
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
            parts.append(nm(v))
        elif name in ("FlagSet", "FlagReset", "FlagGet", "VMStackPushFlag", "FlagCheck") and i == 0:
            parts.append(nm(v))
        elif name in ("VMJumpIf", "VMCallIf") and i == 0:
            parts.append("pop" if v == 0xFF else OPS[v] if v < 6 else str(v))
        else:
            parts.append(str(v))
    return "0x%04x  %s %s" % (off, name, " ".join(parts))


def setter(kind, num, value):
    """want() for the commands that set a flag or work (to value)."""
    def want(ins):
        off, name, args, end = ins
        if kind == "flag":   # Gen 5 reads the flag as a value or work ('a'), Gen 4 as a u16 ('2')
            return name in ("FlagSet", "FlagReset") and args[0] in (("a", num), ("2", num)) and \
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
        return "flag %s %s" % (nm(args[0][1]), "set" if name == "FlagSet" else "clear")
    op = {"WorkSetConst": "=", "WorkSet": "=", "WorkAdd": "+=", "WorkSub": "-="}[name]
    v = args[1][1]
    return "work %s %s %s" % (nm(args[0][1]), op, "work " + nm(v) if name != "WorkSetConst" and v >= 0x4000 else v)


def warper(zone):
    """want() for the commands that change map to zone (None: any)."""
    return lambda ins: ins[1] in MAPCHANGE and bool(ins[2]) and (zone is None or ins[2][0][1] == zone)


def what_warp(ins):
    return "%s to zone %d" % (ins[1], ins[2][0][1])


def who_warps(g, zone, full=False):
    return who_does(g, warper(zone), what_warp, full) or ["no script changes map to zone %d" % zone]


def scripted_edges(game, atlas_data):
    """The scripted doors as atlas edges: [{from, to, tok, how}], one per runner (actor, trigger, BG event) of a script
    with a MapChange* command, from the runner's zone to the command's (none to the runner's own zone); how: the script,
    the command and its first path's conditions (a trigger's work value is in its label)."""
    g = Game(game, atlas_data)
    want, out, seen = warper(None), [], set()
    for fid, idx, off in g.scripts():
        code, _ = reach(g.files[fid], off, g.cmds_for(fid))
        hits = [i for i in code.values() if want(i)]
        if not hits:
            continue
        found, _ = explore(code, off, want)
        conds = {}
        for ins, cs, _ in found:
            conds.setdefault(ins, []).append(cs)
        for z, sid in g.ids(fid, idx):
            for rz, tok, label in g.runs(z, sid):
                for ins in sorted(hits):
                    to = ins[2][0][1]
                    cs = merge(conds.get(ins, []))
                    key = (rz, to, tok)
                    if tok is None or to == rz or key in seen or to >= len(atlas_data["zones"]):
                        continue
                    seen.add(key)
                    out.append({"from": rz, "to": to, "tok": tok, "how": "%s, %s %s @0x%04x%s" % (
                        label, "zone %d script %d" % (z, sid) if z is not None else "global script %d" % sid, ins[1],
                        ins[0], "; if " + ", ".join(cs[0]) if cs and cs[0] else "")})
    return out


VERSIONS = {"b": 21, "w": 20, "b2": 23, "w2": 22}   # GameGetVersion()'s value per game (constants/version.h)
VERSION_COND = re.compile(r"GameGetVersion\(\) (==|!=) (\d+)")


def path_version(conds, ver):
    """None when a path's conditions (explore's) have no GameGetVersion() check, else whether they are all true in version ver."""
    checks = [VERSION_COND.fullmatch(show(c)) for c in conds]
    checks = [m for m in checks if m]
    return all((m.group(1) == "==") == (int(m.group(2)) == ver) for m in checks) if checks else None


def dead_among(moved, warps):
    """The warp indices of one zone that its version never takes. moved: the warps a script moves on a path that needs this
    version (ObjInitWarpGPos under a GameGetVersion() check: it moves the OTHER version's warp off the door); warps: the zone's
    atlas warps. Reversal Mountain's outside has two warps on each door, one into each version's interior (zones 531.. and
    537..). A moved warp is dead when a twin on its tile is not moved: the twin is the one this version takes."""
    tile = {w["i"]: (w.get("x"), w.get("z"), tuple(w.get("rail", ()))) for w in warps}
    return sorted(k for k in moved
                  if k in tile and any(j != k and tile[j] == tile[k] and j not in moved for j in tile))


def version_dead_warps(game, atlas_data):
    """[[zone, warp index]]: the atlas warps this game never takes (dead_among), from every script's ObjInitWarpGPos and the
    GameGetVersion() checks on the paths to it. The atlas holds both versions' warps of a zone (one entity file)."""
    ver, g = VERSIONS[game], Game(game, atlas_data)
    want = lambda ins: ins[1] == "ObjInitWarpGPos"
    moved = {}
    for fid, idx, off in g.scripts():
        code, _ = reach(g.files[fid], off, g.cmds_for(fid))
        if not any(want(i) for i in code.values()):
            continue
        found, _ = explore(code, off, want)
        for ins, cs, _ in found:
            if path_version(cs, ver):
                for z in g.zone_of_file.get(fid, ()):
                    moved.setdefault(z, set()).add(ins[2][0][1])
    return [[z, k] for z, ks in sorted(moved.items()) for k in dead_among(ks, atlas_data["zones"][z]["warps"])]


def who_sets(g, kind, num, value, full=False):
    return who_does(g, setter(kind, num, value), what_set, full) or \
        ["nothing sets %s 0x%x%s" % (kind, num, "" if value is None else " to %d" % value)]


def zone_names(game):
    """{zone number: constant name, less ZONE_} from the constants header the decompilation has for the game: Black 2 and
    White 2 all of them (pokebw2), Black and White the zones it uses so far (include/constants/zones.h)."""
    if game in atlas_gen4.GEN4:   # pret's enum MapHeaderID (Platinum) / MAP_* defines (HGSS)
        kind, root = src4(game)
        if kind == "hg":
            path = root / "include/constants/maps.h"
            return {int(n): name for name, n in re.findall(r"^#define MAP_(\w+)\s+(\d+)\b", path.read_text(), re.M)} \
                if path.exists() else {}
        path = root / "include/constants/fieldmap.h"
        body = re.search(r"enum MapHeaderID \{(.*?)\}", path.read_text(), re.S)[1] if path.exists() else ""
        return dict(enumerate(re.findall(r"MAP_HEADER_(\w+)", body)))
    path = ROOT / ("third_party/pokebw2/generated/constants/zones.h" if game in ("b2", "w2") else "include/constants/zones.h")
    return {int(n): name for name, n in re.findall(r"^#define ZONE_(\w+)\s+(\d+)\b", path.read_text(), re.M)} if path.exists() else {}


def cmd_name(arg, names):
    """The name a command table gives command arg (an id, or a name in any case), or None."""
    try:
        i = int(arg, 0)
        return names[i] if 0 <= i < len(names) and names[i] != "NULL" else None
    except ValueError:
        return next((n for n in names if n.lower() == arg.lower() and n != "NULL"), None)


def who_uses(g, arg, full=False):
    """The scripts that run command arg (an id or a name; calls and jumps followed, so a script that calls another one
    lists its commands too): `zone Z (NAME) script S: its first uses` or `global script S`, by zone. No conditions
    (who-sets and who-warps explore those)."""
    zn = zone_names(g.game)
    allnames = {n.lower() for n in g.cmds[0] + [n for p in g.plugins for n in p[1]] if n}
    try:
        int(arg, 0)
    except ValueError:
        if arg.lower() not in allnames:
            like = sorted(n for n in allnames if arg.lower() in n)[:8]
            sys.exit("no command named %s%s" % (arg, " (similar: %s)" % ", ".join(like) if like else ""))
    rows = []
    for fid, idx, off in g.scripts():
        cmds = g.cmds_for(fid)
        name = cmd_name(arg, cmds[0])
        if name is None:
            continue
        code, _ = reach(g.files[fid], off, cmds)
        hits = sorted(i for i in code.values() if i[1] == name)
        if not hits:
            continue
        ids = g.ids(fid, idx) or [(None, idx + 1)]
        where = ", ".join(("zone %d%s script %d" % (z, " (%s)" % zn[z] if z in zn else "", s)) if z is not None
                          else "global script %d" % s for z, s in ids)
        more = " +%d more" % (len(hits) - 3) if len(hits) > 3 else ""
        rows.append(((ids[0][0] if ids[0][0] is not None else 1 << 30, ids[0][1]),
                     "%s: %s%s" % (where, "; ".join(fmt(i) for i in hits[:3]), more)))
    return cap([r for _, r in sorted(rows)], full, "scripts") or ["no script runs command %s" % arg]


BRIEF = 12   # lines a query shows without --all


def cap(out, full, what="lines"):
    """out cut to BRIEF lines and a pointer to --all, unless full."""
    if full or len(out) <= BRIEF:
        return out
    return out[:BRIEF] + ["... %d more %s: --all" % (len(out) - BRIEF, what)]


def who_does(g, want, what, full=False):
    """For each command want() picks: the script, who runs it and the conditions on the way to it (full: three lines or
    more each; else one line: the first runner and the first condition set, '+N' for the others)."""
    out = []
    for fid, idx, off in g.scripts():
        f = g.files[fid]
        code, _ = reach(f, off, g.cmds_for(fid))
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
            sets = merge(by_ins.get(ins, []))
            if not full:
                run = (runs[0] + (" +%d" % (len(runs) - 1) if len(runs) > 1 else "")) if runs else "nothing in the atlas"
                cond = (", ".join(sets[0]) or "always") + (" +%d" % (len(sets) - 1) if len(sets) > 1 else "") if sets else "always"
                out.append("%s @0x%04x: %s | run by %s | if %s%s" % (where, ins[0], what(ins), run, cond, " (cut)" if cut else ""))
                continue
            out.append("%s @0x%04x: %s" % (where, ins[0], what(ins)))
            out.append("  run by: " + ("; ".join(runs[:6]) or "nothing in the atlas (another script's call, or the field's code: a gym ride)"))
            for cs in sets[:4]:
                out.append("  if: " + (", ".join(cs) or "always"))
            if cut:
                out.append("  (paths cut at the cap)")
    return cap(out, full, "scripts")


# ---- next: what can happen from a save's story values ----

# the save (512 KB, two copies; launcher_core.c's FooterOk): per game the footer's end (the save's size), the backup
# copy's offset, the event work block (include/save/event_work.h's EventWork: u16 works from 0x4000, then the saved
# flags' bits; save block 45, at the offset PKHeX's SAV5BW/SAV5B2W2 give, checked on the saves) and its counts
SAVE = {"b": (0x23F9C, 0x24000, 0x20100, 318, 2912), "w": (0x23F9C, 0x24000, 0x20100, 318, 2912),
        "b2": (0x25FA4, 0x26000, 0x1FF00, 431, 3064), "w2": (0x25FA4, 0x26000, 0x1FF00, 431, 3064)}
SAVE_MAGIC = 0x31053527
SAVE_DATE = 0x19400 + 0x28   # the trainer data block's date of the save (launcher_core.c's LC_SaveFileInfo)
SAVE_PLACE = 0x19580         # the player's zone u16, then x, y, z fx32 (found on saves with known places; both games)
# The save's blocks tools/route_state.py reads (offsets from the start of a copy; the block table of PKHeX's SAV5BW and
# SAV5B2W2 = include/gfl/savedata.h's table: the ids of src/system/save_table.c, sizes from its getSize functions, each
# block + a 4 byte footer padded to 0x100). Confirmed on saves (tools/route_tests.py): w-route1 holds 1 Pokemon and
# 5 balls, w-caught 2 Pokemon, b-badge3 3 badges. Both games of a family share a block's layout, not its offset.
#   bag    id 25, BagSave (include/save/bag.h): pockets of {u16 item, u16 count}
#   party  id 26, PokeParty: s32 capacity, s32 count, 6 x PartyPkm (0xDC bytes, encrypted as on the DS), slotExists
#   info   id 52, TrainerGameInfoSave (src/system/trainer_game_info_savedata.c): u32 cash at +0, u32 badge bits at +4
SAVE_BLOCK = {"b": {"bag": 0x18400, "party": 0x18E00, "info": 0x21200},
              "w": {"bag": 0x18400, "party": 0x18E00, "info": 0x21200},
              "b2": {"bag": 0x18400, "party": 0x18E00, "info": 0x21100},
              "w2": {"bag": 0x18400, "party": 0x18E00, "info": 0x21100}}
SAVE_BAG = (("items", 0x000, 310), ("key", 0x4D8, 83), ("tmhm", 0x624, 109), ("medicine", 0x7D8, 48),
            ("berries", 0x898, 64))   # BagSave's pockets: name, offset in the block, slots (both families)
SAVE_PKM = 0xDC              # a PartyPkm: pid, flags, checksum, 4 shuffled 0x20 blocks (0x08..0x87), then the party part
DAILY_FLAGS = {"b": range(0xAA0, 0xB60), "w": range(0xAA0, 0xB60), "b2": range(0xAA0, 0xB04),
               "w2": range(0xAA0, 0xB04),   # EventWork_ResetDailyFlags: no story step
               "pt": range(0xAA0, 0xB60), "hg": range(0), "ss": range(0)}   # pt: generated/vars_flags.txt; hg: TODO
# Gen 4's VarsFlags (pret vars_flags.h / save_vars_flags.h): vars from 0x4000, flags; map-local flags (1..0x40) and vars
VARS4 = {"pt": (0x120, 2912), "hg": (0x170, 2912), "ss": (0x170, 2912)}
TEMP_FLAGS = range(0x4000, 0x4040)   # the zone's own flags, cleared at each zone load: clear when a zone starts
# FieldScript_ResetMapLocalEvents (script_sys.c, both games) clears these at each map change: a zone's own puzzle state
LOCAL_FLAGS, LOCAL_WORKS = range(0, 0x64), range(0x4000, 0x4020)


class State:
    """The story values of a save: works {id: value} (the saved ones, 0x4000..), flags (bytes), zone and tile."""
    local_flags = LOCAL_FLAGS

    @classmethod
    def new_game(cls, game):
        """A Gen 4 new game (no save reader yet): every value 0, in the start zone (GEN4_SRC)."""
        st = cls.__new__(cls)
        nw, nf = VARS4[game]
        start = GEN4_SRC["pt" if game == "pt" else "hg"][4]
        st.game, st.data, st.copy, st.tile = game, None, None, (0, 0)
        st.works, st.flags, st.nflags = dict.fromkeys(range(0x4000, 0x4000 + nw), 0), bytes(nf // 8), nf
        st.zone = next((z for z, n in zone_names(game).items() if n == start), 0)
        st.local_flags = range(1, 0x41)
        return st

    def _gen4(self, game, data):
        """A Gen 4 save: tools/route_state.py's reader picks the newer slot and reads place, works and flags."""
        import route_state
        if game not in route_state.GEN4:
            raise ValueError("%s: no save reader yet (HeartGold and SoulSilver keep the vars and flags in another general "
                             "block than Platinum's: route_state.GEN4 needs its offsets)" % game)
        s = route_state.Save(game, data)
        nw, nf = VARS4[game]
        flags = bytearray(nf // 8)
        for n in s.flags:
            flags[n // 8] |= 1 << (n % 8)
        self.game, self.data, self.copy, self.zone, self.tile = game, None, None, s.zone, s.tile
        self.works = dict.fromkeys(range(0x4000, 0x4000 + nw), 0)
        self.works.update(s.works)
        self.flags, self.nflags, self.local_flags = bytes(flags), nf, range(1, 0x41)

    def __init__(self, game, data):
        if game in atlas_gen4.GEN4:
            return self._gen4(game, data)
        end, copy2, base, nw, nf = SAVE[game]
        self.game = game
        best = None
        for copy in (0, copy2):
            foot = copy + end - 16
            if len(data) < copy + end or struct.unpack_from("<2I", data, foot + 4) != (end, SAVE_MAGIC):
                continue
            w = struct.unpack_from("<I", data, copy + SAVE_DATE)[0]
            key = (w & 127, w >> 7 & 15, w >> 11 & 31, w >> 16 & 31, w >> 21 & 63) if w != 0xFFFFFFFF else ()
            if best is None or key > best[0]:
                best = (key, copy)
        if best is None:
            raise ValueError("not a %s save" % game)
        c = best[1]
        self.data, self.copy = data, c   # the save and the newer copy's offset (tools/route_state.py reads the blocks)
        self.works = dict(zip(range(0x4000, 0x4000 + nw), struct.unpack_from("<%dH" % nw, data, c + base)))
        self.flags, self.nflags = data[c + base + 2 * nw: c + base + 2 * nw + nf // 8], nf
        self.zone = struct.unpack_from("<H", data, c + SAVE_PLACE)[0]
        x, _, z = struct.unpack_from("<3i", data, c + SAVE_PLACE + 4)
        self.tile = (x >> 16, z >> 16)

    def value(self, e, here=True):
        """A condition operand's value now (here False: in another zone, after the map change cleared the local
        values); None: not a story value (a script variable, a command's result)."""
        if e[0] == "c":
            return e[1]
        if e[0] == "work":
            return 0 if not here and e[1] in LOCAL_WORKS else self.works.get(e[1])
        if e[0] == "flag":
            n = e[1]
            if n in TEMP_FLAGS or not here and n in self.local_flags:
                return 0
            return self.flags[n // 8] >> (n % 8) & 1 if n < self.nflags else None
        return None

    def holds(self, c, here=True):
        """True/False for a condition the values decide, None when they don't."""
        if c[0] != "cmp":
            c = as_cond(c)
        a, b = self.value(c[1], here), self.value(c[3], here)
        return None if a is None or b is None else truth(("cmp", ("c", a), c[2], ("c", b)))


def load_state(game, name):
    """The State of a save: a path, or a name in route.py's saves directory (Gen 4: 'new', a new game)."""
    if name == "new" and game in atlas_gen4.GEN4:
        return State.new_game(game)
    if "/" not in name and not name.endswith(".bin"):
        import route
        name = str(route.SAVES / ("%s.bin" % name))
    p = Path(name)
    return State(game, p.read_bytes())


def story_change(ins, st, zone):
    """What the command changes in the story values st holds (or the map), as a short text; None: nothing (a zone's
    local values count in the save's zone only)."""
    off, name, args, end = ins
    here = zone == st.zone
    if name in MAPCHANGE:
        return "warp to zone %d" % args[0][1] if args and args[0][1] != zone else None
    if name in ("FlagSet", "FlagReset"):
        n = args[0][1]
        if n in DAILY_FLAGS[st.game] or not here and n in st.local_flags:
            return None
        return what_set(ins) if n < st.nflags and st.value(("flag", n)) != (name == "FlagSet") else None
    if not args or args[0][1] not in st.works or not here and args[0][1] in LOCAL_WORKS:
        return None
    if name in ("WorkSetConst", "WorkSet"):
        v = args[1][1]
        new = v if name == "WorkSetConst" or v < 0x4000 else st.works.get(v)
        return what_set(ins) if new != st.works[args[0][1]] else None
    return what_set(ins) if name in ("WorkAdd", "WorkSub") else None


def changer(st):
    """want() for the commands that may change a saved flag or work, or the map."""
    def want(ins):
        name, args = ins[1], ins[2]
        if name in MAPCHANGE:
            return bool(args)
        if name in ("FlagSet", "FlagReset"):
            return args[0][1] < st.nflags
        return name in ("WorkSetConst", "WorkSet", "WorkAdd", "WorkSub") and bool(args) and args[0][1] in st.works
    return want


def checkers(g):
    """{("flag"|"work", n): {(file, index)}}: the scripts that check each flag and work; plus the spawn flags of the
    atlas's actors and the works of its triggers and scene rows under the key's script None."""
    out = {}
    for fid, idx, off in g.scripts():
        code, _ = reach(g.files[fid], off, g.cmds_for(fid))
        for ins in code.values():
            name, args = ins[1], ins[2]
            if name in ("VMStackPushFlag", "FlagGet", "FlagCheck"):
                out.setdefault(("flag", args[0][1]), set()).add((fid, idx))
            elif name in ("VMStackPush", "WorkCmpConst", "WorkCmpWork") and 0x4000 <= args[0][1] < 0x8000:
                out.setdefault(("work", args[0][1]), set()).add((fid, idx))
    for z in g.atlas["zones"]:
        for a in z["npcs"]:
            if a["flag"]:
                out.setdefault(("flag", a["flag"]), set()).add(None)
        for t in z["triggers"]:
            out.setdefault(("work", t["work"]), set()).add(None)
        for _, _, w, _ in g.scene(z["zone"]):
            if w is not None:
                out.setdefault(("work", w), set()).add(None)
    return out


def distances(data, start):
    """{zone: the atlas path length from start} (warps, edges and scripted doors, their conditions not checked)."""
    dist, q = {start: 0}, [start]
    for z in q:
        for n, _ in atlas.neighbours(data, z):
            if n not in dist:
                dist[n] = dist[z] + 1
                q.append(n)
    return dist


def next_steps(g, st, zone=None, n=15, blocked=5):
    """The runnable things (actor, trigger, BG event, scene table row) whose script's checks pass with st's values and
    that change a story value something else checks (or warp to another zone), nearest first; then a few that one
    condition blocks."""
    zone = st.zone if zone is None else zone
    dist = distances(g.atlas, zone)
    gates = checkers(g)
    want, memo = changer(st), {}
    ready, near = [], []
    for zd in g.atlas["zones"]:
        z = zd["zone"]
        if z not in dist:
            continue
        at = (lambda e: " (%s)" % atlas.tile(e)) if g.game in atlas_gen4.GEN4 else (lambda e: "")   # Gen 4: where each runner stands
        runs = [("G:npc:%d%s" % (a["uid"], at(a)), a["scr"], [("cmp", ("flag", a["flag"]), 1, ("c", 0))] if a["flag"] else [])
                for a in zd["npcs"]]
        runs += [("G:trig:%d%s" % (t["i"], at(t)), t["scr"], [("cmp", ("work", t["work"]), 1, ("c", t["value"]))]
                  if t["work"] else []) for t in zd["triggers"]]
        runs += [("G:bg:%d%s" % (i, at(b)), b["scr"], []) for i, b in enumerate(zd["bg"])]
        runs += [("scene %d" % i, s, [("cmp", ("work", w), 1, ("c", v))])
                 for i, (_, s, w, v) in enumerate(g.scene(z)) if w is not None]
        for tok, sid, pre in runs:
            r = g.resolve(z, sid)
            if r is None or r[0] >= len(g.files) or r[1] >= len(script_offsets(g.files[r[0]])):
                continue
            if r not in memo:
                off = script_offsets(g.files[r[0]])[r[1]]
                code, _ = reach(g.files[r[0]], off, g.cmds_for(r[0]))
                memo[r] = explore(code, off, want)[0] if any(want(i) for i in code.values()) else []
            sets, needs, waited = [], {}, False
            for ins, conds, _ in memo[r]:
                what = story_change(ins, st, z)
                key = ("flag" if ins[1].startswith("Flag") else "work", ins[2][0][1])
                if what is None or ins[1] not in MAPCHANGE and not gates.get(key, set()) - {r}:
                    continue   # no change, or a value only its own script checks (a gift's "given" flag)
                fails = [c for c in pre + list(conds) if st.holds(c, z == st.zone) is False]
                if not fails:
                    if what not in sets:
                        sets.append(what)
                    waited |= None in gates.get(key, ())
                elif len(fails) == 1:
                    needs.setdefault(show(fails[0]), []).append(what)
            label = "zone %d%s %s (script %d%s)" % (z, " (%s)" % g.znames[z] if z in g.znames else "", tok, sid,
                                                    " " + g.label(z, sid) if g.label(z, sid) else "")
            if sets:   # the ones that change what an actor's spawn, a trigger or a scene row waits on first: '*'
                text = ", ".join(sets[:4]) + (" ..." if len(sets) > 4 else "")
                ready.append((dist[z], not waited, z, "%s%s: %s" % ("*" if waited else " ", label, text)))
            elif needs:
                need, whats = min(needs.items(), key=lambda kv: -len(kv[1]))
                near.append((dist[z], 0, z, " %s: needs %s -> %s" % (label, need, ", ".join(sorted(set(whats))[:3]))))
    head = "%s save: zone %d tile %d,%d; %d works nonzero, %d flags set%s" % (
        g.game, st.zone, st.tile[0], st.tile[1], sum(1 for v in st.works.values() if v),
        sum(bin(b).count("1") for b in st.flags), "" if zone == st.zone else "; ranked from zone %d" % zone)
    scenes = works_text(st, [k for k in st.works if None in gates.get(("work", k), ())]) if g.game in atlas_gen4.GEN4 else ""
    out = [head] + (["scene works (nonzero, a trigger or scene row waits on them): " + scenes] if scenes else [])
    out += ["can run now (atlas path length; *: changes what an actor spawn flag, trigger or scene row waits on):"]
    out += ["  %2d%s" % (d, t) for d, _, _, t in sorted(ready)[:n]] or ["  nothing"]
    out += ["one condition away:"] + (["  %2d%s" % (d, t) for d, _, _, t in sorted(near)[:blocked]] or ["  nothing"])
    return out


def works_text(st, only, n=12):
    """The nonzero works among `only` on one line (name or number = value), the first n and a count of the rest."""
    ws = ["%s=%d" % (NAMES4.get(k, "0x%x" % k), st.works[k]) for k in sorted(only) if st.works.get(k)]
    return " ".join(ws[:n]) + (" +%d more" % (len(ws) - n) if len(ws) > n else "")


def state_lines(game, st, full=False):
    """A save's story values: the zone, each nonzero work, then the flags set that have a name (daily flags left out; the
    others counted). Gen 4 has the names (NAMES4); Gen 5 prints numbers."""
    if game in atlas_gen4.GEN4:
        load_names4(game)
    flags = [n for n in range(st.nflags) if st.flags[n // 8] >> (n % 8) & 1 and n not in DAILY_FLAGS[game]]
    named = ["flag %s" % nm(n) for n in flags if n in NAMES4]
    zn = zone_names(game).get(st.zone)
    out = ["%s save: zone %d%s tile %d,%d; %d works nonzero, %d flags set (%d named)" % (
        game, st.zone, " (%s)" % zn if zn else "", st.tile[0], st.tile[1], sum(1 for v in st.works.values() if v),
        len(flags), len(named))]
    out += cap(["work %s = %d" % (nm(k), v) for k, v in sorted(st.works.items()) if v], full, "works")
    return out + cap(named, full, "flags") + (["%d flags set without a name" % (len(flags) - len(named))]
                                              if len(flags) > len(named) else [])


def show_script(g, zone, sid, full=False):
    r = g.resolve(zone, sid)
    if r is None or r[0] >= len(g.files):
        return ["no script %d in zone %d" % (sid, zone)]
    f = g.files[r[0]]
    offs = script_offsets(f)
    if r[1] >= len(offs):
        return ["zone %d has %d scripts" % (zone, len(offs))]
    code, bad = reach(f, offs[r[1]], g.cmds_for(r[0]))
    out = ["file %d script %d%s: %d commands" % (r[0], r[1] + 1, " (%s)" % g.label(zone, sid) if g.label(zone, sid) else "",
                                                  len(code))]
    out += [fmt(code[o]) for o in sorted(code)]
    out += ["0x%04x  ?? does not decode" % b for b in bad]
    return out if full or len(out) <= 60 else out[:60] + ["... %d more commands: --all" % (len(out) - 60)]


def summary(code):
    checks, sets = set(), set()
    for ins in code.values():
        off, name, args, end = ins
        if name in ("VMStackPushFlag", "FlagGet", "FlagCheck"):
            checks.add("flag " + nm(args[0][1]))
        elif name in ("VMStackPush", "WorkCmpConst", "WorkCmpWork") and 0x4000 <= args[0][1] < 0x8000:
            checks.add("work " + nm(args[0][1]))
        elif name in ("FlagSet", "FlagReset") or (name in ("WorkSetConst", "WorkSet", "WorkAdd", "WorkSub") and
                                                  0x4000 <= args[0][1] < 0x8000):
            sets.add(what_set(ins))
    return sorted(checks), sorted(sets)


def show_zone_scripts(g, zone, full=False):
    z = g.atlas["zones"][zone]
    f = g.files[z["script"]]
    out = ["zone %d: script file %d, %d scripts" % (zone, z["script"], len(script_offsets(f)))]
    for i, off in enumerate(script_offsets(f)):
        code, bad = reach(f, off, g.cmds_for(z["script"]))
        checks, sets = summary(code)
        runs = g.runners(zone, i + 1)
        if not full:   # one line: the first runner, then what it checks and sets (the first three of each)
            more = lambda xs: ", ".join(xs[:3]) + (" +%d" % (len(xs) - 3) if len(xs) > 3 else "")
            out.append("script %d%s: %s%s%s%s%s" % (i + 1, " " + g.label(zone, i + 1) if g.label(zone, i + 1) else "",
                                                   runs[0] + (" +%d" % (len(runs) - 1) if len(runs) > 1 else "") if runs else "-",
                                                 " | checks " + more(checks) if checks else "", " | sets " + more(sets) if sets else "",
                                                 " | %d don't decode" % len(bad) if bad else "", ""))
            continue
        out.append("script %d%s: %s" % (i + 1, " " + g.label(zone, i + 1) if g.label(zone, i + 1) else "", "; ".join(runs) or "-"))
        if checks:
            out.append("  checks " + ", ".join(checks))
        if sets:
            out.append("  sets " + ", ".join(sets))
        if bad:
            out.append("  %d commands don't decode" % len(bad))
    return out[:1] + cap(out[1:], full, "scripts")


def check(g):
    n = bad = 0
    ops = {}
    for fid, idx, off in g.scripts():
        f = g.files[fid]
        code, b = reach(f, off, g.cmds_for(fid))
        n += len(code)
        bad += len(b)
        for p in b:
            # the command that runs into an undecodable spot is the likely wrong one (a jump or an entry isn't)
            prev = [i[1] for i in code.values() if i[3] == p and i[1] not in STOP] or ["(entry or jump target)"]
            for name in prev:
                ops[name] = ops.get(name, 0) + 1
    out = ["%s: %d commands decoded, %d spots that don't decode" % (g.game, n, bad)]
    out += ["  before a bad spot: %s x%d" % kv for kv in sorted(ops.items(), key=lambda kv: -kv[1])[:15]]
    return out


def main(argv):
    if len(argv) < 2 or argv[0] not in GAMES:
        sys.exit(__doc__)
    game, cmd = argv[0], argv[1]
    full = "--all" in argv
    argv = [a for a in argv if a != "--all"]
    if cmd == "next" and len(argv) >= 3:
        opt = dict(zip(argv[3::2], argv[4::2]))
        g = Game(game)
        print("\n".join(next_steps(g, load_state(game, argv[2]), int(opt["--zone"], 0) if "--zone" in opt else None,
                                    int(opt.get("--n", "15"), 0))))
        return
    if cmd == "state" and len(argv) >= 3:
        print("\n".join(state_lines(game, load_state(game, argv[2]), full)))
        return
    if cmd == "who-uses" and len(argv) >= 3:
        print("\n".join(who_uses(Game(game), argv[2], full)))
        return
    nums = [int(a, 0) for a in argv[2:] if not a.startswith("=") and a not in ("work", "flag")]
    g = Game(game)
    if cmd == "who-sets" and len(argv) >= 4 and argv[2] in ("work", "flag"):
        value = next((int(a[1:], 0) for a in argv[4:] if a.startswith("=")), None)
        out = who_sets(g, argv[2], nums[0], value, full)
    elif cmd == "who-warps" and nums:
        out = who_warps(g, nums[0], full)
    elif cmd == "script" and len(nums) == 2:
        out = show_script(g, nums[0], nums[1], full)
    elif cmd == "zone" and nums:
        out = show_zone_scripts(g, nums[0], full)
    elif cmd == "check":
        out = check(g)
    else:
        sys.exit(__doc__)
    print("\n".join(out))


if __name__ == "__main__":
    main(sys.argv[1:])
