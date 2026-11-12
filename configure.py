#!/usr/bin/env python3
"""Generates build.ninja for the Pokémon Black decompilation.

Run this once after cloning (and again after changing the configs), then run `ninja`. Adapted from pokebw2's
configure.py (GPL-3.0, https://github.com/fuddlesworth/pokebw2).
"""
import io
import json
import os
import shlex
import stat
import subprocess
import sys
import tomllib
import urllib.request
import zipfile
from pathlib import Path

ROOT = Path(__file__).parent.resolve()
sys.path.insert(0, str(ROOT / "tools"))
from two_modules import compiled_alias  # noqa: E402

# Each version builds from the same source with its define, as pokebw2 builds Black 2 and White 2
# (include/constants/version.h). Black's config is kept in config/b_us; another version's is made from it and the
# version's config/<version>/delta.txt by tools/version_config.py (docs/white.md). Black's assets go to build/asset, which
# the PC port reads too, another version's to build/<version>/asset
VERSIONS = {
    "b_us": {"sha1": "26ad0b9967aa279c4a266ee69f52b9b2332399a5", "rom": "pokeblack_us.nds", "defines": ["BLACK"],
             "asset_root": "build"},
    "w_us": {"sha1": "bc696a0dfb448c7b3a8a206f0f8214411a039208", "rom": "pokewhite_us.nds", "defines": ["WHITE"],
             "asset_root": "build/w_us"},
}

WIBO_URL = "https://github.com/decompals/wibo/releases/download/1.2.0/wibo-x86_64"
MWCCARM_URL = "https://decomp.aetias.com/files/mwccarm.zip"
# pokebw2's fork of dsd, which handles DSi hybrid ROMs such as Black. configure.py downloads it again when this changes.
DSD_VERSION = "v0.12.1-dsi.2"
DSD_URL = f"https://github.com/fuddlesworth/ds-decomp/releases/download/{DSD_VERSION}/dsd-linux-x86_64"
# Black's game code matches with dsi/1.1p1, and identically with any build up to dsi/1.3p1 (the maintainers' notes)
MWCC_VERSION = "dsi/1.1p1"
# The linker places the objects and makes the long-branch veneers. It must be dsi/1.1 or later: only those give one
# veneer per call site under -overlaygroup (LD_FLAGS), as Black has; the 1.2 and 2.0 linkers share one per target
MWLD_VERSION = "dsi/1.1"

# pokebw2's flags for the game code
CC_FLAGS = [
    "-O4,p",               # Optimize for speed
    "-proc arm946e",       # ARM9 processor
    "-thumb",              # Most game code is Thumb
    "-interworking",       # ARM/Thumb interworking
    "-enum int",           # Enums are int-sized
    "-char signed",        # char is signed
    "-fp soft",            # Software floating point
    "-lang=c99",
    "-Cpp_exceptions off", # No exception tables
    "-gccext,on",          # GCC extensions
    "-gccinc",             # #include "..." and <...> search the same paths
    "-inline on,noauto",   # Only inline functions marked inline
    "-ipa file",           # Interprocedural analysis within each file
    "-sym on",             # Debug info
    "-requireprotos",      # Calling an undeclared function is an error, instead of an implicit declaration
    "-nolink",
    "-msgstyle gcc",
]

# Libraries built apart from the game, each with its own compiler and flags, as in pokebw2: lib/<name>/ holds a
# library's headers in include/, its sources in src/, and its compiler and flags in library.toml. Its file_flags give
# files built with other flags, each replacing the library's flag of the same option ("-ipa function" for "-ipa file");
# its file_compiler, files built with another compiler than the library's (tools/flag_search.py finds them).
LIBRARIES = {p.parent.name: tomllib.loads(p.read_text()) for p in sorted((ROOT / "lib").glob("*/library.toml"))}
# build/ last, for the assets the build writes from the ROM (#include "asset/<name>.inc", tools/asset.py)
INCLUDE_DIRS = ["include", *(p.relative_to(ROOT).as_posix() for p in sorted((ROOT / "lib").glob("*/include"))),
                "build"]


