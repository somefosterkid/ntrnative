// The PC build (port/meson.build's nnsys option) compiles lib/nnsys/src/snd/sndarc_loader.c through this file
// (tools/port_sources.py --nnsys). A bank file (SBNK) holds four wave archive links of two 4-byte pointers after its
// headers; libntr's sound driver on a 64-bit host takes the bank as its SNDBankData with 8-byte pointers there, so
// the instrument count and the rest sit 0x20 bytes further on (libntr's snd_bank.c adds the 32 to each instrument's
// offset; bank.h WIN_SNDBankData is the file's layout). A whole bank read for loading (NNSi_SndArcLoadFile, whose
// block has LOAD_PADDING, 32 bytes, past the file) is moved into that layout in place, the links cleared, as the DS
// file has them before SND_AssignWaveArc. Wave archives need nothing: their 8-byte topLink takes the first reserved
// word, and the wave count and offsets stay where the file has them. The DS build never sees this file.
#include <nitro.h>
#include <stddef.h>
#include <string.h>

#define PC_BANK_GROWTH (offsetof(SNDBankData, instCount) - offsetof(WIN_SNDBankData, instCount))

static s32 PortSndArcReadFile(u32 fileId, void *buffer, s32 size, s32 offset) {
    s32 result = NNS_SndArcReadFile(fileId, buffer, size, offset);
    u8 *p = buffer;

    if (result == size && offset == 0 && (u32)size == NNS_SndArcGetFileSize(fileId) &&
        (u32)size >= sizeof(WIN_SNDBankData) && p[0] == 'S' && p[1] == 'B' && p[2] == 'N' && p[3] == 'K') {
        memmove(p + offsetof(SNDBankData, instCount), p + offsetof(WIN_SNDBankData, instCount),
                size - offsetof(WIN_SNDBankData, instCount));
        memset(p + offsetof(SNDBankData, waveArcLink), 0, sizeof(((SNDBankData *)0)->waveArcLink));
    }
    return result;
}

#pragma push_macro("NNS_SndArcReadFile")
#define NNS_SndArcReadFile PortSndArcReadFile
#include "../../../../../../lib/nnsys/src/snd/sndarc_loader.c"
#undef NNS_SndArcReadFile
#pragma pop_macro("NNS_SndArcReadFile")

_Static_assert(PC_BANK_GROWTH <= LOAD_PADDING, "a bank in libntr's layout fits its load block");
