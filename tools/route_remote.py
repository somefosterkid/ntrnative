#!/usr/bin/env python3
"""route.py ... --remote [MACHINE]: the same route.py command (look, add, legs, --chain, state) on a free slot of another machine,
with this worktree's UNCOMMITTED changes (docs/route-writer.md, "Leg tries on another machine").

    tools/route.py hg look SAVE "TOKENS" --remote auto
    tools/route.py pt add "name | start | tokens | ? | save" --remote laptop

MACHINE: auto (the slot runner's choice: the first machine with a free slot), laptop, bd, local, or a comma list of them.
$BWPORT_ROUTE_RUNNER names the slot runner (a script with the interface `--on M --wait S --commit SHA --cmd SCRIPT`, then
`--fetch RUN_ID NAME`); $ROUTE_REMOTE_WAIT is how long to queue for a slot (default 900 s).

What it does:
  1. a snapshot commit of the worktree (HEAD + every tracked change + new files git does not ignore; a temporary index, the
     worktree's own index and files are never touched): HEAD itself when nothing changed. Submodule contents are not shipped.
  2. the runner ships that commit to the slot's checkout, builds as its jobs do, runs route.py there with the same arguments and
     packs what the run left: its output, the tracked files it changed (a route file `add` wrote), new files in the checkout, the
     work-dir files it wrote (logs, map dumps, --shot pngs), the saves it stored and its new README lines.
  3. here: the output printed with the slot's paths turned into this worktree's, the changes applied (git apply), the work files
     put in this worktree's RUN_WORK, the saves and README lines added to the save library (an existing save is overwritten
     only when it is --replace's), then one line naming the machine (host and compiler: HG/SS frames differ between clang
     versions, so compare a leg's frames only with runs on the same machine) and the seconds.
Exit: route.py's on the slot; 2 when the runner or the return failed, 3 when every slot stayed busy.
"""
import os
import re
import shlex
import shutil
import subprocess
import sys
import tarfile
import tempfile
import time
from pathlib import Path

MACHINES = ("auto", "laptop", "bd", "local")
UNTRACKED_CAP = 50 << 20   # new files shipped with the snapshot: more than this is a mistake (a stray dump in the tree)
TRIES = 3                  # runner attempts when it failed before the command started (transport_failed)
RET = "ret.txz"            # the return archive's name on the slot machine (RUN_ID.ret.txz)
ID_RE = re.compile(r"^lp_route: (\S+): commit [0-9a-f]{9} ", re.M)
SUBCOMMANDS = ("add", "look", "state", "diff")


def machine_ok(m):
    return all(p in MACHINES[1:] for p in m.split(",")) or m == "auto"


def strip_remote(argv):
    """argv (no program name) -> (MACHINE or None, argv without --remote [MACHINE]); a bare --remote is auto"""
    out, machine, i = [], None, 0
    while i < len(argv):
        a = argv[i]
        if a.startswith("--remote="):
            machine = a.split("=", 1)[1]
        elif a == "--remote":
            if i + 1 < len(argv) and machine_ok(argv[i + 1]):
                machine = argv[i + 1]
                i += 1
            else:
                machine = "auto"
        else:
            out.append(a)
        i += 1
    if machine is not None and not machine_ok(machine):
        raise SystemExit(f"route --remote: MACHINE is one of {', '.join(MACHINES)} or a comma list (got {machine})")
    return machine, out


def git(root, *args, env=None, check=True):
    r = subprocess.run(["git", *args], cwd=root, env=env, capture_output=True, text=True)
    if check and r.returncode:
        raise SystemExit(f"route --remote: git {' '.join(args[:3])} failed: {r.stderr.strip()[:300]}")
    return r.stdout.strip()


