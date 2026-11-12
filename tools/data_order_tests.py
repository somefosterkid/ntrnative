#!/usr/bin/env python3
"""Tests of tools/data_order.py's model run backwards (invert): for random lists of objects, Black's order taken as
the layout of a random shuffle of equal-size definitions, invert() must find a list that lays out so, moving named
objects only between slots of their own size. Then permute() on a shuffle of whole definitions with their strings,
and parse_order() on compiled files of static const scalars (listed after everything, by the functions that take
their addresses), local const scalars (with them, by their functions), tentative definitions and string tables, and
on random files compiled with each compiler and mode of the tree (MODE: sections of their own, -ipa file or not).

    python3 tools/data_order_tests.py
"""
import io
import os
import random
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import data_order as D  # noqa: E402


def obj(name, size, own=False, section=".rodata"):
    return D.Obj(name, size, own, b"", set(), None, section, False)


def main():
    rng = random.Random(7)
    for trial in range(300):
        n = rng.randint(2, 14)
        listed = [obj(f"o{i}" if rng.random() < 0.85 else f"@{i}", rng.choice((2, 4, 4, 8, 8, 12, 16, 70)),
                      section=rng.choice((".rodata", ".rodata", ".data"))) for i in range(n)]
        # Black's: the same sizes per slot, named objects shuffled among equal sizes and sections
        shuffled = list(listed)
        for key in {(o.size, o.section) for o in listed}:
            idx = [i for i, o in enumerate(listed) if (o.size, o.section) == key and not o.name.startswith("@")]
            perm = idx[:]
            rng.shuffle(perm)
            for i, j in zip(idx, perm):
                shuffled[i] = listed[j]
        targets = {sec: [(o.name, o.size, {o.name}) for o in D.layout(shuffled, sec)]
                   for sec in (".rodata", ".data") if any(o.section == sec for o in listed)}
        inv = D.invert(listed, targets)
        assert inv is not None and D.fits(inv, targets), (trial, listed, targets)
        assert all(a.size == b.size for a, b in zip(inv, listed))
        assert all(a is b for a, b in zip(inv, listed) if b.name.startswith("@"))
    # an order that differs by more than ties: none
    listed = [obj("a", 4), obj("b", 8)]
    assert D.invert(listed, {".rodata": [("b", 8, {"b"}), ("a", 4, {"a"})]}) is None
    # tentative definitions trade places among themselves (nd.c's .bss words), not with an initialized one
    D.TENTATIVE.clear()
    D.TENTATIVE.update({"t1", "t2"})
    listed = [obj("i1", 4, section=".bss"), obj("t2", 4, section=".bss"), obj("t1", 4, section=".bss")]
    lay = [o.name for o in D.layout(listed, ".bss")]
    swapped = [{"t1": "t2", "t2": "t1"}.get(n, n) for n in lay]
    inv = D.invert(listed, {".bss": [(n, 4, {n}) for n in swapped]})
    assert inv is not None and [o.name for o in D.layout(inv, ".bss")] == swapped, (lay, inv)
    assert D.invert(listed, {".bss": [(n, 4, {n}) for n in [{"t1": "i1", "i1": "t1"}.get(n, n) for n in lay]]}) is None
    D.TENTATIVE.clear()
    permutations(rng)
    cross_kinds(rng)
    scalars()
    local_scalars()
    compilers(rng)
    link_drops()
    ltdmain()
    twins()
    late_range()
    after_a_function()
    declarators()
    parameter_names()
    print("ok")


def parameter_names():
    """natneg.c's `static void (*natifyCallback)(u32 success, NAT nat);` before `static NAT nat;`: the parameter was
    taken for nat's definition, which listed nat with natifyCallback, and the list didn't reproduce the .bss (t37a's
    natneg). A function pointer's own name is its declarator's."""
    text = NL.join(["typedef unsigned int u32;", "typedef struct { u32 a[60]; } NAT;", "extern void g(const void *);",
                    "static void (*cb)(u32 ok, NAT nat);", "static u32 x1;", "static u32 x2;", "static NAT nat;",
                    "static u32 x3;", "static u32 x4;",
                    "void f(void) { g(&cb); g(&x1); g(&x2); g(&nat); g(&x3); g(&x4); }", ""])
    o = D.compile_c(text)
    listed, notes = D.parse_order(D.listed_objects(o), "src/test.c", D.object_users(o), text=text)
    assert not notes, notes
    for name in ("cb", "nat"):
        assert text[D.POS[name][0]:].startswith("static void (*cb)" if name == "cb" else "static NAT nat;"), name
    ours = [x.name for x in D.data_objects(o, ".bss") if not x.pooled]
    assert [x.name for x in D.layout(listed, ".bss")] == ours, (ours, [x.name for x in listed])


