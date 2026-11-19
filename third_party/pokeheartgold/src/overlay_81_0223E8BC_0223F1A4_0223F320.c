#include "global.h"
#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"
#include "overlay_80_02236B78.h"


s32 ov81_02241FEC(void *);

s32 ov81_0224218C(void *);

void ov81_02242F54(BattleFactoryAppPokeballSprite *ballSprite);

void ov81_02242F94(BattleFactoryAppPokeballSprite *ballSprite, u32 animID);

void ov81_02242FB0(BattleFactoryAppPokeballSprite *ballSprite, int palIndex);

void ov81_0223F320(BattleFactoryApp *app) {
    ov80_02236DD4(app->challengeType);
    ov81_02242F54(app->ballSprites[app->unk_468]);
    ov81_02242FB0(app->ballSprites[app->unk_468], 1);
    ov81_02242F94(app->ballSprites[app->unk_468], 0);
    ov81_022408A0(app, 0, app->numMonsSelected + 1);
    app->printerID = ov81_0224086C(app, 0);
    ov81_02241FEC(app);
    ov81_0224218C(app);
}
