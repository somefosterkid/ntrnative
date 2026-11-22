#!/usr/bin/env python3
"""port_sysroot.py: one pinned Ubuntu 24.04 sysroot for the Linux port build, the same on every build machine (arch1 C1).

A Linux port exe built on Ubuntu 26.04 (glibc 2.43, g++ 15 headers) differs from one built on 24.04 (glibc 2.39, g++ 13), and
so does every object: ccache can't share them and an exe from one machine is not the other's. With the sysroot, clang and
clang++ read the headers and link the libraries of one 24.04 package set whatever the host runs (`--sysroot`: libc, the kernel
headers, libstdc++ 13, SDL2, ENet). The C++ compiler becomes clang++ there (g++ 15 would keep its own libstdc++ headers).
An exe linked against glibc 2.39 runs on 2.43 (not the other way round).

  port_sysroot.py make TAR          on a 24.04 host with the packages installed: a deterministic tar of their files; prints the sha256
  port_sysroot.py install TAR DIR WHEEL
                                    checks TAR's and WHEEL's sha256 against port/machine/sysroot.txt, unpacks TAR to DIR (one fixed
                                    path on every machine: .pch files name it) and the meson wheel to DIR/meson (run as DIR/bin/meson),
                                    writes DIR/clang.cfg (--sysroot=DIR), DIR/native.ini and DIR/native-ccache.ini
  port_sysroot.py check DIR         exit 0 when DIR is an installed sysroot of the pinned tar and meson, and clang is the pinned version

Use: BWPORT_SYSROOT=DIR python3 tools/port_run.py ... (a NEW build dir: meson keeps a dir's compilers; tools/port_run.py setup_args
also puts DIR/bin first in PATH: the pinned meson).
Besides the headers and libraries, two things made one source's exe differ between the machines (11-21, measured on 1591 objects):
- meson: 26.04's 1.10 adds -D_GLIBCXX_ASSERTIONS=1 to every C++ compile of a debug build, 24.04's 1.3 doesn't (9 C++ objects, and the
  exe's dynamic symbols: libstdc++'s std::string members). The sysroot brings one meson (the PyPI wheel pinned in sysroot.txt).
- clang's build name: Ubuntu's "21.1.8 (6ubuntu1)" and apt.llvm.org's "21.1.8 (++2025...)" compile the same code, but
  __clang_version__ and __VERSION__ name the build (imgui_demo.cpp prints it: +48 bytes of .rodata, every address after it moved).
  The native file sets both to the pinned version.
"""
import hashlib
import io
import os
import re
import subprocess
import sys
import tarfile
import zipfile

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PIN = os.path.join(REPO, 'port', 'machine', 'sysroot.txt')
# what the port's compiles and its link read (ldd of build/port/linux/bwport: SDL2, ENet, libstdc++, libm, libgcc_s, libc)
PACKAGES = ['libc6', 'libc6-dev', 'linux-libc-dev', 'libcrypt-dev', 'libcrypt1', 'libgcc-13-dev', 'libstdc++-13-dev',
            'libgcc-s1', 'libstdc++6', 'libsdl2-dev', 'libsdl2-2.0-0', 'libenet-dev', 'libenet7',
            'libx11-dev', 'x11proto-dev']   # SDL_syswm.h includes X11/Xlib.h
SKIP = ('/usr/share/', '/usr/bin/', '/usr/sbin/', '/sbin/', '/bin/', '/etc/', '/var/', '/usr/libexec/')


def pinned():
    """{'sha256': ..., 'packages': [...]} from port/machine/sysroot.txt"""
    out = {'packages': []}
    with open(PIN) as f:
        for line in f:
            line = line.split('#')[0].strip()
            if line.startswith('sha256 '):
                out['sha256'] = line.split()[1]
            elif line.startswith('package '):
                out['packages'].append(line.split(None, 1)[1])
            elif line.startswith('meson '):     # meson VERSION sha256 HASH (the PyPI wheel meson-VERSION-py3-none-any.whl)
                out['meson'] = line.split()[1]
                out['meson_sha256'] = line.split()[3]
            elif line.startswith('clang '):
                out['clang'] = line.split()[1]
    return out


