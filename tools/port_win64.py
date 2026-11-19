#!/usr/bin/env python3
"""bwport.exe built on Windows itself, without WSL (docs/port-build.md "win64 on Windows"; tools/port_win64.sh cross-builds
the same exe from WSL or Linux). From the repo root, in any Windows shell with Git and a Python 3 (python.org's):

    python tools/port_win64.py fetch              the toolchain into the tools dir, once (~2 GB unpacked): WinLibs' MinGW-w64
                                                  (GCC 13.2 posix + LLVM/clang 18.1.3 + MinGW-w64 11.0.1, MSVCRT: the versions of
                                                  Ubuntu 24.04's clang and mingw-w64 that port_win64.sh uses), meson + ninja in a
                                                  venv, WinFlexBison (libntr's makelcf), SDL2 and enet for mingw (enet compiled here),
                                                  Mesa's x64 DLLs (with 7-Zip installed); with --gen4 too MSYS2's libpng, zlib and pkgconf (pret's tools)
    python tools/port_win64.py [build] [--game G] build/port/win64[-plat|-hgss]/bwport.exe (meson setup through tools/port_wraps.py
                                                  with port/machine/win64-native.ini, then ninja bwport.exe)
    python tools/port_win64.py deploy --out DIR   build, then the exe, its DLLs, mesa\\, licenses\\ and bwport.bat into DIR (no ROM)

  --tools DIR  the toolchain's folder (default build/win64-tools; env BWPORT_WIN_TOOLS)
  --game G     platinum, heartgold or soulsilver: that module's build dir, as port_win64.sh --game
  -j N         ninja's jobs;  -D...  more meson setup options (a NEW build dir only, as port_wraps.py takes them)
Without --game it builds Black and White (-Dplatinum=false -Dhgss=false -Db2w2=disabled; -Db2w2=auto tries Black 2 and
White 2, untested here): the Generation IV modules need pret's header generators, which do not build with MinGW yet
(docs/port-build.md "win64 on Windows").
"""
import argparse
import hashlib
import io
import os
import shutil
import subprocess
import sys
import tarfile
import urllib.request
import zipfile

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# (url, sha256, the folder under the tools dir it unpacks to)
WINLIBS = ('https://github.com/brechtsanders/winlibs_mingw/releases/download/13.2.0posix-18.1.3-11.0.1-msvcrt-r7/'
           'winlibs-x86_64-posix-seh-gcc-13.2.0-llvm-18.1.3-mingw-w64msvcrt-11.0.1-r7.zip',
           '5ec70773a808c561a1c0b08dfbf60f0111d985cdf942da0651a3cb299ba8de9f', 'winlibs')
FLEXBISON = ('https://github.com/lexxmark/winflexbison/releases/download/v2.5.25/win_flex_bison-2.5.25.zip',
             '8d324b62be33604b2c45ad1dd34ab93d722534448f55a16ca7292de32b6ac135', 'flexbison')
# docs/libntr/build.md "win64" (the same files and sums)
SDL2 = ('https://github.com/libsdl-org/SDL/releases/download/release-2.32.10/SDL2-devel-2.32.10-mingw.tar.gz',
        '83a5d74012311edc3c0d40ea6faecbe57ad692aa033fa5dc273cc937e3938ff2')
ENET = ('http://enet.bespin.org/download/enet-1.3.18.tar.gz',
        '2a8a0c5360d68bb4fcd11f2e4c47c69976e8d2c85b109dd7d60b1181a4f85d36')
MESA = ('https://github.com/pal1000/mesa-dist-win/releases/download/26.2.4/mesa3d-26.2.4-release-mingw.7z',
        '36a30a04d8cab6e2bea2d3c747c326d15024d9711dd80f45ef97ac6f2c3c5086')