def declarators():
    """e_pow.c's `static const double bp[] = {...},` with `dp_h[] = {...}` on lines of their own, os_thread.c's
    indented `    static int OSi_ThreadIdCount = 0;` in an #ifdef: each placed at its declarator (they were "listed
    first", and the list didn't reproduce the object); nsbma.c's and g2d_CharCanvas.c's local statics that MWCC 2.0
    names <variable>$<N> are their functions' (t35d's 11 sections, t37a)."""
    text = NL.join(["typedef unsigned int u32;", "extern void g(const void *);",
                    "static const u32", "    bp[2] = { 1, 2 },", "    dp_h[2] = { 3, 4 },", "    dp_l[2] = { 5, 6 };",
                    "#if 1", "    static u32 count = 1;", "#endif",
                    "void f(int k) { g(&bp[k]); g(&dp_h[k]); g(&dp_l[k]); g(&count); }", ""])
    o = D.compile_c(text)
    listed, notes = D.parse_order(D.listed_objects(o), "src/test.c", D.object_users(o), text=text)
    assert not notes, notes
    for name in ("bp", "dp_h", "dp_l", "count"):
        assert text[D.POS[name][0]:].lstrip().startswith(("static", name)), (name, D.POS[name])
    for sec in D.SECTIONS:
        ours = [x.name for x in D.data_objects(o, sec) if not x.pooled]
        assert [x.name for x in D.layout(listed, sec)] == ours, (sec, ours, [x.name for x in listed])
    code = NL.join(["void a(void) {", "    const u32 cmask0 = 1;", "}", "void b(void) {", "    const u32 cmask0 = 2;",
                    "    static const u32 shape[2] = { 1, 2 };", "    const u32 *shape2 = shape;", "}", ""])
    D.dollar_statics([obj("cmask0$2502", 4), obj("cmask0$2490", 4), obj("shape$2717", 8)], code,
                     D.function_bodies(code))
    assert D.local_static("cmask0$2490") == ("a", "cmask0") and D.local_static("cmask0$2502") == ("b", "cmask0")
    assert D.local_static("shape$2717") == ("b", "shape"), D.DOLLAR
    D.DOLLAR.clear()


def after_a_function():
    """A definition on the line after a blank one below a function's closing brace is the C's own item, not the
    function's: parse_order() placed btlv_input.c's data_ov094_0220a3d4 at the brace and --apply said it "can't move"
    (a local of a function), t35d/t37a."""
    text = NL.join(["typedef unsigned int u32;", "extern void g(const void *);",
                    "void f(void) {", "    g(0);", "}", "",
                    "static const u32 t[2] = { 1, 2 };", "void h(void) { g(t); }", ""])
    o = D.compile_c(text)
    D.parse_order(D.listed_objects(o), "src/test.c", D.object_users(o), text=text)
    assert D.POS["t"][0] == text.index("static const u32 t"), (D.POS["t"], text.index("static const u32 t"))
    items = D.top_items(text, D.blank(text))
    k = next(k for k, it in enumerate(items) if it.start <= D.POS["t"][0] < it.end)
    assert items[k].kind == "declaration", items[k]