# The game's files built with other flags, as in pokebw2's config/compiler_flags.toml: each replaces the game's flag of
# the same option ("-ipa function" for "-ipa file"). tools/data_order.py says when a file's data needs one.
GAME_FLAGS = (tomllib.loads((ROOT / "config/compiler_flags.toml").read_text())
              if (ROOT / "config/compiler_flags.toml").exists() else {})


def compiler_of(source: Path) -> tuple[str, list[str]]:
    """Returns the compiler and flags of a source file: its library's, or the game's."""
    parts = (ROOT / source).resolve().relative_to(ROOT).parts
    if parts[0] != "lib":
        overrides = GAME_FLAGS.get("file_flags", {}).get("/".join(parts), [])
        options = {flag.split()[0] for flag in overrides}
        return MWCC_VERSION, [flag for flag in CC_FLAGS if flag.split()[0] not in options] + overrides
    library = LIBRARIES[parts[1]]
    overrides = library.get("file_flags", {}).get("/".join(parts[3:]), [])
    # a file ported as it is from another decompilation's SDK (tools/port_lib.py): its headers and defines
    sdk = library.get("sdk", {})
    dropped = set()
    if "/".join(parts[3:]) in sdk.get("files", []):
        overrides = sdk["flags"] + overrides
        dropped = set(sdk.get("without", []))  # the library's flags its SDK build didn't have
    options = {flag.split()[0] for flag in overrides} | dropped
    compiler = library.get("file_compiler", {}).get("/".join(parts[3:]), library["compiler"])
    return compiler, [flag for flag in library["flags"] if flag.split()[0] not in options] + overrides

LD_FLAGS = [
    "-proc arm946e",
    "-nostdlib",
    "-interworking",
    "-map closure,unused",
    "-msgstyle gcc",
    "-m Entry",
    # Every object in a command-line overlay (not the first group), as in Black's link: mwld then makes a long-branch
    # veneer ($Ven$lb$) for each call site, right after its caller, instead of one per target. The linker script still
    # places everything (the maintainers' notes, Measurements 2026-10-14)
    "-overlaygroup game,0",
    "-overlay game",
]


class Writer:
    """Minimal ninja file writer."""

    def __init__(self):
        self.out = io.StringIO()

    def variable(self, key, value):
        self.out.write(f"{key} = {value}\n")

    def rule(self, name, command, description=None, **kwargs):
        self.out.write(f"rule {name}\n  command = {command}\n")
        if description:
            self.out.write(f"  description = {description}\n")
        for key, value in kwargs.items():
            self.out.write(f"  {key} = {value}\n")
        self.out.write("\n")

    def build(self, outputs, rule, inputs=(), implicit=(), variables=None, order_only=()):
        def esc(paths):
            return " ".join(str(p).replace("$", "$$").replace(" ", "$ ").replace(":", "$:") for p in paths)

        line = f"build {esc(outputs)}: {rule} {esc(inputs)}"
        if implicit:
            line += f" | {esc(implicit)}"
        if order_only:
            line += f" || {esc(order_only)}"
        self.out.write(line + "\n")
        for key, value in (variables or {}).items():
            self.out.write(f"  {key} = {value}\n")
        self.out.write("\n")

    def default(self, targets):
        self.out.write(f"default {' '.join(str(t) for t in targets)}\n")


def download(url: str, path: Path):
    print(f"Downloading {url}")
    urllib.request.urlretrieve(url, path)
    path.chmod(path.stat().st_mode | stat.S_IEXEC)


def download_tools(tools_dir: Path):
    tools_dir.mkdir(exist_ok=True)
    if not (tools_dir / "wibo").exists():
        download(WIBO_URL, tools_dir / "wibo")
    compilers = ["dsi", *(c for library in LIBRARIES.values()
                          for c in [library["compiler"], *library.get("file_compiler", {}).values()])]
    if not all((tools_dir / "mwccarm" / compiler).exists() for compiler in compilers):
        print(f"Downloading {MWCCARM_URL}")
        with urllib.request.urlopen(MWCCARM_URL) as response:
            archive = zipfile.ZipFile(io.BytesIO(response.read()))
        prefixes = tuple(f"mwccarm/{compiler}/" for compiler in compilers)
        archive.extractall(tools_dir, [m for m in archive.namelist() if m.startswith(prefixes)])
    # The compilers start with the x87 at 53-bit precision, as Windows starts them (tools/mwcc_fpu.py; idempotent)
    subprocess.run([sys.executable, str(ROOT / "tools" / "mwcc_fpu.py")], check=True, capture_output=True)
    stamp = tools_dir / "dsd.rev"
    if not stamp.exists() or stamp.read_text().strip() != DSD_VERSION:
        download(DSD_URL, tools_dir / "dsd")
        stamp.write_text(DSD_VERSION + "\n")