MSYS2 = [('https://repo.msys2.org/mingw/mingw64/mingw-w64-x86_64-libpng-1.6.59-2-any.pkg.tar.zst',
          '8cf4e17ed9f592d23a5d1f1f8875c6a8da42eaf7772a997908565e94131cfc20'),
         ('https://repo.msys2.org/mingw/mingw64/mingw-w64-x86_64-zlib-1.3.2-3-any.pkg.tar.zst',
          'ffde6f4ed2d879477c93b37ff14c319baf480a2aeafc065edeceafba26b71e90'),
         ('https://repo.msys2.org/mingw/mingw64/mingw-w64-x86_64-pkgconf-1~3.0.9-1-any.pkg.tar.zst',
          '493f658b5bf66df7d7be4e47b5e14ef360029dd111ad329b8247018ff513d455')]
PIP = ['meson==1.12.1', 'ninja==1.13.2', 'zstandard==0.25.0']


def say(msg):
    print('port_win64: ' + msg, flush=True)


def get(tools, url, sha):
    """the file from url in tools/dl (kept), checked against sha"""
    dl = os.path.join(tools, 'dl')
    os.makedirs(dl, exist_ok=True)
    path = os.path.join(dl, url.rsplit('/', 1)[1])
    if not os.path.exists(path):
        say('download ' + url)
        with urllib.request.urlopen(url) as r, open(path + '.part', 'wb') as f:
            shutil.copyfileobj(r, f)
        os.replace(path + '.part', path)
    h = hashlib.sha256(open(path, 'rb').read()).hexdigest()
    if h != sha:
        sys.exit(f'port_win64: {path}: sha256 {h}, expected {sha}')
    return path


def paths(tools):
    """the dirs the build puts first in PATH: the compilers, meson and ninja, flex and bison, Git's sh/cp/true"""
    p = [os.path.join(tools, 'winlibs', 'mingw64', 'bin'), os.path.join(tools, 'venv', 'Scripts'),
         os.path.join(tools, 'flexbison'), os.path.join(tools, 'msys', 'mingw64', 'bin')]
    git = shutil.which('git')
    if git and not shutil.which('sh'):
        # Git for Windows: <Git>/cmd/git.exe, its POSIX tools in <Git>/usr/bin (port/meson.build runs sh for Platinum's AI table)
        p.append(os.path.join(os.path.dirname(os.path.dirname(git)), 'usr', 'bin'))
    return p


def env_for(tools):
    env = dict(os.environ)
    env['PATH'] = os.pathsep.join(paths(tools) + [env.get('PATH', '')])
    env['PKG_CONFIG_PATH'] = os.path.join(tools, 'msys', 'mingw64', 'lib', 'pkgconfig').replace('\\', '/')
    return env


