// Black 2 and White 2 on Linux, Windows and Android (GNU ld, lld; docs/b2w2.md): the game's writable data (its C outside the
// overlays) goes to sections of its own, which tools/port_overlays.py's script gathers into the machine's range (.machdata, .machbss:
// port/src/machine.c puts them back at an in-process restart). As macos_machine_section.h does for ld64. Static locals are in; const
// data is not. With -fno-common (port/meson.build).
// PE (mingw): clang gives the section of a `#pragma clang section` an empty name on COFF (LLVM's COFF lowering reads only a
// section attribute), so the game's data and bss went to one nameless section, which GNU objcopy then wrote with a broken name
// (port_relink.py's b2.o: "unable to load COMDAT section name"). The MS pragmas (-fms-extensions, port/meson.build) set the
// attribute. Their bss is stored zeros (LLVM puts no global with an explicit section in BSS), and data initialised to 0 is data.
#if defined(_WIN32)
#if defined(BLACK2)
#pragma data_seg(".b2machdata")
#pragma bss_seg(".b2machbss")
#elif defined(WHITE2)
#pragma data_seg(".w2machdata")
#pragma bss_seg(".w2machbss")
#elif defined(PLATINUM)
#pragma data_seg(".plmachdata")
#pragma bss_seg(".plmachbss")
#elif defined(HEARTGOLD)
#pragma data_seg(".hgmachdata")
#pragma bss_seg(".hgmachbss")
#else
#error "gnu_machine_section.h is for Black 2, White 2, Platinum and HeartGold"
#endif
#elif defined(BLACK2)
#pragma clang section bss = ".b2machbss" data = ".b2machdata"
#elif defined(WHITE2)
#pragma clang section bss = ".w2machbss" data = ".w2machdata"
#elif defined(PLATINUM)
#pragma clang section bss = ".plmachbss" data = ".plmachdata"
#elif defined(HEARTGOLD)
#pragma clang section bss = ".hgmachbss" data = ".hgmachdata"
#else
#error "gnu_machine_section.h is for Black 2, White 2, Platinum and HeartGold"
#endif