def keep(path):
    """a package file the sysroot needs (headers, libraries, crt objects; no docs, programs or config)"""
    return not path.startswith(SKIP)


def package_files(pkgs):
    """(sorted file paths, ['pkg version', ...]) of the installed packages"""
    files, vers = set(), []
    for p in pkgs:
        r = subprocess.run(['dpkg-query', '-W', '-f=${Package} ${Version}', p + ':amd64'], capture_output=True, text=True)
        q = p + ':amd64' if r.returncode == 0 else p   # x11proto-dev is Architecture: all
        vers.append(subprocess.check_output(['dpkg-query', '-W', '-f=${Package} ${Version}', q], text=True))
        for line in subprocess.check_output(['dpkg', '-L', q], text=True).splitlines():
            if line.startswith('/') and keep(line) and (os.path.islink(line) or os.path.isfile(line)):
                files.add(line)
    return sorted(files), vers


def tar_bytes(files, root='/'):
    """a tar of files (absolute paths under root) with fixed owner and times: the same files give the same bytes; absolute
    symlinks made relative"""
    buf = io.BytesIO()
    with tarfile.open(fileobj=buf, mode='w', format=tarfile.GNU_FORMAT) as t:
        for p in files:
            ti = t.gettarinfo(os.path.join(root, p.lstrip('/')), arcname=p.lstrip('/'))
            ti.uid = ti.gid = 0
            ti.uname = ti.gname = ''
            ti.mtime = 0
            ti.mode = 0o755 if ti.mode & 0o111 else 0o644
            if ti.issym() and ti.linkname.startswith('/'):   # into the sysroot, not the host
                ti.linkname = os.path.relpath(ti.linkname, os.path.dirname(p))
            if ti.isreg():
                with open(os.path.join(root, p.lstrip('/')), 'rb') as f:
                    t.addfile(ti, f)
            else:
                t.addfile(ti)
        for n in ('lib', 'lib64'):   # merged /usr: the linker scripts (libc.so) name /lib/..., /lib64/...
            if not any(p.startswith(f'/{n}/') for p in files):
                ti = tarfile.TarInfo(n)
                ti.type, ti.linkname, ti.mode = tarfile.SYMTYPE, f'usr/{n}', 0o777
                t.addfile(ti)
    return buf.getvalue()


CFG = 'clang.cfg'


def native_ini(d, ccache, clang):
    """the meson native file (after port/machine/linux.ini and linux-ccache.ini): the compilers with --sysroot. In [binaries],
    not c_args: meson's find_library (SDL2, enet) asks the compiler itself for its search dirs. -fdebug-prefix-map: the debug info
    names /sysroot, and ccache hashes the flag. A cached .pch names the sysroot dir it was made in (11-21: one made against an
    older, since deleted sysroot dir was served to a build in another dir and failed "could not find file"): keep the dir's path
    fixed on every machine, and change this flag when a .pch must not come from older cache entries. __clang_version__ and
    __VERSION__: the pinned version instead of the build's name (the module doc).
    --config=DIR/clang.cfg (which holds --sysroot=DIR), not --sysroot=DIR: ccache's base_dir makes every absolute path argument under
    it relative to the compile's cwd, so clang got --sysroot=../../../../sysroot and named the headers that way in __FILE__, the
    assert messages and C++ type names: 48 bytes of .rodata that differ with the checkout's depth (a worktree vs a build slot).
    ccache leaves the config's content alone. -ffile-prefix-map (debug info and __FILE__/__PRETTY_FUNCTION__): the headers are /sysroot/..."""
    cc = "'ccache', " if ccache else ''
    sr = (f"'--config={d}/{CFG}', '-ffile-prefix-map={d}=/sysroot', '-Xclang', '-fno-pch-timestamp', '-Xclang', '-fno-validate-pch', "
          "'-Xclang', '-fmodules-embed-all-files', "
          f"'-U__clang_version__', '-D__clang_version__=\"{clang}\"', '-U__VERSION__', '-D__VERSION__=\"Clang {clang}\"'")
    return (f"# port_sysroot.py install: the Linux port build against the pinned sysroot {d}\n[binaries]\n"
            f"c = [{cc}'clang', {sr}]\ncpp = [{cc}'clang++', {sr}]\n")


