#!/usr/bin/env python3
"""Give the constants that came from pokebw2 with Black 2's values ("Black 2's, not checked against Black") Black's
values, read from Black's files in extract/b_us. Writes only names and numbers into include/constants/.

- Sound sequences (sound.h): the numbers the symbols of Black's sound archive (wb_sound_data.sdat) give their names.
- Text banks (text_banks.h, script_text_banks.h): the Black message file (a/0/0/2, a/0/0/3) that shares the most
  messages with pokebw2's text of the Black 2 bank of the same name, if no other file comes close (Archive.match).
- Species, moves, abilities, items and types: the line of Black's text of their names that gives the constant's name
  as pokebw2's make_constants.py makes it (Black's names file found as a text bank is).
- Zones (zones.h): the names make_constants.py's zone_names gives Black's zone headers (Black's zones are Black 2's
  first 427), and the flags of the places to fly to (EVENT_FLAG_ARRIVED_*), from Black's RESPAWN_ZONE_INFO.
- Trainer classes and trainers: the names make_constants.py gives them, from Black's text and trainer data.
- Archives (arc.h): the number Black's code passes where it loads each (ARCHIVE_LOADERS, read from build/b_us/asm,
  which `ninja asm` writes); an ID past the last of the 235 paths of arm9's archive table is not Black's.
- A Pokémon's fields (pokemon.h): those past the last case of Black's PML_PkmGetParamCore are not Black's.

    python3 tools/black_constants.py           # rewrites the headers: build and check the hash after
    python3 tools/black_constants.py --check   # lists what it would change and what disagrees, changes nothing

What Black's files tell is read once (read_black) and kept in build/black_constants.json until they, the disassembly,
this file or pokebw2's commit change. tools/port_decls.py reads it too, so the constants it brings have Black's numbers
already when this can tell them.

A constant that gets Black's value moves under BLACK_NOTE, one whose name Black lacks under MISSING_NOTE; the rest
keep the "Black 2's" note. A constant outside those notes whose name Black gives another number is only listed: either
matched code needs Black's name for the number (gimmick_passing_vehicles.c plays Black's SEQ_SE_FLD_140, which is
Black 2's SEQ_SE_FLD_141), or the twin evidence that confirmed it didn't cover it (guru2_main.c's array of sequences
in .rodata holds Black's SEQ_SE_SYS_52). Black's sound names fit Black's code wherever it was checked. Reuses
pokebw2's readers (GPL-3.0, https://github.com/fuddlesworth/pokebw2): the sound archive's symbols and the constant
names (tools/data/make_constants.py), the message files and its text of Black 2 (tools/text/), its constant lists.
"""
import argparse
import hashlib
import json
import os
import re
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
FILES = ROOT / "extract/b_us/files"
CONSTANTS = ROOT / "include/constants"
CACHE = ROOT / "build/black_constants.json"
B2_NOTE = "// Black 2's, not checked against Black"  # tools/port_decls.py's GAME_TYPE_NOTE
BLACK_NOTE = "// Black's, read from the ROM by tools/black_constants.py"
MISSING_NOTE = "// Black 2's: Black has nothing of this name (tools/black_constants.py)"
DEFINE = re.compile(r"#define (\w+) (0x[0-9a-fA-F]+|\d+)$")
# pokebw2's names lists that make_constants.py makes from the text of the names: (list, prefix, header)
NAME_LISTS = [("species", "SPECIES_", "species.h"), ("moves", "MOVE_", "moves.h"),
              ("abilities", "ABILITY_", "abilities.h"), ("items", "ITEM_", "items.h"), ("types", "TYPE_", "types.h")]
# The headers it gives Black's numbers in, and the prefix of their constants
HEADERS = {"sound.h": "SEQ_", "text_banks.h": "TEXT_BANK_", "script_text_banks.h": "SCRIPT_TEXT_",
           **{header: prefix for _, prefix, header in NAME_LISTS}, "zones.h": "ZONE_",
           "flags.h": "EVENT_FLAG_ARRIVED_", "trainer_classes.h": "TRAINER_CLASS_", "trainers.h": "TRAINER_",
           "arc.h": "ARCID_", "pokemon.h": "PKM_PARAM_"}
