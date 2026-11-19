#include "global.h"

#include "pokemon_types_def.h"
#include "pokepic.h"

struct Pokepic;
struct PokepicDrawParam;
typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;
typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);
void Pokepic_SetVisible(Pokepic *monSprite, int xOffset, int yOffset, int width, int height);

void ov07_0222BADC(Pokepic *sprite, int xOffset, int width, int yOffsetFromBottom, int height);

void ov07_0222BADC(Pokepic *sprite, int xOffset, int width, int yOffsetFromBottom, int height) {
    Pokepic_SetVisible(sprite, xOffset, (10 * 8) - yOffsetFromBottom, width, height);
}
