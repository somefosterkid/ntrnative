#!/usr/bin/env python3
"""tools/rom_text_check.py: finds the game's text in tracked files (comments, strings, names), so dialogue copied out of
the ROM (pret's msgenc writes it into its generated headers as comments) can't land in the repo again.

rom_leak_check.py looks for ROM bytes; the ROM stores text encoded, so it can't see text. This tool decodes the text
itself, for two formats:
  - Gen 4 (Platinum): every NARC under msgdata/: count, key, an alloc table XOR-ed with 765 * i * key, each message's
    u16 codes XOR-ed with a key starting at i * 596947 and stepping by 18749, i from 1 (pret's tools/msgenc), mapped
    through the Gen 4 character map's Latin part (CHARMAP below: a character map, not game content). A control code
    (0xFFFE: a variable such as a name) or a trainer-name string (0xF100) ends a run.
  - Gen 5 (Black, White, Black 2, White 2): the message NARCs a/0/0/2 (system text) and a/0/0/3 (story text). A member
    is a message file: u16 sections, u16 entries, u32 size, u32 0, u32 section offsets; a section: u32 size, then per
    entry u32 offset (from the section), u16 length (u16 characters), u16 unused. Each string is decrypted backwards:
    key = last ^ 0xFFFF, c[i] = enc[i] ^ key, key = rotl16(key, 13) (the last character is the 0xFFFF end mark). The
    characters are UTF-16 (char5 below: Latin letters, digits and apostrophes make words, anything else separates);
    0xF000 starts a control code (code, parameter count, parameters: a variable) and ends a run; a string that starts
    with 0xF100 (9-bit compressed, ~1% of them, a few characters each: names) is not decoded.
Each message becomes lower-case words (letters and digits, apostrophes dropped, accents folded). Every run of RUN (4)
consecutive words of a message is indexed by hash; a file is the same kind of word stream (CamelCase and `_` split names into
words, any other character separates), and a finding is a maximal stretch of the file whose every 4 words are
indexed. It prints file:line and the number of words, never the words.

What counts (a stretch of 4 or more words every 4 of which are in the ROM's index): English shares 4-word phrases
with any game's text, so a bare match is weak evidence; copied text is long, or is the whole comment. The policy:
  - a window of 4 words that are all common words (COMMON: function words and the commonest verbs) is no match;
  - a stretch of LONG (6) or more words always counts;
  - a shorter stretch (4-5 words) counts only if it is not inside ONE name (CamelCase / snake_case: names made of
    pret's vocabulary share 4 words with the game's text all the time) and it covers at least COVER (60%) of the
    prose words of its lines (code before a // or /* and the parts of names are not prose): a comment that is nothing
    but the game's text, not 4 words of a longer sentence somebody wrote.
It prints file:line, the number of words and an id (a hash of the stretch's words), never the words.

Allowed (tools/rom_text_allow.txt, not scanned; every line needs ` # reason`): `@ID # reason` allows the finding
with that id (the id the tool printed: nobody has to see the text to allow it), `some generic phrase # reason`
(normalised words) allows every 4-word window of that phrase. Only for generic English or code wording that also
happens to be in the game.

    python3 tools/rom_text_check.py PATH...            # files or directories (git-tracked files under them)
    git log -p main..HEAD | python3 tools/rom_text_check.py --stdin    # a stream (history), reported by line
    python3 tools/rom_text_check.py --rom FILE PATH... # these ROMs only (repeatable); the default is every ROM of
                                                       # ROMS (orig/baserom_{plat,b,w,b2,w2}_us.nds) that exists

Exit 1 when something is found; 2 when no ROM was found (nothing to compare with).
"""
import argparse
import hashlib
import mmap
import os
import re
import struct
import subprocess
import sys
import unicodedata

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ALLOW = os.path.join(ROOT, 'tools/rom_text_allow.txt')
ROMS = ['orig/baserom_%s_us.nds' % g for g in ('plat', 'b', 'w', 'b2', 'w2')]
RUN = 4
LONG = 6       # a stretch of this many words always counts
COVER = 0.6    # a shorter one must be this share of the prose words of its lines
GEN5_NARCS = ('a/0/0/2', 'a/0/0/3')
COMMON = frozenset("""
a an the and or but nor so if then else than that this these those there here of to in on at by for from with without
into onto over under about as up down out off is are was were be been being am do does did done have has had having
i me my we us our you your he him his she her it its they them their not no yes can could will would shall should may
might must what which who whom whose when where why how all any some each every more most other such only own same
too very just also now one two get got set go going dont cant wont isnt arent wasnt werent doesnt didnt havent hasnt
im ive youre youll theyre thats lets ill
""".split())   # function words and the commonest verbs: a window of only these is English, not text taken from the game