# Those whose Black's files name everything Black has: a name they lack, Black lacks
WHOLE = ("sound.h", "zones.h", "flags.h", "trainer_classes.h", "trainers.h")
# Black's table of the places to fly to, RESPAWN_ZONE_INFO (struct RespawnZoneInfo, 0x14 bytes a row): overlay,
# address, rows
RESPAWN_TABLE = (10, 0x02170FEC, 17)
TRAINER_DATA = "a/0/9/2"  # Black 2's a/0/9/1
ASM = ROOT / "build/b_us/asm"
# Black's functions that load each archive, from Black's disassembly: the function, the function it passes the
# archive's ID to, which of those calls and the register. Found by the file names and calls of the code that uses them
ARCHIVE_LOADERS = {
    "ARCID_PERSONAL": ("PML_PersonalArcInit", "GFL_ArcSysCreateFileHandle", 1, "r0"),
    "ARCID_GROWTBL": ("PML_GrowTblRead", "GFL_ArcSysRead", 1, "r1"),
    "ARCID_LEARNSETS": ("PML_LearnsetLvUpLoad", "GFL_ArcSysRead", 1, "r1"),
    "ARCID_EVOLUTIONS": ("loadEvolutionFile", "GFL_ArcSysCreateFileHandle", 1, "r0"),
    "ARCID_AREADATA": ("AreaData_Create", "GFL_ArcSysReadRawResource", 1, "r0"),
    "ARCID_TPOKE": ("LoadTPokeData", "GFL_ArcSysReadHeapNewLZGetLen", 1, "r0"),
    "ARCID_MMODEL_TBL": ("LoadTPokeData", "GFL_ArcSysReadHeapNew", 1, "r0"),
    "ARCID_ISS_ZONE": ("ISSZoneSys_LoadArcData", "GFL_ArcSysCreateFileHandle", 1, "r0"),
    "ARCID_ISS_DUNGEON": ("ISSDungeonSys_LoadArcData", "GFL_ArcSysGetDataMax", 1, "r0"),
    "ARCID_ISS_SWITCH": ("func_0202c148", "GFL_ArcSysGetDataMax", 1, "r0"),  # iss_switch_sys.c
    # field_light_status.c: the weather's lights, the zone's, then the area's
    "ARCID_LIGHTS_BATTLE": ("func_0201493c", "func_02014a20", 1, "r0"),
    "ARCID_LIGHTS_ZONE": ("func_0201493c", "func_02014a20", 2, "r0"),
    "ARCID_LIGHTS_FIELD": ("func_0201493c", "func_02014a20", 3, "r0"),
    "ARCID_RAIL_HEADERS": ("FieldNoGridMapper_Load", "GFL_ArcSysGetDataMax", 1, "r0"),
    "ARCID_RAIL_DATA": ("FieldNoGridMapper_LoadByHeader", "GFL_ArcSysRead", 1, "r1"),
    "ARCID_C_GEAR": ("func_ov030_021efecc", "GFL_ArcSysCreateFileHandle", 1, "r0"),  # no_gear.c
    "ARCID_DOWSING": ("func_ov033_021f0760", "GFL_ArcSysCreateFileHandle", 1, "r0"),
    "ARCID_APP_MENU_COMMON": ("func_ov067_021b6dc0", "GFL_ArcSysCreateFileHandle", 1, "r0"),  # yesno_menu.c
    "ARCID_TOWNMAP_GRA": ("func_ov072_021bb820", "GFL_ArcSysCreateFileHandle", 1, "r0"),  # townmap_grh.c
    "ARCID_IRC_COMPAT": ("func_ov105_021b78f4", "GFL_ArcSysCreateFileHandle", 1, "r0"),  # irc_compat_result.c
    "ARCID_MUSICAL_ITEM": ("func_ov112_021f4f14", "GFL_ArcSysReadHeapNewLZ", 1, "r0"),  # mus_item_data.c
    "ARCID_MUSICAL": ("func_ov112_021f6fb0", "GFL_G3DSysReadArcSysResource", 1, "r0"),  # sta_act_bg.c
    "ARCID_GSYNC_MENU": ("func_ov123_021d4884", "GFL_ArcSysCreateFileHandle", 1, "r0"),  # gsync_menu.c
    "ARCID_GSYNC": ("func_ov123_021d65f4", "GFL_ArcSysCreateFileHandle", 1, "r0"),  # gsync_disp.c
    "ARCID_PDWACC": ("func_ov123_021d9d3c", "GFL_ArcSysCreateFileHandle", 1, "r0"),  # pdwacc_disp.c
    "ARCID_MUSICAL_SHOT": ("func_ov133_021b6ba4", "GFL_ArcSysCreateFileHandle", 1, "r0"),  # dup_fitting.c
    "ARCID_THE_END": ("func_ov182_021b9788", "GFL_ArcSysCreateFileHandle", 1, "r0"),
    "ARCID_ZUKAN_GRA": ("func_ov209_021bd4f4", "GFL_ArcSysCreateFileHandle", 1, "r0"),  # zknlist_bmp.c
    "ARCID_SUBWAY_MAP": ("func_ov224_021b9824", "GFL_ArcSysCreateFileHandle", 1, "r0"),
}
# The function whose switch over a Pokémon's fields ends at the last field Black has
PARAM_SWITCH = "PML_PkmGetParamCore"


