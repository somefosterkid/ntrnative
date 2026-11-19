#include "global.h"

#include "message_format.h"
#include "overlay_83_battle_castle_opponent_app.h"
#include "pokemon.h"

void ov83_02244AB0(BattleCastleOpponentApp *app, u32 fieldno, BoxPokemon *boxmon);

void ov83_02244AB0(BattleCastleOpponentApp *app, u32 fieldno, BoxPokemon *boxmon) {
    BufferBoxMonSpeciesName((MessageFormat *)app->strTemplate, fieldno, boxmon);
}