def late_range():
    """un_select.c's .rodata range started 4 bytes late (t37e): its two folded backColor u8 statics, at the module's
    .rodata start in no file's range, showed as another file's. Black's objects start where ours cover the bytes before
    the range, when ours don't fit it (here a 12-byte range for 14 bytes); when they fit, at the range."""
    rom = open(os.path.join(D.ROOT, "extract/b_us/arm9_overlays/ov218.bin"), "rb").read()
    base = 0x021b95a0
    delinks = os.path.join(D.ROOT, "config/b_us/arm9/overlays/ov218/delinks.txt")
    text = re.sub(r"(un_select[.]c:\n(?:    .*\n)*?    [.]rodata\s+)start:0x\w+ end:0x\w+",
                  lambda m: m[1] + "start:0x021bab14 end:0x021bab20", open(delinks).read())
    assert "start:0x021bab14 end:0x021bab20" in text
    real = open
    D.open = lambda path, *a, **k: io.StringIO(text) if path == delinks else real(path, *a, **k)
    color = [D.Obj(f"backColor{i}", 1, False, bytes([15]), set(), None, ".rodata", False) for i in (1, 2)]
    table = D.Obj("data_ov218_021bab14", 12, False, rom[0x021bab14 - base:0x021bab20 - base], set(), None, ".rodata",
                  False)
    D.TARGETS.clear(), D.SITES.clear(), D.EARLY.clear()
    try:
        got = D.black_objects("src/ov218/un_select.c", ".rodata", color + [table])
        assert [(label, size) for label, size, _ in got] == [("0x21bab10", 1), ("0x21bab11", 1),
                                                             ("data_ov218_021bab14", 12)], got
        assert got[0][2] == {"backColor1", "backColor2"}, got
        assert D.EARLY == {("src/ov218/un_select.c", ".rodata"): (0x021bab10, 0x021bab14)}, D.EARLY
        D.EARLY.clear()
        got = D.black_objects("src/ov218/un_select.c", ".rodata", [table])
        assert [label for label, _, _ in got] == ["data_ov218_021bab14"] and not D.EARLY, (got, D.EARLY)
    finally:
        del D.open


def twins():
    """btlv_input.c's two byte-identical s32[7] tables (0x0220a3fc and 0x0220a434) are told apart by the button sets
    that point to them (Black's relocations from 0x0220a3a8+4 and 0x0220a39c+4): either "fit" each label, and an
    order of the twins swapped passed (t37a)."""
    rom = open(os.path.join(D.ROOT, "extract/b_us/arm9_overlays/ov094.bin"), "rb").read()
    base = 0x021f6560
    ours = [D.Obj(f"data_ov094_{a:08x}", 28, False, rom[a - base:a - base + 28], set(), None, ".rodata", False)
            for a in (0x0220a3fc, 0x0220a434)]
    assert ours[0].data == ours[1].data
    D.TARGETS.clear(), D.SITES.clear()
    D.TARGETS.update({"data_ov094_0220a3a8": {4: ("data_ov094_0220a3fc", 0)},
                      "data_ov094_0220a39c": {4: ("data_ov094_0220a434", 0)}})
    try:
        got = {label: names for label, _, names in D.black_objects("src/ov094/btlv_input.c", ".rodata", ours)}
    finally:
        D.TARGETS.clear()
    assert got["data_ov094_0220a3fc"] == {"data_ov094_0220a3fc"}, got.get("data_ov094_0220a3fc")
    assert got["data_ov094_0220a434"] == {"data_ov094_0220a434"}, got.get("data_ov094_0220a434")


def ltdmain():
    """A TwlSDK LTD file (twl/ltdmain_begin.h) has its initialized data in .ltdmain with its code: the model lists
    those objects with the others (not its functions), its code is the .ltdmain sections that hold functions, and its
    module is the LTD autoload (t35g: mi_sharedWram_twl.c's .bss statics read as dropped, as main's)."""
    text = NL.join([
        "typedef unsigned int u32;",
        '#pragma define_section LTDMAIN ".ltdmain" ".ltdmain.bss" abs32 RWX',
        "#pragma section LTDMAIN begin",
        "static u32 table[3] = {1, 2, 3};",
        "u32 get(int i) { return table[i]; }",
        "#pragma section LTDMAIN end", ""])
    o = D.compile_c(text)
    got = [x.name for x in D.data_objects(o, ".ltdmain")]
    assert got == ["table"], got
    code = D.code_of(o)
    assert len(code) == 1 and code[0][0], code
    assert D.module_of("lib/twl/src/mi/mi_sharedWram_twl.c") == "ltd_autoload_0"
    assert D.module_of("lib/nitro/src/mi/mi_dma_itcm.c") == "itcm"