def sound_numbers():
    from tools.data.make_constants import sound_sequence_names

    names = sound_sequence_names((FILES / "wb_sound_data.sdat").read_bytes())
    return {name: number for number, name in enumerate(names) if name}


def messages(data):
    """A message file's lines, written as pokebw2's text files write them. Unlike Black 2's, Black's files give their
    lines attributes, which pokebw2's unpack_file refuses."""
    from tools.text.msgdata import decrypt, decompress
    from tools.text.text_data import COMPRESSED, LINE_END, format_message

    count = struct.unpack_from("<H", data, 2)[0]
    section = struct.unpack_from("<I", data, 12)[0]
    lines = []
    for i in range(count):
        offset, length, _ = struct.unpack_from("<IHH", data, section + 4 + 8 * i)
        chars = decrypt(struct.unpack_from(f"<{length}H", data, section + offset))
        compressed = bool(chars) and chars[0] == COMPRESSED
        chars = decompress(chars) if compressed else chars
        lines.append(format_message(chars[:-1] if chars and chars[-1] == LINE_END else chars, compressed))
    return lines


def bw2_messages(kind, name, prefix):
    """pokebw2's text of a Black 2 bank, with the messages it takes from its game data, or None."""
    from tools.text.text_data import split_message
    from tools.text.text_sources import expand

    path = Path(sys.path[0]) / ("res/text" if (Path(sys.path[0]) / "res/text").exists() else "data/text") / kind / (name.removeprefix(prefix).lower() + ".txt")
    if not path.exists():
        return None
    lines = []
    for line in path.read_text().split("\n")[:-1]:
        source = re.fullmatch(r"\\from\{([\w.]+)\}", line)
        if source:
            lines += expand(source[1])
        elif not line.startswith("\\pad{"):
            lines.append(split_message(line)[1])
    return lines


def plain(lines):
    """The messages as a set, without the spaces and terminators that pad empty ones."""
    return {line.replace("\\x{ffff}", "").strip() for line in lines} - {""}


class Archive:
    def __init__(self, path):
        from tools.data.narc import read_narc

        self.raw = read_narc(path.read_bytes())
        self.files = [messages(data) for data in self.raw]
        self.sets = [plain(lines) for lines in self.files]

    def bw2_match(self, bank):
        """Black's file of a Black 2 system text bank, by its number."""
        from tools.data.gen_constants import ordered_names

        return self.match(bw2_messages("system", ordered_names("text_banks")[bank], "TEXT_BANK_"))[0]

    def text(self, number):
        """A file's lines as make_constants.py reads them."""
        from tools.text.msgdata import read_msgdata

        return read_msgdata(self.raw[number])

    def match(self, lines):
        """The number of the file that shares the most messages with lines, and the scores of the best two: the
        messages shared over the larger count. None unless the best shares them all, or scores a quarter and a fifth
        more than the next."""
        want = plain(lines)
        scores = sorted(((len(want & have) / max(len(want), len(have), 1), number)
                         for number, have in enumerate(self.sets)), reverse=True)
        (best, number), (second, _) = scores[0], scores[1]
        sure = best == 1 > second or (best >= 0.25 and best >= second + 0.2)
        return (number if sure else None), best, second


