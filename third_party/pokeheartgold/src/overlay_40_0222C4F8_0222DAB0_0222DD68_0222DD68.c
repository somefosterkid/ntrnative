#include "global.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"

void *GfGfxLoader_LoadFromNarc_GetSizeOut(enum NarcId narcID, u32 narcMemberIdx, BOOL compressed, enum HeapID heapID, BOOL allocAtEnd, u32 *fileSize);

u16 *ov40_0222DD68(enum HeapID heapID, int unused, int *pokedexLength);

u16 *ov40_0222DD68(enum HeapID heapID, int unused, int *pokedexLength) {
    u32 pokedexSize;
    u16 *pokedexAlphabetical = GfGfxLoader_LoadFromNarc_GetSizeOut((enum NarcId)74, 13, 0, heapID, 0, &pokedexSize);
    *pokedexLength = pokedexSize / sizeof(u16);

    return pokedexAlphabetical;
}
