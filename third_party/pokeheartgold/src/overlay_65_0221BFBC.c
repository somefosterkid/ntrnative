#include "global.h"
#include "sprite.h"

struct Sprite;
void Sprite_SetPalOffset(Sprite *sprite, u32 paletteOffset);
const u8 GetMonIconPaletteEx(u32 species, u32 form, u32 isEgg);

void ov65_0221BFBC(NNSG2dCharacterData *charData, int species, int form, int isEgg, int slotIdx, Sprite *sprite);

void ov65_0221BFBC(NNSG2dCharacterData *charData, int species, int form, int isEgg, int slotIdx, Sprite *sprite) {
    GX_LoadOBJ(charData->pRawData, ((4 * 32 + 18) + slotIdx * (4 * 4)) * (8 * (8 / 2)), (4 * 4) * (8 * (8 / 2)));
    Sprite_SetPalOffset(sprite, GetMonIconPaletteEx(species, form, isEgg) + 10);
}