def charmap():
    """Gen 4 code -> character, the part that makes words (pret's tools/msgenc/charmap.txt, Latin letters and digits);
    any other code separates words."""
    m = {0x121 + i: c for i, c in enumerate('0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz')}
    for i in range(0x40):   # 0x15F..0x19E: Latin-1's 0xC0..0xFF in order (À...ÿ)
        m[0x15F + i] = chr(0xC0 + i)
    m[0x19F], m[0x1A0] = 'Œ', 'œ'
    m[0x1B2] = m[0x1B3] = "'"
    return m


CHARMAP = charmap()


def fold(s):
    """lower case, accents folded, apostrophes dropped: the form both the ROM's and a file's words are compared in."""
    s = unicodedata.normalize('NFKD', s.replace('’', "'").replace('‘', "'"))
    return ''.join(c for c in s if not unicodedata.combining(c) and c != "'").lower()


def char5(c):
    """a Gen 5 character (UTF-16 code unit) -> itself if it makes words: Latin letters (with accents), digits and
    apostrophes; anything else (space, punctuation, other scripts) separates words."""
    if 0x30 <= c <= 0x39 or 0x41 <= c <= 0x5A or 0x61 <= c <= 0x7A or c in (0x27, 0x2018, 0x2019) \
            or (0xC0 <= c <= 0x24F and c not in (0xD7, 0xF7)):
        return chr(c)
    return None


# ---------------------------------------------------------------- the ROM's text

def nds_files(rom):
    """path -> (start, end) of every file of an NDS image (FNT + FAT)."""
    fnt_off, fnt_size, fat_off = struct.unpack_from('<III', rom, 0x40)
    out = {}

    def walk(d, prefix):
        sub, fid = struct.unpack_from('<IH', rom, fnt_off + 8 * d)
        p = fnt_off + sub
        while True:
            b = rom[p]
            p += 1
            if b == 0:
                return
            name = bytes(rom[p:p + (b & 0x7F)]).decode('latin-1')
            p += b & 0x7F
            if b & 0x80:
                walk(struct.unpack_from('<H', rom, p)[0] & 0xFFF, prefix + name + '/')
                p += 2
            else:
                out[prefix + name] = struct.unpack_from('<II', rom, fat_off + 8 * fid)
                fid += 1
    walk(0, '')
    return out


def narc_members(d):
    """the member files of a NARC (BTAF, BTNF, GMIF)."""
    if d[:4] != b'NARC':
        return []
    p = struct.unpack_from('<H', d, 12)[0]
    bsz, cnt = struct.unpack_from('<II', d, p + 4)
    ents = [struct.unpack_from('<II', d, p + 12 + 8 * i) for i in range(cnt)]
    p += bsz
    p += struct.unpack_from('<I', d, p + 4)[0]
    base = p + 8
    return [d[base + s:base + e] for s, e in ents]


def messages(bank):
    """each message of a Gen 4 message bank as a list of u16 codes (decrypted)."""
    if len(bank) < 4:
        return
    count, key = struct.unpack_from('<HH', bank, 0)
    for i in range(1, count + 1):
        k = (765 * i * key) & 0xFFFF
        k |= k << 16
        off, n = struct.unpack_from('<II', bank, 4 + 8 * (i - 1))
        off, n = off ^ k, n ^ k
        if off + 2 * n > len(bank):
            continue
        ck, codes = (i * 596947) & 0xFFFF, []
        for c in struct.unpack_from('<%dH' % n, bank, off):
            codes.append(c ^ ck)
            ck = (ck + 18749) & 0xFFFF
        yield codes


def messages5(bank):
    """each message of a Gen 5 message file as a list of UTF-16 code units (decrypted, with the 0xFFFF end mark). A
    string that is empty, compressed (starts 0xF100) or lies outside the file is skipped; a file that is not a message
    file (the NARC has other members) gives nothing."""
    if len(bank) < 12:
        return
    nsec, nent = struct.unpack_from('<HH', bank, 0)
    if not 0 < nsec <= 4 or 12 + 4 * nsec > len(bank):
        return
    for so in struct.unpack_from('<%dI' % nsec, bank, 12):
        if so + 4 + 8 * nent > len(bank):
            return
        for i in range(nent):
            off, n, _ = struct.unpack_from('<IHH', bank, so + 4 + 8 * i)
            if n == 0 or so + off + 2 * n > len(bank):
                continue
            enc = struct.unpack_from('<%dH' % n, bank, so + off)
            key, codes = enc[n - 1] ^ 0xFFFF, [0] * n
            for j in range(n - 1, -1, -1):
                codes[j] = enc[j] ^ key
                key = ((key >> 3) | (key << 13)) & 0xFFFF
            if codes[0] != 0xF100:
                yield codes