def permutations(rng):
    """permute() finds the declaration order of five string tables (each with its 15-byte strings) that a shuffle
    gives, which moves of single objects don't."""
    for trial in range(20):
        units, listed = [], []
        D.POS.clear(), D.TENTATIVE.clear(), D.DEFERRED.clear()
        k = 0
        for t in range(5):
            strings = [obj(f"@{k + j}", 15, True, ".data") for j in range(rng.randint(1, 3))]
            k += len(strings)
            table = obj(f"t{t}", 4 * len(strings), False, ".data")
            for x in strings:
                D.POS[x.name] = (t * 10, -0.5)
            D.POS[table.name] = (t * 10, 0)
            units.append(strings + [table])
        order = list(range(5))
        rng.shuffle(order)
        listed = [x for u in units for x in u]
        black = [x for k in order for x in units[k]]
        targets = {".data": [(o.name, o.size, {o.name}) for o in D.layout(black, ".data")]}
        found, (bad, _), names, _ = D.permute(listed, targets)
        assert found and bad == 0 and any([o.name for o in f] == [units[k][-1].name for k in order] for f in found)
        assert all(D.fits([x for o in f for u in units if u[-1] is o for x in u], targets) for f in found)
    # more than MAX_UNITS (9) definitions: permute() searched only 9 of them and missed an order of all 12 (t37a)
    for trial in range(5):
        units = []
        D.POS.clear(), D.TENTATIVE.clear(), D.DEFERRED.clear()
        k = 0
        for t in range(12):
            strings = [obj(f"@{k + j}", 15, True, ".data") for j in range(rng.randint(1, 3))]
            k += len(strings)
            table = obj(f"t{t}", 4 * len(strings), False, ".data")
            for x in strings:
                D.POS[x.name] = (t * 10, -0.5)
            D.POS[table.name] = (t * 10, 0)
            units.append(strings + [table])
        order = list(range(12))
        rng.shuffle(order)
        listed = [x for u in units for x in u]
        black = [x for k in order for x in units[k]]
        targets = {".data": [(o.name, o.size, {o.name}) for o in D.layout(black, ".data")]}
        found, (bad, _), names, _ = D.permute(listed, targets)
        assert found and bad == 0 and len(names) > D.MAX_UNITS, (trial, bad, len(names))
        assert D.fits([x for o in found[0] for u in units if u[-1] is o for x in u], targets)


def scalars():
    """MWCC's own layout of small files, each where the model's list must give it: a static const scalar is listed
    after the tentative definitions, with the first function that takes its address, the functions last first."""
    head = "typedef unsigned int u32;" + chr(10) + "extern void g(const void*);" + chr(10)
    fill = "".join(f"static u32 d{i}[1] = {{{i}}};" + chr(10) for i in range(1, 9))
    use = " ".join(f"g(&d{i});" for i in range(1, 9))
    k = ("static const u32 k1 = 3;" + chr(10) + "static u32 t;" + chr(10) + "static const u32 k2 = 4;" + chr(10)
         + "static const u32 k3 = 5;" + chr(10))
    cases = [f"void f(void) {{ {use} g(&k3); g(&k1); g(&t); g(&k2); }}",
             f"void f(void) {{ {use} g(&k2); g(&t); }}" + chr(10) + "void f2(void) { g(&k3); }"
             + chr(10) + "void f3(void) { g(&k1); }",
             f"void f(void) {{ {use} g(&t); }}" + chr(10) + "void fa(void) { g(&k1); g(&k3); }" + chr(10)
             + "void fb(void) { g(&k2); g(&k1); }"]
    for body in cases:
        text = head + fill + k + body + chr(10)
        o = D.compile_c(text)
        listed, _ = D.parse_order(D.listed_objects(o), "src/test.c", D.object_users(o), text=text)
        for sec in D.SECTIONS:
            ours = [x.name for x in D.data_objects(o, sec) if not x.pooled]
            assert [x.name for x in D.layout(listed, sec)] == ours, (body, sec, ours, [x.name for x in listed])
        assert len(D.DEFERRED) == 3


