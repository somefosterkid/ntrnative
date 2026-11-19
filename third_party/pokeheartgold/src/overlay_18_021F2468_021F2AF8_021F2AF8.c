#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite.h"
#include "sprite_system.h"

BOOL ov18_021F2AF8(PokedexAppData *pokedexApp, u32 x, u32 y) {
    s16 posX;
    s16 posY;
    ManagedSprite_GetPositionXY(pokedexApp->unk_0670[0], &posX, &posY);
    if (x >= posX - 0x10U && x < posX + 0x10U && y >= posY - 0x10U && y < posY + 0x10U) {
        return 1;
    }
    return 0;
}