def snapshot(root, cap=UNTRACKED_CAP):
    """-> (commit, changed paths): HEAD plus the worktree's changes, in a commit made through a temporary index"""
    head = git(root, "rev-parse", "HEAD")
    new = [p for p in git(root, "ls-files", "-o", "--exclude-standard", "-z").split("\0") if p]
    size = sum((Path(root) / p).lstat().st_size for p in new if (Path(root) / p).is_file())
    if size > cap:
        big = sorted(new, key=lambda p: -(Path(root) / p).lstat().st_size)[:5]
        raise SystemExit(f"route --remote: {size >> 20} MB of new untracked files would be shipped (cap {cap >> 20} MB): "
                         f"{' '.join(big)} (delete or .gitignore them)")
    when = git(root, "log", "-1", "--format=%cD", "HEAD")   # a fixed date: the same changes on the same HEAD = the same commit,
    with tempfile.TemporaryDirectory() as td:               # which a slot that ran it already has (no bundle, no checkout)
        env = dict(os.environ, GIT_INDEX_FILE=str(Path(td) / "index"), GIT_AUTHOR_DATE=when, GIT_COMMITTER_DATE=when,
                   GIT_AUTHOR_NAME="route-remote",
                   GIT_AUTHOR_EMAIL="route-remote@localhost", GIT_COMMITTER_NAME="route-remote",
                   GIT_COMMITTER_EMAIL="route-remote@localhost")
        git(root, "read-tree", "HEAD", env=env)
        git(root, "add", "-A", "--", ".", env=env)
        tree = git(root, "write-tree", env=env)
        changed = [l for l in git(root, "diff", "--cached", "--name-only", "HEAD", env=env).splitlines() if l]
        if tree == git(root, "rev-parse", "HEAD^{tree}"):
            return head, []
        sha = git(root, "commit-tree", tree, "-p", head, "-m", "route.py --remote: the worktree's changes on HEAD", env=env)
    return sha, changed


def dirty_submodules(root):
    """submodules with changes inside them (not shipped: only their recorded commit is)"""
    st = git(root, "submodule", "status", check=False)
    out = []
    for l in st.splitlines():
        f = l[1:].split()
        if l.startswith("-") or len(f) < 2 or not (Path(root) / f[1] / ".git").exists():
            continue   # not checked out here: nothing inside it to lose
        if git(Path(root) / f[1], "status", "--porcelain", "-uno", check=False):
            out.append(f[1])
    return out


def game_of(argv):
    return argv[0] if argv else ""


def slot_args(game, machine, sha, wait):
    """the runner's arguments before --cmd: machine, queue, commit, the slot family of the game"""
    a = [] if machine == "auto" else ["--on", machine]
    a += ["--wait", str(wait), "--commit", sha]
    if game in ("hg", "ss"):
        a += ["--hg", "--family", "hgss", "--no-port"]
    elif game == "pt":
        a += ["--family", "plat"]   # route.py pt runs build/port/linux's exe: the slot's port build phase makes it
    else:
        a += ["--family", "gen5"]
    return a


def route_args(argv):
    """route.py's arguments on the slot: the caller's, with -j the slot's share in a legs run that names none"""
    a = list(argv)
    if len(a) > 1 and a[1] in SUBCOMMANDS or a[:1] == ["port"]:
        return a, False
    return a, not any(x == "-j" or x.startswith("-j") and x[2:].isdigit() for x in a)


def remote_script(argv):
    """the slot's --cmd: run route.py, then pack what it left into $LP_RD/$NTR_RUN_ID.ret.txz"""
    a, add_j = route_args(argv)
    cmd = " ".join(shlex.quote(x) for x in a) + (' -j "${NTR_SLOT_JOBS:-2}"' if add_j else "")
    return f"""set -u
PY=$(command -v python || command -v python3)
T=$(mktemp -d); trap 'rm -rf "$T"' EXIT
git checkout -q -f HEAD -- . 2>/dev/null   # a slot checkout: tracked files exactly the commit's
eval "$($PY -c 'import sys; sys.path.insert(0, "tools"); import route, shlex; print("W=" + shlex.quote(str(route.WORK)) + "; SV=" + shlex.quote(str(route.SAVES)))')"
mkdir -p "$W" "$T/r/work" "$T/r/saves"
cp "$SV/README" "$T/readme0" 2>/dev/null || : > "$T/readme0"
git ls-files -o --exclude-standard | sort > "$T/new0"
touch "$T/t0"
t0=$(date +%s)
$PY tools/route.py {cmd} > "$T/r/route.out" 2>&1
rc=$?
echo "$rc" > "$T/r/rc"
tail -n 30 "$T/r/route.out" | cut -c1-240
git diff --binary HEAD > "$T/r/changes.patch"
git ls-files -o --exclude-standard | sort | comm -13 "$T/new0" - > "$T/newfiles"
[ -s "$T/newfiles" ] && tar cf "$T/r/newfiles.tar" -T "$T/newfiles"
(cd "$W" && find . -type f -newer "$T/t0" -print0 | tar cf "$T/r/work.tar" --null -T -)
(cd "$SV" && find . -maxdepth 1 -name '*.bin' -newer "$T/t0" -print0 | xargs -0 -r cp -p -t "$T/r/saves")
[ -f "$SV/README" ] && {{ grep -vxF -f "$T/readme0" "$SV/README" || true; }} > "$T/r/readme.add"
[ -s "$T/readme0" ] || : > "$T/r/readme.add"   # no README before: nothing to tell new lines from
{{ echo "root=$PWD"; echo "work=$W"; echo "saves=$SV"; echo "host=$(hostname)"; echo "cc=$(clang --version 2>/dev/null | head -n 1)"
   echo "route_s=$(( $(date +%s) - t0 ))"; }} > "$T/r/meta"
tar cf - -C "$T/r" . | xz -T2 -3 > "$LP_RD/$NTR_RUN_ID.{RET}.tmp" && mv -f "$LP_RD/$NTR_RUN_ID.{RET}.tmp" "$LP_RD/$NTR_RUN_ID.{RET}"
exit $rc
"""


