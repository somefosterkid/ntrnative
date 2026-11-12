// The PC build (port/meson.build's nnsys option) compiles lib/nnsys/src/snd/sndarc.c through this file
// (tools/port_sources.py --nnsys). The archive's file table (NNSSndArcFat, read from the SDAT as it is) holds, per
// file, a 4-byte field for the address the file is loaded at (NNS_SndArcGetFileAddress): on a 64-bit host the address
// doesn't fit. Here the field says only whether the file is loaded (0: not; the SDAT holds 0 there, so a table read
// again starts empty) and the address is kept in a host table per file table. The DS build never sees this file.
#include <nitro.h>
#include <stdlib.h>

// The archive opens by its path (NNS_SndArcInit's one FS_ConvertPathToFileID and FS_OpenFileFast): libntr's FS
// opens the ROM's files on the host by path and has no file table for IDs. The path is kept for the streams
// (sndarc_stream.c's OpenFileStream opens it again by path when the ID doesn't open: port/extern's sndarc_stream.c)
#pragma push_macro("FS_ConvertPathToFileID")
#pragma push_macro("FS_OpenFileFast")
#pragma push_macro("NNS_SndArcInitOnMemory")
#pragma push_macro("NNS_SndArcGetFileAddress")
#pragma push_macro("NNS_SndArcSetFileAddress")
#define FS_ConvertPathToFileID(id, path) ((arc)->filePath = (path), MI_CpuClear8((id), sizeof(*(id))), TRUE)
#define FS_OpenFileFast(file, id) FS_OpenFile((file), (arc)->filePath)
#define NNS_SndArcInitOnMemory NNS_SndArcInitOnMemory_game
#define NNS_SndArcGetFileAddress NNS_SndArcGetFileAddress_game
#define NNS_SndArcSetFileAddress NNS_SndArcSetFileAddress_game
#include "../../../../../../lib/nnsys/src/snd/sndarc.c"
#undef FS_ConvertPathToFileID
#pragma pop_macro("FS_ConvertPathToFileID")
#undef FS_OpenFileFast
#pragma pop_macro("FS_OpenFileFast")
#undef NNS_SndArcInitOnMemory
#pragma pop_macro("NNS_SndArcInitOnMemory")
#undef NNS_SndArcGetFileAddress
#pragma pop_macro("NNS_SndArcGetFileAddress")
#undef NNS_SndArcSetFileAddress
#pragma pop_macro("NNS_SndArcSetFileAddress")

// The file tables' addresses (the game has one archive; a few, in case), each with its files' addresses
typedef struct {
    const NNSSndArcFat *fat;
    u32 count;
    void **mem;
} FatMem;

static FatMem sFatMem[4];
static u32 sFatMemNext;

static void **MemFor(const NNSSndArcFat *fat) {
    FatMem *slot = NULL;
    u32 i;

    for (i = 0; i < NELEMS(sFatMem); i++) {
        if (sFatMem[i].fat == fat) {
            slot = &sFatMem[i];
            break;
        }
    }
    if (slot == NULL) {
        slot = &sFatMem[sFatMemNext++ % NELEMS(sFatMem)];
        slot->fat = fat;
        slot->count = 0;
    }
    if (slot->count < fat->count) {
        slot->mem = realloc(slot->mem, fat->count * sizeof(void *));
        slot->count = fat->count;
    }
    return slot->mem;
}

void NNS_SndArcInitOnMemory(NNSSndArc *arc, void *data) {
    u32 i;
    void **mem;

    NNS_SndArcInitOnMemory_game(arc, data);
    mem = MemFor(arc->fat);
    for (i = 0; i < arc->fat->count; i++) {
        mem[i] = GetAddress(data, arc->fat->files[i].offset);
        arc->fat->files[i].mem = mem[i] != NULL;
    }
}

void *NNS_SndArcGetFileAddress(u32 fileId) {
    const NNSSndArcFat *fat = sCurrent->fat;

    if (fileId >= fat->count || fat->files[fileId].mem == 0) {
        return NULL;
    }
    return MemFor(fat)[fileId];
}

void NNS_SndArcSetFileAddress(u32 fileId, void *address) {
    NNSSndArcFat *fat = sCurrent->fat;

    MemFor(fat)[fileId] = address;
    fat->files[fileId].mem = address != NULL;
}