def text_bank_numbers(names, kind, archive, prefix, notes):
    numbers = {}
    for name in names:
        lines = bw2_messages(kind, name, prefix)
        if lines is None:
            notes[name] = "no text in pokebw2"
            continue
        number, best, second = archive.match(lines)
        if number is None:
            notes[name] = f"best match {best:.2f}, next {second:.2f}"
        else:
            numbers[name] = number
    return numbers


def name_numbers(system, notes):
    """Black's numbers of the constants named after their text, and the names Black has none of."""
    from tools.data.gen_constants import ordered_names
    from tools.data.make_constants import TABLES, identifier

    def names(prefix, lines):
        """The constants that the text of their names gives, with their numbers. make_constants.py's OVERRIDES other
        than NONE name Black 2's numbers: its constants for lines whose text is not a name are left unchecked."""
        given = {}
        for i, line in enumerate(lines):
            given.setdefault(f"{prefix}_{'NONE' if i == 0 else identifier(line.removeprefix(chr(92) + 'c'))}",
                             []).append(i)
        return given

    banks = ordered_names("text_banks")
    numbers = {}
    for list_file, prefix, bank, count, _ in TABLES:
        if list_file.removesuffix(".txt") not in [name for name, _, _ in NAME_LISTS]:
            continue
        bw2_lines = bw2_messages("system", banks[bank], "TEXT_BANK_")
        number, best, second = system.match(bw2_lines)
        if number is None:
            notes[prefix + "_"] = f"no Black file of names (best {best:.2f})"
            continue
        black = names(prefix, system.files[number][:count])
        for name, bw2_numbers in names(prefix, bw2_lines[:count]).items():
            if name.endswith("_"):  # "???": unused
                continue
            if len(black.get(name, ())) == 1:
                numbers[name] = black[name][0]
            elif name in black:
                notes[name] = f"Black's names give it {len(black[name])} numbers"
            elif len(bw2_numbers) == 1:
                numbers[name] = None  # Black has none of this name
    return numbers


def zone_numbers(system):
    """Black's zones by the names pokebw2's make_constants.zone_names gives Black 2's, from Black's zone headers, place
    names and sound names, without its two names for Black 2's zone numbers (ZONE_OVERRIDES)."""
    import tools.data.gen_constants as gen_constants
    import tools.data.make_constants as make_constants

    sequences = {number: name for name, number in sound_numbers().items()}
    gen_constants.constant_names = lambda header, prefix: sequences
    make_constants.PLACE_NAMES = system.bw2_match(make_constants.PLACE_NAMES)
    make_constants.ZONE_OVERRIDES = {}
    return {name: zone for zone, name in make_constants.zone_names(ROOT / "extract/b_us").items()}


def arrived_flags(zones):
    """The flags of the places to fly to, from RESPAWN_TABLE: EVENT_FLAG_ARRIVED_ and the names of the main zones of
    the rows with the flag, less ZONE_, as pokebw2 names Black 2's (EVENT_FLAG_ARRIVED_BLACK_CITY_WHITE_FOREST)."""
    import yaml

    overlay, address, count = RESPAWN_TABLE
    overlays = yaml.safe_load((ROOT / "extract/b_us/arm9_overlays/overlays.yaml").read_text())["overlays"]
    data = (ROOT / f"extract/b_us/arm9_overlays/ov{overlay:03d}.bin").read_bytes()
    offset = address - next(o["base_address"] for o in overlays if o["id"] == overlay)
    names = {zone: name.removeprefix("ZONE_") for name, zone in zones.items()}
    places = {}
    for row in range(count):
        flag, main = struct.unpack_from("<H6xH", data, offset + 0x14 * row)
        places.setdefault(flag, []).append(names[main])
    return {"EVENT_FLAG_ARRIVED_" + "_".join(rows): flag for flag, rows in places.items()}