def local_scalars():
    """A function's local const scalar (`const int left = 0;`, an object `_ZZ...` though every read is folded) is
    listed with the deferred scalars, not where it is declared: after the tentative definitions, each function's
    together, the functions last first, in the order of its declarations and the `&x` of file-level scalars in it
    (namein.c's eight 4-byte locals in three functions), static or not, its address taken or not; a local static
    table and a local initializer stay where they are."""
    nl = chr(10)
    head = "typedef unsigned int u32;" + nl + "extern void use(const void *p, int v);" + nl
    cases = [
        "const u32 tA[1] = {1};" + nl + "void f1(void) { const int a = 5; const int b = 6; use(tA, a + b); }" + nl
        + "const u32 tB[1] = {4};" + nl + "void f2(void) { const int c = 7; use(tB, c); }" + nl
        + "const u32 tC[1] = {9};" + nl + "void f3(void) { const int d = 8; const int e = 9; use(tC, d * e); }" + nl
        + "const u32 tD[1] = {10};" + nl + "void f4(void) { use(tD, 0); }",
        "void f1(void) { const int a = 5; const int b = 6; use(0, a + b); }" + nl
        + "void f2(void) { const int c = 7; use(0, c); }" + nl
        + "void f3(void) { const int d = 8; const int e = 9; use(0, d * e); }" + nl
        + "void f4(void) { const int f = 1; const int g = 2; const int h = 3; use(0, f + g + h); }",
        "void f1(void) { const int a = 5; static const u32 s1[1] = {3}; const int b = 6; use(s1, a + b); }" + nl
        + "const u32 tB[1] = {4};" + nl
        + "void f2(void) { const int c = 7; const u32 loc[3] = {1, 2, 3}; use(tB, c + loc[0]); }" + nl
        + "const u32 tC[1] = {9};" + nl + "void f3(void) { const int d = 8; use(tC, d); }",
        "static const u32 gs = 3;" + nl + "static const u32 gt = 4;" + nl + "const u32 tA[1] = {1};" + nl
        + "void f1(void) { const int a = 5; use(&gs, a); { const int b = 6; use(&gt, b); } }" + nl
        + "void f2(void) { const int c = 7; use(&gt, c); }",
        "const u32 tA[1] = {1};" + nl + "void f1(void) { static const u32 s = 3; const int a = 2; use(&s, a); }" + nl
        + "const u32 tB[1] = {2};" + nl + "void f2(void) { static const u32 t = 4; use(tB, t); }" + nl
        + "const u32 tC[1] = {3};" + nl
        + "void f3(void) { static const u32 v = 5; const u32 *p = tA; use(&v, 1); use(tC, 0); use(p, 0); }",
        "static u32 tent1;" + nl + "static u32 tent2;" + nl + "static u32 init1 = 0;" + nl
        + "void f1(void) { const int a = 5; use(&tent1, a); }" + nl
        + "void f2(void) { const int c = 7; const int d = 8; use(&tent2, c + d); use(&init1, 0); }",
    ]
    for body in cases:
        text = head + body + nl
        o = D.compile_c(text)
        listed, _ = D.parse_order(D.listed_objects(o), "src/test.c", D.object_users(o), text=text)
        for sec in D.SECTIONS:
            ours = [x.name for x in D.data_objects(o, sec) if not x.pooled]
            assert [x.name for x in D.layout(listed, sec)] == ours, (body, sec, ours, [x.name for x in listed])


# Each compiler and mode of the tree's files (configure.py's compiler_of), by one file that has it
MODES = [("dsi/1.1p1", "-ipa file"), ("dsi/1.1p1", ""), ("dsi/1.1p1", "-ipa function"), ("dsi/1.2", "-ipa file"),
         ("2.0/sp2p3", ""), ("2.0/sp2p3", "-ipa file"), ("2.0/sp2p2", ""), ("2.0/sp1p2", "-ipa file"),
         ("2.0/sp1p2", "-ipa function")]  # (1.2/base, lib/spl's 8 files: about 1 in 20 has its ties otherwise)
NL = chr(10)


