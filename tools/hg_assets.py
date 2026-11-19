#!/usr/bin/env python3
"""tools/hg_assets.py: the ROM bytes of the vendored HeartGold/SoulSilver decompilation (third_party/pokeheartgold) as
blanks plus rows, so the repo holds none of them (docs/hgss.md, "Vendored tree").

tools/rom_leak_check.py finds runs of a file's numbers that are ROM bytes. Each data object a run falls in (an asm label's
data, a C initialiser) keeps its layout with every number 0, and gets a row per game: its DS module, address range and
layout (no ROM bytes), found by its original bytes in the HeartGold ROM and then in the SoulSilver one. The rows go to
port/assets_hg.tsv and port/assets_ss.tsv (tools/port_assets.py rows, the format of config/<v>/assets.tsv); a constructor
registers each blank with port/src/asset_load.c (PORT_AssetAdd), which copies the row's bytes from the running game's ROM
at start, as for Black's tables. C objects: const dropped (the fill writes them); a table inside a function moves above
it under a name of its own (a function's automatic one is copied from it where it was). Their original bytes come from a
compile of the file (the HG build's compile command, tools/pokehg_cc.py's tree); an object holding pointers is left as it
is (reported). tools/hg_vendor.py runs it on each vendoring; run here, it applies to the vendored tree in place and
adds to its rows.

    python3 tools/hg_assets.py [--dry-run]     # needs orig/baserom_hg_us.nds and _ss_us.nds and the HG port build
"""
import argparse, ast, json, os, re, shlex, struct, subprocess, sys, tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import rom_modules  # noqa: E402
import pokegen4  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TREE = 'third_party/pokeheartgold'
GAMES = (('IPKE', 'hg', 'orig/baserom_hg_us.nds'), ('IPGE', 'ss', 'orig/baserom_ss_us.nds'))
ROWS = 'port/assets_%s.tsv'
ASM_REG = 'port/asmdata/hg_rom_assets.c'   # (tools/pokehg_cc.py --asmdata: port/meson.build compiles it)
HEAD = 'name\tmodule\tstart\tend\tkind'
DECL = 'void PORT_AssetAdd(const char *game, const char *name, void *p, unsigned long size, const char *layout);'

# ---------------------------------------------------------------- leaks: which characters of a text are ROM bytes


