// Black 2 and White 2 on Linux, Windows and Android (GNU ld, lld; docs/b2w2.md): the game's writable data (its C outside the
// overlays) goes to sections of its own, which tools/port_overlays.py's script gathers into the machine's range (.machdata, .machbss:
// port/src/machine.c puts them back at an in-process restart). As macos_machine_section.h does for ld64. Static locals are in; const
// data is not. With -fno-common (port/meson.build).
#if defined(BLACK2)
#pragma clang section bss = ".b2machbss" data = ".b2machdata"
#elif defined(WHITE2)
#pragma clang section bss = ".w2machbss" data = ".w2machdata"
#else
#error "gnu_machine_section.h is for Black 2 and White 2"
#endif
