"""How many processes a tool may run at once: BW_JOBS, else the cores free in the worker pool's CPU quota.

    import jobs
    n = jobs.jobs()          # BW_JOBS if it is set; else the quota's cores not in use now (at least 4, at most the quota)
    jobs.fork_map(fn, items)  # [fn(x)...] in order, from forked processes (a pool of threads comes to ~1 core)
    wait = jobs.fork_call(fn)  # fn() in a forked process meanwhile; wait() has its result
    with jobs.LivePool(n) as pool: pool.map(fn, xs)   # n processes, or the share now under BW_JOBS_LIVE (a sweep)

The workers' commands run in one systemd slice (bwworkers.slice, ~/bw) with a CPU quota of 24-26 cores; the quota is
read from the cgroup the process is in (or the nearest parent with one), the cores in use from the usage that
cgroup's cpu.stat counted over a quarter of a second. Outside a quota the CPU's threads less two are the default.

A tool that forks BW_JOBS processes also checks the memory it has (local_driver.available_mb()); without BW_JOBS the
free cores are also held to the quota's memory left over (150 MB a process): 19 workers asking at once for 24 each
would not fit.
"""
import os
import time
from pathlib import Path

CGROUP = Path("/sys/fs/cgroup")
FLOOR = 4  # a pool this busy still gets this many: the kernel shares the quota out between the runnable processes
_QUOTA = {}
_LAST = {}  # the last sampling of the free cores
MEM_MB = 150  # memory a process of a pool of compiles takes, with its compiler
STALE = 30  # seconds before the free cores are sampled again


def _cgroups():
    """The cgroup directories of this process, innermost first."""
    try:
        rel = next(l.split("::", 1)[1] for l in Path("/proc/self/cgroup").read_text().splitlines() if l.startswith("0::"))
    except (OSError, StopIteration):
        return []
    parts = rel.strip("/").split("/")
    return [CGROUP.joinpath(*parts[:i]) for i in range(len(parts), 0, -1)]


def quota():
    """(cores of the nearest CPU quota, the cgroup directory that has it), or (None, None)."""
    if not _QUOTA:
        for d in _cgroups():
            try:
                limit, period = (d / "cpu.max").read_text().split()
            except (OSError, ValueError):
                continue
            if limit != "max":
                _QUOTA["q"] = (int(limit) / int(period), d)
                break
        else:
            _QUOTA["q"] = (None, None)
    return _QUOTA["q"]


def usage(d):
    """Microseconds of CPU the cgroup (and the ones below it) has used, or None."""
    try:
        return next(int(l.split()[1]) for l in (d / "cpu.stat").read_text().splitlines() if l.startswith("usage_usec"))
    except (OSError, StopIteration):
        return None


def free_cores(window=0.25):
    """The quota's cores not in use over the last `window` seconds, or None when there is no quota to read."""
    cores, d = quota()
    first = usage(d) if d else None
    if first is None:
        return None
    t = time.monotonic()
    time.sleep(window)
    second = usage(d)
    if second is None:
        return None
    return cores - (second - first) / 1e6 / (time.monotonic() - t)


def headroom_mb(d=None):
    """Megabytes the quota's cgroup can still use (its memory.max less memory.current), or None without a limit."""
    d = d or quota()[1] or (_cgroups() or [None])[0]
    try:
        limit = (d / "memory.max").read_text().strip()
        return None if limit == "max" else (int(limit) - int((d / "memory.current").read_text())) // 2**20
    except (OSError, TypeError, ValueError):
        return None