def random_file(rng):
    head = ("typedef unsigned int u32; typedef unsigned short u16; typedef unsigned char u8;" + NL
            + "typedef struct { u8 a, b, c; } S3;" + NL + "extern void g(const void *p);" + NL)
    items, names, k = [], [], 0
    for _ in range(rng.randint(3, 14)):
        if rng.random() < 0.25 and names:
            uses = rng.sample(names, rng.randint(0, len(names)))
            loc = ""
            if rng.random() < 0.3:
                t = rng.choice(("u32", "u8", "u16"))
                n = rng.randint(2, 9)
                loc = f"const {t} loc[{n}] = {{" + ", ".join(str(rng.randint(1, 9)) for _ in range(n)) + "}; g(loc); "
            items.append(f"void f{k}(void) {{ {loc}" + " ".join(f"g(&{u});" for u in uses) + " }")
            k += 1
            continue
        t = rng.choice(("u8", "u8", "u16", "u32", "u32", "S3"))
        n = rng.choice((None, None, 1, 2, 3, 5, 7, 20, 40))
        const = rng.random() < 0.5
        kind = rng.choice(("init", "init", "zero", "tent")) if not const else "init"
        name = f"d{len(names)}"
        if const and n is None and t != "S3" and rng.random() < 0.3:
            items.append(f"static const {t} {name} = {rng.randint(1, 9)};")  # a deferred scalar
            names.append(name)
            continue
        dims = f"[{n}]" if n else ""
        st = "static " if rng.random() < 0.85 else ""
        if kind == "tent":
            items.append(f"{st}{t} {name}{dims};")
        else:
            one = "{1, 2, 3}" if t == "S3" else str(rng.randint(1, 9)) if kind == "init" else "0"
            if t == "S3" and kind == "zero":
                one = "{0}"
            val = ("{" + ", ".join([one] * n) + "}") if n else one
            items.append(f"{st}{'const ' if const else ''}{t} {name}{dims} = {val};")
        names.append(name)
    items.append(f"void flast(void) {{ " + " ".join(f"g(&{u});" for u in names) + " }")
    return head + NL.join(items) + NL


def compilers(rng, n=20):
    """The model against MWCC's own layout of random files, for each compiler and mode: the sections of their own
    (64 bytes or more under dsi/* with -ipa file; byte arrays under 2.0/*; none by size under dsi/1.1p1 without -ipa
    file), without -ipa file the sort of what comes before the first function's end only, the tentative definitions
    and static const scalars laid out unsorted last."""
    flat = " ".join(D.FLAGS).replace(" -ipa file", "").split()
    for compiler, ipa in MODES:
        flags = flat + ipa.split()
        rng = random.Random(f"{compiler} {ipa}")
        D.GIVEN.update(compiler=compiler, flags=flags)
        for trial in range(n):
            text = random_file(rng)
            o = D.compile_c(text, flags=flags, compiler=compiler)
            listed, _ = D.parse_order(D.listed_objects(o), "src/test.c", D.object_users(o), text=text)
            for sec in D.SECTIONS:
                ours = [x.name for x in D.data_objects(o, sec) if not x.pooled]
                assert [x.name for x in D.layout(listed, sec)] == ours, (compiler, ipa, sec, ours, text)
    # gsPlatformUtil.c (dsi/1.1p1 without -ipa file): its .bss statics laid out the last defined first, its three
    # 3-byte tables sorted from the last; atermset.c (2.0/sp1p2): 1024-byte tables in the shared section
    cases = [("dsi/1.1p1", "", "static const char a[] = {1, 2, 3};" + NL + "static const char b[] = {4, 5, 6};" + NL
              + "static const char c[] = {7, 8, 9};" + NL + "static int r = 1;" + NL + "static int t1;" + NL
              + "void f(void) { g(a); g(b); g(c); g(&r); g(&t1); }" + NL + "static int t2[3];" + NL
              + "void f2(void) { g(&t2); }", {".rodata": ["c", "b", "a"], ".bss": ["t2", "t1"]}),
             ("2.0/sp1p2", "-ipa file", "static const u32 T0[256] = {1};" + NL + "static const u32 T1[256] = {2};"
              + NL + "static const u8 k[7] = {1};" + NL + "static u32 w;" + NL + "static const u32 s4[1] = {5};" + NL
              + "void f(void) { g(T0); g(T1); g(k); g(&w); g(s4); }", {".rodata": ["s4", "T0", "T1", "k"]})]
    for compiler, ipa, body, want in cases:
        flags = flat + ipa.split()
        D.GIVEN.update(compiler=compiler, flags=flags)
        text = "typedef unsigned int u32; typedef unsigned char u8;" + NL + "extern void g(const void *p);" + NL + body + NL
        o = D.compile_c(text, flags=flags, compiler=compiler)
        listed, _ = D.parse_order(D.listed_objects(o), "src/test.c", D.object_users(o), text=text)
        for sec, names in want.items():
            assert [x.name for x in D.data_objects(o, sec)] == names, (compiler, sec)
            assert [x.name for x in D.layout(listed, sec)] == names, (compiler, sec, [x.name for x in listed])
    D.GIVEN.clear()


