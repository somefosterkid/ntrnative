#include "global.h"
struct TradePokemonRevealPhase;
#include "pokemon.h"
#include "pokepic.h"
#include "overlay_71_trade_pokemon_reveal_phase.h"

void ov59_0223C380(struct TradePokemonRevealPhase *arg0, Pokemon **arg1);

void ov59_0223C380(TradePokemonRevealPhase *arg0, Pokemon **arg1) {
    PokepicTemplate sp10;

    GetPokemonSpriteCharAndPlttNarcIds(&sp10, *arg1, 2U);
    *((Pokepic * *)&arg0->scene) = PokepicManager_CreatePokepic(*((PokepicManager * *)&arg0->bgConfig), &sp10, 0xCC, 0x70, 0, 0, NULL, NULL);
}