def rewrite(text, pairs):
    """the slot's paths -> this worktree's, the longest first (work and saves before the checkout root they may sit in)"""
    for old, new in sorted(pairs, key=lambda p: -len(p[0])):
        if old and old != new:
            text = text.replace(old.rstrip("/"), str(new).rstrip("/"))
    return text


def extract(tf, dst, members):
    if hasattr(tarfile, "data_filter"):
        tf.extractall(dst, members=members, filter="data")
    else:
        tf.extractall(dst, members=members)


def safe_members(tf):
    """no absolute paths, no .., no links out: what a slot packed for us, extracted under one directory"""
    for m in tf.getmembers():
        p = Path(m.name)
        if p.is_absolute() or ".." in p.parts or m.islnk() or m.issym() or m.isdev():
            continue
        yield m


def unpack(ret, root, work, saves, replace=()):
    """the return archive -> (rc, route output, report lines): changes, work files, saves, README lines put in place"""
    rep = []
    with tempfile.TemporaryDirectory() as td:
        td = Path(td)
        with tarfile.open(ret, "r:xz") as tf:
            extract(tf, td, list(safe_members(tf)))
        meta = dict(l.split("=", 1) for l in (td / "meta").read_text().splitlines() if "=" in l)
        rc = int((td / "rc").read_text().strip() or 2)
        out = rewrite((td / "route.out").read_text(errors="replace"),
                      [(meta.get("work", ""), work), (meta.get("saves", ""), saves), (meta.get("root", ""), root)])
        patch = td / "changes.patch"
        if patch.exists() and patch.stat().st_size:
            r = subprocess.run(["git", "apply", "--whitespace=nowarn", str(patch)], cwd=root, capture_output=True, text=True)
            names = sorted(set(re.findall(r"^\+\+\+ b/(\S+)", patch.read_text(errors="replace"), re.M)))
            if r.returncode:
                keep = Path(work) / "remote-changes.patch"
                Path(work).mkdir(parents=True, exist_ok=True)
                shutil.copy(patch, keep)
                rep.append(f"the run's changes did NOT apply here ({r.stderr.strip()[:160]}): {keep}")
            else:
                rep.append(f"applied the run's changes: {' '.join(names)}")
        if (td / "newfiles.tar").exists():
            with tarfile.open(td / "newfiles.tar") as tf:
                ms = [m for m in safe_members(tf) if not (Path(root) / m.name).exists()]
                extract(tf, root, ms)
            rep.append(f"new files: {' '.join(m.name for m in ms) or '(all were here)'}")
        if (td / "work.tar").exists():
            Path(work).mkdir(parents=True, exist_ok=True)
            with tarfile.open(td / "work.tar") as tf:
                ms = list(safe_members(tf))
                extract(tf, work, ms)
        Path(saves).mkdir(parents=True, exist_ok=True)
        got = []
        for f in sorted((td / "saves").glob("*.bin")):
            dst = Path(saves) / f.name
            if dst.exists() and dst.read_bytes() == f.read_bytes():
                continue
            if dst.exists() and f.stem not in replace:
                rep.append(f"save {f.name} differs from the library's: left alone (CONFLICT)")
                continue
            shutil.copy(f, str(dst) + ".rr")
            os.replace(str(dst) + ".rr", dst)
            got.append(f.name)
        if got:
            rep.append(f"saves into the library: {' '.join(got)}")
        add = (td / "readme.add").read_text().splitlines() if (td / "readme.add").exists() else []
        readme = Path(saves) / "README"
        have = set(readme.read_text().splitlines()) if readme.exists() else set()
        new = [l for l in add if l not in have]
        if new:
            text = readme.read_text() if readme.exists() else ""
            readme.write_text(text + ("" if text.endswith("\n") or not text else "\n") + "\n".join(new) + "\n")
            rep.append(f"README: {len(new)} line(s) added")
    return rc, out, rep, meta


