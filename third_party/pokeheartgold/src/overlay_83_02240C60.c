#include "global.h"

#include "message_format.h"
#include "overlay_83_battle_castle_self_app.h"
#include "pokemon.h"

void ov83_02240C60(BattleCastleSelfApp *app, u32 fieldno, BoxPokemon *boxmon);

void ov83_02240C60(BattleCastleSelfApp *app, u32 fieldno, BoxPokemon *boxmon) {
    BufferBoxMonNickname((MessageFormat *)app->strTemplate, fieldno, boxmon);
}
