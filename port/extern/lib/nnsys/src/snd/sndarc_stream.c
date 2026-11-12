// The PC build (port/meson.build's nnsys option) compiles lib/nnsys/src/snd/sndarc_stream.c through this file
// (tools/port_sources.py --nnsys). A stream opens the sound archive again: by its file ID on the DS, which libntr's FS
// (the ROM's files on the host, by path) has no table for, so here the ID never opens and OpenFileStream takes its
// own fallback, the archive's path (kept by NNS_SndArcInit: port/extern's sndarc.c). The DS build never sees this file.
#include <nitro.h>

#pragma push_macro("FS_OpenFileFast")
#pragma push_macro("FS_OpenFileEx")
#define FS_OpenFileFast(file, id) FALSE
#define FS_OpenFileEx(file, path, mode) FS_OpenFile((file), (path))   // (libntr: by path on the host)
#include "../../../../../../lib/nnsys/src/snd/sndarc_stream.c"
#undef FS_OpenFileFast
#pragma pop_macro("FS_OpenFileFast")
#undef FS_OpenFileEx
#pragma pop_macro("FS_OpenFileEx")
