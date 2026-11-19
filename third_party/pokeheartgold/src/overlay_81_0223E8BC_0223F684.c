#include "global.h"

#include "overlay_81.h"

void ov81_0223F684(BattleFactoryApp *app) {
    app->selectedIndices[app->numMonsSelected] = app->unk_468;
    app->numMonsSelected = (u8)(app->numMonsSelected + 1);
    app->exchangeMonSelected = TRUE;
}
