// -Dnitro_header=true (port/meson.build; the sysroot builds, no .pch): forced into every C compile, it gives libntr's own targets what
// their nitro_pch.h gave them. The game's targets have port/include on their -I path and their own forced prefix (port_prefix.h, which
// must be read before nitro.h: its u64 alignment trick), so for them this is empty, as for the targets that don't see libntr's headers.
#if __has_include(<nitro.h>) && !__has_include("port_prefix.h") && !defined(BWPORT_NO_NITRO_FORCE)
#include "../../port/subprojects/libntr/include/pch/nitro_pch.h"
#endif