def trainer_numbers(system):
    """Black's trainer classes and trainers by the names make_constants.py's trainer_class_names and trainer_names give
    Black 2's: a class by its name, or for a name several classes share, with its one trainer's name (the classes it
    tells apart further, by sex or number, are left out); a trainer by its class and name, its name alone for
    PKMN_TRAINER, and _2, _3 for the next ones of both."""
    from collections import Counter

    from tools.data.make_constants import GENERIC_CLASS, TRAINER_CLASS_NAMES, TRAINER_NAMES, identifier
    from tools.data.narc import read_narc

    classes = [identifier(name.replace("⒆⒇", "Pkmn")) for name in system.text(system.bw2_match(TRAINER_CLASS_NAMES))]
    texts = system.text(system.bw2_match(TRAINER_NAMES))
    trainers = read_narc((FILES / TRAINER_DATA).read_bytes())
    users = {}
    for i, trainer in enumerate(trainers):
        if len(trainer) == 20 and identifier(texts[i]):
            users.setdefault(trainer[1], set()).add(identifier(texts[i]))
    names = {c: name or "NONE" for c, name in enumerate(classes)}
    shared = Counter(names.values())
    for c, name in names.items():
        if shared[name] > 1 and len(users.get(c, ())) == 1:
            names[c] = f"{name}_{next(iter(users[c]))}"
    shared = Counter(names.values())
    numbers = {f"TRAINER_CLASS_{name}": c for c, name in names.items() if shared[name] == 1}
    unsure = tuple(f"TRAINER_CLASS_{name}" for name, count in shared.items() if count > 1)
    count = Counter()
    for i, trainer in enumerate(trainers[1:], 1):
        name = identifier(texts[i])
        base = "TRAINER_" + "_".join(([] if classes[trainer[1]] == GENERIC_CLASS and name else [classes[trainer[1]]])
                                     + [name or str(i)])
        count[base] += 1
        numbers[base if count[base] == 1 else f"{base}_{count[base]}"] = i
    return numbers, unsure


def asm_functions():
    """Each function of Black's disassembly: its lines and its literal pool's words."""
    functions = {}
    for path in ASM.glob("**/*.s"):
        text = path.read_text(errors="replace")
        pools = dict(re.findall(r"^(\.L_\w+): \.word (\w+)", text, re.M))
        for name, body in re.findall(r"^(\w+): ; 0x\w+\n(.*?)(?=^\w+: ; 0x|\Z)", text, re.M | re.S):
            functions[name] = (body.splitlines(), pools)
    return functions


def argument(function, callee, nth, register):
    """The number in a register at the nth call to callee, a tail call through a register too, or None."""
    lines, pools = function
    values, pointers = {}, {}
    for line in lines:
        move = re.match(r"\s+mov (r\d), #(0x[0-9a-f]+|\d+)$", line)
        load = re.match(r"\s+ldr (r\d), (\.L_\w+)$", line)
        call = re.match(r"\s+(?:blx?|bx) (\w+)", line)
        if move:
            values[move[1]] = int(move[2], 0)
        elif load:
            pointers[load[1]] = pools.get(load[2])
            values.pop(load[1], None)
        elif call:
            if pointers.get(call[1], call[1]) == callee:
                nth -= 1
                if not nth:
                    return values.get(register)
            values, pointers = {}, {}
        elif re.match(r"\s+\w+ (r\d),", line):
            values.pop(line.split()[1].rstrip(","), None)
    return None


def archive_count():
    """The number of Black's archives: the entries of arm9's table of their paths, a/0/0/0 on."""
    arm9 = (ROOT / "extract/b_us/arm9/arm9.bin").read_bytes()
    paths = {0x02004000 + m.start() for m in re.finditer(rb"a/\d/\d/\d\0", arm9)}
    count = best = 0
    for offset in range(0, len(arm9) - 3, 4):
        count = count + 1 if struct.unpack_from("<I", arm9, offset)[0] in paths else 0
        best = max(best, count)
    return best


def archive_numbers(functions, notes):
    """Black's archive IDs from ARCHIVE_LOADERS."""
    numbers = {}
    for name, (function, *call) in ARCHIVE_LOADERS.items():
        if function not in functions:
            notes[name] = f"no function {function} in {ASM}"
        elif (number := argument(functions[function], *call)) is None:
            notes[name] = f"no number at {function}'s call"
        else:
            numbers[name] = number
    return numbers


