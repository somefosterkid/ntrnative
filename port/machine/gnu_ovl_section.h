// Black 2 and White 2 on Linux, Windows and Android: the overlay files' writable data goes to sections of its own, in overlay order
// behind a marker object per overlay (tools/port_overlays.py --macho-layout, --macho-files --gnu-tag), which the script puts at the
// end of .ovldata and .ovlbss; port/src/ovl.c resets each overlay's range on its load from the game's own table. As
// macos_ovl_section.h does for ld64.
// PE (mingw): clang gives the section of a `#pragma clang section` an empty name on COFF (LLVM's COFF lowering reads only a
// section attribute), so the game's data and bss went to one nameless section, which GNU objcopy then wrote with a broken name
// (port_relink.py's b2.o: "unable to load COMDAT section name"). The MS pragmas (-fms-extensions, port/meson.build) set the
// attribute. Their bss is stored zeros (LLVM puts no global with an explicit section in BSS), and data initialised to 0 is data.
#if defined(_WIN32)
#if defined(BLACK2)
#pragma data_seg(".b2ovldata")
#pragma bss_seg(".b2ovlbss")
#elif defined(WHITE2)
#pragma data_seg(".w2ovldata")
#pragma bss_seg(".w2ovlbss")
#elif defined(PLATINUM)
#pragma data_seg(".plovldata")
#pragma bss_seg(".plovlbss")
#elif defined(HEARTGOLD)
#pragma data_seg(".hgovldata")
#pragma bss_seg(".hgovlbss")
#else
#error "gnu_ovl_section.h is for Black 2, White 2, Platinum and HeartGold"
#endif
#elif defined(BLACK2)
#pragma clang section bss = ".b2ovlbss" data = ".b2ovldata"
#elif defined(WHITE2)
#pragma clang section bss = ".w2ovlbss" data = ".w2ovldata"
#elif defined(PLATINUM)
#pragma clang section bss = ".plovlbss" data = ".plovldata"
#elif defined(HEARTGOLD)
#pragma clang section bss = ".hgovlbss" data = ".hgovldata"
#else
#error "gnu_ovl_section.h is for Black 2, White 2, Platinum and HeartGold"
#endif
