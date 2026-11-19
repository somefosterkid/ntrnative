#include "global.h"
#include "overlay_74_gbamigrator.h"
struct GBAMigrator;

s32 AGB_GetBoxMonData(void *, s32, void *);

s32 ov74_02231764(int speciesGBA);

void ov74_02232474(struct GBAMigrator *arg0, s32 arg1);

void ov74_02232474(GBAMigrator *arg0, s32 arg1) {
    ov74_02231764(AGB_GetBoxMonData(*((u8 **)&arg0->pokemonStorage) + 4 + (0x960 * *((s32 *)&arg0->currentBox)) + (0x50 * arg1), 0xB, 0));
}