def last_param(functions):
    """The last case of Black's PARAM_SWITCH (the cmp before its jump table), None without the disassembly."""
    if PARAM_SWITCH not in functions:
        return None
    lines = functions[PARAM_SWITCH][0]
    table = next(i for i, line in enumerate(lines) if "; jump table" in line)
    return next(int(m[1], 0) for line in reversed(lines[:table])
                if (m := re.match(r"\s+cmp r\d, #(0x[0-9a-f]+|\d+)$", line)))


def inputs(bw2):
    """A hash of what read_black() reads: the sizes and times of Black's files, the disassembly and this file, and
    pokebw2's commit."""
    paths = [Path(__file__).resolve(), ROOT / "extract/b_us/arm9/arm9.bin",
             ROOT / "extract/b_us/arm9_overlays/overlays.yaml",
             ROOT / f"extract/b_us/arm9_overlays/ov{RESPAWN_TABLE[0]:03d}.bin",
             *(FILES / name for name in ("wb_sound_data.sdat", "a/0/0/2", "a/0/0/3", "a/0/1/2", TRAINER_DATA)),
             *sorted(ASM.glob("**/*.s"))]
    stats = [(str(path.relative_to(ROOT)), path.stat().st_size, path.stat().st_mtime_ns) for path in paths]
    commit = subprocess.run(["git", "-C", bw2, "rev-parse", "HEAD"], capture_output=True, text=True).stdout
    return hashlib.sha1(repr((stats, commit)).encode()).hexdigest()


def read_black(bw2):
    """What Black's files tell of the constants, for numbers(): {header: {name: Black's number, or None for a name Black
    lacks}}, "unsure" (trainer_numbers), "archive_count", "last_param", and "notes" {name: why it is left unchecked}.
    From CACHE until what it reads changes."""
    key = inputs(bw2)
    if CACHE.exists() and (black := json.loads(CACHE.read_text()))["key"] == key:
        return black
    if sys.path[0] != bw2:  # pokebw2's tools, and bw2_messages() reads its text from there
        sys.path.insert(0, bw2)
    from tools.data.gen_constants import load

    notes = {}
    system = Archive(FILES / "a/0/0/2")
    black = {"key": key, "notes": notes, "sound.h": sound_numbers()}
    for kind, archive, header in (("system", system, "text_banks.h"),
                                  ("script", Archive(FILES / "a/0/0/3"), "script_text_banks.h")):
        black[header] = text_bank_numbers(sorted(load(header.removesuffix(".h"))), kind, archive, HEADERS[header],
                                          notes)
    names = name_numbers(system, notes)
    zones = zone_numbers(system)
    trainers, black["unsure"] = trainer_numbers(system)
    for header, told in [*((header, names) for _, _, header in NAME_LISTS), ("zones.h", zones),
                         ("flags.h", arrived_flags(zones)), ("trainer_classes.h", trainers), ("trainers.h", trainers)]:
        black[header] = {name: number for name, number in told.items() if name.startswith(HEADERS[header])}
    functions = asm_functions()
    black["arc.h"], black["archive_count"] = archive_numbers(functions, notes), archive_count()
    black["last_param"] = last_param(functions)
    CACHE.parent.mkdir(exist_ok=True)
    part = CACHE.with_name(f"{CACHE.name}.{os.getpid()}")  # a run cut short leaves no half-written cache
    part.write_text(json.dumps(black))
    part.replace(CACHE)
    return black


def numbers(black, header, defines):
    """Black's numbers of the constants of a header that Black's files tell, of defines {name: Black 2's number}, None
    for a name Black lacks: what rewrite() takes. A trainer class whose name Black's classes share without telling them
    apart by trainer stays unchecked."""
    told = black.get(header, {})
    names = [name for name in defines if name.startswith(HEADERS[header])]
    if header == "arc.h":  # an ID past the last of Black's archives is not Black's
        return {name: told.get(name) for name in names if name in told or
                name not in ARCHIVE_LOADERS and defines[name] >= black["archive_count"]}
    if header == "pokemon.h":  # nor a Pokémon's field past the last Black has
        last = black["last_param"]
        return {name: None for name in names if last is not None and defines[name] > last}
    if header in WHOLE:
        return {name: told.get(name) for name in names if name in told or not name.startswith(tuple(black["unsure"]))}
    return {name: told[name] for name in names if name in told}