def fetch(tools, gen4):
    os.makedirs(tools, exist_ok=True)
    for url, sha, sub in (WINLIBS, FLEXBISON):
        dst = os.path.join(tools, sub)
        if not os.path.isdir(dst):
            z = get(tools, url, sha)
            say('unpack ' + os.path.basename(z))
            zipfile.ZipFile(z).extractall(dst + '.part')
            os.replace(dst + '.part', dst)
    fb = os.path.join(tools, 'flexbison')
    for a, b in (('win_flex.exe', 'flex.exe'), ('win_bison.exe', 'bison.exe')):   # meson looks for flex and bison
        if not os.path.exists(os.path.join(fb, b)):
            shutil.copy(os.path.join(fb, a), os.path.join(fb, b))
    venv = os.path.join(tools, 'venv')
    py = os.path.join(venv, 'Scripts', 'python.exe')
    if not os.path.exists(py):
        subprocess.check_call([sys.executable, '-m', 'venv', venv])
    subprocess.check_call([py, '-m', 'pip', 'install', '-q', '--disable-pip-version-check'] + PIP)
    env = env_for(tools)
    mingw = os.path.join(tools, 'deps', 'mingw')
    if not os.path.exists(os.path.join(mingw, 'bin', 'SDL2.dll')):
        with tarfile.open(get(tools, *SDL2)) as t:
            pre = 'SDL2-2.32.10/x86_64-w64-mingw32/'
            for m in t.getmembers():
                if m.name.startswith(pre) and len(m.name) > len(pre) and not m.issym():
                    m.name = m.name[len(pre):]
                    t.extract(m, mingw, filter='data')
    if not os.path.exists(os.path.join(mingw, 'bin', 'enet.dll')):
        src = os.path.join(tools, 'dl', 'enet')
        with tarfile.open(get(tools, *ENET)) as t:
            t.extractall(src, filter='data')
        e = os.path.join(src, 'enet-1.3.18')
        gcc = shutil.which('gcc', path=env['PATH'])
        objs = []
        for f in ('callbacks', 'compress', 'host', 'list', 'packet', 'peer', 'protocol', 'unix', 'win32'):
            objs.append(os.path.join(e, f + '.o'))
            subprocess.check_call([gcc, '-O2', '-I' + os.path.join(e, 'include'), '-c', os.path.join(e, f + '.c'), '-o', objs[-1]],
                                  env=env)
        os.makedirs(os.path.join(mingw, 'lib'), exist_ok=True)
        subprocess.check_call([gcc, '-shared', '-o', os.path.join(mingw, 'bin', 'enet.dll')] + objs
                              + ['-lws2_32', '-lwinmm', '-Wl,--out-implib,' + os.path.join(mingw, 'lib', 'libenet.dll.a')], env=env)
        shutil.copytree(os.path.join(e, 'include', 'enet'), os.path.join(mingw, 'include', 'enet'), dirs_exist_ok=True)
    mesa = os.path.join(tools, 'deps', 'mesa')
    seven = shutil.which('7z') or shutil.which('7za') or shutil.which(os.path.join(os.environ.get('ProgramFiles', 'C:/Program Files'), '7-Zip', '7z.exe'))
    if not os.path.exists(os.path.join(mesa, 'opengl32.dll')):
        # (a 7z with BCJ2, which no pip package reads: 7-Zip's own program; without it the exe uses the system's OpenGL only)
        if seven:
            subprocess.check_call([seven, 'e', '-y', '-o' + mesa, get(tools, *MESA), 'x64/*'], stdout=subprocess.DEVNULL)
        else:
            say('no 7-Zip (7z.exe): no mesa folder (Mesa\'s software OpenGL, for machines without a GL driver)')
    if gen4 and not os.path.exists(os.path.join(tools, 'msys', 'mingw64', 'lib', 'pkgconfig', 'libpng.pc')):
        for url, sha in MSYS2:
            subprocess.check_call([py, '-c', 'import io, sys, tarfile, zstandard; '
                                   'raw = zstandard.ZstdDecompressor().stream_reader(open(sys.argv[1], "rb")).read(); '
                                   'tarfile.open(fileobj=io.BytesIO(raw)).extractall(sys.argv[2], filter="data")',
                                   get(tools, url, sha), os.path.join(tools, 'msys')])
    say('tools ready in ' + tools)


def build_dir(game):
    return os.path.join(REPO, 'build', 'port', {'platinum': 'win64-plat', 'heartgold': 'win64-hgss',
                                                 'soulsilver': 'win64-hgss'}.get(game, 'win64'))


def build(tools, game, jobs, extra):
    if not os.path.exists(os.path.join(tools, 'winlibs', 'mingw64', 'bin', 'clang.exe')):
        sys.exit(f'port_win64: no toolchain in {tools}: python tools/port_win64.py fetch')
    b = build_dir(game)
    os.makedirs(b, exist_ok=True)
    # the constants port/machine/win64-native.ini reads (a machine file's constants reach the files after it)
    consts = os.path.join(b, 'win64-native-consts.ini')
    mingw = os.path.join(tools, 'deps', 'mingw').replace('\\', '/')
    with open(consts, 'w') as f:
        f.write(f"[constants]\nmingw = '{mingw}'\n")
    game_args = {'platinum': ['-Dplatinum=true'], 'heartgold': ['-Dhgss=true'], 'soulsilver': ['-Dhgss=true']}.get(
        game, ['-Dplatinum=false', '-Dhgss=false', '-Db2w2=disabled'])
    env = env_for(tools)
    py = os.path.join(tools, 'venv', 'Scripts', 'python.exe')
    subprocess.check_call([py, os.path.join(REPO, 'tools', 'port_wraps.py'), b, '--native-file', consts, '--native-file',
                           os.path.join(REPO, 'port', 'machine', 'win64-native.ini'), '-Dbuild_target=win64'] + game_args + extra,
                          env=env, cwd=REPO)
    # name the target: a plain ninja stops at libntr's ARM7 component (docs/libntr/build.md, win64)
    ninja = shutil.which('ninja', path=env['PATH'])   # (Windows: CreateProcess looks a bare name up in our PATH, not env's)
    subprocess.check_call([ninja, '-C', b] + (['-j', str(jobs)] if jobs else []) + ['bwport.exe'], env=env, cwd=REPO)
    return os.path.join(b, 'bwport.exe')


