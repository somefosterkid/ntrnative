#!/usr/bin/env python3
"""Where a running process's CPU goes, by thread and function: a sampling profiler with no perf binary.

    python3 tools/perf_sample.py PID [--secs 5] [--hz 1000] [--top 25] [--exe PATH]

Opens a software task-clock sampling counter (perf_event_open through ctypes, PERF_SAMPLE_IP | PERF_SAMPLE_TID, user space)
on each thread of PID, reads the ring buffers for SECS seconds, then names each sample's function from the exe's symbol
table (nm; a PIE's load base from /proc/PID/maps; addresses in shared libraries are named by the library) and prints, per
thread name (/proc comm), the samples' share and ms a second by function. Threads that start after the sampler are not
counted. Needs perf_event_paranoid <= 2 (WSL's default)."""
import argparse, bisect, collections, ctypes, mmap, os, struct, subprocess, sys, time

LIBC = ctypes.CDLL(None, use_errno=True)
SYS_PERF_EVENT_OPEN = 298
PERF_TYPE_SOFTWARE, PERF_COUNT_SW_TASK_CLOCK = 1, 1
SAMPLE_IP, SAMPLE_TID = 1, 2
DISABLED, EXCLUDE_KERNEL, EXCLUDE_HV, FREQ = 1 << 0, 1 << 5, 1 << 6, 1 << 10
PERF_RECORD_SAMPLE = 9
IOC_ENABLE = 0x2400


class Attr(ctypes.Structure):
    _fields_ = [('type', ctypes.c_uint32), ('size', ctypes.c_uint32), ('config', ctypes.c_uint64),
                ('sample_period', ctypes.c_uint64), ('sample_type', ctypes.c_uint64), ('read_format', ctypes.c_uint64),
                ('flags', ctypes.c_uint64), ('wakeup_events', ctypes.c_uint32), ('bp_type', ctypes.c_uint32),
                ('config1', ctypes.c_uint64), ('config2', ctypes.c_uint64), ('pad', ctypes.c_uint64 * 8)]


def open_sampler(tid, hz):
    a = Attr()
    a.type, a.size, a.config = PERF_TYPE_SOFTWARE, 112, PERF_COUNT_SW_TASK_CLOCK
    a.sample_period = hz
    a.sample_type = SAMPLE_IP | SAMPLE_TID
    a.flags = DISABLED | EXCLUDE_KERNEL | EXCLUDE_HV | FREQ
    a.wakeup_events = 0
    fd = LIBC.syscall(SYS_PERF_EVENT_OPEN, ctypes.byref(a), tid, -1, -1, 0)
    if fd < 0:
        return None
    return fd


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('pid', type=int)
    ap.add_argument('--secs', type=float, default=5.0)
    ap.add_argument('--hz', type=int, default=1000)
    ap.add_argument('--top', type=int, default=25)
    ap.add_argument('--exe')
    a = ap.parse_args()
    pid = a.pid
    exe = a.exe or os.readlink('/proc/%d/exe' % pid)
    pages = 1 + 256  # header page + 1 MB ring
    psz = mmap.PAGESIZE
    rings = {}
    for t in os.listdir('/proc/%d/task' % pid):
        tid = int(t)
        fd = open_sampler(tid, a.hz)
        if fd is None:
            continue
        try:
            m = mmap.mmap(fd, pages * psz, mmap.MAP_SHARED, mmap.PROT_READ | mmap.PROT_WRITE)
        except OSError:
            os.close(fd)
            continue
        try:
            comm = open('/proc/%d/task/%d/comm' % (pid, tid)).read().strip()
        except OSError:
            comm = '?'
        rings[tid] = [fd, m, 0, comm]
    if not rings:
        sys.exit('perf_sample: no thread could be sampled (perf_event_paranoid?)')
    import fcntl
    for r in rings.values():
        fcntl.ioctl(r[0], IOC_ENABLE, 0)
    samples = collections.defaultdict(collections.Counter)  # comm -> ip -> n
    data_size = (pages - 1) * psz

    def drain():
        for tid, r in rings.items():
            fd, m, tail, comm = r
            head = struct.unpack_from('<Q', m, 1024)[0]
            while tail < head:
                off = psz + tail % data_size
                hdr = bytes(m[off:off + 8]) if off + 8 <= pages * psz else None
                if hdr is None:
                    break
                typ, misc, size = struct.unpack('<IHH', hdr)
                if size == 0:
                    break
                rec = bytearray(size)
                for i in range(size):
                    rec[i] = m[psz + (tail + i) % data_size]
                if typ == PERF_RECORD_SAMPLE and size >= 24:
                    ip, = struct.unpack_from('<Q', rec, 8)
                    samples[comm][ip] += 1
                tail += size
            struct.pack_into('<Q', m, 1032, tail)
            r[2] = tail

    t_end = time.time() + a.secs
    while time.time() < t_end:
        time.sleep(0.02)
        drain()
    drain()
    # symbols
    maps = []
    for line in open('/proc/%d/maps' % pid):
        p = line.split()
        if len(p) >= 6 and 'x' in p[1]:
            lo, hi = (int(x, 16) for x in p[0].split('-'))
            maps.append((lo, hi, int(p[2], 16), p[5]))
    base = None
    for line in open('/proc/%d/maps' % pid):
        p = line.split()
        if len(p) >= 6 and p[5] == exe and int(p[2], 16) == 0:
            base = int(p[0].split('-')[0], 16)
            break
    nm = subprocess.run(['nm', '--defined-only', exe], capture_output=True, text=True).stdout.split('\n')
    syms = []
    for l in nm:
        p = l.split()
        if len(p) == 3 and p[1] in 'tTwW':
            syms.append((int(p[0], 16), p[2]))
    syms.sort()
    addrs = [s[0] for s in syms]

    def name(ip):
        for lo, hi, off, path in maps:
            if lo <= ip < hi:
                if path == exe and base is not None:
                    rel = ip - base
                    i = bisect.bisect_right(addrs, rel) - 1
                    return syms[i][1] if i >= 0 else '?exe'
                return os.path.basename(path)
        return '?'

    total = sum(sum(c.values()) for c in samples.values())
    print('perf_sample: pid %d, %.1f s, %d samples at %d Hz (task clock)' % (pid, a.secs, total, a.hz))
    for comm, c in sorted(samples.items(), key=lambda kv: -sum(kv[1].values())):
        n = sum(c.values())
        byf = collections.Counter()
        for ip, k in c.items():
            byf[name(ip)] += k
        print('== %s: %d samples, %.1f ms/s' % (comm, n, n * 1000.0 / a.hz / a.secs))
        for f, k in byf.most_common(a.top):
            print('  %5.1f%%  %6.2f ms/s  %s' % (100.0 * k / n, k * 1000.0 / a.hz / a.secs, f))


if __name__ == '__main__':
    main()