def run_of(text):
    """the runner's output -> (run id, machine) of the attempt that got a slot ("" when none did: each busy try prints an id too)"""
    got = re.search(r"^chain_slot: got a slot on (\S+)", text, re.M)
    if not got:
        return "", ""
    ids = ID_RE.findall(text[:got.start()])
    return (ids[-1] if ids else ""), got.group(1)


def transport_failed(rc, text):
    """the runner's last line when it failed (rc 2) before the slot's run started (ssh / WSL interop hiccups under load: a retry
    helps); "" when the command ran or the failure is another kind"""
    if rc != 2 or re.search(r"^lp_route: [0-9:]+ phase ", text, re.M):   # the slot's run started: its own failure
        return ""
    m = re.findall(r"^lp_route: (.*)$", text, re.M)
    return m[-1][:100] if m else "no output"


def replace_names(argv):
    return {argv[i + 1] for i, a in enumerate(argv[:-1]) if a == "--replace"} | \
        {a.split("=", 1)[1] for a in argv if a.startswith("--replace=")}


def main(argv, root, saves, work, machine):
    runner = os.environ.get("BWPORT_ROUTE_RUNNER", "")
    if not runner or not Path(runner).exists():
        print("route --remote: $BWPORT_ROUTE_RUNNER (the slot runner script) is not set or missing")
        return 2
    t0 = time.time()
    for s in dirty_submodules(root):
        print(f"route --remote: {s} has changes inside it: NOT shipped (only its recorded commit is)")
    sha, changed = snapshot(root)
    ref = f"refs/route-remote/{os.getpid()}"
    git(root, "update-ref", ref, sha)
    print(f"route --remote: {sha[:9]} = HEAD" + (f" + {len(changed)} changed file(s)" if changed else ""), flush=True)
    wait = int(os.environ.get("ROUTE_REMOTE_WAIT", "900"))
    with tempfile.TemporaryDirectory() as td:
        cmd = remote_script(argv)
        args = ["bash", runner, *slot_args(game_of(argv), machine, sha, wait), "--cmd", cmd]
        log = Path(td) / "runner.log"
        for attempt in range(TRIES):
            with open(log, "w") as f:
                p = subprocess.run(args, cwd=str(Path(runner).parent), stdout=f, stderr=subprocess.STDOUT, stdin=subprocess.DEVNULL)
            text = log.read_text(errors="replace")
            if not transport_failed(p.returncode, text) or attempt == TRIES - 1:
                break
            print(f"route --remote: the runner failed before the command started ({transport_failed(p.returncode, text)}): again",
                  flush=True)
            time.sleep(10)
        git(root, "update-ref", "-d", ref, check=False)
        rid, on = run_of(text)
        timing = re.findall(r"^run \S+ slot \S+ commit \S+: (.*)$", text, re.M)
        if not rid:
            tail = [l for l in text.splitlines() if l.strip()][-6:]
            print("route --remote: no slot run (runner rc=%d):\n  " % p.returncode + "\n  ".join(tail))
            return 3 if p.returncode == 3 else 2
        ret = Path(td) / RET
        with open(ret, "wb") as f:
            subprocess.run(["bash", runner, "--fetch", rid, RET], cwd=str(Path(runner).parent), stdout=f,
                           stderr=subprocess.DEVNULL, stdin=subprocess.DEVNULL)
        if ret.stat().st_size == 0:
            tail = [l for l in text.splitlines() if l.strip()][-12:]
            print(f"route --remote: run {rid} on {on} brought nothing back (runner rc={p.returncode}):\n  " +
                  "\n  ".join(tail))
            return 2
        rc, out, rep, meta = unpack(ret, root, work, saves, replace_names(argv))
    sys.stdout.write(out if out.endswith("\n") or not out else out + "\n")
    for l in rep:
        print("route --remote: " + l)
    cc = re.sub(r"^.*?clang version (\d+[.\d]*).*$", r"clang \1", meta.get("cc", "")) or "?"
    print(f"route --remote: ran on {on} ({meta.get('host', '?')}, {cc}) as {rid}: "
          f"{time.time() - t0:.0f} s here, route.py {meta.get('route_s', '?')} s there"
          + (f" ({timing[0].split(', command')[0]})" if timing else ""))
    return rc