def fmt(value, like):
    return hex(value) if like.startswith("0x") else str(value)


def rewrite(path, numbers, prefixes, check, report):
    """Moves the "Black 2's" defines of the names in numbers to BLACK_NOTE with Black's number, or MISSING_NOTE when
    numbers has the name as None; lists the other defines whose number Black gives another name."""
    found, lacking, out = [], [], []
    block = comment = None  # the note over the block, where the comment right above the line starts in out
    changed = 0
    for line in path.read_text().split("\n"):
        if not line.strip():
            block = None
        elif line in (B2_NOTE, BLACK_NOTE, MISSING_NOTE):
            block = line
        define = DEFINE.match(line)
        if define and define[1].startswith(prefixes) and define[1] in numbers:
            name, here, black = define[1], define[2], numbers[define[1]]
            if block == B2_NOTE:
                # The define's own comment goes with it
                own = out[comment:] if comment is not None else []
                del out[len(out) - len(own):]
                if black is None:
                    lacking += own + [line]
                else:
                    found += own + [f"#define {name} {fmt(black, here)}"]
                    changed += black != int(here, 0)
                comment = None
                continue
            if black is not None and black != int(here, 0):
                report.append(f"{path.name}: {name} is {here} here but {fmt(black, here)} in Black: check its users")
        plain_comment = line.startswith("//") and line not in (B2_NOTE, BLACK_NOTE, MISSING_NOTE)
        comment = (len(out) if comment is None else comment) if plain_comment else None
        out.append(line)
    count = sum(bool(DEFINE.match(line)) for line in found), sum(bool(DEFINE.match(line)) for line in lacking)
    report.append(f"{path.name}: {count[0]} read from the ROM ({changed} changed), {count[1]} not in Black")
    if check or not (found or lacking):
        return
    # A "Black 2's" note left with comments but no define under it goes, with them
    blocks = "\n".join(out).split("\n\n")
    text = "\n\n".join(b for b in blocks if not (b.startswith(B2_NOTE) and all(
        line.startswith("//") for line in b.strip("\n").split("\n"))))
    for note, lines in ((BLACK_NOTE, found), (MISSING_NOTE, lacking)):
        if lines:
            text = add_under(text, note, lines)
    text = re.sub(r"\n{3,}", "\n\n", text)
    path.write_text(text)


def add_under(text, note, lines):
    """A header's text with lines at the end of the block under note, which a blank line or another note ends, or in a
    new one under it at the end."""
    if note in text:
        start = text.index(note) + len(note)
        end = min(at for after in ("\n\n", "\n#endif", *("\n" + n + "\n" for n in (B2_NOTE, BLACK_NOTE, MISSING_NOTE)))
                  if (at := text.find(after, start)) >= 0)
        return text[:end] + "\n" + "\n".join(lines) + text[end:]
    end = text.rindex("\n#endif")
    return text[:end].rstrip("\n") + "\n\n" + note + "\n" + "\n".join(lines) + "\n" + text[end:]


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--bw2", default=os.path.expanduser("~/ref/pokebw2"), help="pokebw2 checkout (read only)")
    ap.add_argument("--check", action="store_true", help="only list, change nothing")
    args = ap.parse_args()
    sys.path.insert(0, args.bw2)
    black = read_black(args.bw2)
    report, notes = [], []
    for header, prefix in HEADERS.items():
        path = CONSTANTS / header
        defines = {m[1]: int(m[2], 0) for m in map(DEFINE.match, path.read_text().split("\n")) if m}
        notes += [f"{name}: {note}" for name, note in black["notes"].items() if name in defines or name == prefix]
        rewrite(path, numbers(black, header, defines), (prefix,), args.check, report)
    print("\n".join(notes + report))


if __name__ == "__main__":
    main()