def bat_text():
    """bwport.bat as tools/port_win64.sh writes it (its heredoc: one copy of the text), with CRLF line ends"""
    s = open(os.path.join(REPO, 'tools', 'port_win64.sh'), encoding='utf-8').read()
    start = s.index("<<'BAT'\n") + len("<<'BAT'\n")
    return s[start:s.index('\nBAT\n', start) + 1].replace('\n', '\r\n')


def deploy(tools, exe, out):
    os.makedirs(out, exist_ok=True)
    shutil.copy(exe, out)
    for d in (os.path.join(tools, 'deps', 'mingw', 'bin', 'SDL2.dll'), os.path.join(tools, 'deps', 'mingw', 'bin', 'enet.dll'),
              os.path.join(tools, 'winlibs', 'mingw64', 'bin', 'libwinpthread-1.dll')):
        shutil.copy(d, out)
    mesa = os.path.join(tools, 'deps', 'mesa')
    if os.path.exists(os.path.join(mesa, 'opengl32.dll')):   # (port_win64.sh fill_folder: in mesa\, never beside the exe)
        os.makedirs(os.path.join(out, 'mesa'), exist_ok=True)
        for d in ('opengl32.dll', 'libgallium_wgl.dll'):
            shutil.copy(os.path.join(mesa, d), os.path.join(out, 'mesa'))
    lic = os.path.join(out, 'licenses')
    shutil.rmtree(lic, ignore_errors=True)
    os.makedirs(lic)
    for f in os.listdir(os.path.join(REPO, 'licenses')):
        shutil.copy(os.path.join(REPO, 'licenses', f), lic)
    for f in ('THIRD_PARTY.md', 'LICENSE'):
        shutil.copy(os.path.join(REPO, f), lic)
    with open(os.path.join(out, 'bwport.bat'), 'w', newline='') as f:
        f.write(bat_text())
    say(f'{out}: ' + ' '.join(sorted(os.listdir(out))))


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('cmd', nargs='?', default='build', choices=['fetch', 'build', 'deploy'])
    ap.add_argument('--tools', default=os.environ.get('BWPORT_WIN_TOOLS', os.path.join(REPO, 'build', 'win64-tools')))
    ap.add_argument('--game', choices=['platinum', 'heartgold', 'soulsilver'])
    ap.add_argument('--out')
    ap.add_argument('--gen4', action='store_true', help='fetch: the Generation IV generators\' libraries too')
    ap.add_argument('-j', type=int)
    a, extra = ap.parse_known_args()
    if any(not x.startswith('-D') for x in extra):
        ap.error('unknown arguments: ' + ' '.join(extra))
    tools = os.path.abspath(a.tools)
    if a.cmd == 'fetch':
        return fetch(tools, a.gen4)
    if os.name != 'nt':
        sys.exit('port_win64: builds on Windows only (WSL or Linux: tools/port_win64.sh)')
    exe = build(tools, a.game, a.j, extra)
    if a.cmd == 'deploy':
        if not a.out:
            ap.error('deploy: --out DIR')
        deploy(tools, exe, os.path.abspath(a.out))
    else:
        say(exe)


if __name__ == '__main__':
    main()
