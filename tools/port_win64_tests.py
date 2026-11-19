#!/usr/bin/env python3
"""Tests for tools/port_win64.py and the Windows-native build's small fixes (no downloads, no build, no ROM; any OS).

    python3 tools/port_win64_tests.py        # prints "ok" per test, exits 1 on a failure
"""
import os
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import port_win64 as pw  # noqa: E402

REPO = Path(__file__).resolve().parent.parent


def test_bat_is_port_win64_sh_heredoc_with_crlf():
    bat = pw.bat_text()
    assert bat.startswith('@echo off\r\n'), bat[:40]
    assert bat.endswith('exit /b %RC%\r\n'), bat[-40:]
    assert '\r\n' in bat and '\n' not in bat.replace('\r\n', ''), 'bare LF left'
    assert 'BAT' not in bat.splitlines()[-1]


def test_build_dirs_match_port_win64_sh():
    assert pw.build_dir(None).endswith(os.path.join('build', 'port', 'win64'))
    assert pw.build_dir('platinum').endswith('win64-plat')
    assert pw.build_dir('heartgold') == pw.build_dir('soulsilver')
    assert pw.build_dir('soulsilver').endswith('win64-hgss')


def test_env_puts_the_toolchain_first():
    tools = tempfile.mkdtemp()
    env = pw.env_for(tools)
    first = env['PATH'].split(os.pathsep)[0]
    assert first == os.path.join(tools, 'winlibs', 'mingw64', 'bin'), first
    assert '\\' not in env['PKG_CONFIG_PATH']


def test_downloads_are_pinned():
    for entry in [pw.WINLIBS, pw.FLEXBISON, pw.SDL2, pw.ENET, pw.MESA] + pw.MSYS2:
        url, sha = entry[0], entry[1]
        assert url.startswith('http') and len(sha) == 64 and int(sha, 16) >= 0, entry
    assert any(p.startswith('meson==') for p in pw.PIP)


def test_native_machine_file_has_no_shim_and_uses_the_constant():
    ini = (REPO / 'port/machine/win64-native.ini').read_text()
    assert "shim" not in ini.split('[built-in options]')[1]   # Windows.h == windows.h on Windows: the shim would include itself
    assert "mingw + '/include'" in ini and "mingw + '/lib'" in ini


def test_line_paths_use_forward_slashes():
    import port_assets
    p = os.path.join(os.getcwd(), 'src', 'x.c')
    assert '\\' not in port_assets.shown(p), port_assets.shown(p)


def main():
    failed = 0
    for name, fn in sorted(globals().items()):
        if name.startswith("test_") and callable(fn):
            try:
                fn()
                print("ok", name)
            except Exception as e:  # noqa: BLE001
                import traceback
                traceback.print_exc()
                print("FAIL", name, type(e).__name__, e)
                failed += 1
    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