def message_words(codes, gen=4):
    """a message's runs of words (lists of folded words); a control code (Gen 4: 0xFFFE, or a trainer-name string,
    0xF100; Gen 5: 0xF000) ends a run."""
    runs, words, cur = [], [], ''
    ctl = (0xFFFE, 0xF100) if gen == 4 else (0xF000,)
    j = 0
    while j < len(codes):
        c = codes[j]
        ch = CHARMAP.get(c) if gen == 4 else char5(c)
        if ch is not None:
            cur += ch
            j += 1
            continue
        if cur:
            words.append(fold(cur))
            cur = ''
        if c == 0xFFFF:
            break
        if c in ctl:   # a variable (code, command, nparams, params...) or a compressed Gen 4 trainer name
            if words:
                runs.append(words)
            words = []
            if c == 0xF100 and gen == 4:
                break
            j += 3 + (codes[j + 2] if j + 2 < len(codes) else 0)
            continue
        j += 1   # space, punctuation, newline, scroll: words go on
    if cur:
        words.append(fold(cur))
    if words:
        runs.append(words)
    return [[w for w in r if w] for r in runs]


def h(words):
    return hashlib.blake2b(' '.join(words).encode(), digest_size=8).digest()


def rom_banks(rom):
    """(gen, [bank]) of an NDS image: the message files of Gen 5 (a/0/0/2, a/0/0/3) when it has them, else the Gen 4
    banks under msgdata/."""
    files = nds_files(rom)
    gen5 = [p for p in GEN5_NARCS if p in files]
    paths = gen5 or [p for p in sorted(files) if p.startswith('msgdata/')]
    return (5 if gen5 else 4), [b for p in paths if rom[files[p][0]:files[p][0] + 4] == b'NARC'
                                for b in narc_members(rom[files[p][0]:files[p][1]])]


def rom_index(rom_path, idx=None):
    """(idx, number of messages, gen): idx gets the hashes of every RUN consecutive words of the ROM's messages."""
    idx = set() if idx is None else idx
    with open(rom_path, 'rb') as f, mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ) as rom:
        gen, banks = rom_banks(rom)
        n = 0
        for bank in banks:
            for codes in (messages(bank) if gen == 4 else messages5(bank)):
                n += 1
                for r in message_words(codes, gen):
                    for i in range(len(r) - RUN + 1):
                        idx.add(h(r[i:i + RUN]))
    return idx, n, gen


# ---------------------------------------------------------------- the files' words

WORD = re.compile(r"[A-Za-z0-9_À-ɏ'’]+")
CAMEL = re.compile(r"[A-Z]+(?![a-z])|[A-Z]?[a-zß-ÿ'’]+|[0-9]+|[À-ɏ]+")


def text_words(text):
    """(folded word, line, token, in_code) of a text: a token is a run of letters/digits/apostrophes/underscores (a word
    of prose or one name); a CamelCase or snake_case name gives its parts, all with the same token number. in_code:
    the token is on a line before a // or /* (code, not comment)."""
    out, tok = [], 0
    for ln, line in enumerate(text.split('\n'), 1):
        marks = [i for i in (line.find('//'), line.find('/*')) if i >= 0]
        c0 = min(marks) if marks else 0
        for m in WORD.finditer(line):
            tok += 1
            for part in CAMEL.findall(m.group(0)):
                w = fold(part)
                if w:
                    out.append((w, ln, tok, m.start() < c0))
    return out


def load_allow(path=ALLOW):
    """the allowed window hashes (bytes) and stretch ids (str)."""
    allow = set()
    if os.path.exists(path):
        for line in open(path, encoding='utf-8'):
            phrase = line.split('#', 1)[0].split()
            if len(phrase) == 1 and phrase[0].startswith('@'):
                allow.add(phrase[0][1:])
                continue
            for i in range(len(phrase) - RUN + 1):
                allow.add(h(phrase[i:i + RUN]))
    return allow