def jobs(cap=None):
    """BW_JOBS when it is set (a number above 0), else the free cores of the quota (FLOOR to the quota), else the CPU's
    threads less two; at most cap."""
    try:
        n = int(os.environ.get("BW_JOBS") or 0)
    except ValueError:
        n = 0
    if n <= 0:
        if time.monotonic() - _LAST.get("at", -STALE) > STALE:  # (a run asks for it again and again: sampled once in a while)
            free = free_cores()
            cores = quota()[0]
            n = max(FLOOR, int(free)) if free is not None and cores else max(1, (os.cpu_count() or 4) - 2)
            n = min(n, int(cores)) if cores else n
            room = headroom_mb()  # a process of a pool takes up to MEM_MB: not more of them than the memory holds
            _LAST.update(at=time.monotonic(), n=max(FLOOR, min(n, room // MEM_MB)) if room is not None else n)
        n = _LAST["n"]
    return max(1, min(n, cap) if cap else n)


_FORKED = {}


def _forked_chunk(chunk):
    fn = _FORKED["fn"]
    return [fn(x) for x in chunk]


def fork_map(fn, items, n=None):
    """[fn(x) for x in items], in order, from n processes forked from this one (n: jobs()): what fn reads is shared by
    the fork and needs no copying, only the results are pickled. Python runs one thread code at a time, so a pool of
    threads never comes to more than a core. Falls back to a plain loop for one process or a few items; a pool that
    breaks (a process killed) finishes in this one."""
    import multiprocessing
    from concurrent.futures import ProcessPoolExecutor
    from concurrent.futures.process import BrokenProcessPool
    items = list(items)
    n = n or jobs()
    if n < 2 or len(items) < 4:
        return [fn(x) for x in items]
    size = max(1, min(64, len(items) // (n * 4)))
    chunks = [items[i:i + size] for i in range(0, len(items), size)]
    _FORKED["fn"] = fn
    out = []
    try:
        with ProcessPoolExecutor(min(n, len(chunks)), mp_context=multiprocessing.get_context("fork")) as pool:
            for part in pool.map(_forked_chunk, chunks):
                out.extend(part)
    except BrokenProcessPool:
        out = [fn(x) for x in items]
    return out


def fork_call(fn):
    """fn() run in a forked process meanwhile; returns a function that waits for it and gives its result (or raises
    what it raised). For two slow independent pieces of a tool that would run one after the other."""
    import os
    import pickle
    import tempfile
    path = tempfile.mkstemp(prefix="bwfork")[1]
    pid = os.fork()
    if not pid:
        code = 1
        try:
            try:
                result = (True, fn())
            except BaseException as e:  # noqa: BLE001
                result = (False, f"{type(e).__name__}: {e}")
            with open(path, "wb") as f:
                pickle.dump(result, f)
            code = 0
        finally:
            os._exit(code)

    def wait():
        os.waitpid(pid, 0)
        try:
            with open(path, "rb") as f:
                ok, value = pickle.load(f)
        except (OSError, EOFError, pickle.UnpicklingError):
            ok, value = False, "the process died"
        finally:
            os.unlink(path)
        if not ok:
            raise RuntimeError(value)
        return value
    return wait


def live_share(default):
    """The processes a search may run now when BW_JOBS_LIVE names a directory with a file for each worker sharing the
    pool (tools/sweep_split.py's workers still going): the pool (BW_POOL, else the quota's cores) split among them, at
    least 2; else default."""
    d = os.environ.get("BW_JOBS_LIVE")
    if not d:
        return default
    try:
        k = len(os.listdir(d))
    except OSError:
        return default
    pool = int(os.environ.get("BW_POOL") or 0) or int(quota()[0] or 0) or (os.cpu_count() or 4)
    return max(2, pool // max(1, k))


class LivePool:
    """A pool of n forked processes for map() (a list, in order), as ProcessPoolExecutor(n).map is. With BW_JOBS_LIVE
    (live_share()) the tasks in flight follow the share now, and the pool grows between tasks as the other workers
    end: a share fixed when a search began held a sweep's four dearest searches to 2 processes each for its last three
    minutes while the cores the others gave back stayed idle (tools/sweep_split.py, 10-19). room(): the processes the
    memory holds, a cap on the growth."""

    def __init__(self, n, room=None):
        self.n, self.room, self.pools = max(1, n), room, []

    def __enter__(self):
        self.pools.append(self._new(self.n))
        return self

    def __exit__(self, *exc):
        for p in self.pools:
            p.shutdown(wait=True)
        return False

    @staticmethod
    def _new(n):
        import multiprocessing
        from concurrent.futures import ProcessPoolExecutor
        return ProcessPoolExecutor(n, mp_context=multiprocessing.get_context("fork"))

    def map(self, fn, *iterables):
        if not os.environ.get("BW_JOBS_LIVE"):
            return list(self.pools[-1].map(fn, *iterables))
        from concurrent.futures import FIRST_COMPLETED, wait
        args = list(zip(*iterables))
        out, pending, i = [None] * len(args), {}, 0
        while i < len(args) or pending:
            want = live_share(self.n)
            if want > self.n and self.room:
                want = max(self.n, min(want, self.room()))
            if want > self.n:  # a bigger pool for the tasks from here; the old one's finish meanwhile
                self.pools.append(self._new(want))
                self.n = want
            while i < len(args) and len(pending) < want:
                pending[self.pools[-1].submit(fn, *args[i])] = i
                i += 1
            done, _ = wait(pending, return_when=FIRST_COMPLETED)
            for f in done:
                out[pending.pop(f)] = f.result()
        for p in self.pools[:-1]:  # idle now
            p.shutdown(wait=True)
        del self.pools[:-1]
        return out