MESON_MAIN = '''#!/usr/bin/env python3
# port_sysroot.py install: the pinned meson (the wheel in ../meson)
import os, sys
sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'meson'))
from mesonbuild.mesonmain import main
sys.exit(main())
'''


def install(tar, d, pin, wheel):
    data = open(tar, 'rb').read()
    got = hashlib.sha256(data).hexdigest()
    if got != pin['sha256']:
        sys.exit(f'port_sysroot: {tar} has sha256 {got}, port/machine/sysroot.txt pins {pin["sha256"]}')
    wgot = hashlib.sha256(open(wheel, 'rb').read()).hexdigest()
    if wgot != pin['meson_sha256']:
        sys.exit(f'port_sysroot: {wheel} has sha256 {wgot}, port/machine/sysroot.txt pins meson {pin["meson_sha256"]}')
    os.makedirs(d, exist_ok=True)
    with tarfile.open(fileobj=io.BytesIO(data)) as t:
        t.extractall(d, filter='data') if hasattr(tarfile, 'data_filter') else t.extractall(d)
    with zipfile.ZipFile(wheel) as z:
        z.extractall(os.path.join(d, 'meson'))
    os.makedirs(os.path.join(d, 'bin'), exist_ok=True)
    with open(os.path.join(d, 'bin', 'meson'), 'w') as f:
        f.write(MESON_MAIN)
    os.chmod(os.path.join(d, 'bin', 'meson'), 0o755)
    with open(os.path.join(d, CFG), 'w') as f:
        f.write(f'--sysroot={d}\n')
    for name, cc in (('native.ini', False), ('native-ccache.ini', True)):
        with open(os.path.join(d, name), 'w') as f:
            f.write(native_ini(d, cc, pin['clang']))
    with open(os.path.join(d, 'SYSROOT_SHA256'), 'w') as f:
        f.write(got + '\n')
    print(f'port_sysroot: {d} = {got}, meson {pin["meson"]}')


def check(d, pin):
    """the problems of DIR as the pinned sysroot (empty: none)"""
    bad = []
    try:
        have = open(os.path.join(d, 'SYSROOT_SHA256')).read().strip()
    except OSError:
        have = ''
    if have != pin['sha256']:
        bad.append(f'{d} is not the pinned sysroot ({have or "not installed"})')
    try:
        mv = subprocess.run([os.path.join(d, 'bin', 'meson'), '--version'], capture_output=True, text=True).stdout.strip()
    except OSError:
        mv = ''
    if mv != pin['meson']:
        bad.append(f'{d}/bin/meson is {mv or "missing"}, the pin {pin["meson"]}')
    try:
        cv = subprocess.run(['clang', '--version'], capture_output=True, text=True).stdout
    except OSError:
        cv = ''
    m = re.search(r'clang version (\S+)', cv)
    if not m or m.group(1) != pin['clang']:
        bad.append(f'clang is {m.group(1) if m else "missing"}, the pin {pin["clang"]}')
    return bad


def main(argv):
    if len(argv) >= 2 and argv[0] == 'make':
        files, vers = package_files(PACKAGES)
        data = tar_bytes(files)
        with open(argv[1], 'wb') as f:
            f.write(data)
        print(f'sha256 {hashlib.sha256(data).hexdigest()}  ({len(files)} files, {len(data) >> 20} MB)')
        for v in vers:
            print('package', v)
        return 0
    if len(argv) >= 4 and argv[0] == 'install':
        install(argv[1], os.path.abspath(argv[2]), pinned(), argv[3])
        return 0
    if len(argv) >= 2 and argv[0] == 'check':
        bad = check(argv[1], pinned())
        for b in bad:
            print('port_sysroot:', b)
        return 1 if bad else 0
    print(__doc__)
    return 2


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