def add_version(n: Writer, version: str, dsd: Path) -> tuple[list[Path], list[str]]:
    """Adds the build steps of one version. Returns its check targets and dsd config files."""
    baserom = Path("orig") / f"baserom_{version}.nds"
    extract_dir = Path("extract") / version
    config_dir = Path("config") / version
    stamp_dir = Path("build") / version / "stamps"
    arm9_config = config_dir / "arm9" / "config.yaml"
    rom = Path("build") / VERSIONS[version]["rom"]
    if (ROOT / config_dir / "delta.txt").exists():
        # made from Black's config (tools/version_config.py); a file is written only when it changes
        result = subprocess.run([sys.executable, str(ROOT / "tools" / "version_config.py"), version],
                                capture_output=True, text=True, cwd=ROOT)
        if result.returncode != 0:
            sys.exit(result.stderr)
    dsd_configs = sorted(str(p) for p in config_dir.rglob("*.txt") if p.name != "delta.txt") + [str(arm9_config)]

    # The delink targets are known ahead of time from the configs
    result = subprocess.run([str(dsd), "json", "delinks", "--config-path", str(arm9_config)],
                            capture_output=True, text=True, cwd=ROOT)
    if result.returncode != 0:
        sys.exit(f"dsd json delinks failed for {version}:\n{result.stderr}")
    delinks = json.loads(result.stdout)
    lcf_file = delinks["arm9_lcf_file"]
    objects_file = delinks["arm9_objects_file"]

    baserom_ok = stamp_dir / "baserom.ok"
    n.build([baserom_ok], "sha1", [baserom], variables={"sha1": VERSIONS[version]["sha1"]})
    n.build([extract_dir / "config.yaml"], "extract", [baserom], implicit=[baserom_ok],
            variables={"extract_dir": str(extract_dir)})
    delink_outputs = sorted({f["delink_file"] for f in delinks["files"]})
    n.build(delink_outputs, "delink", dsd_configs, implicit=[extract_dir / "config.yaml"],
            variables={"config": str(arm9_config), "out_dir": os.path.commonpath(delink_outputs)})
    n.build([lcf_file, objects_file], "lcf", dsd_configs,
            implicit=[extract_dir / "config.yaml", "tools/two_modules.py"],
            variables={"config": str(arm9_config), "lcf": lcf_file, "objects": objects_file})

    # The game's assets stay out of the C: each row of assets.tsv is written from the extracted ROM to
    # <asset_root>/asset/<name>.inc, which the C includes (tools/asset.py): each version's from its own ROM
    asset_root = VERSIONS[version]["asset_root"]
    assets = []
    if (config_dir / "assets.tsv").exists():
        for row in (config_dir / "assets.tsv").read_text().splitlines()[1:]:
            name, mod, start, end, kind = row.split("\t")
            assets.append(Path(asset_root) / "asset" / f"{name}.inc")
            # a table's names come from the symbols of its module and of main
            symbols = [] if kind in ("u8", "u16", "u32", "s8", "s16", "s32") else                 [config_dir / "arm9" / "symbols.txt"] + ([config_dir / "arm9/overlays" / mod / "symbols.txt"] if mod.startswith("ov") else [])
            n.build([assets[-1]], "asset", [], implicit=[extract_dir / "config.yaml", "tools/asset.py", "tools/asset_table.py", *symbols],
                    variables={"args": f"--version {version} {mod} {start} {end} {shlex.quote(kind)}"})

    # Source files are listed in delinks.txt by their path. Every one that exists is compiled, and dsd links the
    # compiled object in place of the delinked one once the file is marked complete
    compiled = []
    # constants/version.h goes in front of every file: `#if GAME_VERSION == VERSION_WHITE` in a file that did not include
    # it was true in Black (both undefined, 0 == 0), and compiled White's branch into Black
    defines = " ".join(f"-d {define}" for define in VERSIONS[version]["defines"]) + " -include constants/version.h"
    for f in delinks["files"]:
        source = Path(f["name"])
        if source.suffix == ".c" and source.exists():
            obj = Path("build") / version / source.with_suffix(".o")
            compiler, flags = compiler_of(source)
            n.build([obj], "mwcc", [source], order_only=assets,
                    variables={"compiler": compiler, "flags": " ".join(flags), "dep": obj.with_suffix(".d"),
                               "defines": defines, "asset_root": asset_root})
            compiled.append(obj)
            # a source of two dots (crt0.FLX.c): dsd's object list and delinked object name it up to the first dot
            # (crt0.o), so the link gets a copy of that name (tools/two_modules.py names it so in the linker script)
            short = Path(f["delink_file"].replace(f"build/{version}/delinks/", f"build/{version}/", 1))
            if short != obj and short.name == source.name.split(".")[0] + ".o":
                n.build([short], "copy", [obj])

    arm9_o = Path("build") / version / "arm9.o"
    # (a complete alias's object is its file's, tools/two_modules.py)
    linked = [f["object_to_link"] for f in delinks["files"] if not compiled_alias(f["object_to_link"])]
    n.build([arm9_o], "mwld", linked,
            implicit=[lcf_file, objects_file, "tools/dead_strip.py"],
            variables={"objects": objects_file, "lcf": lcf_file})
    rom_config = Path("build") / version / "build" / "rom_config.yaml"
    n.build([rom_config], "rom_config", [arm9_o], variables={"config": str(arm9_config)})
    n.build([rom], "rom_build", [rom_config])

    modules_ok = stamp_dir / "modules.ok"
    n.build([modules_ok], "check_modules", [rom_config], variables={"config": str(arm9_config)})
    rom_ok = stamp_dir / "rom.ok"
    n.build([rom_ok], "sha1", [rom], variables={"sha1": VERSIONS[version]["sha1"]})
    n.build([version], "phony", [modules_ok, rom_ok])

    # The disassembly, read one function at a time with tools/show_func.py
    asm_ok = stamp_dir / "asm.ok"
    n.build([asm_ok], "dis", dsd_configs, implicit=[extract_dir / "config.yaml"],
            variables={"config": str(arm9_config), "asm_dir": str(Path("build") / version / "asm")})
    n.build([f"{version}_asm"], "phony", [asm_ok])
    # The checks also compile the incomplete files, which are not linked, so that one can't stop compiling unnoticed
    return [modules_ok, rom_ok, *compiled], dsd_configs