def leak_spans(data):
    """[(first char, last char)] of each run of the text's numbers that rom_leak_check.py finds in a ROM (its index must
    be built: rom_leak_check.build_index)."""
    import rom_leak_check as L
    nums = L.numbers(data)
    out = []
    for kind, d in L.streams(data):
        w = {'u8': 1, 'u16': 2, 'u32': 4}.get(kind, 0)
        if not w:
            continue
        mask = (1 << 8 * w) - 1
        at = [p for p, v in nums if -(mask >> 1) - 1 <= v <= mask]
        for st, ln, _, _ in L.scan_stream(d, w):
            out.append((at[st // w], at[(st + ln - 1) // w]))
    return out + static_spans(data)


def static_spans(data):
    """[(first char, last char)] of each run of numbers rom_leak_check.py --static calls a data table (one the full check
    may pass as trivial, a ramp of arc indices: still the ROM's table, so blanked too)."""
    import rom_leak_check as L
    out, run = [], []
    for m in L.NUM_RE.finditer(data):
        if not (run and L.GAP_RE.fullmatch(data, run[-1].end(), m.start())):
            run = []
        run.append(m)
        if len(run) == L.STATIC_MAX_NUMS:
            out.append([run[0].start(), m.start()])
        if len(run) >= L.STATIC_MAX_NUMS:
            out[-1][1] = m.start()
    return [tuple(s) for s in out]


# ---------------------------------------------------------------- asm: labels and their data

LABEL = re.compile(r'^([A-Za-z_.$][\w.$]*):\s*$')
DATA = {'.byte': 1, '.short': 2, '.hword': 2, '.2byte': 2, '.word': 4, '.4byte': 4, '.long': 4}
SYMBOL = re.compile(r'[A-Za-z_.$][\w.$]*')
ITEM = re.compile(r'[^,]+')


def asm_value(item):
    """An item's value, or None: it names a symbol (a pointer in a .word) or is no plain expression."""
    s = item.strip()
    if SYMBOL.search(re.sub(r'0[xX][0-9a-fA-F]+', '', s)) or not re.fullmatch(r'[-+*/()<>|&^~ 0-9a-fA-FxX]+', s):
        return None
    try:
        return int(eval(s, {'__builtins__': {}}))
    except Exception:
        return None


def asm_objects(text):
    """Each label's data: {name, span (chars), items [(size, value or None, (start, end) of a number)], ok}."""
    out, cur, pos = [], None, 0
    for line in text.splitlines(True):
        code = line.split(';', 1)[0].split('//', 1)[0]
        m = LABEL.match(code.strip())
        w = code.split()
        if m:
            cur = {'name': m[1], 'span': [pos + len(line), pos + len(line)], 'items': [], 'ok': True}
            out.append(cur)
        elif cur is not None and w and w[0] in DATA:
            size = DATA[w[0]]
            base = pos + code.index(w[0]) + len(w[0])
            for it in ITEM.finditer(code[code.index(w[0]) + len(w[0]):]):
                v = asm_value(it[0])
                st = base + it.start() + len(it[0]) - len(it[0].lstrip())
                cur['items'].append((size, v, (st, base + it.end() - (len(it[0]) - len(it[0].rstrip())))))
            cur['span'][1] = pos + len(line)
        elif cur is not None and w and w[0] == '.space':
            n = asm_value(code.split(None, 1)[1].split(',')[0])
            if n is None or ',' in code:
                cur['ok'] = False
            else:
                cur['items'] += [(1, 0, None)] * n
            cur['span'][1] = pos + len(line)
        elif cur is not None and w and w[0] in ('.ascii', '.asciz', '.string'):
            try:   # (its bytes known, not blanked: text is rom_text_check.py's)
                b = ast.literal_eval('b' + code.split(None, 1)[1].strip())
            except (ValueError, SyntaxError, IndexError):
                b, cur['ok'] = b'', False
            cur['items'] += [(1, c, None) for c in b + (b'' if w[0] == '.ascii' else b'\0')]
            cur['span'][1] = pos + len(line)
        elif w[:2] and w[0] in ('.global', '.set') and w[1].startswith('hgasset_'):
            pass   # (the alias apply() gives a label, after it: its data goes on)
        elif w:
            cur = None
        pos += len(line)
    return out


def asm_layout(items):
    """(DS bytes, DS mask (1: a known byte), segments [(DS offset, host offset, length)]) of a label's items: the runs of
    numbers between its pointers. The host's: tools/pokehg_asm.py makes a .word that names a symbol a .quad, 8-aligned
    (natural()), from a label start 8-aligned too; the rest as on the DS."""
    data, mask, segs, host = bytearray(), bytearray(), [], 0
    for size, v, _ in items:
        if size == 4 and v is None:   # a pointer
            data += bytes(4)
            mask += bytes(4)
            host = (host + 7) // 8 * 8 + 8
            continue
        if segs and segs[-1][0] + segs[-1][2] == len(data) and segs[-1][1] + segs[-1][2] == host:
            segs[-1][2] += size
        else:
            segs.append([len(data), host, size])
        # (a name in a .byte or .short: a constant, the ROM's value copied as it is)
        data += bytes(size) if v is None else (v & (1 << 8 * size) - 1).to_bytes(size, 'little')
        mask += (b'\0' if v is None else b'\1') * size
        host += size
    return bytes(data), bytes(mask), [tuple(g) for g in segs]


def alias(path, label):
    """The global name tools/hg_assets.py gives an asm label (`.set`): unique, as the label's may not be."""
    return 'hgasset_%s_%s' % (re.sub(r'\W', '_', path), label.replace('.', '_'))


def blank_spans(text, spans):
    """text with each (start, end) replaced by 0."""
    for a, b in sorted(spans, reverse=True):
        text = text[:a] + '0' + text[b:]
    return text


# ---------------------------------------------------------------- C: initialisers, moved and blanked

C_NUM = re.compile(r'(?<![\w.])(0[xX][0-9a-fA-F]+[uUlL]*|\d+\.\d*(?:[eE][-+]?\d+)?[fF]?|\d+[uUlL]*)(?![\w.])')
INIT = re.compile(r'([A-Za-z_]\w*)\s*((?:\[[^\]=;{}]*\]\s*)*)(?:ATTRIBUTE_ALIGN\(\d+\)\s*)?=\s*\{')


def code_mask(text):
    """text with comments, strings, character constants and preprocessor lines as spaces (same length)."""
    out, i, n, bol = list(text), 0, len(text), True
    while i < n:
        c = text[i]
        if bol and c == '#':
            j = i
            while j < n and (text[j] != '\n' or text[j - 1] == '\\'):
                j += 1
        elif text.startswith('//', i):
            j = text.find('\n', i)
            j = n if j < 0 else j
        elif text.startswith('/*', i):
            j = text.find('*/', i + 2)
            j = n if j < 0 else j + 2
        elif c in '"\'':
            j = i + 1
            while j < n and text[j] != c:
                j += 2 if text[j] == '\\' else 1
            j += 1
        else:
            bol = c == '\n' or (bol and c in ' \t')
            i += 1
            continue
        for k in range(i, min(j, n)):
            if out[k] != '\n':
                out[k] = ' '
        i = j
    return ''.join(out)


def c_objects(text):
    """The brace initialisers: {name, decl (start of the declaration), init (start, end of the braces), func (the start of
    the enclosing function's declaration, or None), static, dims}."""
    m_text = code_mask(text)
    depth, d = [], 0
    for ch in m_text:
        depth.append(d)
        d += (ch == '{') - (ch == '}')
    out, i = [], 0
    while True:
        m = INIT.search(m_text, i)
        if not m:
            return out
        k, j = 1, m.end()
        while k and j < len(m_text):
            k += (m_text[j] == '{') - (m_text[j] == '}')
            j += 1
        lvl = depth[m.start()]
        s = m.start() - 1   # the statement before: a ';' at this depth, a '}' back to it or the '{' that opened it
        while s >= 0 and not (m_text[s] in ';{}' and depth[s] + (m_text[s] == '{') - (m_text[s] == '}') == lvl):
            s -= 1
        decl = _token(m_text, s + 1)
        func = None
        if lvl:
            f = m.start()
            while f > 0 and depth[f] > 0:
                f -= 1
            f -= 1
            while f >= 0 and not (m_text[f] == ';' and depth[f] == 0 or m_text[f] == '}' and depth[f] == 1):
                f -= 1
            func = _token(m_text, f + 1)
        prefix = m_text[decl:m.start()]
        out.append({'name': m[1], 'decl': decl, 'brace': m.end() - 1, 'end': j, 'func': func, 'dims': m[2].rstrip(),
                    'prefix': text[decl:m.start()], 'static': bool(re.search(r'\bstatic\b', prefix)),
                    'odd': bool(re.search(r'[,=()]', re.sub(r'ATTRIBUTE_ALIGN\(\d+\)', '', prefix))) or not m_text[j:].lstrip().startswith(';')})
        i = j


def c_blank(init):
    """An initialiser's text with its numbers 0, but the indices of designators ([n] = ...)."""
    keep, d = [], 0
    for k, ch in enumerate(init):
        d += (ch == '[') - (ch == ']')
        keep.append(d > 0 or ch == ']')
    mask, out, last = code_mask(init), [], 0
    for m in C_NUM.finditer(mask):
        if keep[m.start()]:
            continue
        out += [init[last:m.start()], '0']
        last = m.end()
    return ''.join(out) + init[last:]


def c_register(path, names, text='', lays={}):
    """The constructor that registers a file's blanks (a header's: in each file that includes it, each its copy)."""
    reg = ['', '// tools/hg_assets.py: the tables above are blanks, port/src/asset_load.c fills them from the ROM at start',
           DECL, '__attribute__((constructor)) static void hgasset_register_%s_%d(void) {' % (
               re.sub(r'\W', '_', path), text.count('hgasset_register_'))]
    for n in names:
        for code, _, _ in GAMES:
            reg.append('    PORT_AssetAdd("%s", "%s:%s", (void *)&%s, sizeof(%s), "%s");' % (code, path, n, n, n,
                                                                                         lays.get(n, 'u8')))
    return '\n'.join(reg) + '\n}\n'


def unconst_decls(text, name):
    """text with `const` dropped from the declarations of name that have no initialiser (`extern const T name[];`)."""
    return re.sub(r'(?m)^[^\n=;{}()]*\b%s\s*(?:\[[^\]\n]*\]\s*)*;' % re.escape(name),
                  lambda m: re.sub(r'\bconst\b\s*', '', m[0]), text)


# The C types of the tables that hold pointers: their fields (port/src/asset_load.c's Layout: u8 u16 u32 ptr, `*n`),
# whose DS layout and host layout its Fill derives (natural alignment; a pointer 4 bytes on the DS, the host's here)
TYPE_LAYOUT = {
    'UnkStruct_020F3DCC': 'u32*8,ptr',                      # port/asmdata/unk_02087FD4.c
    'WeatherSystem_Sub0': 'u16*2,u32,ptr*2,u16*2,ptr*2',    # src/field/weather_manager.c
}


def fields(layout):
    """[(DS offset, host offset, size or 0 for a pointer)], DS record size, host record size: as Fill."""
    out, ds, host, da, ha = [], 0, 0, 1, 1
    for item in layout.split(','):
        kind, _, n = item.partition('*')
        for _ in range(int(n or 1)):
            d, h = (4, 8) if kind == 'ptr' else (int(kind[1:]) // 8,) * 2
            ds, host = (ds + d - 1) // d * d, (host + h - 1) // h * h
            out.append((ds, host, 0 if kind == 'ptr' else d))
            ds, host, da, ha = ds + d, host + h, max(da, d), max(ha, h)
    return out, (ds + da - 1) // da * da, (host + ha - 1) // ha * ha


def ds_from_host(host, layout):
    """(DS bytes, mask) of a table of records of layout from its host bytes (a pointer: unknown bytes)."""
    fs, dsz, hsz = fields(layout)
    data, mask = bytearray(dsz * (len(host) // hsz)), bytearray(dsz * (len(host) // hsz))
    for r in range(len(host) // hsz):
        for d, h, n in fs:
            if n:
                data[r * dsz + d:r * dsz + d + n] = host[r * hsz + h:r * hsz + h + n]
                mask[r * dsz + d:r * dsz + d + n] = b'\1' * n
    return bytes(data), bytes(mask)


def unconst(prefix):
    return re.sub(r'\b(const|static)\b\s*', '', prefix)


# a blank is zeros: in .data all the same, as port_assets.py's placeholders (in .bss, an overlay load zeroes the filled table:
# port/src/ovl.c's PortOvl_Keep puts it back into the .data snapshot only). The section's name is port_hgss.h's (".data", Apple's
# "__DATA,__data": Mach-O names the segment too)
DATA_ATTR = ' __attribute__((section(BWPORT_DATA_SECTION)))'


def c_rewrite(text, objs):
    """(text with each obj's table at file scope, [file-scope symbol of each obj]). A function's table moves above the
    function as `static <type> hgasset_<function>_<name>`; an automatic one is copied from it where it was declared."""
    sym = [o['name'] for o in objs]
    funcs = sorted({o['func'] for o in objs if o['func'] is not None}, reverse=True)
    for f in funcs:   # (from the end: each function's edits are after its start, the tables at it)
        mine = sorted([k for k, o in enumerate(objs) if o['func'] == f], key=lambda k: objs[k]['decl'], reverse=True)
        fm = re.search(r'(\w+)\s*\([^()]*\)\s*$', code_mask(text[f:code_mask(text).index('{', f)]))
        tables = []
        for k in mine:
            o = objs[k]
            new = sym[k] = 'hgasset_%s_%s' % (fm[1] if fm else 'f', o['name'])
            tables.insert(0, 'static %s%s%s = %s;\n\n' % (unconst(o['prefix']), new, o['dims'], text[o['brace']:o['end']]))
            semi = text.index(';', o['end']) + 1
            if o['static']:
                end = _body_end(text, f)
                text = text[:semi] + re.sub(r'\b%s\b' % o['name'], new, text[semi:end]) + text[end:]
                local = ''
            else:
                local = '__typeof__(%s) %s; __builtin_memcpy((void *)&%s, &%s, sizeof(%s));' % (
                    new, o['name'], o['name'], new, o['name'])
            text = text[:o['decl']] + local + text[semi:]
        text = text[:f] + ''.join(tables) + text[f:]
    return text, sym


def _token(m_text, i):
    """The first character at or after i that is code (not space, a comment or a preprocessor line)."""
    while i < len(m_text) and m_text[i].isspace():
        i += 1
    return i


def _body_end(text, func):
    m = code_mask(text)
    i, d = m.index('{', func), 0
    while True:
        d += (m[i] == '{') - (m[i] == '}')
        i += 1
        if d == 0:
            return i


# ---------------------------------------------------------------- C: the original bytes from a compile


def patched(path, text):
    """text (the vendored file's) with the port's patches to it applied (port/subprojects/pokeheartgold-*.patch, in name
    order, as tools/pokehg_gen.py does): the C the build compiles."""
    import glob
    with tempfile.TemporaryDirectory() as d:
        f = os.path.join(d, path)
        os.makedirs(os.path.dirname(f), exist_ok=True)
        open(f, 'w', encoding='latin-1').write(text)
        for p in sorted(glob.glob(os.path.join(ROOT, 'port/subprojects/pokeheartgold-*.patch'))):
            subprocess.run(['git', 'apply', '--include=' + path, '--whitespace=nowarn', p], cwd=d, check=True,
                           capture_output=True)
        return open(f, encoding='latin-1').read()


def compile_template(build):
    """A HeartGold C file's compile command (argument list, directory) from the build's compile_commands.json."""
    cc = json.load(open(os.path.join(build, 'compile_commands.json')))
    for e in cc:
        if '/pokeheartgold/src/' in e['file']:
            return e.get('arguments') or shlex.split(e['command']), e['directory'], e['file']
    sys.exit('hg_assets: no HeartGold file in %s/compile_commands.json (build the HG port first)' % build)


def compile_objects(text, path, tmpl, hg_root):
    """{symbol: (bytes, holds a pointer)} of a C text compiled as path (in hg_root, for its includes)."""
    args, cwd, orig = tmpl
    src_dir = os.path.join(hg_root, os.path.dirname(path))
    fd, src = tempfile.mkstemp(suffix='.c', prefix='.hgasset_', dir=src_dir)
    os.write(fd, text.encode())
    os.close(fd)
    obj = src[:-2] + '.o'
    old_root = orig[:orig.index('/pokeheartgold/') + len('/pokeheartgold')]
    cmd, skip = [], False
    for a in args:
        if skip:
            skip = False
        elif a in ('-o', '-MF', '-MQ', '-c'):
            skip = True
        elif a in ('ccache',) or a.startswith('-MD') or a == orig:
            pass
        else:
            cmd.append(a.replace(old_root, hg_root))
    try:
        r = subprocess.run(cmd + ['-c', src, '-o', obj], cwd=cwd, capture_output=True, text=True)
        if r.returncode:
            raise RuntimeError('%s: compile failed: %s' % (path, [l for l in r.stderr.splitlines() if 'error:' in l][:2]))
        return elf_objects(open(obj, 'rb').read())
    finally:
        for f in (src, obj):
            if os.path.exists(f):
                os.remove(f)


def elf_objects(d):
    """{symbol: (bytes, a relocation inside)} of an ELF64 relocatable's data objects."""
    shoff, = struct.unpack_from('<Q', d, 0x28)
    shentsize, shnum = struct.unpack_from('<HH', d, 0x3A)
    secs = [struct.unpack_from('<IIQQQQIIQQ', d, shoff + i * shentsize) for i in range(shnum)]
    relocs = {}
    for s in secs:
        if s[1] in (4, 9):   # SHT_RELA, SHT_REL: info = the section they apply to
            ent = 24 if s[1] == 4 else 16
            relocs.setdefault(s[7], []).extend(struct.unpack_from('<Q', d, s[4] + ent * k)[0] for k in range(s[5] // ent))
    out = {}
    for s in secs:
        if s[1] != 2:   # SHT_SYMTAB
            continue
        strtab = secs[s[6]][4]
        for k in range(s[5] // 24):
            name, info, _, shndx, value, size = struct.unpack_from('<IBBHQQ', d, s[4] + 24 * k)
            if info & 0xF != 1 or not 0 < shndx < shnum:   # STT_OBJECT
                continue
            n = d[strtab + name:d.index(b'\0', strtab + name)].decode()
            sec = secs[shndx]
            data = bytes(size) if sec[1] == 8 else d[sec[4] + value:sec[4] + value + size]
            out[n] = (data, any(value <= o < value + size for o in relocs.get(shndx, ())))
    return out


# ---------------------------------------------------------------- the ROMs


class Rom:
    def __init__(self, path):
        rom = open(path, 'rb').read()
        self.mods = {-1: (struct.unpack_from('<I', rom, 0x28)[0], rom_modules.arm9(rom))}
        ovt, size = struct.unpack_from('<II', rom, 0x50)
        ram = {struct.unpack_from('<I', rom, e)[0]: struct.unpack_from('<I', rom, e + 4)[0] for e in range(ovt, ovt + size, 32)}
        for i, img in rom_modules.overlays(rom):
            self.mods[i] = (ram[i], img)

    def match(self, mod, addr, data, mask):
        base, img = self.mods.get(mod, (0, b''))
        o = addr - base
        if o < 0 or o + len(data) > len(img):
            return False
        return all(m == 0 or img[o + k] == b for k, (b, m) in enumerate(zip(data, mask)))

    def find(self, data, mask, hint=None):
        """[(module, address)] where data matches (mask 0: any byte), the hint module's alone when it has one."""
        chunks = [m for m in re.finditer(rb'\x01+', mask)]
        if not chunks:
            return []
        c = max(chunks, key=lambda m: m.end() - m.start())
        key = data[c.start():c.end()]
        hits = []
        for mod, (base, img) in self.mods.items():
            i = img.find(key)
            while i >= 0:
                a = base + i - c.start()
                if i >= c.start() and self.match(mod, a, data, mask):
                    hits.append((mod, a))
                i = img.find(key, i + 1)
        # (the same bytes in several places: any of them fills the table alike; the hint module's first)
        return sorted(hits, key=lambda h: (h[0] != hint, h))


def name_address(name):
    m = re.search(r'(?:ov(\d+)_)?([0-9A-Fa-f]{8})$', name)
    return (int(m[1]) if m[1] else None, int(m[2], 16)) if m else (None, None)


def path_module(path):
    m = re.search(r'overlay_(\d+)|ov(\d+)_', os.path.basename(path))
    return int(m[1] or m[2]) if m else None


def locate(roms, name, path, data, mask):
    """{game: (module, start)} of an object's bytes, or a reason string."""
    hint = path_module(path)
    nmod, naddr = name_address(name)
    hint = nmod if nmod is not None else hint
    hg = roms['hg']
    loc = None
    if naddr is not None:
        for mod in ([hint] if hint is not None else []) + [-1]:
            if hg.match(mod, naddr, data, mask):
                loc = (mod, naddr)
                break
    if loc is None:
        hits = hg.find(data, mask, hint)
        if not hits:
            return 'not in the HeartGold ROM'
        loc = hits[0]
    out = {'hg': loc}
    ss = roms['ss']
    if ss.match(*loc, data, mask):
        out['ss'] = loc
    else:
        hits = ss.find(data, mask, loc[0])
        if not hits:
            return 'HeartGold %s, not in the SoulSilver ROM' % fmt_loc(loc)
        out['ss'] = hits[0]
    return out


def fmt_loc(loc):
    return '%s %08x' % ('main' if loc[0] == -1 else 'ov%d' % loc[0], loc[1])


# ---------------------------------------------------------------- the whole tree


def apply(files, hg_root, build, roms):
    """Blank files' ROM bytes ({path: bytes}, changed in place); ({game: [row]}, [problems])."""
    import rom_leak_check as L
    L.build_index([r for r in L.DEFAULT_ROMS if os.path.exists(r[1]) and (r[2] is None or os.path.isdir(r[2]))],
                  os.environ.get('ROMLEAK_CACHE', os.path.join(L._main_checkout(), 'orig', 'leak_cache')))
    import pokehg_asm
    c_data = pokehg_asm.c_data(hg_root)
    rows, asm_reg, tmpl = {'hg': [], 'ss': []}, [], None
    del LEFT[:]
    # (passes: a run can span tables, its numbers in several; with some of them blanks the rest may still be one)
    todo, passes = sorted(files), 0
    while todo and passes < 4:
        todo, passes = pass_files(files, todo, hg_root, build, roms, c_data, rows, asm_reg, tmpl), passes + 1
    problems = []
    for path in todo:
        problems.append('%s: still a run after %d passes' % (path, passes))
    problems += sorted(set(LEFT))
    if asm_reg:
        reg = ['// tools/hg_assets.py: the asm data labels whose numbers are blanks; port/src/asset_load.c fills them from',
               "// the running game's ROM at start (the rows: port/assets_hg.tsv, port/assets_ss.tsv)", DECL, '']
        reg += ['extern char %s[];' % n for n in sorted({r[0] for r in asm_reg})]
        reg += ['', '__attribute__((constructor)) static void hgasset_register_asm_(void) {']
        for label, name, host, n in asm_reg:
            for code, _, _ in GAMES:
                reg.append('    PORT_AssetAdd("%s", "%s", (void *)(%s + %d), %d, "u8");' % (code, name, label, host, n))
        files[ASM_REG] = ('\n'.join(reg) + '\n}\n').encode()
    return rows, problems


LEFT = []


def pass_files(files, paths, hg_root, build, roms, c_data, rows, asm_reg, tmpl):
    """One pass over paths: blanks and rows for each run found; the paths changed (to scan again)."""
    changed, problems = [], LEFT
    for path in paths:
        if not path.endswith(('.s', '.c', '.h', '.inc')):
            continue
        spans = leak_spans(files[path])
        if not spans:
            continue
        text = files[path].decode('latin-1')
        hit = lambda a, b: any(x <= b and a <= y for x, y in spans)
        if path.endswith(('.s', '.inc')):
            blanks, labels = [], set()
            for o in asm_objects(text):
                if not hit(*o['span']) or all(v in (None, 0) for _, v, _ in o['items']):
                    continue   # (a label of zeros or names: nothing of the ROM's)
                if o['name'] in c_data:   # (renamed NAME__ds by tools/pokehg_asm.py: port/asmdata's C is the one read)
                    blanks += [sp for _, v, sp in o['items'] if v not in (None, 0) and sp]
                    continue
                if not o['ok']:
                    problems.append('%s: %s: not plain data' % (path, o['name']))
                    continue
                data, mask, segs = asm_layout(o['items'])
                where = locate(roms, o['name'], path, data, mask)
                if not isinstance(where, dict):
                    problems.append('%s: %s: %s' % (path, o['name'], where))
                    continue
                for ds, host, n in segs:
                    if not any(data[ds:ds + n]):
                        continue
                    # (a label's name can repeat in other files, as a local one: the row is the file's)
                    name = '%s:%s' % (path, o['name']) + ('' if (ds, n) == (0, len(data)) else '+%d' % ds)
                    for g in ('hg', 'ss'):
                        rows[g].append((name, (where[g][0], where[g][1] + ds), n, 'u8'))
                    asm_reg.append((alias(path, o['name']), name, host, n))
                    labels.add(o['name'])
                blanks += [sp for _, v, sp in o['items'] if v not in (None, 0) and sp]
            if blanks:
                text = blank_spans(text, blanks)
                if labels:   # (the fill writes them: .data, as tools/pokehg_asm.py's .rodata is read-only once relocated)
                    text = re.sub(r'(?m)^([ \t]*)\.(rodata|text)[ \t]*$', r'\1.data', text)
                for name in sorted(labels):   # a global alias of the label, unique (the registration's extern)
                    a = alias(path, name)
                    if a not in text:
                        text = re.sub(r'(?m)^%s:' % re.escape(name), '%s:\n\t.global %s\n\t.set %s, %s' % (name, a, a, name),
                                      text, 1)
        else:
            objs = [o for o in c_objects(text) if hit(o['brace'], o['end']) and
                    re.search(r'(?<![\w.])[1-9]|0[xX]0*[1-9a-fA-F]', code_mask(text[o['brace']:o['end']]))]
            for o in [o for o in objs if o['odd']]:
                problems.append('%s: %s: not a plain declaration' % (path, o['name']))
            objs = [o for o in objs if not o['odd']]
            if not objs:
                continue
            moved, sym = c_rewrite(text, objs)
            if tmpl is None:
                tmpl = compile_template(build)
            via_patch = False
            try:
                compiled = compile_objects(moved + c_register(path, sym, moved), path, tmpl, hg_root)
            except RuntimeError as e:
                # (a file the port's patches change may not build against their headers unpatched: the build's
                # patched copy then, the same tables)
                try:
                    ptext = patched(path, text)
                    pobjs = [o for o in c_objects(ptext) if o['name'] in {x['name'] for x in objs}]
                    pmoved, psym = c_rewrite(ptext, pobjs)
                    compiled = compile_objects(pmoved + c_register(path, psym, pmoved), path, tmpl, hg_root)
                    via_patch = True
                except (RuntimeError, OSError, subprocess.CalledProcessError):
                    problems.append(str(e))
                    continue
            done = []
            lays = {}
            for o, s in zip(objs, sym):
                ctype = (re.findall(r'\w+', unconst(o['prefix'])) or [''])[-1]
                if s not in compiled or compiled[s][1] and ctype not in TYPE_LAYOUT:
                    problems.append('%s: %s: %s' % (path, s, 'holds pointers' if s in compiled else 'not compiled'))
                    continue
                data, lay = compiled[s][0], TYPE_LAYOUT.get(ctype, 'u8')
                if not any(data):
                    continue   # (zeros: nothing of the ROM's)
                data, mask = ds_from_host(data, lay) if lay != 'u8' else (data, b'\1' * len(data))
                where = locate(roms, s, path, data, mask)
                if not isinstance(where, dict):
                    problems.append('%s: %s: %s' % (path, s, where))
                    continue
                for g in ('hg', 'ss'):
                    rows[g].append(('%s:%s' % (path, s), where[g], len(data), lay))
                done.append(s)
                lays[s] = lay
            if not done:
                continue
            objs2 = c_objects(moved)
            names = set(done)
            edits = []
            for o in objs2:
                if o['name'] in names and o['func'] is None:
                    pre = re.sub(r'\bconst\b\s*', '', o['prefix'])
                    edits.append((o['decl'], o['end'], pre + o['name'] + o['dims'] + DATA_ATTR + ' = ' +
                                   c_blank(moved[o['brace']:o['end']])))
            for a, b, s in sorted(edits, reverse=True):
                moved = moved[:a] + s + moved[b:]
            text = moved.rstrip('\n') + '\n' + c_register(path, sorted(names), moved, lays)
            for n in names:   # (the table's other declarations lose their const too: in the file and the headers)
                text = unconst_decls(text, n)
                for h in [h for h in files if h.endswith('.h') and h != path]:
                    if n.encode() in files[h]:
                        files[h] = unconst_decls(files[h].decode('latin-1'), n).encode('latin-1')
            try:   # (the blanked file must build: a header's extern of a table that lost its const would not)
                if not via_patch:
                    compile_objects(text, path, tmpl, hg_root)
            except RuntimeError as e:
                problems.append('blanked ' + str(e))
                for g in rows:
                    rows[g] = [r for r in rows[g] if not r[0].startswith(path + ':')]
                continue
        if text.encode('latin-1') != files[path]:
            changed.append(path)
        files[path] = text.encode('latin-1')
    return changed


def row_lines(rows):
    out = []
    for name, (mod, start), size, lay in rows:
        out.append('%s\t%s\t%08x\t%08x\t%s' % (name, 'main' if mod == -1 else 'ov%03d' % mod, start, start + size, lay))
    return out


def write_rows(files, rows, keep=()):
    """port/assets_{hg,ss}.tsv into files (keep: the rows already there, by name, kept unless found again)."""
    for g, rs in rows.items():
        p = ROWS % g
        old = [l for l in files.get(p, b'').decode().splitlines()[1:] if l.strip()] if keep else []
        new = row_lines(rs)
        names = {l.split('\t')[0] for l in new}
        files[p] = ('\n'.join([HEAD] + [l for l in old if l.split('\t')[0] not in names] + new) + '\n').encode()


def load_roms():
    roms = {}
    for _, g, p in GAMES:
        if not os.path.exists(os.path.join(ROOT, p)):
            sys.exit('hg_assets: %s is needed' % p)
        roms[g] = Rom(os.path.join(ROOT, p))
    return roms


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    ap.add_argument('--dry-run', action='store_true', help='report, change nothing')
    ap.add_argument('--build', default=os.path.join(ROOT, pokegen4.hgss_build()))
    o = ap.parse_args()
    # (the tables compile against build/pokeheartgold: made from this tree, not the one before an edit or a checkout, whose
    # headers can hold the blanks' declarations already)
    subprocess.run([sys.executable, os.path.join(ROOT, 'tools/pokehg_gen.py')], check=True, stdout=subprocess.DEVNULL)
    tree = os.path.join(ROOT, TREE)
    files = {}
    for dp, _, fs in os.walk(tree):
        for f in fs:
            p = os.path.relpath(os.path.join(dp, f), tree)
            files[p] = open(os.path.join(dp, f), 'rb').read()
    before = dict(files)
    rows, problems = apply(files, os.path.join(ROOT, 'build/pokeheartgold'), o.build, load_roms())
    if ASM_REG in before and ASM_REG in files:   # a second run: the labels registered before stay
        files[ASM_REG] = merge_reg(before[ASM_REG], files[ASM_REG])
    write_rows(files, rows, keep=True)
    changed = [p for p in files if files[p] != before.get(p)]
    for p in problems:
        print('hg_assets: left: ' + p)
    print('hg_assets: %d rows, %d files changed, %d left' % (len(rows['hg']), len(changed), len(problems)))
    if not o.dry_run:
        for p in changed:
            os.makedirs(os.path.dirname(os.path.join(tree, p)), exist_ok=True)
            open(os.path.join(tree, p), 'wb').write(files[p])


def merge_reg(old, new):
    """The asm registration file of two runs: both runs' externs and calls."""
    o, n = old.decode().splitlines(), new.decode().splitlines()
    ext = sorted({l for l in o + n if l.startswith('extern ')})
    calls = []
    for l in o + n:
        if l.startswith('    PORT_AssetAdd') and l not in calls:
            calls.append(l)
    head = [l for l in n if l.startswith('//') or l == DECL]
    return ('\n'.join(head + [''] + ext + ['', '__attribute__((constructor)) static void hgasset_register_asm_(void) {'] +
                      calls) + '\n}\n').encode()


if __name__ == '__main__':
    main()
