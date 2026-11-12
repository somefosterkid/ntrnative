// Black 2 and White 2 on Linux, Windows and Android: the overlay files' writable data goes to sections of its own, in overlay order
// behind a marker object per overlay (tools/port_overlays.py --macho-layout, --macho-files --gnu-tag), which the script puts at the
// end of .ovldata and .ovlbss; port/src/ovl.c resets each overlay's range on its load from the game's own table. As
// macos_ovl_section.h does for ld64.
#if defined(BLACK2)
#pragma clang section bss = ".b2ovlbss" data = ".b2ovldata"
#elif defined(WHITE2)
#pragma clang section bss = ".w2ovlbss" data = ".w2ovldata"
#else
#error "gnu_ovl_section.h is for Black 2 and White 2"
#endif