def main():
    versions = [v for v in VERSIONS if (ROOT / "orig" / f"baserom_{v}.nds").exists()]
    if not versions:
        sys.exit("no base ROMs found in orig/, see README.md")

    tools_dir = ROOT / "tools"
    download_tools(tools_dir)
    # build.ninja names the tools relative to the checkout: every worktree's commands are then main's, and a copy of
    # main's build is up to date in it (tools/setup_worktree.sh)
    tools_rel = Path("tools")
    dsd = tools_rel / "dsd"
    mwld = tools_rel / "mwccarm" / MWLD_VERSION / "mwldarm.exe"

    n = Writer()
    n.out.write(f"# Generated by configure.py for {', '.join(versions)}, do not edit\n")
    n.variable("dsd", shlex.quote(str(dsd)))
    n.variable("wibo", shlex.quote(str(tools_rel / "wibo")))
    n.variable("python", shlex.quote(sys.executable))
    n.out.write("\n")

    n.rule("sha1", "echo '$sha1  $in' | sha1sum --quiet -c - && mkdir -p $$(dirname $out) && touch $out",
           "Checking $in")
    # dsd logs hundreds of INFO lines per run, so its rules keep the log in build/logs/ and print it only on failure
    def quiet(command, log):
        return (f"mkdir -p build/logs && {{ {command}; }} > build/logs/{log}.log 2>&1 "
                f"|| {{ grep -v INFO build/logs/{log}.log | tail -n 30; exit 1; }}")

    n.rule("extract", quiet("$dsd rom extract --rom $in --output-path $extract_dir", "extract"), "Extracting $in")
    # Gaps are numbered, so a file added or renamed leaves stale objects behind, which twins.py and clones.py would read
    n.rule("delink", quiet("rm -rf $out_dir && $dsd delink --config-path $config", "delink"), "Delinking $config")
    # A file in two modules (TwlSDK's card_rom.c: main's .text and the LTD autoload's .ltdmain) is named in the second
    # by an alias, card_rom@ltd.c: tools/two_modules.py gives a complete one's lines the file's object
    n.rule("lcf", "$dsd lcf --config-path $config && $python tools/two_modules.py $lcf $objects",
           "Generating linker script for $config")
    # mwccarm writes the dependency file next to the object, with Windows paths that fix_depfile.py converts. Each file
    # has its compiler (a directory under tools/mwccarm) and flags from compiler_of
    n.rule("mwcc", f"mkdir -p $$(dirname $out) && $wibo {shlex.quote(str(tools_rel))}/mwccarm/$compiler/mwccarm.exe "
           "$flags $defines -gccdep -MD "
           f"{' '.join(f'-i {d}' for d in INCLUDE_DIRS[:-1])} -i $asset_root -o $out $in && "
           "$python tools/fix_depfile.py $dep",
           "Compiling $in", depfile="$dep", deps="gcc")
    # dead-stripping, as the original link: tools/dead_strip.py forces in what Black has, so only what our C adds goes
    n.rule("mwld", f"$python tools/dead_strip.py $lcf $objects ${{out}}.lcf ${{out}}.objects && "
           f"$wibo {shlex.quote(str(mwld))} {' '.join(LD_FLAGS)} @${{out}}.objects ${{out}}.lcf -o $out", "Linking $out")
    n.rule("rom_config", quiet("$dsd rom config --elf $in --config $config", "rom_config"), "Configuring ROM for $config")
    n.rule("rom_build", quiet("$dsd rom build --config $in --rom $out", "rom_build"), "Building $out")
    n.rule("check_modules", quiet("$dsd check modules --config-path $config --fail && touch $out", "check_modules"),
           "Checking modules")
    n.rule("dis", quiet("rm -rf $asm_dir && $dsd dis --config-path $config --asm-path $asm_dir && touch $out", "dis"),
           "Disassembling $config")
    n.rule("asset", "$python tools/asset.py $args $out", "Writing $out")
    n.rule("copy", "cp $in $out", "Copying $out")
    n.rule("configure", "$python configure.py", "Reconfiguring", generator="1")

    checks, configs = [], []
    for version in versions:
        version_checks, version_configs = add_version(n, version, dsd)
        checks += version_checks
        configs += version_configs

    n.build(["build.ninja"], "configure", ["configure.py"],
            implicit=[*configs, *(f"lib/{name}/library.toml" for name in LIBRARIES), "config/compiler_flags.toml",
                      *(str(p.relative_to(ROOT)) for p in sorted((ROOT / "config").glob("*/assets.tsv"))),
                      *(str(p.relative_to(ROOT)) for p in sorted((ROOT / "config").glob("*/delta.txt"))),
                      "tools/version_config.py"])
    n.build(["check"], "phony", checks)
    n.build(["asm"], "phony", [f"{version}_asm" for version in versions])
    n.default(["check"])

    (ROOT / "build.ninja").write_text(n.out.getvalue())
    # The twin tables are committed, made from pokebw2's commit in config/b_us/pokebw2.txt; build/ links them, so that
    # tools/twins.py and tools/twin_files.py write them in place
    for table in (ROOT / "config").glob("*/twin*.tsv"):
        link = ROOT / "build" / table.name
        if not link.is_symlink():
            link.parent.mkdir(exist_ok=True)
            link.unlink(missing_ok=True)
            link.symlink_to(Path("..") / table.relative_to(ROOT))
    print(f"Wrote build.ninja for {', '.join(versions)}, now run ninja")


if __name__ == "__main__":
    main()