def link_drops():
    """The link drops what nothing kept reaches, by whole sections: a dropped function's pooled string goes; a table
    it reads (or its own local static) that shares a section with a kept one stays, its strings too
    (sakeRequestModify.c's); every global stays. And a relocation's addend counts where Black's points."""
    text = NL.join([
        "typedef unsigned int u32;", "extern void g(const void *p);",
        'static const char *const kept[2] = { "ab", "cd" };',
        'static const char *const shared[2] = { "efg", "hij" };',
        "static const u32 small[1] = {1};",
        "u32 global[3] = {1, 2, 3};",
        'void live(void) { g(kept); g(small); g("live string"); }',
        'void dead(void) { static const char *const names[2] = { "klmn", "opqr" }; g(names); g(shared); '
        'g("dead string"); }', ""])
    o = D.compile_c(text)
    D.read_names, old = (lambda root: ({}, {"live": [("ov001", 0x100)]})), D.read_names
    try:
        dropped = D.link_drops("src/ov001/test.c", o)
    finally:
        D.read_names = old
    names = {x.name: x for sec in D.SECTIONS for x in D.data_objects(o, sec)}
    text_of = lambda n: names[n].data.rstrip(bytes(1)).decode()
    gone = sorted(text_of(n) if n.startswith("@") else n for n in dropped)
    # (the 8-byte local table shares .data with kept: it and its strings stay; the pooled string goes)
    assert gone == ["dead string"], gone
    assert D.pointed("from:0x10 kind:load to:0x20 add:0x8 module:overlay(16)" + NL + "from:0x14 kind:load to:0x30"
                     + NL + "from:0x18 kind:load to:0x40 add:-0x4") == {0x10: 0x28, 0x14: 0x30, 0x18: 0x3c}


def cross_kinds(rng):
    """invert() trades equal sizes across sections and kinds (clact.c: a global into a table's slot, a .data table
    into a .rodata slot): Black's list is a shuffle of the named objects among equal sizes, whatever their section."""
    D.TENTATIVE.clear()
    for trial in range(300):
        n = rng.randint(2, 10)
        listed = [obj(f"o{i}" if rng.random() < 0.85 else f"@{i}", rng.choice((4, 4, 8, 8, 12, 70)),
                      own=rng.random() < 0.3, section=rng.choice((".rodata", ".data"))) for i in range(n)]
        shuffled = list(listed)
        for size in {o.size for o in listed}:
            idx = [i for i, o in enumerate(listed) if o.size == size and not o.name.startswith("@")]
            perm = idx[:]
            rng.shuffle(perm)
            for i, j in zip(idx, perm):
                shuffled[i] = listed[j]
        targets = {sec: [(o.name, o.size, {o.name}) for o in D.layout(shuffled, sec)]
                   for sec in (".rodata", ".data") if any(o.section == sec for o in listed)}
        inv = D.invert(listed, targets)
        assert inv is not None and D.fits(inv, targets), (trial, listed, targets)
        assert sorted(o.name for o in inv) == sorted(o.name for o in listed)
        assert all(a.size == b.size for a, b in zip(inv, listed))
        assert all(a is b for a, b in zip(inv, listed) if b.name.startswith("@"))

if __name__ == "__main__":
    main()