def scan_text(text, idx, allow=frozenset()):
    """[(first line, last line, words, id)] of each stretch of text that counts as game text (see the policy above)
    and is not allowed."""
    ws = text_words(text)
    parts = {}   # token -> number of words: names have several
    prose = {}   # line -> number of prose words (not in a name, not code)
    for w, ln, tok, code in ws:
        parts[tok] = parts.get(tok, 0) + 1
    for w, ln, tok, code in ws:
        if parts[tok] == 1 and not code:
            prose[ln] = prose.get(ln, 0) + 1
    hits = []   # per window of RUN words: its hash if it may be game text, else None
    for i in range(len(ws) - RUN + 1):
        win = ws[i:i + RUN]
        k = h([x[0] for x in win])
        hits.append(k if k in idx and any(x[0] not in COMMON for x in win) else None)
    found, i = [], 0
    while i < len(hits):
        if hits[i] is None:
            i += 1
            continue
        j = i
        while j + 1 < len(hits) and hits[j + 1] is not None:
            j += 1
        seg = ws[i:j + RUN]
        n = len(seg)
        counts = n >= LONG
        if not counts and len({x[2] for x in seg}) > 1:
            total = sum(prose.get(ln, 0) for ln in range(seg[0][1], seg[-1][1] + 1))
            mine = sum(1 for x in seg if parts[x[2]] == 1 and not x[3])
            counts = total > 0 and mine >= COVER * total
        sid = h([x[0] for x in seg]).hex()[:12]
        if counts and sid not in allow and not all(x in allow for x in hits[i:j + 1]):
            found.append((seg[0][1], seg[-1][1], n, sid))
        i = j + 1
    return found


def tracked(paths):
    out = []
    for p in paths:
        if os.path.isfile(p):
            out.append(p)
        else:
            r = subprocess.run(['git', '-C', ROOT, 'ls-files', '-z', '--', os.path.relpath(os.path.abspath(p), ROOT)],
                               capture_output=True, check=True)
            out += [os.path.join(ROOT, x.decode()) for x in r.stdout.split(b'\0') if x]
    return [p for p in out if os.path.abspath(p) != os.path.abspath(ALLOW)]


def find_rom(rom):
    """rom, or in a worktree without orig/ the main checkout's."""
    if not os.path.exists(rom):
        common = subprocess.run(['git', '-C', ROOT, 'rev-parse', '--path-format=absolute', '--git-common-dir'],
                                capture_output=True, text=True).stdout.strip()
        rom = os.path.join(os.path.dirname(common), os.path.relpath(rom, ROOT))
    return rom


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('paths', nargs='*')
    ap.add_argument('--stdin', action='store_true', help='scan standard input (e.g. git log -p)')
    ap.add_argument('--rom', action='append', help='a ROM to take the text from (repeatable; default: all of ROMS)')
    a = ap.parse_args(argv)
    roms = [find_rom(r) for r in a.rom] if a.rom else \
        [r for r in (find_rom(os.path.join(ROOT, r)) for r in ROMS) if os.path.exists(r)]
    if not roms or not all(os.path.exists(r) for r in roms):
        print('rom_text_check: no ROM to take the text from (orig/baserom_*_us.nds)', file=sys.stderr)
        return 2
    idx, nmsg = set(), 0
    for r in roms:
        before = len(idx)
        _, n, gen = rom_index(r, idx)
        nmsg += n
        print('rom_text_check: %s: Gen %d, %d messages, %d new runs' % (os.path.basename(r), gen, n, len(idx) - before),
              file=sys.stderr)
    allow = load_allow()
    found = 0
    if a.stdin:
        for first, last, n, sid in scan_text(sys.stdin.buffer.read().decode('utf-8', 'replace'), idx, allow):
            print('stdin:%d-%d: %d words of game text [%s]' % (first, last, n, sid))
            found += 1
        nfiles = 1
    else:
        files = tracked(a.paths)
        nfiles = len(files)
        for p in files:
            try:
                text = open(p, 'rb').read().decode('utf-8')
            except (UnicodeDecodeError, OSError):
                continue
            for first, last, n, sid in scan_text(text, idx, allow):
                print('%s:%d-%d: %d words of game text [%s]' % (os.path.relpath(p, ROOT), first, last, n, sid))
                found += 1
    print('rom_text_check: %d ROMs, %d messages, %d %d-word runs indexed; %d files, %d findings'
          % (len(roms), nmsg, len(idx), RUN, nfiles, found), file=sys.stderr)
    return 1 if found else 0


if __name__ == '__main__':
    sys.exit(main())
